//=============================================================================
//【 Manager 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <algorithm>
#include "Manager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Task
{
#define TRACE( ... ) //printf( __VA_ARGS__ )

//=============================================================================
// ■ Manager クラス
//=============================================================================

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    Manager::Manager()
        : mHandler                      ( NULL )
        , mPipelineNum                  ( 2 )
        , mMasterTask                   ( NULL )
        , mMasterPipelineThread         ( NULL )
        , mPipelineBarrier              ( NULL )
        , mEndRequested                 ( NULL )
        , mTaskRunning                  ( NULL )
        , mPipelineEventBarrier         ( NULL )
        , mEnableMultiCoreOptimization  ( true )
    {
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    Manager::~Manager()
    {
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void Manager::initialize(ITaskExecuteHandler* handler_)
    {
        mHandler = handler_;

        mMasterPipelineThread = LN_NEW MasterPipelineThread( this );
        mPipelineBarrier = LN_NEW Threading::Barrier( mPipelineNum );
        mEndRequested = LN_NEW Threading::EventFlag( false );
        mTaskRunning = LN_NEW Threading::EventFlag( false );
        mPipelineEventBarrier = LN_NEW Threading::EventBarrier( 1 );

        mPipelineThreadArray.push_back( LN_NEW PipelineThread( this, 1 ) );
        mTaskPipelineArray.resize( mPipelineNum );

        ln_foreach( PipelineThread* t, mPipelineThreadArray )
        {
            t->start(); // 開始直後、バリアで待機状態になる
        }
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void Manager::finalize()
    {
        if ( mEndRequested )
        {
            mEndRequested->setTrue();
            mPipelineBarrier->setNotifyFlag( true );
            mPipelineEventBarrier->setNotifyFlag( true );
            mTaskRunning->setTrue();

            ln_foreach( PipelineThread* t, mPipelineThreadArray )
            {
                t->wait();
            }

            LN_SAFE_RELEASE( mPipelineBarrier );
            LN_SAFE_RELEASE( mEndRequested );
            LN_SAFE_RELEASE( mTaskRunning );
            LN_SAFE_RELEASE( mPipelineEventBarrier );
        }

        if ( mMasterPipelineThread )
        {
            mMasterPipelineThread->endRequest();
            mMasterPipelineThread->wait();
            SAFE_DELETE( mMasterPipelineThread );
        }

        // 固定タスク
        ln_foreach( TaskBase* t, mConstTaskArray )
        {
            SAFE_DELETE( t );
        }
        mConstTaskArray.clear();

        ln_foreach( TaskBase* t, mExternalConstTaskArray )
        {
            SAFE_DELETE( t );
        }
        mExternalConstTaskArray.clear();

        // サブスレッド
        ln_foreach( PipelineThread* t, mPipelineThreadArray )
        {
            SAFE_DELETE( t );
        }
        mPipelineThreadArray.clear(); 
        SAFE_DELETE( mMasterTask );
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void Manager::execute()
    {
        if ( mEnableMultiCoreOptimization )
        {
            mPipelineEventBarrier->waitMain();
            mTaskArray.clear();
        }

        if ( mConstTaskArray.empty() ) return;

        if ( mEnableMultiCoreOptimization )
        {
            TRACE( "---- execute begin ----\n" );
            // 固定タスクを追加して、実行フェーズでソート
            mTaskArray.insert( mTaskArray.end(), mConstTaskArray.begin(), mConstTaskArray.end() );
            std::sort( mTaskArray.begin(), mTaskArray.end(), _cmpTasks );

            int current_phase = mTaskArray.front()->mTaskExecutePhase;
            int pipeline_index = 0;
            TaskArray::iterator itr = mTaskArray.begin();
            TaskArray::iterator end = mTaskArray.end();
            for ( ; itr != end; ++itr )
            {
                if ( current_phase == (*itr)->mTaskExecutePhase )
                {
                    mTaskPipelineArray[ pipeline_index ].push_back( *itr );
                    pipeline_index = ++pipeline_index % mPipelineNum;
                }
                // 違うフェーズが見つかったら、そこまでプッシュしたタスクを実行
                else
                {
                    _executeCurrentPhase( current_phase );

                    current_phase = (*itr)->mTaskExecutePhase;
                }
            }

            // 最後の分
            _executeCurrentPhase( current_phase );


            //-----------------------------------------------------
            // 同期フェーズ
            
            mHandler->OnFrameWait();

            // この if は false の場合はマスターの処理に遅延が発生しているという意味
            if ( mMasterPipelineThread->isWaiting() )
            {
                mMasterPipelineThread->waitForTaskWait();   // マスターが止まるまで待つ
                mHandler->OnUpdateDrawContents();           // 同期処理実行
                mMasterPipelineThread->enableRunning();     // ★ マスター実行開始 (初期化後、初回の実行もここから)
            }

            //-----------------------------------------------------
            // 

            // 外部並列実行タスクを準備
		    //		(ユーザーに制御を返した後も、メインスレッド以外で動作するタスク。
		    //		メインスレッドのパイプライン以外に詰める)
            mPipelineEventBarrier->waitMain();
            pipeline_index = 0;
            ln_foreach( TaskBase* t, mExternalConstTaskArray )
            {
                mTaskPipelineArray[ 1 + pipeline_index ].push_back( t );
                pipeline_index = ++pipeline_index % (mPipelineNum - 1);
            }

            mPipelineEventBarrier->notifyAllSub();

            TRACE( "---- execute end ----\n" );
        }
        else
        {
            ln_foreach( TaskBase* t, mConstTaskArray )
            {
                t->execute();
            }
            
            mHandler->OnFrameWait();
            mHandler->OnUpdateDrawContents();
            mMasterTask->execute();

            ln_foreach( TaskBase* t, mExternalConstTaskArray )
            {
                t->execute();
            }
        }
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void Manager::pushConstTask( Base::Delegate00 method_, int phase_ )
    {
        if ( phase_ == LN_EXTERNAL_SUB_TASK_EXECUTE_PHASE )
        {
            mExternalConstTaskArray.push_back( LN_NEW DelegateTask( method_, phase_ ) );
        }
        else
        {
            mConstTaskArray.push_back( LN_NEW DelegateTask( method_, phase_ ) );
        }
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void Manager::setMasterTask( Base::Delegate00 method_, Base::Delegate00 init_method_, int sync_phase_ )
    {
        SAFE_DELETE( mMasterTask );
        mMasterTaskInitializeMethod = init_method_;
        mMasterTask = LN_NEW DelegateTask( method_, sync_phase_ );
        mMasterPipelineThread->start();
        mMasterPipelineThread->waitForInitialized();
        
        // この時点でマスターの初期化処理は終了して、
        // 実行フェーズの★の部分で待機している。
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void Manager::_executeCurrentPhase( int current_phase_ )
    {
        TRACE( "    < Task Execute Phase %d >\n", current_phase_ );
#if 1
        //if ( mMasterTask->mTaskExecutePhase == current_phase_ )
        //{
        //    mHandler->OnFrameWait();

        //    // 描画が終了している場合
        //    if (mMasterPipelineThread->isWaiting())
        //    {
        //        mHandler->OnUpdateDrawContents();
        //    }
        //}

        // メインタスクが入ることで全パイプラインを実行状態にする
        mPipelineEventBarrier->notifyAllSub();

        // メインスレッドは 0 番パイプラインを実行
        _executePipeline( 0 );

#else
        // マスタータスクの待機フェーズの場合は、マスタータスクが待機するまで待つ
        if ( mMasterTask->mTaskExecutePhase == current_phase_ )
        {
            //mMasterPipelineThread->disableRunning();
            mMasterPipelineThread->waitForTaskWait();
        }
        // マスタータスク開始 
        //      ホントはひとつ前のフェーズで停止したかを保持して判定する方がいいけど、
        //      今のところは問題なのでこのまま。
        else
        {
            mMasterPipelineThread->enableRunning();
        }

        // メインタスクが入ることで全パイプラインを実行状態にする
        mPipelineEventBarrier->notifyAllSub();

        // メインスレッドは 0 番パイプラインを実行
        _executePipeline( 0 );
#endif
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    bool Manager::_cmpTasks( const TaskBase* l_, const TaskBase* r_ )
    {
        // 昇順
        return l_->mTaskExecutePhase < r_->mTaskExecutePhase;
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void Manager::_executePipeline( int pipeline_index_ )
    {
        TRACE( "_executePipeline(%d)\n", pipeline_index_ );

        TaskArray& list = mTaskPipelineArray[ pipeline_index_ ];
        ln_foreach( TaskBase* t, list )
        {
            t->execute();
        }
        list.clear();
    }

//=============================================================================
// ■ PipelineThread
//=============================================================================

    void Manager::PipelineThread::run()
    {
        while ( true )
        {
            mManager->mPipelineEventBarrier->waitSub();

            if ( mManager->mEndRequested->isTrue() ) return;

            mManager->_executePipeline( mPipelineIndex );
        }
    }

//=============================================================================
// ■ MasterPipelineThread
//=============================================================================

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    Manager::MasterPipelineThread::MasterPipelineThread( Manager* manager )
        : mManager          ( manager )
		, mEndNotifyFlag    ( false )
		, mBeginNotifyFlag  ( false )
		, mStandbyNotifyFlag( false )
        , mWaiting          ( false )
        , mRunnning         ( false )
        , mEndRequested     ( false )
    {}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void Manager::MasterPipelineThread::waitForInitialized()
	{
        mEndNotifyFlag.wait();
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void Manager::MasterPipelineThread::waitForTaskWait()
	{
		mEndNotifyFlag.wait();
		mEndNotifyFlag.setFalse();
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void Manager::MasterPipelineThread::enableRunning()
	{
		mBeginNotifyFlag.setTrue();
		mStandbyNotifyFlag.wait();
		mStandbyNotifyFlag.setFalse();
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void Manager::MasterPipelineThread::endRequest()
	{ 
		mEndRequested.setTrue(); 
		mStandbyNotifyFlag.setTrue();
		enableRunning(); 
	}
    
	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    bool Manager::MasterPipelineThread::isWaiting()
    {
        return mEndNotifyFlag.isTrue();
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void Manager::MasterPipelineThread::run()
    {
        try
        {
            if (!mManager->mMasterTaskInitializeMethod.isEmpty())
            {
                mManager->mMasterTaskInitializeMethod.call();
            }

            int state = 0;

            while ( true )
            {
    			switch (state)
                {
                    // 待機
                    case 0:
                    {
                        mEndNotifyFlag.setTrue();

                        mBeginNotifyFlag.wait();    // ★ メインから開始通知が来るまでここで待機

                        mBeginNotifyFlag.setFalse();

                        state = 1;

                        mStandbyNotifyFlag.setTrue();

                        if ( mEndRequested.isTrue() ) return;
                        break;
                    }
                    // 実行(描画)
                    case 1:
                    {
                        mManager->mMasterTask->execute();

                        state = 0;
                        break;
                    }
                }
            }
        }
        catch ( Base::Exception& e )
        {
            e.process();
        }
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Task
} // namespace Core
} // namespace LNote

//=============================================================================
//
//=============================================================================