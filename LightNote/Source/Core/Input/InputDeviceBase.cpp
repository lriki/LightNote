//==============================================================================
// InputDeviceBase 
//==============================================================================

#include "stdafx.h"
#include "Manager.h"
#include "InputDeviceBase.h"

namespace LNote
{
namespace Core
{
namespace Input
{

//==============================================================================
// Å° InputDeviceBase
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	InputDeviceBase::InputDeviceBase()
		: mEnableKeyBuffering	( false )
	{
        memset( mKeyStatus, 0, sizeof( mKeyStatus ) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	InputDeviceBase::~InputDeviceBase()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool InputDeviceBase::getKeyState( LNVirtualKeyCode vkey )
    {
        return mKeyStatus[vkey];
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    LNVirtualKeyCode InputDeviceBase::getKeyBufferState()
    {
        if ( !mKeyBuffer.empty() )
        {
            LNVirtualKeyCode key = mKeyBuffer.front();
            mKeyBuffer.pop_front();
            return key;
        }
        /*
        lnU32 key = 0;

	    if ( mKeyBufferCount < mKeyBufferMax )
	    {
            do {
                if ( mKeyBuffer[ mKeyBufferCount ].dwData & 0x80 )
		        {
			        key = mKeyBuffer[ mKeyBufferCount ].dwOfs;
		        }
                mKeyBufferCount++;

            } while ( key == 0 && mKeyBufferCount < mKeyBufferMax );
 
	    }

	    return gVirtualKeyTableDItoLN[ key ];
        */
        return LN_KEY_UNKNOWN;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool InputDeviceBase::onEvent( const System::EventArgs& e )
	{
        const System::KeyEventArgs& ev_key = (const System::KeyEventArgs&)(e);

        switch ( e.Type )
        {
            case LN_EVENT_KEY_DOWN:
                if ( !mKeyStatus[ev_key.KeyCode] )
                {
                    mKeyStatus[ev_key.KeyCode] = true;
					if (mEnableKeyBuffering)
					{
						mKeyBuffer.push_back((LNVirtualKeyCode)ev_key.KeyCode);
					}
                }
                break;

            case LN_EVENT_KEY_UP:
                mKeyStatus[ev_key.KeyCode] = false;
                break;
        }
        return false;
	}

} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================