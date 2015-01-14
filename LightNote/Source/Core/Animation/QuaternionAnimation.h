//==============================================================================
// QuaternionAnimation 
//------------------------------------------------------------------------------
///**
//  @file       QuaternionAnimation.h
//  @brief      QuaternionAnimation
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
// �� QuaternionAnimationBase
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class QuaternionAnimationBase
	: public AnimationTimelineBase
{
public:
	virtual ~QuaternionAnimationBase() {}

public:

	/// �l�̎擾
    virtual const LQuaternion& getValue() const = 0;

public:
	virtual ValueType getValueType() const { return ValueType_Quaternion; }
};

//==============================================================================
// �� BezierAnimation
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class BezierAnimation
	: public QuaternionAnimationBase
{
public:

    struct KeyFrame
    {
	    float	    FramePos;		// �t���[���ʒu
		LQuaternion	Rotation;	    // ��](�N�H�[�^�j�I��)
		BezierTable	RotInterBezier;	// ��]���
    };

public:
	BezierAnimation();
	virtual ~BezierAnimation();

public:

	/// �L�[�t���[���ǉ� (VMD �p)
    void addKeyFrame( float framePos, const LQuaternion& rot, char* interpolation );

	/// �L�[�t���[���̃\�[�g
    void sortKeyFrame();

public:
	virtual void setTime( double time );
	virtual double getLastFramePos();
	virtual const LQuaternion& getValue() const { return mQuaternion; }

private:
	typedef std::vector<KeyFrame>	KeyFrameArray;
	KeyFrameArray	mKeyFrameArray;
	LQuaternion		mQuaternion;
};

} // namespace Animation
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================