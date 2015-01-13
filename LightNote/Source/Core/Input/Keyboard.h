//==============================================================================
// Keyboard 
//------------------------------------------------------------------------------
///**
//  @file       Keyboard.h
//  @brief      Keyboard
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "InputDeviceBase.h"

namespace LNote
{
namespace Core
{
namespace Input
{

//==============================================================================
// ■ Keyboard クラス
//------------------------------------------------------------------------------
///**
//  @brief      キーボードからの入力を扱うクラス
//*/
//==============================================================================
class Keyboard
    : public Base::ReferenceObject
{
public:
	Keyboard( Manager* manager );
	virtual ~Keyboard() {};

public:
	
	/// キーが現在押されているかを判定する
	bool isPress( LNVirtualKeyCode vkey );

	/// キーが新しく押された瞬間を判定する
	bool isOnTrigger( LNVirtualKeyCode vkey );

	/// キーが離された瞬間を判定する
	bool isOffTrigger( LNVirtualKeyCode vkey );

	/// ボタンが新しく押された瞬間とリピート状態を判定する
	bool isRepeat( LNVirtualKeyCode vkey );

	/// キーボードのバッファリングデータの取得
    LNVirtualKeyCode getKeyBufferState() { return mInputDevice->getKeyBufferState(); }

	/// リピート間隔の設定
	void setRepeatInterval( lnU32 first, lnU32 interval );

private:

	/// フレーム更新 ( Manager から呼ばれる )
	void update();

private:

	Manager*			mManager;					///< 管理クラス
    InputDeviceBase*	mInputDevice;
	lnS8				mKeyStatus[ LN_MAX_KEYS ];  ///< キーの状態
	lnS8				mFirstRepeatInterval;		///< リピート開始までの待ちフレーム数
	lnS8				mRemRepeatInterval;			///< リピート中の1回の待ちフレーム数

	friend class Manager;
};

} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================