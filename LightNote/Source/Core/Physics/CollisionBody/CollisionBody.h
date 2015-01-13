//==============================================================================
// CollisionBody 
//------------------------------------------------------------------------------
///**
//  @file       CollisionBody.h
//  @brief      CollisionBody
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../BodyBase.h"

namespace LNote
{
namespace Core
{
namespace Physics
{

//==============================================================================
// �� CollisionBody
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class CollisionBody
    : public BodyBase
{
public:
    CollisionBody();
    virtual ~CollisionBody();

protected:
    void create( Manager* manager, btCollisionObject* obj, lnU16 group, lnU16 mask );

public:

	///// �ʒu�̐ݒ�
	//void setPosition( const LVector3& position );

	///// ��]�̐ݒ�
	//void setRotation( const LQuaternion& rotation );

	///// ��]�p�x�̐ݒ�
	//void setAngle( const LVector3& euler );

	///// �ړ��E��]�����l�̐ݒ�
 //   void setDamping( float linDamping, float angDamping );

 //   /// ���˗��̐ݒ�
 //   void setRestitution( float value_ );

 //   /// ���˗��̎擾
 //   float getRestitution() const;

 //   /// ���C�W���̐ݒ�
 //   void setFriction( float value_ );

 //   /// ���C�W���̎擾
 //   float getFriction() const;

	///// ���[���h�ϊ��s��̎擾
	//const LMatrix& getWorldMatrix() const;

	///// (�L�l�}�e�B�b�N�ȍ��̗p setWorldMatrix())
	//void setKinematicAlignmentMatrix( const LMatrix& matrix );

	///// �w��̎p���������I�ɐݒ肷�� (���x�� 0 �Ƀ��Z�b�g�����)
	//void moveToForced( const LMatrix& matrix );

public:
	virtual BodyType getBodyType() const { return BodyType_CollisionBody; }

public:
	btCollisionObject*	getBtCollisionObject() { return mBtCollisionObject; }
	lnU16				getGroup() const { return mGroup; }
	lnU16				getGroupMask() const { return mGroupMask; }

protected:
    btCollisionObject*	mBtCollisionObject;
	lnU16				mGroup;
	lnU16				mGroupMask;
	//LMatrix				mWorldMatrix;
	bool				mCollisionEnable;
};

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================