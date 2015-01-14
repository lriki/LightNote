//==============================================================================
// RigidBody 
//==============================================================================

#include "stdafx.h"
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btMotionState.h>
#include "../BulletUtil.h"
#include "RigidBody.h"

namespace LNote
{
namespace Core
{
namespace Physics
{

	struct	DefaultMotionState : public btMotionState
{
	btTransform m_graphicsWorldTrans;
	btTransform	m_centerOfMassOffset;
	btTransform m_startWorldTrans;
	void*		m_userPointer;

	DefaultMotionState(const btTransform& startTrans = btTransform::getIdentity(),const btTransform& centerOfMassOffset = btTransform::getIdentity())
		: m_graphicsWorldTrans(startTrans),
		m_centerOfMassOffset(centerOfMassOffset),
		m_startWorldTrans(startTrans),
		m_userPointer(0)

	{
		
	}

	///synchronizes world transform from user to physics
	virtual void	getWorldTransform(btTransform& centerOfMassWorldTrans ) const 
	{
			centerOfMassWorldTrans = 	m_centerOfMassOffset.inverse() * m_graphicsWorldTrans ;

			//BulletUtil::dumpBtTransform(centerOfMassWorldTrans);
	}

	///synchronizes world transform from physics to user
	///Bullet only calls the update of worldtransform for active objects
	virtual void	setWorldTransform(const btTransform& centerOfMassWorldTrans)
	{
		//BulletUtil::dumpBtTransform(centerOfMassWorldTrans);

		/*printf("setWorldTransform() %f %f %f\n", 
			centerOfMassWorldTrans.getOrigin().x(), 
			centerOfMassWorldTrans.getOrigin().y(), 
			centerOfMassWorldTrans.getOrigin().z());
			m_graphicsWorldTrans = centerOfMassWorldTrans * m_centerOfMassOffset ;*/
			//if (LMath::isNaN( centerOfMassWorldTrans.getOrigin().x() ))
			//{
			//	int a = 0;
			//}

			/* ここで設定されるものと mBtRigidBody->getWorldTransform() で取得したものは、たとえシミュレーション直後であっても微妙に違う。
			const btMatrix3x3& r = m_graphicsWorldTrans.getBasis();
        const btVector3&   p = m_graphicsWorldTrans.getOrigin();
	    LMatrix m(
		    r[0].x(), r[1].x(), r[2].x(), 0.0f,
		    r[0].y(), r[1].y(), r[2].y(), 0.0f,
		    r[0].z(), r[1].z(), r[2].z(), 0.0f,
		    p.x(),   p.y(),    p.z(),    1.0f );

		printf("setWorldTransform()\n");
		m.cdump();
		*/
	}

	

};



struct RigidBody::KinematicMotionState
    : public btMotionState
{
	RigidBody* mBody;
	btTransform m_graphicsWorldTrans;

	KinematicMotionState( const btTransform& startTrans )
		: mBody					( NULL )
		, m_graphicsWorldTrans	( startTrans )
	{
	}

	virtual void getWorldTransform( btTransform& centerOfMassWorldTrans ) const
	{
		/* stepSimulation() から呼ばれる。非同期にする場合は注意 */
		//printf("getWorldTransform()\n");
		/*
		btTransform		bttrBoneTransform;

        LMatrix gl_mat = mFrame->getWorldMatrix();
        
		bttrBoneTransform.setFromOpenGLMatrix( (float *)(&gl_mat) );

		centerOfMassWorldTrans = bttrBoneTransform * m_BoneOffset;
		*/
		centerOfMassWorldTrans = m_graphicsWorldTrans;
	}

	virtual void setWorldTransform( const btTransform& centerOfMassWorldTrans )
	{
		//printf("setWorldTransform()\n");
		m_graphicsWorldTrans = centerOfMassWorldTrans;
	}

	void setMatrix( const LMatrix& matrix )
	{
		m_graphicsWorldTrans.setFromOpenGLMatrix( (btScalar*)&matrix );
	}
};


//==============================================================================
// ■ RigidBody
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    RigidBody::RigidBody()
        : BodyBase      ()
        , mBtRigidBody  ( NULL )
		, mGroup		( 0xffff )
		, mGroupMask	( 0xffff )
		, mKinematicMotionState	( NULL )
		, mMoveToForced			( false )
		, mSetWorldMatrix		( false )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    RigidBody::~RigidBody()
    {
//		printf("~RigidBody()\n");
		if ( mManager )
		{
			btMotionState* pState = this->mBtRigidBody->getMotionState();
			SAFE_DELETE( pState );

			btCollisionShape* pShape = this->mBtRigidBody->getCollisionShape();
			SAFE_DELETE( pShape );

			mManager->getDynamicsWorld()->removeCollisionObject( this->mBtRigidBody );
			SAFE_DELETE( this->mBtRigidBody );
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void RigidBody::initialize( Manager* manager, const RigidBodyInitArgs& args, const ConfigData& configData )
    {
		if (args.RigidBody == NULL)
		{
			btTransform transform;						// 初期姿勢

			if ( configData.InitialTransform )
			{
				mWorldMatrix = *configData.InitialTransform;
				//mWorldMatrix.m01 = configData.InitialTransform->m10;
				//mWorldMatrix.m02 = configData.InitialTransform->m20;
				//mWorldMatrix.m10 = configData.InitialTransform->m01;
				//mWorldMatrix.m12 = configData.InitialTransform->m21;
				//mWorldMatrix.m20 = configData.InitialTransform->m02;
				//mWorldMatrix.m21 = configData.InitialTransform->m12;
#if 0
				LMatrix m = mWorldMatrix;
				LMatrix::transformBasis( &m );
				transform.setIdentity();
				transform.setFromOpenGLMatrix( (const btScalar*)&m );
#else
				//mWorldMatrix.cdump();
				transform.setFromOpenGLMatrix( (const btScalar*)&mWorldMatrix );
#endif
				transform.getOpenGLMatrix( (btScalar*)&mWorldMatrix );
				//mWorldMatrix.cdump();
			}
			else
			{
				transform.setIdentity();
				mWorldMatrix = LMatrix::Identity;
			}

			// 慣性テンソルを計算して localInertia に格納
			float mass = 0.0f;
			btVector3 localInertia( 0.0f, 0.0f, 0.0f );	// 慣性
			if ( configData.KinematicObject == false ) mass = configData.Mass;
			if ( mass != 0.0f ) args.Shape->calculateLocalInertia( mass, localInertia );

			//BulletUtil::dumpBtVector3(transform.getBasis().getRow(0));
			//BulletUtil::dumpBtTransform(transform);

			// 剛体の作成
			btMotionState* motionState;
			if ( configData.KinematicObject )
			{
				mKinematicMotionState = new KinematicMotionState( transform );
				motionState = mKinematicMotionState;
				mKinematicMotionState->setMatrix( *configData.InitialTransform );
				//if ( configData.InitialTransform ) setKinematicAlignmentMatrix( *configData.InitialTransform );
			}
			else
			{
				motionState = new DefaultMotionState( transform );
			}
			btRigidBody::btRigidBodyConstructionInfo bodyInfo( mass, motionState, args.Shape, localInertia );
			bodyInfo.m_linearDamping  = configData.LinearDamping;	    // 移動減
			bodyInfo.m_angularDamping = configData.AngularDamping;	// 回転減
			bodyInfo.m_restitution    = configData.Restitution;	    // 反発力
			bodyInfo.m_friction       = configData.Friction;		    // 摩擦力
			bodyInfo.m_additionalDamping = configData.AdditionalDamping;

			btRigidBody* rigidBody = new btRigidBody( bodyInfo );
			//rigidBody->setCollisionFlags( rigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK );
//			rigidBody->setMassProps();
			this->mBtRigidBody = rigidBody;
		}
		else
		{
			this->mBtRigidBody = args.RigidBody;
		}


		if ( configData.KinematicObject )
		{
			// CF_KINEMATIC_OBJECT と DISABLE_DEACTIVATION はセット。決まり事。
			// http://bulletjpn.web.fc2.com/07_RigidBodyDynamics.html
			this->mBtRigidBody->setCollisionFlags( /*this->mBtRigidBody->getCollisionFlags() | */btCollisionObject::CF_KINEMATIC_OBJECT );
            this->mBtRigidBody->setActivationState( /*this->mBtRigidBody->getActivationState() | */DISABLE_DEACTIVATION );
		}
		else
		{
			this->mBtRigidBody->setActivationState( /*this->mBtRigidBody->getActivationState() | */DISABLE_DEACTIVATION );
		}
		this->mBtRigidBody->setSleepingThresholds( 0.0f, 0.0f );

		this->mGroup = configData.Group;
		this->mGroupMask = configData.GroupMask;

        BodyBase::initialize( manager, args.Shape, mBtRigidBody );
		mManager->addRigidBody( this );
    }

#if 0
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void RigidBody::setPosition( const LVector3& position )
	{
		btVector3 pos = BulletUtil::LNVector3ToBtVector3( position );

		if ( mBtRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT )
		{
			btTransform Transform = mBtRigidBody->getWorldTransform();
			Transform.setOrigin( pos );
			mBtRigidBody->setWorldTransform( Transform );
		}
		else
		{
			mBtRigidBody->getWorldTransform().setOrigin( pos );
		}
		mBtRigidBody->activate( true );

		mBtRigidBody->getWorldTransform().getOpenGLMatrix((btScalar*)&mWorldMatrix);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void RigidBody::setRotation( const LQuaternion& rotation )
	{
		btQuaternion q = BulletUtil::LNQuaternionToBtQuaternion( rotation );
		mBtRigidBody->getWorldTransform().setRotation( q );

		
		//mBtRigidBody->getWorldTransform().setBasis().
		//btTransform transform = mBtRigidBody->getCenterOfMassTransform().setRotation( q );
		//transform.setRotation( q );
		//mBtRigidBody

		mBtRigidBody->getWorldTransform().getOpenGLMatrix((btScalar*)&mWorldMatrix);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void RigidBody::setAngle( const LVector3& euler )
	{
		btMatrix3x3 m;
		//m.setEulerZYX( euler.x, euler.y, euler.z );
		m.setEulerYPR( euler.y, euler.x, euler.z );
#if 1
		mBtRigidBody->getWorldTransform().setBasis(m);
#else
		

		btTransform transform = mBtRigidBody->getCenterOfMassTransform();
		transform.setBasis( m );

		mBtRigidBody->setCenterOfMassTransform( transform );
		//mBtRigidBody->getWorldTransform().setBasis( m );
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void RigidBody::setDamping( float linDamping, float angDamping )
	{
		mBtRigidBody->setDamping( linDamping, angDamping );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void RigidBody::setRestitution( float value ) 
	{ 
		return mBtRigidBody->setRestitution( value ); 
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    float RigidBody::getRestitution() const 
	{ 
		return mBtRigidBody->getRestitution(); 
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void RigidBody::setFriction( float value ) 
	{ 
		mBtRigidBody->setFriction( value ); 
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    float RigidBody::getFriction() const 
	{ 
		return mBtRigidBody->getFriction(); 
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	const LMatrix& RigidBody::getWorldMatrix() const
	{
		return mWorldMatrix;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void RigidBody::setKinematicAlignmentMatrix( const LMatrix& matrix )
	{
		mKinematicAlignmentMatrix = matrix;
		if ( mKinematicMotionState )
		{
#if 0
			LMatrix m = mKinematicAlignmentMatrix;
			LMatrix::transformBasis( &m );
			mKinematicMotionState->setMatrix( m );
#else
			mKinematicMotionState->setMatrix( mKinematicAlignmentMatrix );
#endif
		}
	}
#endif
	
	
	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
	void RigidBody::activate()
	{
		mBtRigidBody->activate( true );
	}

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
	void RigidBody::setWorldMatrix( const LMatrix& matrix )
	{ 
		mWorldMatrix = matrix; 
		mSetWorldMatrix = true; 
		
#if 0
		mWorldMatrix = matrix;

		if ( mKinematicMotionState )
		{
			mKinematicMotionState->setMatrix( mWorldMatrix );
		}
		else
		{
			btTransform transform;
			transform.setFromOpenGLMatrix( (btScalar*)&mWorldMatrix );
			mBtRigidBody->setCenterOfMassTransform( transform );
		}
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	const LMatrix& RigidBody::getWorldMatrix() const
	{
		return mWorldMatrix;
#if 0
		if ( mKinematicMotionState )
		{
			// static オブジェクトなので受け取る必要はない
		}
		else
		{
			mBtRigidBody->getWorldTransform().getOpenGLMatrix((btScalar*)&mWorldMatrix);
		}
		return mWorldMatrix;
#endif
	}

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
	void RigidBody::moveToForced( const LMatrix& matrix )
	{
		setWorldMatrix( matrix );
		mMoveToForced = true;
#if 0
		setWorldMatrix( matrix );
		
		mBtRigidBody->setLinearVelocity( btVector3( 0.0f, 0.0f, 0.0f ) );
		mBtRigidBody->setAngularVelocity( btVector3( 0.0f, 0.0f, 0.0f ) );
		mBtRigidBody->setInterpolationLinearVelocity( btVector3( 0.0f, 0.0f, 0.0f ) );
		mBtRigidBody->setInterpolationAngularVelocity( btVector3( 0.0f, 0.0f, 0.0f ) );
		mBtRigidBody->setInterpolationWorldTransform( mBtRigidBody->getCenterOfMassTransform() );
		mBtRigidBody->clearForces();
		mBtRigidBody->activate( true );

		//mMoveToForced = true;
		//mSetWorldMatrix = true;
#endif
	}
	
	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
	void RigidBody::syncBeforeUpdate()
	{
		// ユーザーから設定されている姿勢がある
		if ( mSetWorldMatrix )
		{
			if ( mKinematicMotionState )
			{
				mKinematicMotionState->setMatrix( mWorldMatrix );
			}
			else
			{
				btTransform transform;
				transform.setFromOpenGLMatrix( (btScalar*)&mWorldMatrix );
				mBtRigidBody->setCenterOfMassTransform( transform );
			}

			if ( mMoveToForced )
			{
				mBtRigidBody->setLinearVelocity( btVector3( 0.0f, 0.0f, 0.0f ) );
				mBtRigidBody->setAngularVelocity( btVector3( 0.0f, 0.0f, 0.0f ) );
				mBtRigidBody->setInterpolationLinearVelocity( btVector3( 0.0f, 0.0f, 0.0f ) );
				mBtRigidBody->setInterpolationAngularVelocity( btVector3( 0.0f, 0.0f, 0.0f ) );
				mBtRigidBody->setInterpolationWorldTransform( mBtRigidBody->getCenterOfMassTransform() );
				mBtRigidBody->clearForces();
			}
		}

		mMoveToForced = false;
		mSetWorldMatrix = false;
	}

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
	void RigidBody::syncUpdating()
	{
#if 1
		// 非同期更新中に set が発生した
		if ( false )//mSetWorldMatrix )
		{
			// 各種 set フラグは維持。次の syncBeforeUpdate() で再設定させる。
			// そのほかも、Bullet からは受け取らずに維持する。
			// → set された方が新しいはず
		}
		// ユーザーから設定されている姿勢が無いので、演算結果を受け取る
		else
		{
			if ( mKinematicMotionState )
			{
				// static オブジェクトなので受け取る必要はない
			}
			else
			{
				mBtRigidBody->getWorldTransform().getOpenGLMatrix((btScalar*)&mWorldMatrix);
			}
		}
#endif
	}

	////----------------------------------------------------------------------
	//// 
	////----------------------------------------------------------------------
 //   void RigidBody::preUpdate()
 //   {
 //   }

	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
 //   void RigidBody::postUpdate()
 //   {
 //       /*
 //       btTransform t;
 //       mBtRigidBody->getMotionState()->getWorldTransform( t );

 //       const btMatrix3x3& r = t.getBasis();//mBtRigidBody->getCenterOfMassTransform().getBasis();
 //       const btVector3&   p = t.getOrigin();//mBtRigidBody->getCenterOfMassTransform().getOrigin();
 //       */
	//	/*
 //       const btMatrix3x3& r = mBtRigidBody->getWorldTransform().getBasis();
 //       const btVector3&   p = mBtRigidBody->getWorldTransform().getOrigin();
	//    mWorldMatrix.set(
	//	    r[0].x(), r[1].x(), r[2].x(), 0.0f,
	//	    r[0].y(), r[1].y(), r[2].y(), 0.0f,
	//	    r[0].z(), r[1].z(), r[2].z(), 0.0f,
	//	    p.x(),   p.y(),    p.z(),    1.0f );
	//		*/
 //   }


} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================