//=============================================================================
//【 Manager 】
//-----------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include <queue>
#include "../Threading/Thread.h"
#include "../Threading/Barrier.h"
#include "Task.h"
#include "ITaskExecuteHandler.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
static const int LN_EXTERNAL_SUB_TASK_EXECUTE_PHASE = -1;

namespace Task
{

//=============================================================================
// ■ Manager
//-----------------------------------------------------------------------------
///**
//  @brief		
//
//  @note
//              とりあえずバリア同期の単位を「フェーズ」と呼んでみる。
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

    Manager();

    virtual ~Manager();

public:

	/// 初期化
    void initialize(ITaskExecuteHandler* handler_);

	/// 終了処理
    void finalize();

	/// 実行
    void execute();

	/// 固定タスク追加 (デリゲートから登録。phase_ に LN_EXTERNAL_SUB_TASK_EXECUTE_PHASE を指定すると、制御を返した後～次の begin() まで)
	//      固定タスク：Manager の更新処理など、initialize() 後、一度だけ登録されて、以降削除しないタスク
    void pushConstTask( Base::Delegate00 method_, int phase_ );

	/// マスタータスクの設定 (execute() の外に出ても実行されているタスク。sync_phase_ は同期をとるフェーズ)
	//      sync_phase_ のフェーズに入った時、そのフェーズのタスクを実行する前に
	//      マスタータスクの終了を待つ。次のフェーズの実行前にマスタータスクを開始する。
    void setMasterTask( Base::Delegate00 method_, Base::Delegate00 init_method_, int sync_phase_ );

    void waitMasterTaskWait() {mMasterPipelineThread->waitForInitialized();};
    
    void setEnableMultiCoreOptimization( bool flag ) { mEnableMultiCoreOptimization = flag; }

    bool getEnableMultiCoreOptimization() const { return mEnableMultiCoreOptimization; }

private:

    void _executeCurrentPhase( int current_phase_ );

	/// タスクのソート関数
    static bool _cmpTasks( const TaskBase* l_, const TaskBase* r_ );


    void _executePipeline( int pipeline_index_ );

private:

	/// PipelineThread
    class PipelineThread
        : public Threading::Thread
    {
    public:

        /// pipeline_index_ は 1 スタート
        PipelineThread( Manager* manager_, int pipeline_index_ )
            : mManager          ( manager_ )
            , mPipelineIndex    ( pipeline_index_ )
            //, mWaiting          ( false )
        {}

        virtual void run();

        //void waitForTaskWait() { mWaiting.wait(); }
 
    private:

        Manager*    mManager;
        int         mPipelineIndex;
        //Threading::EventFlag   mWaiting;
    };

    class MasterPipelineThread
        : public Threading::Thread
    {
    public:

        MasterPipelineThread( Manager* manager );

        virtual void run();
 
        /// スレッドの初期化処理が終了し、メインループの先頭で待機するのをまつ
        void waitForInitialized();

        void waitForTaskWait();

		void enableRunning();

        void endRequest();
        
        bool isWaiting();

    private:

		Threading::EventFlag   mEndNotifyFlag;
        Threading::EventFlag   mBeginNotifyFlag;
        Threading::EventFlag   mStandbyNotifyFlag;

        Manager*            mManager;
        Threading::EventFlag   mWaiting;
        Threading::EventFlag   mRunnning;
        Threading::EventFlag   mEndRequested;
		Threading::Mutex		mMutex;
    };

private:

    typedef std::vector< TaskBase* >        TaskArray;
    typedef std::vector< TaskArray >        TaskPipelineArray;
    typedef std::vector< PipelineThread* >  PipelineThreadArray;

private:

    ITaskExecuteHandler*    mHandler;
    int                 mPipelineNum;           ///< メインスレッドも含むパイプライン数

    TaskArray           mTaskArray;
    TaskArray           mConstTaskArray;
    TaskArray           mExternalConstTaskArray;
    TaskPipelineArray   mTaskPipelineArray;     ///< メインスレッドも含む、パイプラインごとの実行タスク配列
    PipelineThreadArray mPipelineThreadArray;   ///< メインスレッド以外のスレッド配列
    TaskBase*               mMasterTask;
    MasterPipelineThread*   mMasterPipelineThread;
    Base::Delegate00        mMasterTaskInitializeMethod;
	//Threading::EventFlag*  mMasterTaskWaiting;

    Threading::Barrier*    mPipelineBarrier;
    Threading::EventFlag*  mEndRequested;
    Threading::EventFlag*  mTaskRunning;

    Threading::EventBarrier*   mPipelineEventBarrier;
    bool                    mEnableMultiCoreOptimization;

    friend class PipelineThread;

public:

	/// (OpenGL Shader 用暫定対応)
    MasterPipelineThread* getMasterPipelineThread() { return mMasterPipelineThread; }
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Task
} // namespace Core
} // namespace LNote

//=============================================================================
//
//=============================================================================