//==============================================================================
// ParallelTask 
//------------------------------------------------------------------------------
///**
//  @file       ParallelTask.h
//  @brief      ParallelTask
//*/
//==============================================================================

#pragma once

#include "../Base/Delegate.h"
#include "../Threading/Thread.h"
#include "../Threading/EventFlag.h"
#include "../Threading/Mutex.h"

namespace LNote
{
namespace Core
{
namespace Task
{

//==============================================================================
// ParallelTask
//==============================================================================
class ParallelTask
    : public Threading::Thread
{
public:
    ParallelTask();
	virtual ~ParallelTask() {}

public:
	void setInitializeMethod( Base::Delegate00& method ) { mInitializeMethod = method; }
	void setUpdateMethod( Base::Delegate00& method ) { mUpdateMethod = method; }

public:

    virtual void run();
 
	/// スレッドの初期化処理が終了し、メインループの先頭で待機するのをまつ
    void waitForInitialized();

    void waitForTaskWait();

	void enableRunning();

    void endRequest();
        
    bool isWaiting();

private:

	Base::Delegate00		mInitializeMethod;
	Base::Delegate00		mUpdateMethod;

	Threading::EventFlag	mEndNotifyFlag;
    Threading::EventFlag	mBeginNotifyFlag;
    Threading::EventFlag	mStandbyNotifyFlag;

    Threading::EventFlag	mWaiting;
    Threading::EventFlag	mRunnning;
    Threading::EventFlag	mEndRequested;
	Threading::Mutex		mMutex;
};

} // namespace Task
} // namespace Core
} // namespace LNote
