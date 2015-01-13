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

#include "../Common.h"
#include "XInputModule.h"

namespace LNote
{
namespace Core
{
namespace Input
{

//==============================================================================
// JoypadDevice
//------------------------------------------------------------------------------
/**
// @brief      ジョイパッドを扱うクラス
//*/
//==============================================================================
class JoypadDevice
{
public:

	/// スティックの状態を表す構造体
	struct AnalogState
	{
		lnS32	X1;
		lnS32	Y1;
		lnS32	X2;
		lnS32	Y2;
	};

public:
	JoypadDevice();
	~JoypadDevice();

public:

	/// 初期化 (xinputNumber は XInput 用コントローラの割り当て番号。XInput 以外のものは -1 を渡すこと)
	void initialize( IDirectInputDevice8* device, HWND hwnd, int xinputNumber, XInputModule* xinput, bool forcefeedback );

	/// 解放
	void dispose();

	/// ボタン状態の取得
    bool getButtonState( lnU32 button ) const { return mButtonState[button] != 0; }

	/// ボタン状態の取得
	void getButtonState( lnU8* buf ) const;

	/// 方向コントローラの状態の取得
	lnS32 getPOVState() const { return mPOVState; }

	/// 軸の状態の取得 (MAX_AXIS_NUM 個)
	void getAxisState( float* axes ) const;

	/// 軸の状態の取得
    float getAxisState( lnU32 axis ) const { return mAxisState[axis]; }

	/// XInput デバイスかを判定する
    bool isXInputDevice() const { return mXInputNo >= 0; }

	/// ジョイパッドの振動を開始する
    void startVibration( int power, int time );

	/// ジョイパッドの振動を停止する
    void stopVibration();

	/// フレーム更新
	void update();

private:

	/// ジョイスティックの軸を列挙する関数
	static BOOL CALLBACK _enumAxesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID thisDevice );

public:

	static const int AXIS_RANGE     = 1000;		///< 軸の範囲 ( -1000～1000 )
    static const int MAX_BUTTON_NUM = LN_MAX_JOYPAD_BUTTONS;	///< ボタンの数
	static const int MAX_AXIS_NUM   = LN_MAX_JOYPAD_AXIS;	    ///< 軸の数

private:

    Base::LogFile*          mLogFile;
    XInputModule*           mXInputModule;
    IDirectInputDevice8*	mDevice;

    IDirectInputEffect*     mDeviceEffect;
    DWORD                   mDIAxes[2];
    LONG                    mDIDirection[2];
    DIPERIODIC              mDIPeriodic;
    DIENVELOPE              mDIEnvelope;
    DIEFFECT                mDIEffect;
    lnU32                   mVibrationStartTime;
    lnU32                   mVibrationTime;

    float                   mPlayRange;

	lnU8					mButtonState[ MAX_BUTTON_NUM ];	///< ボタンの状態
	lnU32					mPOVState;						///< POV 状態 (POVDir の組み合わせ)
	float	                mAxisState[ MAX_AXIS_NUM ];		///< 軸の状態
    int                     mXInputNo;
};

} // namespace Input
} // namespace Core
} // namespace LNote
