//==============================================================================
// Keyboard 
//==============================================================================

#include "stdafx.h"
#include "Manager.h"
#include "Keyboard.h"

namespace LNote
{
namespace Core
{
namespace Input
{

//==============================================================================
// ■ Keyboard
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Keyboard::Keyboard( Manager* manager )
		: mManager              ( manager )
        , mInputDevice          ( manager->getInputDevice() )
		, mFirstRepeatInterval  ( 20 )
		, mRemRepeatInterval    ( 5 )
	{
		memset( mKeyStatus, 0, sizeof( mKeyStatus ) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Keyboard::isPress( LNVirtualKeyCode vkey )
	{
        if ( vkey < 0 || vkey >= LN_MAX_KEYS ) { 
			return false;
		}
		return ( mKeyStatus[ vkey ] > 0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Keyboard::isOnTrigger( LNVirtualKeyCode vkey )
	{
		if ( vkey < 0 || vkey >= LN_MAX_KEYS ) { 
			return false;
		}
		return ( mKeyStatus[ vkey ] == 1 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Keyboard::isOffTrigger( LNVirtualKeyCode vkey )
	{
		if ( vkey < 0 || vkey >= LN_MAX_KEYS ) { 
			return false;
		}
		return ( mKeyStatus[ vkey ] == -1 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Keyboard::isRepeat( LNVirtualKeyCode vkey )
	{
		if ( vkey < 0 || vkey >= LN_MAX_KEYS ) { 
			return false;
		}
		int state = mKeyStatus[ vkey ];
		return ( ( state == 1 )  ||  ( state > mFirstRepeatInterval && state % mRemRepeatInterval == 0 ) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Keyboard::setRepeatInterval( lnU32 first, lnU32 interval )
	{
		mFirstRepeatInterval = static_cast<lnS8>( first );
		mRemRepeatInterval   = static_cast<lnS8>( interval );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Keyboard::update()
	{
		for ( int i = 0; i < LN_MAX_KEYS; ++i )
		{
			// が押されている場合
			if ( mInputDevice->getKeyState( (LNVirtualKeyCode)i ) )
			{
				// 値を増やしていく
				++mKeyStatus[ i ];

				if ( mKeyStatus[ i ] > mRemRepeatInterval + mFirstRepeatInterval )
				{
					mKeyStatus[ i ] = mFirstRepeatInterval;
				}
			}
			// 押されていない場合、直前まで押されていた場合は -1、それ以外は 0 を設定する
			else
			{
				if ( mKeyStatus[ i ] > 0 )
				{
					mKeyStatus[ i ] = -1;
				}
				else
				{
					mKeyStatus[ i ] = 0;
				}
			}
        }
	}

} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================