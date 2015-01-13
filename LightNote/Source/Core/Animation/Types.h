//==============================================================================
// Types 
//------------------------------------------------------------------------------
///**
//  @file       Types.h
//  @brief      Types
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <vector>

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

typedef double lnTimeFloat;

/// �����_���l�t���A�j���[�V�����̃����_���l�͈̔�
enum LNAnimRandMode
{
    LN_ANIM_RAND_MEDIAN = 0,    ///< �l��  v-r �` v �` v+r 
    LN_ANIM_RAND_MIN,           ///< �l��  v-r �` v
    LN_ANIM_RAND_MAX            ///< �l��         v �` v+r 
};

/// �A�j���[�V�����̕�ԕ��@
enum LNAnimInterMode
{
    LN_ANIM_INTER_NONE = 0,     ///< ��ԂȂ�
    LN_ANIM_INTER_LINEAR,       ///< ���`���
    LN_ANIM_INTER_ACCEL,        ///< �������x
    LN_ANIM_INTER_CUBIC,        ///< 3�����
    LN_ANIM_INTER_CATMULLROM,   ///< Catmull-Rom ���
};

/// �A�j���[�V�����̃��[�v���@
enum LNAnimationLoopMode
{
	LN_ANIMATIONLOOPMODE_NO_LOOP = 0,		///< ���[�v���Ȃ�
	LN_ANIMATIONLOOPMODE_REPEAT_LOOP,		///< �擪���烋�[�v�Đ�
	LN_ANIMATIONLOOPMODE_ROUND_TRIP_LOOP,	///< �������[�v�Đ�
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

namespace LNote
{
namespace Core
{
namespace Animation
{
class AnimationTimelineBase;
class EventAnimationKeyBase;

typedef std::vector<AnimationTimelineBase*> TimelineArray;
typedef std::vector<EventAnimationKeyBase*>	EventAnimationKeyArray; 

typedef wchar_t			lnKeyChar;
typedef lnRefStringW	lnRefKeyString;

enum ValueType
{
	ValueType_Float = 0,
	ValueType_Vector3,
	ValueType_Quaternion,
	ValueType_SQTTransform,
	ValueType_Event,
};

} // namespace Animation
} // namespace Core
} // namespace LNote


//==============================================================================
//
//==============================================================================