//==============================================================================
// ReadWriteMutex 
//==============================================================================

#include "stdafx.h"
#include "../ReadWriteMutex.h"

namespace LNote
{
namespace Core
{
namespace Threading
{

//==============================================================================
// ■ ReadWriteMutex (win32)
//==============================================================================
#if defined(LNOTE_WIN32)
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ReadWriteMutex::ReadWriteMutex()
        : mNoReaders    ( NULL )
        , mReaderCount  ( 0 )
    {
        ::InitializeCriticalSection( &mReaderCountLock );
        ::InitializeCriticalSection( &mWriterLock );
        
        mNoReaders = ::CreateEvent( NULL, TRUE, TRUE, NULL );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ReadWriteMutex::~ReadWriteMutex()
    {
        if ( mNoReaders )
        {
            ::CloseHandle( mNoReaders );
            mNoReaders = NULL;
        }
        ::DeleteCriticalSection( &mWriterLock );
        ::DeleteCriticalSection( &mReaderCountLock );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ReadWriteMutex::lockRead()
    {
        ::EnterCriticalSection( &mWriterLock );
        ::EnterCriticalSection( &mReaderCountLock );
        if ( ++mReaderCount == 1 )
        {
            ::ResetEvent( mNoReaders );
        }
        ::LeaveCriticalSection( &mReaderCountLock );
        ::LeaveCriticalSection( &mWriterLock );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ReadWriteMutex::unlockRead()
    {
        ::EnterCriticalSection( &mReaderCountLock );
        //assert( mReaderCount > 0 );
        if ( --mReaderCount == 0 )
        {
            ::SetEvent( mNoReaders );
        }
        ::LeaveCriticalSection( &mReaderCountLock );
    }
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ReadWriteMutex::lockWrite()
    {
        ::EnterCriticalSection( &mWriterLock );
        if ( mReaderCount > 0 )
        {
            ::WaitForSingleObject( mNoReaders, INFINITE );
        }
    }
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ReadWriteMutex::unlockWrite()
    {
        ::LeaveCriticalSection( &mWriterLock );
    }

//==============================================================================
// ■ ReadWriteMutex (pthread)
//==============================================================================
#else
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ReadWriteMutex::ReadWriteMutex()
        : mReaders          ( 0 )
        , mWriters          ( 0 )
        , mReadWaiters      ( 0 )
        , mWriteWaiters     ( 0 )
    {
        pthread_mutex_init( &mLock, NULL );
        pthread_cond_init( &mRead, NULL );
        pthread_cond_init( &mWrite, NULL );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ReadWriteMutex::~ReadWriteMutex()
    {
        pthread_mutex_destroy( &mLock );
        pthread_cond_destroy( &mRead );
        pthread_cond_destroy( &mWrite );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ReadWriteMutex::lockRead()
    {
        pthread_mutex_lock( &mLock );
        if ( mWriters || mWriteWaiters )
        {
            ++mReadWaiters;
            do pthread_cond_wait( &mRead, &mLock );
            while ( mWriters || mWriteWaiters );
            --mReadWaiters;
        }
        ++mReaders;
        pthread_mutex_unlock( &mLock );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ReadWriteMutex::unlockRead()
    {
        pthread_mutex_lock( &mLock );
        --mReaders;
        if ( mWriteWaiters )
        {
            pthread_cond_signal( &mWrite );
        }
        pthread_mutex_unlock( &mLock );
    }
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ReadWriteMutex::lockWrite()
    {
        pthread_mutex_lock( &mLock );
        if ( mReaders || mWriters )
        {
            ++mWriteWaiters;
            do pthread_cond_wait( &mWrite, &mLock );
            while ( mReaders || mWriters );
            --mWriteWaiters;
        }
        mWriters = 1;
        pthread_mutex_unlock( &mLock );
    }
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ReadWriteMutex::unlockWrite()
    {
        pthread_mutex_lock( &mLock );
        mWriters = 0;
        if ( mWriteWaiters )
        {
            pthread_cond_signal( &mWrite );
        }
        else if ( mReadWaiters )
        {
            pthread_cond_broadcast( &mRead );
        }
        pthread_mutex_unlock( &mLock );
    }
    
#endif

} // namespace Threading
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================