//==============================================================================
// AnimationTimelineBase 
//==============================================================================

#include "stdafx.h"
#include "AnimationTimelineBase.h"

namespace LNote
{
namespace Core
{
namespace Animation
{

//==============================================================================
// �� AnimationTimelineBase
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	AnimationTimelineBase::AnimationTimelineBase()
		: mTimeTick			( 60.0f )
		, mTime				( 0 )
		, mCurrentFramePos	( 0 )
		, mLoopMode			( LN_ANIMATIONLOOPMODE_NO_LOOP )
		, mLoopBeginPos		( 0 )
		, mLoopEndPos		( 0 )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AnimationTimelineBase::setLoopState( LNAnimationLoopMode loop_mode, lnFloat begin_frame_pos, lnFloat end_frame_pos )
	{
		mLoopMode = loop_mode;
		mLoopBeginPos = begin_frame_pos;
		mLoopEndPos = end_frame_pos;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool AnimationTimelineBase::isFinished()
	{
		if ( mLoopMode != LN_ANIMATIONLOOPMODE_NO_LOOP ) return false;

		double last_frame_pos = getLastFramePos();
		if ( last_frame_pos == 0.0 ) return true;
		if ( mCurrentFramePos < last_frame_pos ) return false;
		return true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AnimationTimelineBase::setTime( double time )
	{
		mTime = time;

		time *= mTimeTick;	// �t���[���ʒu�֕ϊ�

		double last_frame_pos = getLastFramePos();
		if ( last_frame_pos == 0 )
		{
			time = 0;
		}
		else if ( mLoopMode == LN_ANIMATIONLOOPMODE_REPEAT_LOOP )
		{
			// ���[�v�ʒu���ݒ�
			if ( mLoopEndPos == 0.0f && mLoopBeginPos == 0.0f )
			{
				// �I�[�̃L�[�t���[���̎��Ԃ��g���ă��[�v
				if ( time > last_frame_pos )
				{
					time = fmod( time, last_frame_pos );
				}
			}
			// ���[�v�ʒu�ݒ�ς�
			else 
			{
				// time_ �����[�v�̈�𒴂��Ă����ꍇ�͊J�n���Ԃ��烋�[�v
				if ( time > mLoopEndPos )
				{
					time -= mLoopBeginPos;
					time = fmod( time, ( mLoopEndPos - mLoopBeginPos ) );
					time = mLoopBeginPos + time;
				}
			}
		}
		else if ( mLoopMode == LN_ANIMATIONLOOPMODE_REPEAT_LOOP )
		{
			LN_PRINT_NOT_IMPL_FUNCTION;
		}

		mCurrentFramePos = time;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AnimationTimelineBase::advanceTime( lnFloat deltaTime )
	{
		setTime( mTime + static_cast< double >( deltaTime ) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AnimationTimelineBase::advanceTime( double time, lnFloat deltaTime )
	{
		mTime = time;
		advanceTime( deltaTime );
	}

} // namespace Animation
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================