//==============================================================================
// GLFWInputDevice 
//==============================================================================

#pragma once

#include "stdafx.h"
#include <glfw/include/GLFW/glfw3.h>
#include "../../System/GLFW/GLFWWindow.h"
#include "../../System/Environment.h"
#include "GLFWInputDevice.h"

namespace LNote
{
namespace Core
{
namespace Input
{

//==============================================================================
// ■ GLFWInputDevice
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    GLFWInputDevice::GLFWInputDevice()
        : mWindow				( NULL )
        , mJoypadCount			( 0 )
    {
        //memset( mKeyStatus, 0, sizeof( mKeyStatus ) );
        memset( &mMouseState, 0, sizeof( mMouseState ) );
        memset( mJoysticks, 0, sizeof( mJoysticks ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    GLFWInputDevice::~GLFWInputDevice()
    {
        _release();
        LN_SAFE_RELEASE( mWindow );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWInputDevice::initialize( const ConfigData& configData )
    {
		System::GLFWWindow* window = dynamic_cast<System::GLFWWindow*>( configData.Window );
		LN_ERR2_ASSERT( window );

		LN_REFOBJ_SET( mWindow, window );
        

        refresh();
		update();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GLFWInputDevice::getJoypadState( int joypadNumber, LNJoypadDeviceState* state )
    {
		Joystick* joy = &mJoysticks[joypadNumber];

		memset( state->Axes, 0, sizeof(float) * LN_MAX_JOYPAD_AXIS );
		memset( state->Buttons, 0, sizeof(lnU8) * LN_MAX_JOYPAD_BUTTONS );
        state->POV = 0;

		if ( joy->present )
		{
			for ( int i = 0; i < joy->axis_count; ++i )
			{
				state->Axes[i] = joy->axes[i];
			}
			for ( int i = 0; i < joy->button_count; ++i )
			{
				state->Buttons[i] = joy->buttons[i];
			}
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GLFWInputDevice::startVibration( int joypadNumber, int power, int time )
    {
		// 非対応
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GLFWInputDevice::stopVibration( int joypadNumber )
    {
		// 非対応
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    float GLFWInputDevice::getVirtualButtonState( lnU32 button, bool keyboard, bool mouse, int joypadNumber )
    {
        // キーボード
        if ( keyboard && LN_VBUTTON_KEYBOARD_BEGIN <= button && button <= LN_VBUTTON_KEYBOARD_END )
        {
            return ( getKeyState( (LNVirtualKeyCode)button ) ) ? 1.0f : 0.0f;
        }

        // マウス
        if ( mouse && LN_VBUTTON_MOUSE_BEGIN <= button && button <= LN_VBUTTON_MOUSE_END )
        {
            return ( mMouseState.Buttons[button - LN_VBUTTON_MOUSE_BEGIN] ) ? 1.0f : 0.0f;
        }

        // ジョイパッド
        if ( joypadNumber >= 0 && joypadNumber < mJoypadCount )
        {
			Joystick* joy = &mJoysticks[joypadNumber];
            // ボタン
            if ( LN_VBUTTON_JOYPAD_BUTTON_BEGIN <= button && button <= LN_VBUTTON_JOYPAD_BUTTON_END )
            {
				int buttonNo = button - LN_VBUTTON_JOYPAD_BUTTON_BEGIN;
				if ( buttonNo < joy->button_count ) {
					return (joy->buttons[buttonNo]) ? 1.0f : 0.0f;
				}
				else {
					return 0.0f;
				}
            }

            // POV
            if ( LN_VBUTTON_JOYPAD_POV_BEGIN <= button && button <= LN_VBUTTON_JOYPAD_POV_END )
            {
				return 0.0f;
            }

            // 軸
            if ( LN_VBUTTON_JOYPAD_AXIS_BEGIN <= button && button <= LN_VBUTTON_JOYPAD_AXIS_END )
            {
				int axisNo = (button - LN_VBUTTON_JOYPAD_AXIS_BEGIN) / 2;
				if ( axisNo < joy->axis_count ) {
					lnFloat v = joy->axes[axisNo];
					if ( ( button - LN_VBUTTON_JOYPAD_AXIS_BEGIN ) & 1 ) {  // 奇数はプラスかを判定
						return v;
					}
					else {
						return -v;
					}
				}
				else {
					return 0.0f;
				}
            }
        }

        return 0.0f;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GLFWInputDevice::setMousePoint( const LVector2& point )
    {
      //  if ( mWindow->isActive() )
      //  {
      //      POINT pt;
		    //pt.x = static_cast<LONG>( point.x );
		    //pt.y = static_cast<LONG>( point.y );
		    //::ClientToScreen( mWindowHandle, &pt );
		    //::SetCursorPos( pt.x, pt.y );
      //  }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GLFWInputDevice::setMouseCursorCenter()
    {
        if ( mWindow->isActive() )
        {
            const Geometry::Size& size = mWindow->getSize();
            setMousePoint(
                LVector2(
                    static_cast<lnFloat>( size.w / 2 ),
                    static_cast<lnFloat>( size.h / 2 ) ) );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GLFWInputDevice::update()
    {
		GLFWwindow* window = mWindow->getGLFWWindow();

		// キーをポーリングでとるときは glfwGetKey()

		//-----------------------------------------------------
        // マウス
		double mouseX = 0.0;
		double mouseY = 0.0;
		glfwGetCursorPos( window, &mouseX, &mouseY );
		mMouseState.X = (long)mouseX;
		mMouseState.Y = (long)mouseY;
		mMouseState.RelX = mMouseState.X - mPrevMousePoint.x;
		mMouseState.RelX = mMouseState.Y - mPrevMousePoint.y;
		mMouseState.Wheel = 0;
		mMouseState.Succeeded = true;

		for ( int i = 0 ; i < GLFW_MOUSE_BUTTON_LAST && i < LN_MAX_MOUSE_BUTTONS; ++i )
		{
			mMouseState.Buttons[i] = (glfwGetMouseButton( window, i ) == GLFW_PRESS);
		}
		
		//-----------------------------------------------------
        // ジョイスティック
#if 1	// refresh() 併用
		for ( int i = 0; i < mJoypadCount; ++i )
		{
			Joystick* joy = &mJoysticks[i];
			if ( glfwJoystickPresent( joy->DeviceIndex ) )
			{
				// 軸
				int axis_count;
				const float* axes = glfwGetJoystickAxes( joy->DeviceIndex, &axis_count );
				if ( axis_count != joy->axis_count ) {
					joy->axis_count = axis_count;
					SAFE_DELETE_ARRAY( joy->axes );
					joy->axes = LN_NEW float[joy->axis_count];
				}
				memcpy( joy->axes, axes, joy->axis_count * sizeof(float) );

				// ボタン
				int button_count;
				const unsigned char* buttons = glfwGetJoystickButtons( joy->DeviceIndex, &button_count );
				if ( button_count != joy->button_count ) {
					joy->button_count = button_count;
					SAFE_DELETE_ARRAY( joy->buttons );
					joy->buttons = LN_NEW unsigned char[joy->button_count];
				}
				memcpy( joy->buttons, buttons, joy->button_count * sizeof(unsigned char) );
				
				joy->present = GL_TRUE;
			}
			else
			{
				memcpy( joy->axes, 0, joy->axis_count * sizeof(float) );
				memset( joy->buttons, 0, joy->button_count * sizeof(unsigned char) );
				joy->present = GL_FALSE;
			}
		}
#else
		int validCount = 0;
		for ( int i = 0; i < LN_MAX_JOYPADS; ++i )
		{
			Joystick* joy = &mJoysticks[validCount];
			if ( glfwJoystickPresent( GLFW_JOYSTICK_1 + i ) )
			{
				// 軸
				int axis_count;
				const float* axes = glfwGetJoystickAxes( GLFW_JOYSTICK_1 + i, &axis_count );
				if ( axis_count != joy->axis_count ) {
					joy->axis_count = axis_count;
					SAFE_DELETE_ARRAY( joy->axes );
					joy->axes = LN_NEW float[joy->axis_count];
				}
				memcpy( joy->axes, axes, joy->axis_count * sizeof(float) );

				// ボタン
				int button_count;
				const unsigned char* buttons = glfwGetJoystickButtons( GLFW_JOYSTICK_1 + i, &button_count );
				if ( button_count != joy->button_count ) {
					joy->button_count = button_count;
					SAFE_DELETE_ARRAY( joy->buttons );
					joy->buttons = LN_NEW unsigned char[joy->button_count];
				}
				memcpy( joy->buttons, buttons, joy->button_count * sizeof(unsigned char) );

				joy->present = GL_TRUE;
				joy->DeviceIndex = i;

				++validCount;
			}
			else
			{
				SAFE_DELETE_ARRAY( joy->axes );
				SAFE_DELETE_ARRAY( joy->buttons );
				memset( joy, 0, sizeof(Joystick) );
			}

		}
		mJoypadCount = validCount;
#endif
		
#if 0
        HRESULT hr;

        //-----------------------------------------------------
        // マウス

        DIMOUSESTATE2 dims;
        hr = mMouseDevice->GetDeviceState( sizeof( DIMOUSESTATE2 ), &dims );

        if ( SUCCEEDED( hr ) )
        {
            POINT pt;
		    ::GetCursorPos( &pt );
		    ::ScreenToClient( mWindowHandle, &pt );

		    mMouseState.X = pt.x;
		    mMouseState.Y = pt.y;
		    mMouseState.RelX = dims.lX;
		    mMouseState.RelY = dims.lY;
		    mMouseState.Wheel = dims.lZ / WHEEL_DELTA;
		    mMouseState.Succeeded = true;
		    for ( int i = 0; i < LN_MAX_MOUSE_BUTTONS; ++i )
		    {
			    mMouseState.Buttons[ i ] = ( dims.rgbButtons[ i ] & 0x80 ) != 0;
		    }
            /*
            printf( "X:%d Y:%d RX:%d RY:%d W:%d BT:%d %d %d %d\n", 
                        mMouseState.X,
				        mMouseState.Y,
				        mMouseState.RelX,
				        mMouseState.RelY,
				        mMouseState.Wheel,
                        mMouseState.Buttons[ 0 ],
                        mMouseState.Buttons[ 1 ],
                        mMouseState.Buttons[ 2 ],
                        mMouseState.Buttons[ 3 ] );
            */
        }
        else
        {
            mMouseDevice->Acquire();
            memset( &mMouseState, 0, sizeof( mMouseState ) );
        }

        //-----------------------------------------------------
        // ジョイパッド

        for ( int i = 0; i < mJoypadNum; ++i )
		{
			mJoypads[ i ]->update();
		}
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GLFWInputDevice::refresh()
    {
#if 1	// refresh() 併用
		_release();

		for ( int i = 0; i < LN_MAX_JOYPADS; ++i )
		{
			Joystick* joy = &mJoysticks[mJoypadCount];
			if ( glfwJoystickPresent( GLFW_JOYSTICK_1 + i ) )
			{
				const float* axes = glfwGetJoystickAxes( GLFW_JOYSTICK_1 + i, &joy->axis_count );
				joy->axes = LN_NEW float[joy->axis_count];
				memcpy( joy->axes, axes, joy->axis_count * sizeof(float) );

				const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1 + i, &joy->button_count);
				joy->buttons = LN_NEW unsigned char[joy->button_count];
				memcpy( joy->buttons, buttons, joy->button_count * sizeof(unsigned char) );

				joy->present = GL_TRUE;
				joy->DeviceIndex = i;

				++mJoypadCount;
			}
		}
#else
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWInputDevice::_release()
	{
		for ( int i = 0; i < LN_MAX_JOYPADS; ++i )
		{
			Joystick* joy = &mJoysticks[i];
			SAFE_DELETE_ARRAY( joy->axes );
			SAFE_DELETE_ARRAY( joy->buttons );
			memset( joy, 0, sizeof(Joystick) );
		}
		mJoypadCount = 0;
	}

} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================