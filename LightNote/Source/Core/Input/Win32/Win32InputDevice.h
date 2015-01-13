//==============================================================================
// Win32InputDevice 
//------------------------------------------------------------------------------
///**
//  @file       Win32InputDevice.h
//  @brief      Win32InputDevice
//  @version    1.0
//  @date       2012/2/16
//*/
//==============================================================================

#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include "XInputModule.h"
#include <dinput.h>
#include "../InputDeviceBase.h"

namespace LNote
{
namespace Core
{
namespace Input
{
class JoypadDevice;

//==============================================================================
// ■ Win32InputDevice
//------------------------------------------------------------------------------
///**
// @brief      DirectInput および XInput
//*/
//==============================================================================
class Win32InputDevice
    : public InputDeviceBase
{
public:

	/// initialize() に渡す初期化データ
	struct ConfigData
	{
        System::Window*		Window;     ///< DirectInput の初期化に使うウィンドウ
	};

public:
	Win32InputDevice();
	virtual ~Win32InputDevice();

public:

	/// 初期化
	void initialize( const ConfigData& configData );

public:
    virtual int getJoypadNum() { return mJoypadNum; }
    virtual const LNMouseDeviceState& getMouseState() { return mMouseState; }
	//virtual bool getKeyState( LNVirtualKeyCode vkey );
	//virtual LNVirtualKeyCode getKeyBufferState();
	virtual void getJoypadState( int joypadNumber, LNJoypadDeviceState* state );
	virtual void startVibration( int joypadNumber, int power, int time );
	virtual void stopVibration( int joypadNumber );
    virtual void getTouchState( int joypadNumber, LNTouchDeviceState* state ) { state->Touch = false; }
	virtual float getVirtualButtonState( lnU32 button, bool keyboard, bool mouse, int joypadNumber );
	virtual void setMousePoint( const LVector2& point );
    virtual void setMouseCursorCenter();
	virtual void update();
	virtual void refresh();
    virtual System::Window* getWindow() { return mWindow; }
	//virtual bool onEvent( const System::EventArgs& e );

private:
    void _release();
    void _releaseJoypads();
    void _initMouse();

private:

	/// ジョイスティックを列挙するコールバック関数
	static BOOL CALLBACK _enumJoysticksCallback( const DIDEVICEINSTANCE* instance, VOID* thisInputDevice );

	/// ジョイスティックのデバイスオブジェクトを作成する
	void _createJoysticksDevice( const DIDEVICEINSTANCE* instance );

	/// Xbox コントローラかを判定する
    static BOOL _isXInputDevice( const GUID* pGuidProductFromDirectInput );


private:

    System::Window*				mWindow;
    HWND                        mWindowHandle;
    IDirectInput8*              mDirectInput;
    XInputModule*               mXInputModule;


    IDirectInputDevice8*	    mMouseDevice;				///< マウスデバイス
	LNMouseDeviceState	        mMouseState;				///< マウスの状態

    JoypadDevice*               mJoypads[ LN_MAX_JOYPADS ];
    int                         mJoypadNum;
    int                         mXInputNo;

    
    bool                        mCoInited;
};

} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================