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

/// ランダム値付きアニメーションのランダム値の範囲
enum LNAnimRandMode
{
    LN_ANIM_RAND_MEDIAN = 0,    ///< 値は  v-r ～ v ～ v+r 
    LN_ANIM_RAND_MIN,           ///< 値は  v-r ～ v
    LN_ANIM_RAND_MAX            ///< 値は         v ～ v+r 
};

/// アニメーションの補間方法
enum LNAnimInterMode
{
    LN_ANIM_INTER_NONE = 0,     ///< 補間なし
    LN_ANIM_INTER_LINEAR,       ///< 線形補間
    LN_ANIM_INTER_ACCEL,        ///< 等加速度
    LN_ANIM_INTER_CUBIC,        ///< 3次補間
    LN_ANIM_INTER_CATMULLROM,   ///< Catmull-Rom 補間
};

/// アニメーションのループ方法
enum LNAnimationLoopMode
{
	LN_ANIMATIONLOOPMODE_NO_LOOP = 0,		///< ループしない
	LN_ANIMATIONLOOPMODE_REPEAT_LOOP,		///< 先頭からループ再生
	LN_ANIMATIONLOOPMODE_ROUND_TRIP_LOOP,	///< 往復ループ再生
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