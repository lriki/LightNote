//==============================================================================
// Manager 
//------------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include "../Base/ReferenceObjectArray.h"
#include "../Math/LMath.h"
#include "Types.h"

class btDynamicsWorld;
class  btDefaultCollisionConfiguration;
class  btCollisionDispatcher;
struct btDbvtBroadphase;
class  btAxisSweep3;
class  btSequentialImpulseConstraintSolver;
class  btDiscreteDynamicsWorld;
class btSoftRigidDynamicsWorld;
class btCollisionShape;
class btRigidBody;
class btTypedConstraint;
class btTriangleIndexVertexArray;
struct btSoftBodyWorldInfo;

class btCollisionObject;
class btManifoldPoint;
struct btCollisionObjectWrapper;

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Physics
{

//==============================================================================
// �� Manager �N���X
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

	/// �������f�[�^
	struct InitData
	{
        Base::LogFile*      LogFile;
    };

public:

    Manager();

    LN_REFOBJ_METHODS;

public:

	/// ������
    LNRESULT initialize( const InitData& init_data_ );

	/// �I������
    void finalize();

	/// �t���[�����[�g�̐ݒ� (�f�t�H���g�� 60)
    void setFrameRate( int frame_rate_ );

	/// [���C���X���b�h]
	void updateBefore();

	/// [�����X�V�X���b�h] �V�~�����[�V�����X�V
    void update( lnFloat elapsed_time_ );

	/// [���C���X���b�h]����
	void updateContents();

	/// �ڐG���Ă��镨�̂��`�F�b�N���ăC�x���g�𔭐�������
	//void updateContactPoints();

	/// �d�͂̐ݒ�
    void setGravity( const LVector3& gravity_ );

	/// ���b�V���̍쐬
    TriMesh* createStaticTriangleMesh( const LNTriangleMeshBodyInfo& info_, lnU16 group_ = 0xffff, lnU16 mask_ = 0xffff );

	/// ���b�V���̍쐬
    TriMesh* createStaticTriangleMesh( const LNTriangleMeshBodyInfo* info_, lnU16 info_count_, lnU16 group_ = 0xffff, lnU16 mask_ = 0xffff );

	/// ���C�L���X�g
	//bool rayTestSingle( const BodyBase* target_body_, const LVector3& from_, const LVector3& to_, LVector3* hit_pos_, LVector3* hit_normal_ );

	/// ���C�L���X�g
    bool rayCast( const LVector3& from_, const LVector3& to_, LVector3* hit_pos_, LVector3* hit_normal_, BodyBase** hit_body_, lnU16 group_ = 0xffff, lnU16 mask_ = 0xffff );

	/// �f�o�b�O�`��
    LNRESULT drawDebugShape( IDebugRenderer* renderer_ );

public:

	/// �������Z���[���h�̎擾
    btDynamicsWorld* getDynamicsWorld() const;

	void addRigidBody( RigidBody* body );
	void addCollisionBody( CollisionBody* body );
    void addJoint( JointBase* joint );

protected:

    virtual ~Manager();

private:
	//static bool CustomMaterialCombinerCallback( btManifoldPoint& Point, const btCollisionObject* pCollObj0, int partId0, int index0, const btCollisionObject* pCollObj1, int partId1, int index1 );
	static bool CustomMaterialCombinerCallback(btManifoldPoint& cp,	const btCollisionObjectWrapper* colObj0Wrap,int partId0,int index0,const btCollisionObjectWrapper* colObj1Wrap,int partId1,int index1);

private:
	// ���ꂼ�ꃏ�[���h�ւ̒ǉ����\�b�h���Ⴄ�̂ŕ����Ă���
	typedef Base::ReferenceObjectArray<RigidBody*>		RigidBodyArray;		// �\�t�g�{�f�B�Ƃ̓��[���h�ւ̒ǉ����@���Ⴄ�̂ŕ����Ă���
	typedef Base::ReferenceObjectArray<CollisionBody*>	CollisionBodyArray;
	typedef Base::ReferenceObjectArray<JointBase*>		JointArray;

private:

    Base::LogFile*              mLogFile;

    btDefaultCollisionConfiguration*        mBtCollisionConfig;
	btCollisionDispatcher*                  mBtCollisionDispatcher;
    btDbvtBroadphase*					    mBtBroadphase;
	//btAxisSweep3*                         mBtBroadphase;
	btSequentialImpulseConstraintSolver*    mBtSolver;
	btDiscreteDynamicsWorld*                mBtWorld;
	//btSoftRigidDynamicsWorld*               mBtWorld;

	//btSoftRigidDynamicsWorld*			            mBtWorld;
	//btSoftBodyWorldInfo							mBtSoftBodyWorldInfo;
	//btSoftBodyRigidBodyCollisionConfiguration*	mBtCollisionConfiguration;

    RigidBodyArray			mRigidBodyArrayList;
	CollisionBodyArray		mCollisionBodyArray;
    JointArray				mJointArray;

//#define LN_USE_PARALLEL

#ifdef LN_USE_PARALLEL
	class	btThreadSupportInterface*		m_threadSupportCollision;
	class	btThreadSupportInterface*		m_threadSupportSolver;
#endif

public:
    btSoftBodyWorldInfo*	mSoftBodyWorldInfo;

	lnU32	mLastUpdateTime;
	
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================