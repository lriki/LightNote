//==============================================================================
// JointBase 
//------------------------------------------------------------------------------
///**
//  @file       JointBase.h
//  @brief      JointBase
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Math/Matrix.h"
#include "../Manager.h"

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
// Å° JointBase ÉNÉâÉX
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class JointBase
    : public Base::ReferenceObject
{
public:
    JointBase();
    virtual ~JointBase();

public:
	btTypedConstraint*	getBtConstraint() { return mBtConstraint; }

protected:public:
    void create( Manager* manager, btTypedConstraint* constraint );

public:
	virtual BodyType getBodyType() const { return BodyType_Joint; }

protected:
    Manager*	        mManager;
    btTypedConstraint*  mBtConstraint;
};

//==============================================================================
// Å° DofSpringJoint
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class DofSpringJoint
    : public JointBase
{
public:
    DofSpringJoint();
    virtual ~DofSpringJoint();

public:

	/// çÏê¨
    void create( Manager* manager, RigidBody* body0, RigidBody* body1, const LMatrix& localOffset0, const LMatrix& localOffset1 );

	void enableSpring( int index, bool isEnable );
	void setStiffness( int index, float stiffness );
	void setDamping( int index, float damping );
	void setEquilibriumPoint();
	void setEquilibriumPoint( int index );

	void setLimit( int index, float low, float hi );
	void setLinearLowerLimit( const Math::Vector3& linearLower );
	void setLinearUpperLimit( const Math::Vector3& linearUpper );
	void setAngularLowerLimit( const Math::Vector3& angularLower );
	void setAngularUpperLimit( const Math::Vector3& angularUpper );

public:
	btGeneric6DofSpringConstraint*	mBtDofSpringConstraint;
	RigidBody*	mBody0;
	RigidBody*	mBody1;
};

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================