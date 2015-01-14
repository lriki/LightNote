//==============================================================================
// SQTTransformAnimation 
//------------------------------------------------------------------------------
///**
//  @file       SQTTransformAnimation.h
//  @brief      SQTTransformAnimation
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Math/LMath.h"
#include "Types.h"
#include "BezierTable.h"
#include "AnimationTimelineBase.h"

namespace LNote
{
namespace Core
{
namespace Animation
{

//==============================================================================
// ■ SQTTransformAnimation
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class SQTTransformAnimation
	: public AnimationTimelineBase
{
public:
	virtual ~SQTTransformAnimation() {}

public:

	/// 値の取得
    virtual const LSQTTransform& getValue() const = 0;

public:
	virtual ValueType getValueType() const { return ValueType_SQTTransform; }
};

//==============================================================================
// ■ BezierSQTTransformAnimation
//------------------------------------------------------------------------------
///**
//  @brief      VMD用
//*/
//==============================================================================
class BezierSQTTransformAnimation
	: public SQTTransformAnimation
{
public:

    struct KeyFrame
    {
	    float			FramePos;			// フレーム位置
		LVector3	    Position;			// 位置
	    LQuaternion		Rotation;			// 回転(クォータニオン)

	    BezierTable	    PosXInterBezier;	// X軸移動補間
	    BezierTable	    PosYInterBezier;	// Y軸移動補間
	    BezierTable	    PosZInterBezier;	// Z軸移動補間
	    BezierTable	    RotInterBezier;	    // 回転補間
    };

public:
	BezierSQTTransformAnimation();
	virtual ~BezierSQTTransformAnimation();

public:

	/// キーフレーム追加
    void addKeyFrame(
        float framePos, const LVector3& pos, const LQuaternion& rot,
        char* interpolation_x,
	    char* interpolation_y,
	    char* interpolation_z,
	    char* interpolation_rot );

	/// キーフレームのソート
    void sortKeyFrame();

public:
	virtual void setTime( double time );
	virtual double getLastFramePos();
	virtual const LSQTTransform& getValue() const { return mTransform; }

private:
	typedef std::vector<KeyFrame>	KeyFrameArray;
	KeyFrameArray	mKeyFrameArray;
	LSQTTransform	mTransform;
};

} // namespace Animation
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================