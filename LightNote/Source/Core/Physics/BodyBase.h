//==============================================================================
// BodyBase 
//------------------------------------------------------------------------------
///**
//  @file       BodyBase.h
//  @brief      BodyBase
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Math/LMath.h"
#include "IDebugRenderer.h"
#include "Manager.h"

class btCollisionObject;

namespace LNote
{
namespace Core
{
namespace Physics
{

//==============================================================================
// �� BodyBase
//------------------------------------------------------------------------------
///**
//  @brief		���́E�\�t�g�{�f�B�̊��N���X
//*/
//==============================================================================
class BodyBase
    : public Base::ReferenceObject
{
public:
    BodyBase();
    virtual ~BodyBase();

public:
	/// �ڐG�_�̐�
	int getContactCount() const { return mContactList.size(); }

	/// �ڐG�_�̎擾
	const ContactInfo& getContact( int index ) const { return mContactList[index]; }

	/// ���[�U�[�f�[�^�̐ݒ�
	void setUserData( void* data ) { mUserData = data; }

	/// ���[�U�[�f�[�^�̎擾
	void* getUserData() const { return mUserData; }

public:

	/// ������
    void initialize( Manager* manager, btCollisionShape* shape, btCollisionObject* obj );

	/// ���
	virtual BodyType getBodyType() const = 0;

	///// �V�~�����[�V�����X�V���O�̏���
	//virtual void preUpdate() {}

	///// �V�~�����[�V�����X�V����̏���
	//virtual void postUpdate() {}

	/// �f�o�b�O�p�̌`��`��
    virtual void drawDebugShape( IDebugRenderer* renderer ) {}

public:
	void clearContactList() { mContactList.clear(); }
	void addContact( const ContactInfo& contact ) { mContactList.push_back( contact ); }

protected:
    Manager*				    mManager;

private:
	//btCollisionShape*		    mCollisionShape;
	void*				        mUserData;
	ContactInfoArray			mContactList;
};

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================