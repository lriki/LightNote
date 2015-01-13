//==============================================================================
// EventAnimation 
//==============================================================================

#include "stdafx.h"
#include <algorithm>
#include <Dependencies/cpp-TimSort/timsort.hpp>
#include "EventAnimation.h"

namespace LNote
{
namespace Core
{
namespace Animation
{

//==============================================================================
// ■ EventAnimationKeyBase
//==============================================================================

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
EventAnimationKeyBase::EventAnimationKeyBase()
	: FramePos	( 0 )
{
}

//==============================================================================
// ■ EventAnimation
//==============================================================================

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
EventAnimation::EventAnimation()
	: mNextExecuteIndex	( 0 )
{
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
EventAnimation::~EventAnimation() 
{
	ln_foreach( EventAnimationKeyBase* k, mEventAnimationKeyArray )
	{
		k->release();
	}
	mEventAnimationKeyArray.clear();
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void EventAnimation::addKeyFrame( EventAnimationKeyBase* key )
{
	LN_ERR2_ASSERT( key );

	if ( mEventAnimationKeyArray.empty() || mEventAnimationKeyArray.back()->FramePos <= key->FramePos )
	{
		mEventAnimationKeyArray.push_back( key );
	}
	else
	{
		mEventAnimationKeyArray.push_back( key );
		sortKeys();
	}

	LN_SAFE_ADDREF( key );
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void EventAnimation::setTimeWithEvent( double time )
{
	double old_frame_pos = mCurrentFramePos;
	AnimationTimelineBase::setTime( time );

	if ( mEventAnimationKeyArray.empty() ) return;

	// 前方向に進んだ
	if ( mCurrentFramePos > old_frame_pos )
	{
		executeRange( old_frame_pos, mCurrentFramePos );
	}
	// ループ再生で先頭に戻った
	else if ( mCurrentFramePos < old_frame_pos )
	{
		executeRange( old_frame_pos, mEventAnimationKeyArray.back()->FramePos );
		mNextExecuteIndex = 0;
		executeRange( 0, mCurrentFramePos );
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void EventAnimation::sortKeys()
{
	// 比較用ファンクタ
	struct compare
	{
		bool operator()( const EventAnimationKeyBase* l, const EventAnimationKeyBase* r )
		{
			return l->FramePos < r->FramePos;
		}
	};
	gfx::timsort( mEventAnimationKeyArray.begin(), mEventAnimationKeyArray.end(), compare() );
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
double EventAnimation::getLastFramePos()
{
	if ( mEventAnimationKeyArray.empty() ) return 0;
	return mEventAnimationKeyArray.back()->FramePos;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void EventAnimation::setTime( double time )
{
	AnimationTimelineBase::setTime( time );
	mNextExecuteIndex = 0;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void EventAnimation::advanceTime( lnFloat delta_time )
{
	setTimeWithEvent( mTime + delta_time );
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void EventAnimation::executeRange( double begin_frame, double end_frame )
{
	size_t size = mEventAnimationKeyArray.size();
	for ( size_t i = mNextExecuteIndex; i < size; ++i )
	{
		EventAnimationKeyBase* key = mEventAnimationKeyArray[i];
		if ( begin_frame <= key->FramePos && key->FramePos < end_frame )
		{
			key->execute();
		}
		else if ( key->FramePos >= mCurrentFramePos )
		{
			mNextExecuteIndex = i;
			break;
		}
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
XML::Element* EventAnimation::serializeXMLElement( XML::Document* doc )
{

    return NULL;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void EventAnimation::deserializeXMLElement( XML::Element* element )
{

}

} // namespace Animation
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================