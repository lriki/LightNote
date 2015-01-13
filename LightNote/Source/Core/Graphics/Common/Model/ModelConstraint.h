//==============================================================================
// ModelConstraint 
//------------------------------------------------------------------------------
///**
//  @file       ModelConstraint.h
//  @brief      ModelConstraint
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../Base/RefString.h"
#include "../../../Physics/Joint/JointBase.h"
#include "PMDTypes.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ModelJoint
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelJoint
{
public:
    ModelJoint();
    virtual ~ModelJoint();

public:

	/// 作成
    void create( 
		Model2* ownerModel, 
		ModelConstraintCore* jointCore );

	void updateBeforePhysics();

private:

	Model2*						mOwnerModel;
	ModelConstraintCore*		mJointCore;
	Physics::DofSpringJoint*	mDofSpringJoint;
	float						mRigidBodyDistance;		///< 接合する2つの剛体の距離 (A - B)
};


class ModelRigidBody;

//==============================================================================
// ■ ModelConstraint
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelConstraint
	//: public Physics::JointBase
{
public:
    ModelConstraint();
    virtual ~ModelConstraint();

public:

	/// 初期化
    void initializePMD( ModelManager* manager_, const PMD_Constraint* pmd_constraint_, ModelRigidBody* body_a_, ModelRigidBody* body_b_ );

private:
	Physics::DofSpringJoint* mDofSpringJoint;
	//btGeneric6DofSpringConstraint*  mBtDofConstraint;   ///< (Physics::JointBase に管理されるので delete の必要はナシ)

};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================