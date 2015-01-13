//==============================================================================
// AnimationClip 
//==============================================================================

#include "stdafx.h"
#include "AnimationClip.h"

namespace LNote
{
namespace Core
{
namespace Animation
{

//==============================================================================
// AnimationClip
//==============================================================================

	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
 //   AnimationClip2::AnimationClip2()
	//	: mLastFramePosition	( 0 )
	//	, mWrapMode				( WrapMode_Once )
	//{
	//}

	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
	//AnimationClip2::~AnimationClip2()
	//{
	//	ln_foreach( AnimationCurveState& state, mAnimationCurveStateArray )
	//	{
	//		state.AnimationCurve->release();
	//	}
	//}

	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
	//void AnimationClip2::addAnimationCurve( const wchar_t* relativePath, AnimationTimelineBase* curve )
	//{
	//	mAnimationCurveStateArray.push_back( AnimationCurveState() );
	//	AnimationCurveState& state = mAnimationCurveStateArray.back();
	//	state.RelativePath = relativePath;
	//	state.AnimationCurve = curve;
	//	curve->addRef();
	//}

	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
	//void AnimationClip2::advanceTime( double currentTime, lnFloat deltaTime )
	//{
	//	ln_foreach( AnimationCurveState& state, mAnimationCurveStateArray )
	//	{
	//		state.AnimationCurve->advanceTime( currentTime, deltaTime );
	//	}
	//}


//==============================================================================
// ¡ AnimationClip
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    AnimationClip::AnimationClip()
		: mLastFramePosition	( 0 )
		, mWrapMode				( WrapMode_Once )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	AnimationClip::~AnimationClip()
	{
		ln_foreach( AnimationCurveState& state, mAnimationCurveStateArray )
		{
			state.AnimationCurve->release();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AnimationClip::addAnimationCurve( const lnKeyChar* relativePath, AnimationTimelineBase* curve )
	{
		mAnimationCurveStateArray.push_back( AnimationCurveState() );
		AnimationCurveState& state = mAnimationCurveStateArray.back();
		state.RelativePath = relativePath;
		state.AnimationCurve = curve;
		curve->addRef();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AnimationClip::advanceTime( double currentTime, lnFloat deltaTime )
	{
		ln_foreach( AnimationCurveState& state, mAnimationCurveStateArray )
		{
			
			state.AnimationCurve->advanceTime( currentTime, deltaTime );
		}
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================