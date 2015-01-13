//==============================================================================
// EventFlag 
//==============================================================================

#include "stdafx.h"
#include "../EventFlag.h"

namespace LNote
{
namespace Core
{
namespace Threading
{

//==============================================================================
// ■ EventFlag (Windows)
//==============================================================================
#if defined(LNOTE_WIN32)

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    EventFlag::EventFlag()
    	: mHandle( NULL )
    {
        mHandle = CreateEvent( NULL, TRUE, FALSE, NULL );
        ::InitializeCriticalSection( &mCS );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    EventFlag::EventFlag( bool init_flag )
    	: mHandle( NULL )
    {
        BOOL t = ( init_flag ) ? TRUE : FALSE;
        // 手動リセットで作成
        mHandle = CreateEvent( NULL, TRUE, t, NULL );

        ::InitializeCriticalSection( &mCS );
       
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    EventFlag::~EventFlag()
    {
         ::DeleteCriticalSection( &mCS );
        if ( mHandle )
        {
            ::CloseHandle( mHandle );
            mHandle = NULL;
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void EventFlag::setTrue()
    {
        if ( mHandle )
        {
            ::SetEvent( mHandle );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void EventFlag::setFalse()
    {
        if ( mHandle )
        {
            ::ResetEvent( mHandle );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool EventFlag::isTrue()
    {
        if ( mHandle )
        {
            return ( ::WaitForSingleObject( mHandle, 0 ) == WAIT_OBJECT_0 );
        }
        return true;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void EventFlag::wait()
    {
        if ( mHandle )
        {
#if LNOTE_ENABLE_THREAD_DEBUG
			if (::WaitForSingleObject( mHandle, 10000 ) != WAIT_OBJECT_0)
			{
				printf("EventFlag::wait() timeout");
				*reinterpret_cast< int* >( 0 ) = 0;
			}
#else
			::WaitForSingleObject( mHandle, INFINITE );
#endif
        }
    }
    
//==============================================================================
// ■ EventFlag (pthread)
//==============================================================================
#else

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    EventFlag::EventFlag()
    {
		pthread_mutex_init( &mMutex, NULL );
        pthread_cond_init( &mWait, NULL );
        mSignal = false;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    EventFlag::EventFlag( bool init_flag )
    {
        pthread_mutex_init( &mMutex, NULL );
        pthread_cond_init( &mWait, NULL );
        mSignal = init_flag;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    EventFlag::~EventFlag()
    {
        pthread_cond_destroy( &mWait );
        pthread_mutex_destroy( &mMutex );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void EventFlag::setTrue()
    {
        // ミューテックスをロックする
        int r = pthread_mutex_lock( &mMutex );
        if ( r != 0 )
        { 
        	errno = r; 
        	return;
        }
        
        mSignal = true;
        
        // 中断しているスレッドに通知を送る
        r = pthread_cond_broadcast( &mWait );//pthread_cond_signal( &mWait );
        if ( r != 0 )
        { 
        	errno = r; 
	        return;
        }
     
        // ミューテックスをアンロックする
        r = pthread_mutex_unlock( &mMutex );
        if ( r != 0 )
        { 
        	errno = r; 
	        return;
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void EventFlag::setFalse()
    {
        pthread_mutex_lock( &mMutex );
        mSignal = false;
        pthread_mutex_unlock( &mMutex );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool EventFlag::isTrue()
    {
        pthread_mutex_lock( &mMutex );
        bool b = mSignal;
        pthread_mutex_unlock( &mMutex );
        return b;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void EventFlag::wait()
    {
        // ミューテックスをロックする
        int r = pthread_mutex_lock( &mMutex );
        if ( r != 0 ) { 
	errno = r; 
	return; }
        
        // true ならアンロックして中断してみる
        if ( mSignal )
        {
            pthread_mutex_unlock( &mMutex );
            return;
        }
        
        // スレッドをスリープさせる
        r = pthread_cond_wait( &mWait, &mMutex );
        if ( r != 0 ) { 
        	errno = r; 
	return; }
        
        // アンロック
        pthread_mutex_unlock( &mMutex );
        if ( r != 0 ) { 
        	errno = r; 
	return; }
    }
#endif

} // namespace Threading
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================