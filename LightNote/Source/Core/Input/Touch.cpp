//==============================================================================
// Touch 
//==============================================================================

#include "stdafx.h"
#include "Manager.h"
#include "Touch.h"

namespace LNote
{
namespace Core
{
namespace Input
{

//==============================================================================
// ■ Touch
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Touch::Touch( Manager* manager )
		: mManager				( manager )
		, mInputDevice			( manager->getInputDevice() )
		, mFirstRepeatInterval	( 20 )
		, mRemRepeatInterval	( 5 )
	{
		memset( mPressCounts, 0, sizeof( mPressCounts ) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Touch::~Touch()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Touch::isPress( lnU32 id )
	{
		if ( id < 0 || id >= LN_MAX_TOUCH_DATA ) { 
			return false;
		}
		return ( mPressCounts[ id ] > 0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Touch::isOnTrigger( lnU32 id )
	{
		if ( id < 0 || id >= LN_MAX_TOUCH_DATA ) { 
			return false;
		}
		return ( mPressCounts[ id ] == 1 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Touch::isOffTrigger( lnU32 id )
	{
		if ( id < 0 || id >= LN_MAX_TOUCH_DATA ) { 
			return false;
		}
		return ( mPressCounts[ id ] == -1 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Touch::isRepeat( lnU32 id )
	{
		if ( id < 0 || id >= LN_MAX_TOUCH_DATA ){ 
			return 0; 
		}
		int state = mPressCounts[ id ];
		return ( ( state == 1 )  ||  ( state > mFirstRepeatInterval && state % mRemRepeatInterval == 0 ) );
	}


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Touch::setRepeatInterval( lnU32 first, lnU32 interval )
	{
		mFirstRepeatInterval = first;
		mRemRepeatInterval = interval;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Touch::update()
	{
        const LNMouseDeviceState& mouse = mInputDevice->getMouseState();

	    mInputDevice->getTouchState( 0, &mTouchState[0] );
	    mInputDevice->getTouchState( 1, &mTouchState[1] );
 
		for ( int i = 0; i < LN_MAX_TOUCH_DATA; ++i )
		{
            if ( mTouchState[ i ].Touch || ( i == 0 && mouse.Buttons[0] ) )
			{
				++mPressCounts[ i ];
				
				if ( mPressCounts[ i ] > mRemRepeatInterval + mFirstRepeatInterval )
				{
					mPressCounts[ i ] = mFirstRepeatInterval;
				}
			}
			else
			{
				if ( mPressCounts[ i ] > 0 )
				{
					mPressCounts[ i ] = -1;
				}
				else
				{
					mPressCounts[ i ] = 0;
				}
			}
		    
            if ( mTouchState[ i ].Touch )
            {
                mPosition[i].X = static_cast< lnFloat >( mTouchState[i].X );
	            mPosition[i].Y = static_cast< lnFloat >( mTouchState[i].Y ); 
            }
            else if ( i == 0 && mouse.Buttons[0] )
            {
                mPosition[i].X = static_cast< lnFloat >( mouse.X );
	            mPosition[i].Y = static_cast< lnFloat >( mouse.Y ); 
            }

			mPosition[i].TransformCoord(mManager->getMouseTransform());
    		    
	        mDistance[i].X = mPosition[i].X - mLastPosition[i].X;
	        mDistance[i].Y = mPosition[i].Y - mLastPosition[i].Y;
            mLastPosition[i] = mPosition[i];
		}
	}

} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================