//==============================================================================
// AnimationController 
//------------------------------------------------------------------------------
/*
	����{�\��
		AnimationController
			MotionState
			MotionState
			MotionState
				Motion
					TargetName IAnimation
					TargetName IAnimation
					TargetName IAnimation
		Motion �� IAnimation �̏W���B����Scheduler�Ƌ��L�\�B
		MotionState �͂��̃X�P�W���[�����ł̃��C���[��Đ����x�A�u�����h���@���Ǘ�����B

		�c�ł��AMotionState �͌��J����K�v�Ȃ������B
		unity �ł� animation["jump"].layer = 1; �Ƃ������������߂̂��́B
		�Â������Ƃ������Ȃ���΁A��ȂƂ����

			setMotionLayer( "jump", 1 );
			setMotionBlendMode( "jump", Loop );
			setMotionSpeed( "jump", 0.5 );

		��3���������邾���łقƂ�ǂ͑��肻���B




	��Model �����X�P�W���[��
		�{�[���A�j���ƃ��[�t�B���O�A�j���́A��ԏ����͑S�������B
		�K�p����Ώۂ��Ⴄ���߁A�X�ŗp�ӂ���B

		Model
			getBoneMotionScheduler()	// �{�[���p�X�P�W���[��
			getMorphMotionScheduler()	// ���[�t�B���O�p�X�P�W���[��

		�g�����c
			scheduler1 = model1->getBoneMotionScheduler();
			scheduler1->play("�W�����v");
		
	�����[�V�������֘A�t����
		motion1 = new Motion();
		motion1->create("���[�V�����t�@�C��.vmd�Ƃ�");
		_p(motion1->getName()); // �g���q���������t�@�C����s

		scheduler1->attach( motion1 );
		scheduler1->play( motion1->getName() );
*/
//==============================================================================

#include "stdafx.h"
#include <algorithm>
#include <Dependencies/cpp-TimSort/timsort.hpp>
#include "ScalarAnimation.h"
#include "QuaternionAnimation.h"
#include "SQTTransformAnimation.h"
#include "AnimationClip.h"
#include "AnimationController.h"

