//=============================================================================
//�y Manager �z
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
// �� Manager �N���X
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
            t->start(); // �J�n����A�o���A�őҋ@��ԂɂȂ�
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

        // �Œ�^�X�N
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

        // �T�u�X���b�h
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
            // �Œ�^�X�N��ǉ����āA���s�t�F�[�Y�Ń\�[�g
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
                // �Ⴄ�t�F�[�Y������������A�����܂Ńv�b�V�������^�X�N�����s
                else
                {
                    _executeCurrentPhase( current_phase );

                    current_phase = (*itr)->mTaskExecutePhase;
                }
            }

            // �Ō�̕�
            _executeCurrentPhase( current_phase );


            //-----------------------------------------------------
            // �����t�F�[�Y
            
            mHandler->OnFrameWait();

            // ���� if �� false �̏ꍇ�̓}�X�^�[�̏����ɒx�����������Ă���Ƃ����Ӗ�
            if ( mMasterPipelineThread->isWaiting() )
            {
                mMasterPipelineThread->waitForTaskWait();   // �}�X�^�[���~�܂�܂ő҂�
                mHandler->OnUpdateDrawContents();           // �����������s
                mMasterPipelineThread->enableRunning();     // �� �}�X�^�[���s�J�n (��������A����̎��s����������)
            }

            //-----------------------------------------------------
            // 

            // �O��������s�^�X�N������
		    //		(���[�U�[�ɐ����Ԃ�������A���C���X���b�h�ȊO�œ��삷��^�X�N�B
		    //		���C���X���b�h�̃p�C�v���C���ȊO�ɋl�߂�)
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
        
        // ���̎��_�Ń}�X�^�[�̏����������͏I�����āA
        // ���s�t�F�[�Y�́��̕����őҋ@���Ă���B
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

        //    // �`�悪�I�����Ă���ꍇ
        //    if (mMasterPipelineThread->isWaiting())
        //    {
        //        mHandler->OnUpdateDrawContents();
        //    }
        //}

        // ���C���^�X�N�����邱�ƂőS�p�C�v���C�������s��Ԃɂ���
        mPipelineEventBarrier->notifyAllSub();

        // ���C���X���b�h�� 0 �ԃp�C�v���C�������s
        _executePipeline( 0 );

#else
        // �}�X�^�[�^�X�N�̑ҋ@�t�F�[�Y�̏ꍇ�́A�}�X�^�[�^�X�N���ҋ@����܂ő҂�
        if ( mMasterTask->mTaskExecutePhase == current_phase_ )
        {
            //mMasterPipelineThread->disableRunning();
            mMasterPipelineThread->waitForTaskWait();
        }
        // �}�X�^�[�^�X�N�J�n 
        //      �z���g�͂ЂƂO�̃t�F�[�Y�Œ�~��������ێ����Ĕ��肷������������ǁA
        //      ���̂Ƃ���͖��Ȃ̂ł��̂܂܁B
        else
        {
            mMasterPipelineThread->enableRunning();
        }

        // ���C���^�X�N�����邱�ƂőS�p�C�v���C�������s��Ԃɂ���
        mPipelineEventBarrier->notifyAllSub();

        // ���C���X���b�h�� 0 �ԃp�C�v���C�������s
        _executePipeline( 0 );
#endif
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    bool Manager::_cmpTasks( const TaskBase* l_, const TaskBase* r_ )
    {
        // ����
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
// �� PipelineThread
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
// �� MasterPipelineThread
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
                    // �ҋ@
                    case 0:
                    {
                        mEndNotifyFlag.setTrue();

                        mBeginNotifyFlag.wait();    // �� ���C������J�n�ʒm������܂ł����őҋ@

                        mBeginNotifyFlag.setFalse();

                        state = 1;

                        mStandbyNotifyFlag.setTrue();

                        if ( mEndRequested.isTrue() ) return;
                        break;
                    }
                    // ���s(�`��)
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