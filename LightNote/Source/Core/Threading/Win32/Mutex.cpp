//==============================================================================
// Mutex 
//==============================================================================

#include "stdafx.h"
#include "../Mutex.h"

namespace LNote
{
namespace Core
{
namespace Threading
{

//==============================================================================
// ■ Mutex (win32)
//==============================================================================
#if defined(LNOTE_WIN32)
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Mutex::Mutex()
	{
#ifdef LN_MUTEX_USE_CRITICAL_SECTION
        ::InitializeCriticalSection( &mCriticalSection );
#else
		mHandle = ::CreateSemaphore( NULL, 1, 1, NULL );
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Mutex::~Mutex()
	{
#ifdef LN_MUTEX_USE_CRITICAL_SECTION
		::DeleteCriticalSection( &mCriticalSection );
#else
		if ( mHandle )
		{
			::CloseHandle( mHandle );
		}
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Mutex::lock()
	{
#ifdef LN_MUTEX_USE_CRITICAL_SECTION
		::EnterCriticalSection( &mCriticalSection );
#else
		if ( mHandle )
		{
#if LNOTE_ENABLE_THREAD_DEBUG
			if (::WaitForSingleObject( mHandle, 10000 ) != WAIT_OBJECT_0)
			{
				printf("Mutex::lock() timeout");
				*reinterpret_cast< int* >( 0 ) = 0;
			}
#else
			::WaitForSingleObject( mHandle, INFINITE );
#endif
		}
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Mutex::unlock()
	{
#ifdef LN_MUTEX_USE_CRITICAL_SECTION
		::LeaveCriticalSection( &mCriticalSection );
#else
		if ( mHandle )
		{
			LONG prev;
			::ReleaseSemaphore( mHandle, 1, &prev );

			//LN_ASSERT_S_EXP_( ( prev != 0 ), LNException::Thread, "< Core::Threading::Mutex::unlock >\n\nReleaseSemaphore\nunlock() が複数回呼ばれた可能性があります。" );
		}
#endif
	}

//==============================================================================
// ■ Mutex (pthread)
//==============================================================================
#else
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Mutex::Mutex()
    {
        pthread_mutex_init( &mMutex, 0 );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Mutex::~Mutex()
    {
        pthread_mutex_destroy( &mMutex );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Mutex::lock()
    {
        pthread_mutex_lock( &mMutex );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Mutex::unlock()
    {
        pthread_mutex_unlock( &mMutex );
    }
    
#endif

} // namespace Threading
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================