//==============================================================================
// WindowBase 
//==============================================================================

#include "stdafx.h"
#include <algorithm>
#include "WindowManagerBase.h"
#include "WindowBase.h"

namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// ■ WindowBase
//==============================================================================

// イベントリスナー削除用の関数オブジェクト
class WindowBase::IsEven
{
public:

    IEventListener* mListener;

public:

    IsEven( IEventListener* listener ) : mListener( listener ) {}

    bool operator()( const ListenerEntry& e ) const
    {
        return e.EventListener == mListener;
    }
};

// EventListener ソート用の比較関数
bool WindowBase::_cmpEventListener( const ListenerEntry& l, const ListenerEntry& r )
{
    return l.Priority < r.Priority;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
WindowBase::WindowBase( WindowManagerBase* manager )
	: mWindowManager	( manager )
{
	LN_SAFE_ADDREF( mWindowManager );
	if ( mWindowManager ) mWindowManager->addWindow( this );
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
WindowBase::~WindowBase()
{
	if ( mWindowManager ) mWindowManager->removeWindow( this );
	LN_SAFE_RELEASE( mWindowManager );
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
bool WindowBase::sendEventToAllListener( const EventArgs& e )
{
	ListenerEntryList::iterator itr = mListenerEntryList.begin();
    ListenerEntryList::iterator end = mListenerEntryList.end();
    for ( ; itr != end; ++itr )
    {
        if ( (*itr).EventListener->onEvent( e ) ) return true;
    }
	return false;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void WindowBase::attachEventListener( IEventListener* listener, int priority )
{
    ListenerEntry e;
    e.EventListener = listener;
    e.Priority = priority;
    mListenerEntryList.push_back( e );
    mListenerEntryList.sort( _cmpEventListener );
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void WindowBase::detachEventListener( IEventListener* listener )
{
    mListenerEntryList.remove_if( IsEven( listener ) );
}

} // namespace System
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================