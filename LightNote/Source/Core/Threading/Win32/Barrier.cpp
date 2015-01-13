//==============================================================================
// Barrier 
//==============================================================================

#include "stdafx.h"
#include "../Barrier.h"

namespace LNote
{
namespace Core
{
namespace Threading
{

//==============================================================================
// ■ Barrier (win32)
//==============================================================================
#if defined(LNOTE_WIN32)

#if LNOTE_ENABLE_THREAD_DEBUG
	#define DEBUG_WaitForSingleObject( h_, str_ ) \
		if (::WaitForSingleObject( h_, 10000 ) != WAIT_OBJECT_0) \
		{ \
			printf(str_); \
			*reinterpret_cast< int* >( 0 ) = 0; \
		}
#else
	#define DEBUG_WaitForSingleObject( h_, str_ ) ::WaitForSingleObject( h_, INFINITE );
#endif

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Barrier::Barrier( int maxCount )
        : mNotifyFlag	( false )
		, mAllLeaved	( true )
    {
        mMaxNum = ( maxCount < 1 ) ? 1 : maxCount;    // 最低 1
        //mWaitThreadNum.set( mMaxNum );
        mWaitThreadNum = mMaxNum;

        mFirstThreadSignal = ::CreateEvent( NULL, TRUE, FALSE, NULL );
        mSignalWasDone     = ::CreateEvent( NULL, TRUE, FALSE, NULL );
        
        ::InitializeCriticalSection( &mCS );
        ::InitializeCriticalSection( &mThreadNumCS );

    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Barrier::~Barrier()
    {
        ::DeleteCriticalSection( &mThreadNumCS );
        ::DeleteCriticalSection( &mCS );

        if ( mFirstThreadSignal )
        {
            ::CloseHandle( mFirstThreadSignal );
            mFirstThreadSignal = NULL;
        }
        if ( mSignalWasDone )
        {
            ::CloseHandle( mSignalWasDone );
            mSignalWasDone = NULL;
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Barrier::wait()
    {
        if ( mNotifyFlag.isTrue() )
        {
            return;
        }

		mAllLeaved.wait();
        mExternalMutex.lock();

		// 最後のスレッド待ち状態になろうとしている時 (すべて開始する)
        if ( mWaitThreadNum == 1 )
        {
			mAllLeaved.setFalse();

            ::ResetEvent( mSignalWasDone );
            ::SetEvent( mFirstThreadSignal );   // 最初に待ってるスレッドを再開
            
			DEBUG_WaitForSingleObject(mSignalWasDone, "Barrier::wait() 1 timeout");

            ::ResetEvent( mFirstThreadSignal );

			mAllLeaved.setTrue();

        }
        else
        {
            --mWaitThreadNum;
            mExternalMutex.unlock();

            ::EnterCriticalSection( &mCS ); // 最初のひとつ以外はここで待つ
			DEBUG_WaitForSingleObject(mFirstThreadSignal, "Barrier::wait() 2 timeout");	// 最初のひとつはここで待つ
            ++mWaitThreadNum;
            
            ::LeaveCriticalSection( &mCS );

            if ( mWaitThreadNum == mMaxNum )
            {
                ::SetEvent( mSignalWasDone );
            }
            
            // 一度開始しても、notify_all の部分を呼んだスレッド (最後のスレッド) が
            // ↓の unlock() を呼ぶまではここで待機する

            mExternalMutex.lock();
        }

        mExternalMutex.unlock();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Barrier::setNotifyFlag( bool flag )
    {
        if ( flag )
        {
            mNotifyFlag.setTrue();

            // 待機スレッドがある場合はすべて開始してから自分を開始する
            if ( mWaitThreadNum != mMaxNum )
            {
                ::ResetEvent( mSignalWasDone );
                ::SetEvent( mFirstThreadSignal );
                //::WaitForSingleObject( mSignalWasDone, INFINITE );
				DEBUG_WaitForSingleObject(mSignalWasDone, "Barrier::setNotifyFlag() timeout");
                ::ResetEvent( mFirstThreadSignal );
            }
        }
        else
        {
            mNotifyFlag.setFalse();
        }
    }

//==============================================================================
// ■ Barrier (pthread)
//==============================================================================
#else
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Barrier::Barrier( int maxCount )
        : mNotifyFlag( false )
    {
        mMaxNum = ( maxCount < 1 ) ? 1 : maxCount;    // 最低 1
        mWaitThreadNum = 0;
        
        pthread_mutex_init( &mMutex, NULL );
        pthread_cond_init( &mCond, NULL );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Barrier::~Barrier()
    {
        pthread_mutex_init( &mMutex, NULL );
        pthread_cond_init( &mCond, NULL );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Barrier::wait()
    {
        pthread_mutex_lock( &mMutex );
        
        ++mWaitThreadNum;
        
        // 最後のスレッドが来たら全部再開
        if ( mWaitThreadNum == mMaxNum )
        {
            pthread_cond_broadcast( &mCond );
        }
        else
        {
            // ここで mutex はアンロックされる。
            // pthread_cond_broadcast() が呼ばれて cond が使用可能になると
            // mutex を再びロックして再開する。
            pthread_cond_wait( &mCond, &mMutex );
        }
        
        --mWaitThreadNum;
        
        pthread_mutex_unlock( &mMutex );
        
    }
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Barrier::setNotifyFlag( bool flag_ )
    {
        if ( flag_ )
        {
            mNotifyFlag.setTrue();
            pthread_cond_broadcast( &mCond );
        }
        else
        {
            mNotifyFlag.setFalse();
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    /*
    void Barrier::notify()
    {
        pthread_mutex_lock( &mMutex );
        pthread_cond_broadcast( &mCond );
        //mWaitThreadNum = 0;
        pthread_mutex_unlock( &mMutex );
    }
    */

#endif


//==============================================================================
// ■ EventBarrier
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    EventBarrier::EventBarrier( int maxSubCount )
        : mMaxWaitingSubNum ( maxSubCount )
        , mWaitingSubNum    ( 0 )
        , mBarrier          ( maxSubCount + 1 )
        , mMainNotifyFlag   ( true )
		, mAllLeaved		( true )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    EventBarrier::~EventBarrier()
    {
    }
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void EventBarrier::waitMain()
    {
        mMainNotifyFlag.wait();
        mMainNotifyFlag.setFalse();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void EventBarrier::waitSub()
    {
		mAllLeaved.wait();

		mMutex.lock(); // mWaitingSubNum.increment() と次のifまでに再びincrementされないように備える
        mWaitingSubNum.increment();
        if ( mWaitingSubNum.get() == mMaxWaitingSubNum )
        {
			mMutex.unlock();
			mAllLeaved.setFalse();
            mMainNotifyFlag.setTrue();  // メインスレッド実行
			
        }
		else
		{
			mMutex.unlock();
		}
        mBarrier.wait();

		mMutex.lock();
        mWaitingSubNum.decrement();
		if ( mWaitingSubNum.get() == 0 )
        {
			mAllLeaved.setTrue();
		}
		mMutex.unlock();

		// ひとつだけ先行してこの関数から出て、
		// 他のスレッドがこの関数から出る前に
		// もう一度戻ってきて increment された場合…
		//		メインスレッドが waitMain() でサブスレッドを
		//		すべて待つ前に処理を開始してしまう。

    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void EventBarrier::notifyAllSub()
    {
        mBarrier.wait();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void EventBarrier::setNotifyFlag( bool flag_ )
    {
        mBarrier.setNotifyFlag( flag_ );
        if ( flag_ )
        {
            mMainNotifyFlag.setTrue();
        }
    }
    
} // namespace Threading
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================