//==============================================================================
// Mutex 
//------------------------------------------------------------------------------
///**
//  @file       Mutex.h
//  @brief      Mutex
//  @author     Riki
//*/
//==============================================================================

#pragma once

#define LN_MUTEX_USE_CRITICAL_SECTION

namespace LNote
{
namespace Core
{
namespace Threading
{

//==============================================================================
// ■ Mutex
//------------------------------------------------------------------------------
///**
//  @brief      ミューテックスのクラス
//*/
//------------------------------------------------------------------------------
//	MemoryManager で使っているので、このクラスの中で LN_NEW しないこと。
//==============================================================================
class Mutex
    : public Base::ReferenceObject
{
public:
    Mutex();
    ~Mutex();

public:
	
	/// ロック
    void lock();

	/// アンロック
    void unlock();

private:

#if defined(LNOTE_WIN32)
#ifdef LN_MUTEX_USE_CRITICAL_SECTION
    mutable CRITICAL_SECTION mCriticalSection;
#else
	HANDLE mHandle;
#endif
#else
    pthread_mutex_t mMutex;
#endif
};

//==============================================================================
// ■ ScopedLock
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class ScopedLock  
{
public:

    ScopedLock( Mutex& mutex )
        : mMutex( mutex )
    {
        mMutex.lock();
    }

    ~ScopedLock()
    {
        mMutex.unlock();
    }

private:

    Mutex& mMutex;
};

} // namespace Threading
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================