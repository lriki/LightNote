//==============================================================================
// JointBase 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <btBulletDynamicsCommon.h>
#include "../BulletUtil.h"
#include "../RigidBody/RigidBody.h"
#include "JointBase.h"

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
// ■ JointBase クラス
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    JointBase::JointBase()
        : mManager      ( NULL )
        , mBtConstraint ( NULL )
    {
        
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    JointBase::~JointBase()
    {
        if ( mManager )
        {
            if ( mBtConstraint )
            {
                mManager->getDynamicsWorld()->removeConstraint( mBtConstraint );
                SAFE_DELETE( mBtConstraint );
            }
            LN_SAFE_RELEASE( mManager );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void JointBase::create( Manager* manager, btTypedConstraint* constraint )
    {
        mManager = manager;
        LN_SAFE_ADDREF( mManager );
		
        mBtConstraint = constraint;

        mManager->addJoint( this );
    }

//==============================================================================
// ■ DofSpringJoint
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DofSpringJoint::DofSpringJoint()
		: mBtDofSpringConstraint	( NULL )
		, mBody0					( NULL )
		, mBody1					( NULL )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	DofSpringJoint::~DofSpringJoint()
	{
		// コンストレイントを先に解放する
		if ( mBtConstraint )
        {
            mManager->getDynamicsWorld()->removeConstraint( mBtConstraint );
            SAFE_DELETE( mBtConstraint );
        }

		LN_SAFE_RELEASE( this->mBody0 );
		LN_SAFE_RELEASE( this->mBody1 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DofSpringJoint::create( Manager* manager, RigidBody* body0, RigidBody* body1, const LMatrix& localOffset0, const LMatrix& localOffset1 )
	{
		// コンストレイントは接続している剛体を削除する前に削除する必要があるので、
		// 接続する剛体を覚えておく
		LN_REFOBJ_SET( this->mBody0, body0 );
		LN_REFOBJ_SET( this->mBody1, body1 );

#if 0
		LMatrix m0 = localOffset0;
		LMatrix m1 = localOffset1;
		LMatrix::transformBasis( &m0 );
		LMatrix::transformBasis( &m1 );
		btTransform local0, local1;
		local0.setFromOpenGLMatrix( (const btScalar*)&m0 );
		local1.setFromOpenGLMatrix( (const btScalar*)&m1 );
#else
		btTransform local0, local1;
		local0.setFromOpenGLMatrix( (const btScalar*)&localOffset0 );
		local1.setFromOpenGLMatrix( (const btScalar*)&localOffset1 );
#endif
		this->mBtDofSpringConstraint = new btGeneric6DofSpringConstraint(
            *body0->getBtRigidBody(),
            *body1->getBtRigidBody(),
		    local0,
            local1,
            true );

		JointBase::create( manager, mBtDofSpringConstraint );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DofSpringJoint::enableSpring( int index, bool isEnable )
	{
		this->mBtDofSpringConstraint->enableSpring( index, isEnable );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DofSpringJoint::setStiffness( int index, float stiffness )
	{
		this->mBtDofSpringConstraint->setStiffness( index, stiffness );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DofSpringJoint::setDamping( int index, float damping )
	{
		this->mBtDofSpringConstraint->setDamping( index, damping );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DofSpringJoint::setEquilibriumPoint()
	{
		this->mBtDofSpringConstraint->calculateTransforms();
		this->mBtDofSpringConstraint->setEquilibriumPoint();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DofSpringJoint::setEquilibriumPoint( int index )
	{
		this->mBtDofSpringConstraint->setEquilibriumPoint( index );
	}


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DofSpringJoint::setLimit( int index, float low, float hi )
	{
		this->mBtDofSpringConstraint->setLimit( index, low, hi );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DofSpringJoint::setLinearLowerLimit( const Math::Vector3& linearLower )
	{
		this->mBtDofSpringConstraint->setLinearLowerLimit(
			BulletUtil::LNVector3ToBtVector3( linearLower ) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DofSpringJoint::setLinearUpperLimit( const Math::Vector3& linearUpper )
	{
		this->mBtDofSpringConstraint->setLinearUpperLimit(
			BulletUtil::LNVector3ToBtVector3( linearUpper ) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DofSpringJoint::setAngularLowerLimit( const Math::Vector3& angularLower )
	{
		this->mBtDofSpringConstraint->setAngularLowerLimit(
			BulletUtil::LNVector3ToBtVector3( angularLower ) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DofSpringJoint::setAngularUpperLimit( const Math::Vector3& angularUpper )
	{
		this->mBtDofSpringConstraint->setAngularUpperLimit(
			BulletUtil::LNVector3ToBtVector3( angularUpper ) );
	}


} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================