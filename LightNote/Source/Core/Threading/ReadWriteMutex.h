﻿//==============================================================================
// ReadWriteMutex 
//------------------------------------------------------------------------------
///**
//  @file       ReadWriteMutex.h
//  @brief      ReadWriteMutex
//  @author     Riki
//*/
//==============================================================================

#pragma once

namespace LNote
{
namespace Core
{
namespace Threading
{

//==============================================================================
// ■ ReadWriteMutex
//------------------------------------------------------------------------------
///**
//  @brief      Read/Write Lock 用ミューテックスのクラス
//*/
//==============================================================================
class ReadWriteMutex
    : public Base::ReferenceObject
{
public:

	/// コンストラクタ
    ReadWriteMutex();

	/// デストラクタ
    ~ReadWriteMutex();

public:

	//----------------------------------------------------------------------
	///**
	//  @brief      読み取りロック
	//*/
	//----------------------------------------------------------------------
    void lockRead();
    
	//----------------------------------------------------------------------
	///**
	//  @brief      読み取りアンロック
	//*/
	//----------------------------------------------------------------------
    void unlockRead();
    
	//----------------------------------------------------------------------
	///**
	//  @brief      書き込みロック
	//*/
	//----------------------------------------------------------------------
    void lockWrite();
    
	//----------------------------------------------------------------------
	///**
	//  @brief      書き込みアンロック
	//*/
	//----------------------------------------------------------------------
    void unlockWrite();

private:

#if defined(LNOTE_WIN32)
    CRITICAL_SECTION    mReaderCountLock;
    CRITICAL_SECTION    mWriterLock;
    HANDLE              mNoReaders;
    int                 mReaderCount;
#else
    pthread_mutex_t     mLock;
    pthread_cond_t      mRead;
    pthread_cond_t      mWrite;
    unsigned            mReaders;
    unsigned            mWriters;
    unsigned            mReadWaiters;
    unsigned            mWriteWaiters;
#endif
};

} // namespace Threading
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================