﻿//==============================================================================
// Atomic 
//------------------------------------------------------------------------------
///**
//  @file       Atomic.h
//  @brief      Atomic
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
// Atomic
//------------------------------------------------------------------------------
///**
// *	@brief		スレッドセーフなインクリメント・デクリメントを行う整数変数のクラス
// */
//==============================================================================
class Atomic
    : private Base::NonCopyable    // 今のところ NonCopyable
{
public:
    
	//----------------------------------------------------------------------
	///**
	//  @brief      コンストラクタ
	//  @param[in]  value : 初期値
	//*/
	//----------------------------------------------------------------------
	Atomic( lnS32 value = 0 );

	/// デストラクタ
	~Atomic();

public:

	//----------------------------------------------------------------------
	///**
	//  @brief      値を取得する
	//*/
	//----------------------------------------------------------------------
	lnS32 get() const;

	//----------------------------------------------------------------------
	///**
	//  @brief      値を設定する
	//*/
	//----------------------------------------------------------------------
    void set( lnS32 value );

	//----------------------------------------------------------------------
	///**
	//  @brief      値をインクリメントする
	//  @return     インクリメント後の値
	//*/
	//----------------------------------------------------------------------
    lnS32 increment();

	//----------------------------------------------------------------------
	///**
	//  @brief      値をデクリメントする
	//  @return     デクリメント後の値
	//*/
	//----------------------------------------------------------------------
    lnS32 decrement();

private:
#if defined(LNOTE_MSVC)
	volatile LONG			mValue;
#elif defined(LNOTE_GNUC)
	volatile Int32			mValue;
	mutable pthread_mutex_t	mMutex;
#endif
};

} // namespace Threading
} // namespace Core
} // namespace LNote
