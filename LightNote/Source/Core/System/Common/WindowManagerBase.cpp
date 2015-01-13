//==============================================================================
// WindowManagerBase 
//==============================================================================

#include "stdafx.h"
#include "../Manager.h"
#include "WindowBase.h"
#include "WindowManagerBase.h"

namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// ■ WindowManagerBase
//==============================================================================

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
WindowManagerBase::WindowManagerBase( Manager* manager )
	: mManager	( manager )
{
	LN_SAFE_ADDREF( mManager );
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
WindowManagerBase::~WindowManagerBase()
{
	LN_SAFE_RELEASE( mManager );
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void WindowManagerBase::setMouseCursorVisibleState( bool visible, int invisible_time )
{
	mMouseCursorVisibleCounter.setMouseCursorVisibleState( visible, invisible_time );
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
bool WindowManagerBase::sendEventToAllListener( const EventArgs& e )
{
	ln_foreach( WindowBase* w, mWindowArray )
	{
		if ( w->sendEventToAllListener( e ) ) return true;
	}
	return false;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void WindowManagerBase::finalize()
{
	mWindowArray.clear();
}

} // namespace System
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================