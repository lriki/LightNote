//==============================================================================
// ModelIKLimitter
//------------------------------------------------------------------------------
///**
//  @file       ModelIKLimitter.h
//  @brief      ModelIKLimitter
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Common.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ModelIKLimitter
//------------------------------------------------------------------------------
///**
//  @brief		MMDX �X�^�C���� IK ���~�b�^
//*/
//==============================================================================
class ModelIKLimitter2
	: public Base::ReferenceObject
{
public:
	ModelIKLimitter2();
    virtual ~ModelIKLimitter2() {}

public:
	void setRotationLimits( const LVector3& min, const LVector3& max );
	void setRotationRestitution( bool x, bool y, bool z, const LVector3& restitution );
	float adjustRotation( float value, int index );

	// �{�[���ւ̐����K�p(�󂯎���� SQTTransform ����x�p�x�ɕ������A�����������Ė߂�)
	void adjustRotation( LSQTTransform* localTransform );	

	void setAxisLimits( bool x, bool y, bool z );
	void adjustAxisLimits( LVector3* rot );

public:

	LVector3	RotationMin;	///< �ŏ���]
	LVector3	RotationMax;	///< �ő��]
	bool		Mirror[3];		///< �p�x�̔��˒����@�\�g�p�t���O
	LVector3	Restitution;	///< �p�x�̔��˒����̔����W��

	bool		AxisLimits[3];	///< X,Y,Z ���ɑΉ��Btrue �̏ꍇ�A�p�x�� 0 �ɂ���

	bool		EnableRotationLimit;
	bool		EnableAxisLimit;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
