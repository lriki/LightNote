//==============================================================================
// GLFWInputDevice 
//------------------------------------------------------------------------------
///**
//  @file       GLFWInputDevice.h
//  @brief      GLFWInputDevice
//  @version    1.0
//  @date       2012/2/16
//*/
//==============================================================================

#pragma once

#include <deque>
#include "../InputDeviceBase.h"

namespace LNote
{
namespace Core
{
namespace Input
{

//==============================================================================
// ■ GLFWInputDevice
//------------------------------------------------------------------------------
///**
// @brief
//*/
//==============================================================================
class GLFWInputDevice
    : public InputDeviceBase
{
public:

	/// initialize() に渡す初期化データ
	struct ConfigData
	{
        System::Window*		Window;     ///< DirectInput の初期化に使うウィンドウ
	};

public:
	GLFWInputDevice();
	virtual ~GLFWInputDevice();

public:

	/// 初期化
	void initialize( const ConfigData& configData );

public:
    virtual int getJoypadNum() { return mJoypadCount; }
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

private:
	typedef struct Joystick
	{
		GLboolean		present;
		//char*			name;
		float*			axes;
		unsigned char*	buttons;
		int				axis_count;
		int				button_count;
		int				DeviceIndex;
	} Joystick;

private:
    typedef std::deque< LNVirtualKeyCode >     KeyBuffer;

    System::GLFWWindow*		mWindow;
	LNMouseDeviceState		mMouseState;
	LPoint					mPrevMousePoint;

	Joystick				mJoysticks[LN_MAX_JOYPADS];
	//JoypadDevice*               mJoypads[LN_MAX_JOYPADS];
    int						mJoypadCount;
};

} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================