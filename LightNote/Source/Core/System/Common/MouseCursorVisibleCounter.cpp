//==============================================================================
// MouseCursorVisibleCounter 
//==============================================================================

#include "stdafx.h"
#include <algorithm>
#include "../Environment.h"
#include "MouseCursorVisibleCounter.h"

namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// ■ MouseCursorVisibleCounter
//==============================================================================

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
MouseCursorVisibleCounter::MouseCursorVisibleCounter()
	: mVisibleMouseCursor	( true )
	, mMouseVisibleTime		( 0 )
	, mStartTime			( 0 )
	, mRequestForced		( false )
{
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
MouseCursorVisibleCounter::~MouseCursorVisibleCounter()
{
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
bool MouseCursorVisibleCounter::checkVisible() const
{
	if ( mRequestForced ) return true;
	if ( mVisibleMouseCursor ) return true;
	if ( mMouseVisibleTime == 0 ) return false;
	return ( ( Environment::getSystemTime() - mStartTime ) < mMouseVisibleTime );
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void MouseCursorVisibleCounter::setMouseCursorVisibleState( bool visible, int time )
{
	LN_ERR2_ASSERT( time >= 0 );

    mVisibleMouseCursor	= visible;
    mMouseVisibleTime	= std::max( 0, time );
	mStartTime			= Environment::getSystemTime();
	mRequestForced		= false;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void MouseCursorVisibleCounter::onMoveCursor( bool on_client_area )
{
	// 表示しない場合
    if ( !mVisibleMouseCursor && mMouseVisibleTime == 0 )
    {
		mRequestForced = false;
    }
    else
    {
        if ( on_client_area )
        {
			mStartTime = Environment::getSystemTime();
			mRequestForced = false;
        }
        else
        {
            mRequestForced = true;
        }
    }
}

} // namespace System
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================