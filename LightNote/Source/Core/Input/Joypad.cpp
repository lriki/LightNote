//==============================================================================
// Joypad 
//==============================================================================

#include "stdafx.h"
#include "Manager.h"
#include "Joypad.h"

namespace LNote
{
namespace Core
{
namespace Input
{

//==============================================================================
// ■ Joypad
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Joypad::Joypad( Manager* manager, int joypad_id )
		: mManager				( manager )
        , mInputDevice          ( manager->getInputDevice() )
		, mJoypadID				( joypad_id )
		, mPOVState				( 0 )
        , mFirstRepeatInterval	( 20 )
		, mRemRepeatInterval	( 5 )
	{
		memset( mButtonStatus, 0, sizeof( mButtonStatus ) );
		memset( mAxisStatus, 0, sizeof( mAxisStatus ) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Joypad::isPress( int button )
	{
        if ( button < 0 || LN_MAX_JOYPAD_BUTTONS <= button ) { 
			return false;
		}
		return ( mButtonStatus[ button ] > 0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Joypad::isOnTrigger( int button )
	{
		if ( button < 0 || LN_MAX_JOYPAD_BUTTONS <= button ) { 
			return false;
		}
		return ( mButtonStatus[ button ] == 1 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Joypad::isOffTrigger( int button )
	{
		if ( button < 0 || LN_MAX_JOYPAD_BUTTONS <= button ) { 
			return false;
		}
		return ( mButtonStatus[ button ] == -1 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Joypad::isRepeat( int button )
	{
		if ( button < 0 || LN_MAX_JOYPAD_BUTTONS <= button ) { 
			return false;
		}
		int state = mButtonStatus[ button ];
		return ( ( state == 1 )  ||  ( state > mFirstRepeatInterval && state % mRemRepeatInterval == 0 ) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    float Joypad::getAxisState( int axis )
	{
        if ( axis < 0 || LN_MAX_JOYPAD_AXIS <= axis ) { 
			return 0.0f;
		}
		return mAxisStatus[ axis ];
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Joypad::setRepeatInterval( lnU32 first, lnU32 interval )
	{
		mFirstRepeatInterval = first;
		mRemRepeatInterval = interval;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Joypad::update()
	{
		if ( mJoypadID >= mManager->getInputDevice()->getJoypadNum() ) { 
			return; 
		}

        LNJoypadDeviceState state;
        mManager->getInputDevice()->getJoypadState( mJoypadID, &state );

		// ボタン状態の記憶
		for ( int i = 0; i < LN_MAX_JOYPAD_BUTTONS; ++i )
		{
			if ( state.Buttons[ i ] )
			{
				++mButtonStatus[ i ];

				if ( mButtonStatus[ i ] > mRemRepeatInterval + mFirstRepeatInterval )
				{
					mButtonStatus[ i ] = mFirstRepeatInterval;
				}
			}
			else
			{
				if ( mButtonStatus[ i ] > 0 )
				{
					mButtonStatus[ i ] = -1;
				}
				else
				{
					mButtonStatus[ i ] = 0;
				}
			}
		}

		// 軸状態の記憶
		for ( int i = 0; i < LN_MAX_JOYPAD_AXIS; ++i )
		{
			mAxisStatus[ i ] = state.Axes[ i ];
		}

		// POV
		mPOVState = state.POV;
	}

} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================