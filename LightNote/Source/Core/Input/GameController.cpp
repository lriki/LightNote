//==============================================================================
// GameController 
//==============================================================================

#include "stdafx.h"
#include <algorithm>
#include "Manager.h"
#include "GameController.h"

namespace LNote
{
namespace Core
{
namespace Input
{

//==============================================================================
// GameController
//==============================================================================

	LN_TYPE_INFO_ACCESS_IMPL(GameController);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    GameController::GameController( Manager* manager, int joypad_id )
        : mManager              ( manager )
        , mInputDevice          ( manager->getInputDevice() )
        , mDeviceAttachFlag     ( LN_INPUTATTACH_ALL )
        , mJoypadID             ( joypad_id )
        , mFirstRepeatInterval	( 20 )
		, mRemRepeatInterval	( 5 )
        , mPressedTriggerFlag   ( false )
    {
        memset( mButtonStatus, 0, sizeof( mButtonStatus ) );
        memset( mButtonPower, 0, sizeof( mButtonPower ) );
        memset( mAxisState, 0, sizeof( mAxisState ) );

        mAttachedVButtons[ LN_AT_BUTTON_A ].push_back( LN_KEY_Z );
        mAttachedVButtons[ LN_AT_BUTTON_A ].push_back( LN_VBUTTON_JOYPAD_BUTTON_1 );
	    mAttachedVButtons[ LN_AT_BUTTON_B ].push_back( LN_KEY_X );
        mAttachedVButtons[ LN_AT_BUTTON_B ].push_back( LN_VBUTTON_JOYPAD_BUTTON_2 );
	    mAttachedVButtons[ LN_AT_BUTTON_C ].push_back( LN_KEY_C );
        mAttachedVButtons[ LN_AT_BUTTON_C ].push_back( LN_VBUTTON_JOYPAD_BUTTON_3 );
	    mAttachedVButtons[ LN_AT_BUTTON_X ].push_back( LN_KEY_A );
        mAttachedVButtons[ LN_AT_BUTTON_X ].push_back( LN_VBUTTON_JOYPAD_BUTTON_4 );
	    mAttachedVButtons[ LN_AT_BUTTON_Y ].push_back( LN_KEY_S );
        mAttachedVButtons[ LN_AT_BUTTON_Y ].push_back( LN_VBUTTON_JOYPAD_BUTTON_5 );
	    mAttachedVButtons[ LN_AT_BUTTON_Z ].push_back( LN_KEY_D );
        mAttachedVButtons[ LN_AT_BUTTON_Z ].push_back( LN_VBUTTON_JOYPAD_BUTTON_6 );
	    mAttachedVButtons[ LN_AT_BUTTON_L ].push_back( LN_KEY_Q );
        mAttachedVButtons[ LN_AT_BUTTON_L ].push_back( LN_VBUTTON_JOYPAD_BUTTON_7 );
	    mAttachedVButtons[ LN_AT_BUTTON_R ].push_back( LN_KEY_W );
        mAttachedVButtons[ LN_AT_BUTTON_R ].push_back( LN_VBUTTON_JOYPAD_BUTTON_8 );

	    mAttachedVButtons[ LN_AT_BUTTON_L2 ].push_back( LN_VBUTTON_JOYPAD_BUTTON_9 );
	    mAttachedVButtons[ LN_AT_BUTTON_R2 ].push_back( LN_VBUTTON_JOYPAD_BUTTON_10 );
	    mAttachedVButtons[ LN_AT_BUTTON_L3 ].push_back( LN_VBUTTON_JOYPAD_BUTTON_11 );
	    mAttachedVButtons[ LN_AT_BUTTON_R3 ].push_back( LN_VBUTTON_JOYPAD_BUTTON_12 );
	    mAttachedVButtons[ LN_AT_BUTTON_START ].push_back( LN_VBUTTON_JOYPAD_BUTTON_13 );
	    mAttachedVButtons[ LN_AT_BUTTON_SELECT ].push_back( LN_VBUTTON_JOYPAD_BUTTON_14 );
	    mAttachedVButtons[ LN_AT_BUTTON_SUB1 ].push_back( LN_VBUTTON_JOYPAD_BUTTON_15 );
	    mAttachedVButtons[ LN_AT_BUTTON_SUB2 ].push_back( LN_VBUTTON_JOYPAD_BUTTON_16 );

	    mAttachedVButtons[ LN_AT_BUTTON_DOWN ].push_back( LN_KEY_DOWN );
        mAttachedVButtons[ LN_AT_BUTTON_DOWN ].push_back( LN_VBUTTON_JOYPAD_POV_DOWN );
        mAttachedVButtons[ LN_AT_BUTTON_DOWN ].push_back( LN_VBUTTON_JOYPAD_AXIS_2_PLUS );

	    mAttachedVButtons[ LN_AT_BUTTON_LEFT ].push_back( LN_KEY_LEFT );
        mAttachedVButtons[ LN_AT_BUTTON_LEFT ].push_back( LN_VBUTTON_JOYPAD_POV_LEFT );
        mAttachedVButtons[ LN_AT_BUTTON_LEFT ].push_back( LN_VBUTTON_JOYPAD_AXIS_1_MINUIS );

	    mAttachedVButtons[ LN_AT_BUTTON_RIGHT ].push_back( LN_KEY_RIGHT );
        mAttachedVButtons[ LN_AT_BUTTON_RIGHT ].push_back( LN_VBUTTON_JOYPAD_POV_RIGHT );
        mAttachedVButtons[ LN_AT_BUTTON_RIGHT ].push_back( LN_VBUTTON_JOYPAD_AXIS_1_PLUS );

	    mAttachedVButtons[ LN_AT_BUTTON_UP ].push_back( LN_KEY_UP );
        mAttachedVButtons[ LN_AT_BUTTON_UP ].push_back( LN_VBUTTON_JOYPAD_POV_UP );
        mAttachedVButtons[ LN_AT_BUTTON_UP ].push_back( LN_VBUTTON_JOYPAD_AXIS_2_MINUIS );

        mAttachedVButtons[ LN_AT_AXIS_1X_MINUIS ].push_back( LN_KEY_LEFT );
        mAttachedVButtons[ LN_AT_AXIS_1X_MINUIS ].push_back( LN_VBUTTON_JOYPAD_POV_LEFT );
        mAttachedVButtons[ LN_AT_AXIS_1X_MINUIS ].push_back( LN_VBUTTON_JOYPAD_AXIS_1_MINUIS );

        mAttachedVButtons[ LN_AT_AXIS_1X_PLUS   ].push_back( LN_KEY_RIGHT );
        mAttachedVButtons[ LN_AT_AXIS_1X_PLUS   ].push_back( LN_VBUTTON_JOYPAD_POV_RIGHT );
        mAttachedVButtons[ LN_AT_AXIS_1X_PLUS   ].push_back( LN_VBUTTON_JOYPAD_AXIS_1_PLUS );

        mAttachedVButtons[ LN_AT_AXIS_1Y_MINUIS ].push_back( LN_KEY_UP );
        mAttachedVButtons[ LN_AT_AXIS_1Y_MINUIS ].push_back( LN_VBUTTON_JOYPAD_POV_UP );
        mAttachedVButtons[ LN_AT_AXIS_1Y_MINUIS ].push_back( LN_VBUTTON_JOYPAD_AXIS_2_MINUIS );

        mAttachedVButtons[ LN_AT_AXIS_1Y_PLUS   ].push_back( LN_KEY_DOWN );
        mAttachedVButtons[ LN_AT_AXIS_1Y_PLUS   ].push_back( LN_VBUTTON_JOYPAD_POV_DOWN );
        mAttachedVButtons[ LN_AT_AXIS_1Y_PLUS   ].push_back( LN_VBUTTON_JOYPAD_AXIS_2_PLUS );

        mAttachedVButtons[ LN_AT_AXIS_2X_MINUIS ].push_back( LN_VBUTTON_JOYPAD_AXIS_3_MINUIS );
        mAttachedVButtons[ LN_AT_AXIS_2X_PLUS   ].push_back( LN_VBUTTON_JOYPAD_AXIS_3_PLUS );
        mAttachedVButtons[ LN_AT_AXIS_2Y_MINUIS ].push_back( LN_VBUTTON_JOYPAD_AXIS_4_MINUIS );
        mAttachedVButtons[ LN_AT_AXIS_2Y_PLUS   ].push_back( LN_VBUTTON_JOYPAD_AXIS_4_PLUS );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    GameController::~GameController()
    {
    }
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool GameController::isPress( LNGameControllerButton button )
    {
        return ( mButtonStatus[ button ] > 0 );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool GameController::isOnTrigger( LNGameControllerButton button )
    {
        return ( mButtonStatus[ button ] == 1 );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool GameController::isOffTrigger( LNGameControllerButton button )
    {
        return ( mButtonStatus[ button ] == -1 );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool GameController::isRepeat( LNGameControllerButton button )
    {
        lnS32 s = mButtonStatus[ button ];
		return ( ( s == 1 ) || ( s > mFirstRepeatInterval && s % mRemRepeatInterval == 0 ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    float GameController::getPower( LNGameControllerButton button )
    {
        return mButtonPower[ button ];
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    const LVector2& GameController::getAxisState( int axis )
    {
        if ( axis < 0 || LN_MAX_JOYPAD_AXIS / 2 <= axis ) return LVector2::Zero;
		return mAxisState[ axis ];
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    int GameController::getPressedButton()
    {
        bool mouse    = ( mDeviceAttachFlag & LN_INPUTATTACH_MOUSE ) != 0;
        bool keyboard = ( mDeviceAttachFlag & LN_INPUTATTACH_KEYBOARD ) != 0;
        int  joypad   = ( ( mDeviceAttachFlag & LN_INPUTATTACH_JOYPAD ) != 0 ) ? mJoypadID : -1;
        int  button = LN_VBUTTON_NULL;
        for ( int i = 1; i < LN_VBUTTON_LAST; ++i )
        {
            if ( mInputDevice->getVirtualButtonState( i, keyboard, mouse, joypad ) >= LN_JOYPAD_SWITCH_RANGE )
            {
                button = i;
            }
        }

        // 何か押されていた
        if ( button != LN_VBUTTON_NULL )
        {
            if ( mPressedTriggerFlag )
            {
                button = LN_VBUTTON_NULL;
            }
            else
            {
                mPressedTriggerFlag = true;
            }
        }
        else
        {
            mPressedTriggerFlag = false;
        }
        return button;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GameController::attachVirtualButton( LNGameControllerAttachTarget target, int v_button, bool duplication )
    {
        AttachedVButtons& buttons = mAttachedVButtons[ target ];

		// v_button がアタッチされていなければ追加
        if ( std::find( buttons.begin(), buttons.end(), v_button ) == buttons.end() )
        {
            buttons.push_back( v_button );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GameController::detachVirtualButton( LNGameControllerAttachTarget target, int v_button )
    {
        AttachedVButtons& buttons = mAttachedVButtons[ target ];

		if ( v_button == LN_VBUTTON_NULL ) 
		{
			// 全アタッチ解除
			buttons.clear();
		}
		else
		{
			AttachedVButtons::iterator end_it = std::remove( buttons.begin(), buttons.end(), v_button );
			buttons.erase( end_it, buttons.end() );
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GameController::update()
    {
        bool mouse    = ( mDeviceAttachFlag & LN_INPUTATTACH_MOUSE ) != 0;
        bool keyboard = ( mDeviceAttachFlag & LN_INPUTATTACH_KEYBOARD ) != 0;
        int  joypad   = ( ( mDeviceAttachFlag & LN_INPUTATTACH_JOYPAD ) != 0 ) ? mJoypadID : -1;
        lnFloat v;
        lnFloat power;

        AttachedVButtons::iterator itr;
        AttachedVButtons::iterator end;

        memset( mAxisState, 0, sizeof( mAxisState ) );

        for ( lnU32 i = 0; i < LN_MAX_AT_TARGETS; ++i )
        {
            power = 0.0f;
            itr = mAttachedVButtons[ i ].begin();
            end = mAttachedVButtons[ i ].end();

            for ( ; itr != end; ++itr )
            {
                v = mInputDevice->getVirtualButtonState( (*itr), keyboard, mouse, joypad );
                if ( v > power )
                {
                    power = v;
                } 
            }

            // 仮想コントローラのボタンに対する入力
            //if ( LN_AT_BUTTON_BEGIN <= i && i <= LN_AT_BUTTON_END )
            {
                mButtonPower[ i ] = power;

                // 十分に押されている場合
                if ( power >= LN_JOYPAD_SWITCH_RANGE )
                {
                    ++mButtonStatus[ i ];

				    //if ( mButtonStatus[ i ] > mRemRepeatInterval + mFirstRepeatInterval )
				    //{
					   // mButtonStatus[ i ] = mFirstRepeatInterval;
				    //}
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
            // 仮想コントローラのレバーに対する入力
            if ( LN_AT_AXIS_BEGIN <= i && i <= LN_AT_AXIS_END )
            {
                if ( ( (i - LN_AT_AXIS_BEGIN) & 1 ) == 0 )
                {
                    power *= -1.0f;
                }
                lnFloat* w = &((lnFloat*)mAxisState)[ ( i - LN_AT_AXIS_BEGIN ) / 2 ];
                *w += power;
                if ( *w > 1.0f )
                {
                    *w = 1.0f;
                }
                else if ( *w < -1.0f )
                {
                    *w = -1.0f;
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