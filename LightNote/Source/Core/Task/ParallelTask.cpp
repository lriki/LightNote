//==============================================================================
// ParallelTask 
//==============================================================================

#pragma once

#include "stdafx.h"
#include "ParallelTask.h"

namespace LNote
{
namespace Core
{
namespace Task
{

//==============================================================================
// ParallelTask
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ParallelTask::ParallelTask()
        : mEndNotifyFlag    ( false )
		, mBeginNotifyFlag  ( false )
		, mStandbyNotifyFlag( false )
        , mWaiting          ( false )
        , mRunnning         ( false )
        , mEndRequested     ( false )
    {}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ParallelTask::waitForInitialized()
	{
        mEndNotifyFlag.wait();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ParallelTask::waitForTaskWait()
	{
		if (!mWaiting.isTrue())
		{
			mEndNotifyFlag.wait();
			mEndNotifyFlag.setFalse();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ParallelTask::enableRunning()
	{
		mBeginNotifyFlag.setTrue();
		mStandbyNotifyFlag.wait();
		mStandbyNotifyFlag.setFalse();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ParallelTask::endRequest()
	{ 
		mEndRequested.setTrue(); 
		mStandbyNotifyFlag.setTrue();
		enableRunning(); 
	}
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool ParallelTask::isWaiting()
    {
		return mWaiting.isTrue();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ParallelTask::run()
    {
        try
        {
			if ( !mInitializeMethod.isEmpty() ) {
				mInitializeMethod.call();
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
						mWaiting.setTrue();			// これは制御フラグじゃなくてただのフラグ

                        mBeginNotifyFlag.wait();    // ★ メインから開始通知が来るまでここで待機

                        mBeginNotifyFlag.setFalse();

                        state = 1;
						mWaiting.setFalse();

                        mStandbyNotifyFlag.setTrue();


                        if ( mEndRequested.isTrue() ) return;
                        break;
                    }
                    // 実行(描画)
                    case 1:
                    {
						if ( !mUpdateMethod.isEmpty() ) {
							mUpdateMethod.call();
						}

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

} // namespace Task
} // namespace Core
} // namespace LNote
