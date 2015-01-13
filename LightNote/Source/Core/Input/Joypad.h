//==============================================================================
// Joypad 
//------------------------------------------------------------------------------
///**
//  @file       Joypad.h
//  @brief      Joypad
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
// ■ Joypad
//------------------------------------------------------------------------------
///**
//  @brief      ジョイパッドからの入力を扱うクラス
//*/
//==============================================================================
class Joypad
    : public Base::ReferenceObject
{
public:
	Joypad( Manager* manager, int joypad_id );
	virtual ~Joypad() {};

public:

	/// 接続済みかをチェックする
    virtual bool isConnected() { return ( mJoypadID >= 0 ); }

	/// ボタンが現在押されているかを判定する (0 ～ LN_MAX_JOYPAD_BUTTONS - 1)
	virtual bool isPress( int button );

	/// ボタンが新しく押された瞬間を判定する (0 ～ LN_MAX_JOYPAD_BUTTONS - 1)
	virtual bool isOnTrigger( int button );

	/// ボタンが離された瞬間を判定する (0 ～ LN_MAX_JOYPAD_BUTTONS - 1)
	virtual bool isOffTrigger( int button );

	/// ボタンが新しく押された瞬間とリピート状態を判定する (0 ～ LN_MAX_JOYPAD_BUTTONS - 1)
	virtual bool isRepeat( int button );

	/// 軸の状態の取得 (0 ～ LN_MAX_JOYPAD_AXIS - 1)
    virtual float getAxisState( int axis );

	/// POV の状態の取得
	virtual lnU32 getPOVState() { return mPOVState; }

	/// ジョイパッドの振動を開始する
    virtual void startVibration( int power, int time ) { mInputDevice->startVibration( mJoypadID, power, time ); }

	/// ジョイパッドの振動を停止する
	virtual void stopVibration() { mInputDevice->stopVibration( mJoypadID ); }

	/// リピート間隔の設定 ( Manager から呼ばれる )
	virtual void setRepeatInterval( lnU32 first, lnU32 interval );

private:

	/// フレーム更新 ( Manager から呼ばれる )
	void update();

private:

	Manager*			mManager;							    ///< 管理クラス
    InputDeviceBase*	mInputDevice;
	int					mJoypadID;							    ///< 割り当てられているジョイパッドの番号
	lnS32		        mButtonStatus[ LN_MAX_JOYPAD_BUTTONS ]; ///< ボタンの状態
	float				mAxisStatus[ LN_MAX_JOYPAD_AXIS ];	    ///< 軸の状態
	lnU32		        mPOVState;							    ///< POV の状態
	lnS32		        mFirstRepeatInterval;				    ///< リピート開始までの待ちフレーム数
	lnS32		        mRemRepeatInterval;					    ///< リピート中の1回の待ちフレーム数

	friend class Manager;
};

} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================