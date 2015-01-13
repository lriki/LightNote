//==============================================================================
// WindowBase 
//------------------------------------------------------------------------------
///**
//  @file       WindowBase.h
//  @brief      WindowBase
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <list>
#include "../Interface.h"

namespace LNote
{
namespace Core
{
namespace System
{
class WindowManagerBase;

//==============================================================================
// ■ WindowBase クラス
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class WindowBase
    : public Window
{
public:
	WindowBase( WindowManagerBase* manager );
    virtual ~WindowBase();
	LN_REFOBJ_METHODS

public:

	/// 終了処理
    virtual void finalize() = 0;

	/// 全てのイベントリスナーにイベントを送信する
	bool sendEventToAllListener( const EventArgs& e );

public:
    virtual void attachEventListener( IEventListener* listener, int priority );
    virtual void detachEventListener( IEventListener* listener );

private:

	struct ListenerEntry
    {
        IEventListener* EventListener;
        int             Priority;
    };
    typedef std::list<ListenerEntry> ListenerEntryList;

	class IsEven;
	static bool _cmpEventListener( const ListenerEntry& l, const ListenerEntry& r );

private:
	WindowManagerBase*	mWindowManager;
	ListenerEntryList   mListenerEntryList;
};

} // namespace System
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================