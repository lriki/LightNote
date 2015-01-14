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
// �� SQTTransformAnimation
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

	/// �l�̎擾
    virtual const LSQTTransform& getValue() const = 0;

public:
	virtual ValueType getValueType() const { return ValueType_SQTTransform; }
};

//==============================================================================
// �� BezierSQTTransformAnimation
//------------------------------------------------------------------------------
///**
//  @brief      VMD�p
//*/
//==============================================================================
class BezierSQTTransformAnimation
	: public SQTTransformAnimation
{
public:

    struct KeyFrame
    {
	    float			FramePos;			// �t���[���ʒu
		LVector3	    Position;			// �ʒu
	    LQuaternion		Rotation;			// ��](�N�H�[�^�j�I��)

	    BezierTable	    PosXInterBezier;	// X���ړ����
	    BezierTable	    PosYInterBezier;	// Y���ړ����
	    BezierTable	    PosZInterBezier;	// Z���ړ����
	    BezierTable	    RotInterBezier;	    // ��]���
    };

public:
	BezierSQTTransformAnimation();
	virtual ~BezierSQTTransformAnimation();

public:

	/// �L�[�t���[���ǉ�
    void addKeyFrame(
        float framePos, const LVector3& pos, const LQuaternion& rot,
        char* interpolation_x,
	    char* interpolation_y,
	    char* interpolation_z,
	    char* interpolation_rot );

	/// �L�[�t���[���̃\�[�g
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