namespace LNote
{
namespace Core
{
namespace Animation
{

//==============================================================================
// �� AnimationController
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    AnimationController::AnimationController()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	AnimationController::~AnimationController()
	{
		ln_foreach( AnimationState* state, mAnimationStateArray )
		{
			LN_SAFE_RELEASE( state->AnimationClip );
			SAFE_DELETE( state );
		}
	} 

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool AnimationController::isPlaying() const
	{
		LN_THROW_NotImpl( 0 );
	}
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AnimationController::play( const lnKeyChar* animName, PlayMode mode )
	{
		LN_THROW_NotImpl( 0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AnimationController::blend( const lnKeyChar* animName, lnFloat targetWeight, lnFloat fadeLength )
	{
		LN_THROW_NotImpl( 0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AnimationController::crossFade( const lnKeyChar* animName, lnFloat fadeLength, PlayMode mode )
	{
		LN_THROW_NotImpl( 0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AnimationController::playQueued( const lnKeyChar* animName, QueueMode queueMode, PlayMode playMode )
	{
		LN_THROW_NotImpl( 0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AnimationController::crossFadeQueued( const lnKeyChar* animName, lnFloat fadeLength, QueueMode queueMode, PlayMode playMode )
	{
		LN_THROW_NotImpl( 0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AnimationController::syncLayer( int layer )
	{
		LN_THROW_NotImpl( 0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AnimationController::advanceTime( lnFloat deltaTime )
	{
		ln_foreach( AnimationState* state, mAnimationStateArray )
		{
			state->AnimationClip->advanceTime( state->mTime, deltaTime );
			state->mTime += deltaTime;


			ln_foreach( AnimationState::AnimationTargetSet& set, state->mAnimationTargetSetArray )
			{
				if ( set.TargetState )
				{
					set.TargetState->ValueType = set.Animation->getValueType();
					switch ( set.TargetState->ValueType )
					{
						case ValueType_Float:
							*((lnFloat*)set.TargetState->Value) = 
								static_cast<ScalarAnimation*>(set.Animation)->getValue();
							break;
						case ValueType_Vector3:
							LN_THROW_NotImpl( 0 );
							break;
						case ValueType_Quaternion:
							LN_THROW_NotImpl( 0 );
							break;
						case ValueType_SQTTransform:
							*((LSQTTransform*)set.TargetState->Value) = 
								static_cast<SQTTransformAnimation*>(set.Animation)->getValue();
							break;
						case ValueType_Event:
							// �l�^�ł͂Ȃ��̂ŉ������Ȃ�
							break;
					}
				}
				else
				{
					// �A�j���[�V�������ɂ͑��݂��邪�A�Ώۃ{�[�����Ȃ�
				}
			}
		}

		ln_foreach( AnimationApplyTargetState& state, mAnimationApplyTargetStateArray )
		{
			state.Target->onUpdateAnimationValue( state.ValueType, state.Value );
		}

	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AnimationController::addAnimationClip( AnimationClip* animationClip )
	{
		struct 
		{
			bool operator()( const AnimationState* l, const AnimationState* r ) const
			{
				return l->AnimationClip->getName() < r->AnimationClip->getName();
			}
		} compare;

		
		AnimationState* state = LN_NEW AnimationState;
		state->AnimationClip = animationClip;
		state->AnimationClip->addRef();
		mAnimationStateArray.push_back( state );

		// �^�[�Q�b�g�Ƃ̊֘A�t��
		AnimationClip::AnimationCurveStateArray& stateArray = animationClip->getAnimationCurveStateArray();
		ln_foreach( AnimationClip::AnimationCurveState& clipState, stateArray )
		{
			AnimationState::AnimationTargetSet set;
			set.Animation	= clipState.AnimationCurve;
			set.TargetState = findAnimationApplyTargetState( clipState.RelativePath.c_str() );
			if (set.TargetState != NULL)
			{
			
				state->mAnimationTargetSetArray.push_back( set );
			}
		}

		// �ǉ���A�񕪒T���p�Ƀ\�[�g
		gfx::timsort( mAnimationStateArray.begin(), mAnimationStateArray.end(), compare );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AnimationController::removeAnimationClip( const lnKeyChar* clipName )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	AnimationState* AnimationController::findAnimationState( const lnKeyChar* clipName )
	{
#if 0
		
		struct cmp1
		{
			bool operator()( const AnimationState* lhs, const lnRefString& rhs ) const
			{
				return lhs->AnimationClip->getName() < rhs;
			}

			bool operator()( const lnRefString& lhs, const AnimationState* rhs ) const
			{
				return rhs < lhs->AnimationClip->getName();
			}
		} compare;

		lnRefString key = clipName;
#endif
#if 1
		struct compare
		{
			static bool func( const AnimationState* l, const lnKeyChar* clipName )
			{
				return l->AnimationClip->getName() < clipName;
			}
		};

		AnimationStateArray::iterator itr = std::lower_bound(
			mAnimationStateArray.begin(), mAnimationStateArray.end(), clipName, compare::func );
		if ( itr != mAnimationStateArray.end() &&
			 (*itr)->AnimationClip->getName() == clipName )
			return (*itr);
#endif
		return NULL;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AnimationController::addAnimationApplyTarget( IAnimationApplyTarget* target )
	{
		struct 
		{
			bool operator()( const AnimationApplyTargetState& l, const AnimationApplyTargetState& r ) const
			{
				return l.Target->getAnimationTargetName() < r.Target->getAnimationTargetName();
			}
		} compare;

		AnimationApplyTargetState state;
		state.Target = target;
		mAnimationApplyTargetStateArray.push_back( state );

		// �ǉ���A�񕪒T���p�Ƀ\�[�g
		std::sort( mAnimationApplyTargetStateArray.begin(), mAnimationApplyTargetStateArray.end(), compare );
		//gfx::timsort( mAnimationApplyTargetStateArray.begin(), mAnimationApplyTargetStateArray.end(), compare );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	AnimationApplyTargetState* AnimationController::findAnimationApplyTargetState( const lnKeyChar* naem )
	{
#if 1
		struct compare
		{
			static bool func( const AnimationApplyTargetState& l, const lnKeyChar* naem )
			{
				return l.Target->getAnimationTargetName() < naem;
			}
		};

		AnimationApplyTargetStateArray::iterator itr = std::lower_bound(
			mAnimationApplyTargetStateArray.begin(), mAnimationApplyTargetStateArray.end(), naem, compare::func );
		if ( itr != mAnimationApplyTargetStateArray.end() &&
			 (*itr).Target->getAnimationTargetName() == naem )
			return &(*itr);
#endif
		return NULL;
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================