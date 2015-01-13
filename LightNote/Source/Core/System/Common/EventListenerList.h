//==============================================================================
// EventListenerList 
//------------------------------------------------------------------------------
///**
//  @file       EventListenerList.h
//  @brief      EventListenerList
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <list>

namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// ■ EventListenerList
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
template<class PTR_T>
class EventListenerList
{
public:
	struct ListenerEntry
    {
        PTR_T	EventListener;
        int		Priority;
    };
    typedef std::list<ListenerEntry> ListenerEntryList;

public:
	EventListenerList(){}
	~EventListenerList(){}

public:

	/// アタッチ
    void attach( PTR_T listener, int priority )
	{
		ListenerEntry e;
		e.EventListener = listener;
		e.Priority = priority;
		mListenerEntryList.push_back( e );
		mListenerEntryList.sort( _cmpEventListener );
	}

	/// デタッチ
    void detach( PTR_T listener )
	{
		mListenerEntryList.remove_if( IsEven( listener ) );
	}

	/// リスナーリストの取得
	ListenerEntryList& getListenerEntryList() { return mListenerEntryList; }

private:

	class IsEven
	{
	public:

		PTR_T mListener;

	public:

		IsEven( PTR_T listener ) : mListener( listener ) {}

		bool operator()( const ListenerEntry& e ) const
		{
			return e.EventListener == mListener;
		}
	};

	static bool _cmpEventListener( const ListenerEntry& l, const ListenerEntry& r )
	{
		return l.Priority < r.Priority;
	}

private:
	ListenerEntryList   mListenerEntryList;
};

} // namespace System
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================