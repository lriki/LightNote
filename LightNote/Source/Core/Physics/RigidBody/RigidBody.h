//==============================================================================
// RigidBody 
//------------------------------------------------------------------------------
///**
//  @file       RigidBody.h
//  @brief      RigidBody
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Math/Quaternion.h"
#include "../BodyBase.h"

namespace LNote
{
namespace Core
{
namespace Physics
{

struct RigidBodyInitArgs
{
    btCollisionShape*   Shape;      ///< (BodyBase  �폜���� delete �����)
    btRigidBody*        RigidBody;  ///< (RigidBody �폜���� delete �����	PMD �����C��������폜�\��)
	//lnU16               Group;
	//lnU16               Mask;
	//float				Mass;
	//bool				AdditionalDamping;
	//bool				KinematicObject;	///< Kinematic�I�u�W�F�N�g�Ƃ��� (MotionState �� getWorldTransform() ���Ă΂��悤�ɂȂ�)

	RigidBodyInitArgs()
	{
		//AdditionalDamping = false;
		RigidBody = NULL;
		//KinematicObject = false;
	}
};

//==============================================================================
// �� RigidBody
//------------------------------------------------------------------------------
///**
//  @brief		���̂̃N���X
//*/
//==============================================================================
class RigidBody
    : public BodyBase
{
public:
	struct ConfigData
	{
		lnFloat			Mass;				///< ����
		lnU16			Group;				///< �Փ˃O���[�v
		lnU16			GroupMask;			///< ��Փ˃O���[�v
	    lnFloat			Restitution;	    ///< ������
	    lnFloat			Friction;		    ///< ���C��
		lnFloat			LinearDamping;	    ///< �ړ���
	    lnFloat			AngularDamping;		///< ��]��
		const LMatrix*	InitialTransform;	///< �����p�� (NULL �� Identity)
		bool			AdditionalDamping;	///< �����̗L��
		bool			KinematicObject;	///< Kinematic�I�u�W�F�N�g�Ƃ��� (���ʂ� 0.0 �Ƃ��Ĉ����AMotionState �� getWorldTransform() ���Ă΂��悤�ɂȂ�)

		ConfigData()
		{
			Group = 0xffff;
			GroupMask = 0xffff;
			Mass = 0.0;
			Restitution = 0.0;
			Friction = 0.5;
			LinearDamping = 0.0;
			AngularDamping = 0.0;
			InitialTransform = NULL;
			AdditionalDamping = false;
			KinematicObject = false;
		}
	};

public:
    RigidBody();
    virtual ~RigidBody();

public:

	/// ������ (���̂��󂯎���ă��[���h�ɒǉ�����) (���sPMD�p��public�B��� protected �ɂ���)
    void initialize( Manager* manager, const RigidBodyInitArgs& args, const ConfigData& configData );

#if 0
	/// �ʒu�̐ݒ�
	void setPosition( const LVector3& position );

	/// ��]�̐ݒ�
	void setRotation( const LQuaternion& rotation );

	/// ��]�p�x�̐ݒ�
	void setAngle( const LVector3& euler );

	/// �ړ��E��]�����l�̐ݒ�
    void setDamping( float linDamping, float angDamping );

	/// ���˗��̐ݒ�
    void setRestitution( float value_ );

	/// ���˗��̎擾
    float getRestitution() const;

	/// ���C�W���̐ݒ�
    void setFriction( float value_ );

	/// ���C�W���̎擾
    float getFriction() const;

	/// ���[���h�ϊ��s��̎擾
	const LMatrix& getWorldMatrix() const;

	/// (�L�l�}�e�B�b�N�ȍ��̗p setWorldMatrix())
	void setKinematicAlignmentMatrix( const LMatrix& matrix );
#endif
	void activate();

	/// ���[���h�ϊ��s��̐ݒ�
	void setWorldMatrix( const LMatrix& matrix );

	/// ���[���h�ϊ��s��̎擾
	const LMatrix& getWorldMatrix() const;

	/// �w��̎p���������I�ɐݒ肷�� (���x�� 0 �Ƀ��Z�b�g�����)
	void moveToForced( const LMatrix& matrix );

	/// �������Z�̑Ώۂł��邩 (false �̏ꍇ�A�Փ˔���̂ݑΏ�)
	bool isContactResponse() const { return true; }

public:
	void syncBeforeUpdate();
	void syncUpdating();

public:
	virtual BodyType getBodyType() const { return BodyType_RigidBody; }
	//virtual void preUpdate();
	//virtual void postUpdate();

public:
	btRigidBody*	getBtRigidBody() { return mBtRigidBody; }
	lnU16			getGroup() const { return mGroup; }
	lnU16			getGroupMask() const { return mGroupMask; }

protected:
	struct KinematicMotionState;

    btRigidBody*    mBtRigidBody;
	lnU16			mGroup;
	lnU16			mGroupMask;
    LMatrix         mWorldMatrix;   ///< (postUpdate() �Őݒ肳���)
	KinematicMotionState*	mKinematicMotionState;
	//LMatrix			mKinematicAlignmentMatrix;

	//LMatrix			mMoveToForcedMatrix;

	bool			mMoveToForced;
	bool			mSetWorldMatrix;
};

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================