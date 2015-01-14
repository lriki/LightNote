//==============================================================================
// Mouse 
//==============================================================================

#include "stdafx.h"
#include "Manager.h"
#include "Mouse.h"

namespace LNote
{
namespace Core
{
namespace Input
{

//==============================================================================
// ■ Mouse
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Mouse::Mouse( Manager* manager )
		: mManager				( manager )
		, mInputDevice			( manager->getInputDevice() )
		, mFirstRepeatInterval	( 20 )
		, mRemRepeatInterval	( 5 )
        , mIsCenterFix          ( false )
		, mWheelState			( 0 )
		//, mMouseCursorMode		( LN_MOUSEMODE_MOVER_VISIBLE )
		, mMouseVisibleCount	( 0 )
		, mIsSystemMouseShow	( true )
	{
		memset( mButtonStatus, 0, sizeof( mButtonStatus ) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Mouse::~Mouse()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Mouse::isPress( LNMouseButton button )
	{
		if ( button < 0 || button >= MAX_BUTTON_NUM ) { 
			return false;
		}
		return ( mButtonStatus[ button ] > 0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Mouse::isOnTrigger( LNMouseButton button )
	{
		if ( button < 0 || button >= MAX_BUTTON_NUM ){ 
			return false;
		}
		return ( mButtonStatus[ button ] == 1 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Mouse::isOffTrigger( LNMouseButton button )
	{
		if ( button < 0 || button >= MAX_BUTTON_NUM ) { 
			return false;
		}
		return ( mButtonStatus[ button ] == -1 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Mouse::isRepeat( LNMouseButton button )
	{
		if ( button < 0 || button >= MAX_BUTTON_NUM ){ 
			return 0;
		}
		int state = mButtonStatus[ button ];
		return ( ( state == 1 )  ||  ( state > mFirstRepeatInterval && state % mRemRepeatInterval == 0 ) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Mouse::setRepeatInterval( lnU32 first, lnU32 interval )
	{
		mFirstRepeatInterval = first;
		mRemRepeatInterval = interval;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Mouse::update()
	{
        const LNMouseDeviceState& state = mInputDevice->getMouseState();

		for ( int i = 0; i < MAX_BUTTON_NUM; ++i )
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

	    mPosition.X = static_cast<lnFloat>( state.X );
	    mPosition.Y = static_cast<lnFloat>( state.Y );
        mPosition.TransformCoord( mManager->getMouseTransform() );

        // 画面中央にカーソルを固定する場合
		if ( mIsCenterFix )
		{
            if ( mManager->canSetMousePoint() )
            {
                mDistance.X = mPosition.X - mLastPosition.X;
		        mDistance.Y = mPosition.Y - mLastPosition.Y;

			    mInputDevice->setMouseCursorCenter();

                const Geometry::Size& size = mInputDevice->getWindow()->getSize();
                mLastPosition.X = static_cast< lnFloat >( size.w / 2 );
                mLastPosition.Y = static_cast< lnFloat >( size.h / 2 );
            }
            else
            {
                mDistance.X = 0;
                mDistance.Y = 0;
            }
		}
        else
        {
		    mDistance.X = mPosition.X - mLastPosition.X;
		    mDistance.Y = mPosition.Y - mLastPosition.Y;
            mLastPosition = mPosition;
        }

        mWheelState = state.Wheel;
		
	}

#if 0

	//----------------------------------------------------------------------
	// ● 何からかのマウス操作メッセージがある場合に呼ぶ
	//----------------------------------------------------------------------
	void Mouse::_onOperatedMouse( bool is_client_ )
	{
		_postMoveCursor( is_client_ );
	}

	//----------------------------------------------------------------------
	// ● マウスカーソルの表示状態を設定する
	//----------------------------------------------------------------------
	void Mouse::_showCursor( bool flag_ )
	{
		if ( flag_ )
		{
			mMouseVisibleCount = 0;
		}

		// マウスカーソルの表示は内部で参照カウントのようなもので管理されている。
		// http://msdn.microsoft.com/ja-jp/library/cc364876.aspx
		// そのため、こんな感じで if を噛ませておかないと、カウントが増えすぎたりして不具合が出る。
		if ( flag_ != mIsSystemMouseShow )
		{
			if ( flag_ )
			{
				::ShowCursor( TRUE );
			}
			else
			{
				mMouseVisibleCount = MOUSE_CURSOR_COUNT_MAX;
				::ShowCursor( FALSE );
			}
			mIsSystemMouseShow = flag_;
		}
	}

	//----------------------------------------------------------------------
	// ● マウスカーソルの操作後に呼んで、現在の設定に応じてマウスカーソルの表示・非表示を切り替える
	//----------------------------------------------------------------------
	void Mouse::_postMoveCursor( bool is_client_ )
	{
		// 常に表示しない以外は、一度表示させる
		if ( mMouseCursorMode != LN_MOUSEMODE_INVISIBLE && mMouseCursorMode != LN_MOUSEMODE_CENTER_FIX )
		{
			_showCursor( true );
		}
		else
		{
			// 表示しない場合はクライアント領域内でのみ表示しない
			if ( is_client_ || ( mMouseCursorMode == LN_MOUSEMODE_CENTER_FIX && mInputDevice->isActive() ) )
			{
				_showCursor( false );
			}
			else
			{
				_showCursor( true );
			}
		}
	}
#endif

} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================