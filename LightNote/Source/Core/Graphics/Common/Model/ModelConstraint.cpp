//==============================================================================
// ModelConstraint 
//==============================================================================

#include "stdafx.h"
#include <btBulletDynamicsCommon.h>
#include "Core/ModelCore.h"
#include "Core/ModelConstraintCore.h"
#include "ModelManager.h"
#include "ModelRigidBody.h"
#include "ModelConstraint.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ModelJoint
//==============================================================================


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelJoint::ModelJoint()
		: mDofSpringJoint	( NULL )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelJoint::~ModelJoint()
	{
		LN_SAFE_RELEASE( mDofSpringJoint );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ModelJoint::create( 
		Model2* ownerModel, 
		ModelConstraintCore* jointCore )
	{	
		mOwnerModel = ownerModel;
		mJointCore = jointCore;

		ModelRigidBody2* bodyA = mOwnerModel->getRigidBody( mJointCore->RigidBodyAIndex );
		ModelRigidBody2* bodyB = mOwnerModel->getRigidBody( mJointCore->RigidBodyBIndex );

		// モデルのローカル空間内でのジョイントの位置と姿勢
		LMatrix jointOffset;
#if 1
		jointOffset = LMatrix::RotationYawPitchRoll(
			jointCore->Rotation.Y,
			jointCore->Rotation.X,
			jointCore->Rotation.Z );
		jointOffset.Translation(
			jointCore->Position.X,
			jointCore->Position.Y,
			jointCore->Position.Z );
#else
		LMatrix bias;
		bias.rotationZ( jointCore->Rotation.z );
		bias.rotationX( jointCore->Rotation.x );
		bias.rotationY( jointCore->Rotation.y );
		jointOffset.translation(
			jointCore->Position.x,
			jointCore->Position.y,
			jointCore->Position.z);
#endif

		LMatrix worldInvA = bodyA->getRigidBody()->getWorldMatrix();
		LMatrix worldInvB = bodyB->getRigidBody()->getWorldMatrix();
		worldInvA.Inverse();
		worldInvB.Inverse();
		
		// 各剛体のローカルから見たジョイント位置
		LMatrix frameInA = jointOffset * worldInvA;
		LMatrix frameInB = jointOffset * worldInvB;

		// ジョイント作成
		mDofSpringJoint = LN_NEW Physics::DofSpringJoint();
		mDofSpringJoint->create(
			ownerModel->getModelCore()->Manager->getPhysicsManager(),
			bodyA->getRigidBody(),
            bodyB->getRigidBody(),
			frameInA,
			frameInB );

		// 各種制限パラメータのセット
	    mDofSpringJoint->setLinearLowerLimit( jointCore->PositionLimitLower );
	    mDofSpringJoint->setLinearUpperLimit( jointCore->PositionLimitUpper );
		mDofSpringJoint->setAngularLowerLimit( jointCore->RotationLimitLower );
		mDofSpringJoint->setAngularUpperLimit( jointCore->RotationLimitUpper );
		
		const LVector3& springPos = jointCore->SpringPositionStiffness;
		
		// 0 : translation X
	    if ( springPos.X != 0.0f )
	    {
		    mDofSpringJoint->enableSpring( 0, true );
		    mDofSpringJoint->setStiffness( 0, springPos.X );
	    }

		// 1 : translation Y
	    if ( springPos.Y != 0.0f )
	    {
		    mDofSpringJoint->enableSpring( 1, true );
		    mDofSpringJoint->setStiffness( 1, springPos.Y );
	    }

		// 2 : translation Z
	    if ( springPos.Z != 0.0f )
	    {
		    mDofSpringJoint->enableSpring( 2, true );
		    mDofSpringJoint->setStiffness( 2, springPos.Z );
	    }

		// 3 : rotation X (3rd Euler rotational around new position of X axis, range [-PI+epsilon, PI-epsilon] )
		// 4 : rotation Y (2nd Euler rotational around new position of Y axis, range [-PI/2+epsilon, PI/2-epsilon] )
		// 5 : rotation Z (1st Euler rotational around Z axis, range [-PI+epsilon, PI-epsilon] )
	    mDofSpringJoint->enableSpring( 3, true );	
		mDofSpringJoint->setStiffness( 3, jointCore->SpringRotationStiffness.X );

	    mDofSpringJoint->enableSpring( 4, true );	
		mDofSpringJoint->setStiffness( 4, jointCore->SpringRotationStiffness.Y );

	    mDofSpringJoint->enableSpring( 5, true );	
		mDofSpringJoint->setStiffness( 5, jointCore->SpringRotationStiffness.Z );

		//springPos.dump();

		//mDofSpringJoint->calculateTransforms();
		mDofSpringJoint->setEquilibriumPoint();


		
		const LVector3 posA = bodyA->getRigidBody()->getWorldMatrix().GetPosition();
		const LVector3 posB = bodyB->getRigidBody()->getWorldMatrix().GetPosition();
		mRigidBodyDistance = (posA - posB).GetLength();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelJoint::updateBeforePhysics()
	{
		// A B が極端に離れていた場合は補正する

		ModelRigidBody2* bodyA = mOwnerModel->getRigidBody( mJointCore->RigidBodyAIndex );
		ModelRigidBody2* bodyB = mOwnerModel->getRigidBody( mJointCore->RigidBodyBIndex );
		const LVector3 posA = bodyA->getRigidBody()->getWorldMatrix().GetPosition();
		const LVector3 posB = bodyB->getRigidBody()->getWorldMatrix().GetPosition();

		float d = (posA - posB).GetLength();
		if ( d - mRigidBodyDistance > 2.0f )	// 最大 2.0。これ以上離さない
		{
			LVector3 diff = posA - posB;		// B にこれを乗算すれば、Aになる
			diff.Normalize();
			diff *= (d - mRigidBodyDistance);	// これを最大移動量にする

			LMatrix matB = bodyB->getRigidBody()->getWorldMatrix();
			matB.Translation( diff );
			bodyB->getRigidBody()->moveToForced( matB );
		}
	}

//==============================================================================
// ■ ModelConstraint
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelConstraint::ModelConstraint()
        : mDofSpringJoint	( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelConstraint::~ModelConstraint()
    {
		LN_SAFE_RELEASE( mDofSpringJoint );
    }


	void btTransformToLMatrix( LMatrix* out, const btTransform* in )
	{
		in->getOpenGLMatrix( (btScalar*)out );
		/*
		out->m00 = in->getBasis().getRow( 0 ).x();
		out->m01 = in->getBasis().getRow( 1 ).x();
		out->m02 = in->getBasis().getRow( 2 ).x();
		out->m03 = 0;
		out->m10 = in->getBasis().getRow( 0 ).y();
		out->m11 = in->getBasis().getRow( 1 ).y();
		out->m12 = in->getBasis().getRow( 2 ).y();
		out->m13 = 0;
		out->m20 = in->getBasis().getRow( 0 ).z();
		out->m21 = in->getBasis().getRow( 1 ).z();
		out->m22 = in->getBasis().getRow( 2 ).z();
		out->m23 = 0;
		out->m30 = in->getOrigin().x();
		out->m31 = in->getOrigin().y();
		out->m32 = in->getOrigin().z();
		out->m33 = 1;
		*/
	}

	void LMatrixTobtTransform( btTransform* out, const LMatrix& in )
	{
		btMatrix3x3 btm(
			in.M[0][0], in.M[1][0], in.M[2][0],
			in.M[0][1], in.M[1][1], in.M[2][1],
			in.M[0][2], in.M[1][2], in.M[2][2]);
		*out = btTransform(btm, btVector3(in.M[3][0], in.M[3][1], in.M[3][2]));
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ModelConstraint::initializePMD( ModelManager* manager_, const PMD_Constraint* pmd_constraint_, ModelRigidBody* body_a_, ModelRigidBody* body_b_ )
    {
		// モデルのローカル空間内でのジョイントの位置と姿勢
		LMatrix jointOffset;
		//jointOffset.rotation( 
		//	-pmd_constraint_->vec3Rotation.x,	// LMatrix は左手ベースなので符号反転
		//	-pmd_constraint_->vec3Rotation.y, 	// LMatrix は左手ベースなので符号反転
		//	pmd_constraint_->vec3Rotation.z,
		//	LN_ROTORDER_ZYX );
		//D3DXMatrixRotationYawPitchRoll(
		//	(D3DXMATRIX*)&jointOffset, 
		//	pmd_constraint_->vec3Rotation.y, 
		//	pmd_constraint_->vec3Rotation.x, 
		//	pmd_constraint_->vec3Rotation.z );
		LMatrix bias;
		bias.RotationZ( pmd_constraint_->vec3Rotation.Z );
		bias.RotationX( pmd_constraint_->vec3Rotation.X );
		bias.RotationY( pmd_constraint_->vec3Rotation.Y );
		jointOffset.Translation(pmd_constraint_->vec3Position);
//#pragma comment (lib, "d3dx9.lib");

		//jointOffset.cdump();

		

#if 0
		LMatrix worldInvA;
		worldInvA.translation(body_a_->getRigidBody()->getWorldMatrix().getPosition());
		LMatrix worldInvB;
		worldInvB.translation(body_b_->getRigidBody()->getWorldMatrix().getPosition());
#else
		LMatrix worldInvA = body_a_->getRigidBody()->getWorldMatrix();
		LMatrix worldInvB = body_b_->getRigidBody()->getWorldMatrix();
#endif
		//worldInvA.cdump();
		//btTransform		mmm = body_a_->getBtRigidBody()->getWorldTransform();
		//btTransformToLMatrix( &worldInvA, &body_a_->getBtRigidBody()->getWorldTransform() );
		//btTransformToLMatrix( &worldInvB, &body_b_->getBtRigidBody()->getWorldTransform() );
		//worldInvB.cdump();

		//D3DXMATRIX scale(
		//	1.0f, 0, 0, 0,  
		//	0, 1.0f, 0, 0,  
		//	0, 0, -1.0f, 0,  
		//	0, 0, 0, 1.0f);
		//jointOffset = (LMatrix&)(scale * (D3DXMATRIX&)jointOffset * scale);
		//worldInvA = (LMatrix&)(scale * (D3DXMATRIX&)worldInvA * scale);
		//worldInvB = (LMatrix&)(scale * (D3DXMATRIX&)worldInvB * scale);
		
		
		//D3DXMATRIX dm = (D3DXMATRIX&)worldInvA;
		//D3DXMATRIX im;
		//D3DXMatrixInverse( &im, NULL, &dm );
		//printf("%f %f %f \n", im._11, im._12, im._13);
		//printf("%f %f %f \n", im._21, im._22, im._23);
		//printf("%f %f %f \n", im._31, im._32, im._33);
		//printf("%f %f %f \n", im._41, im._42, im._43);
		worldInvA.Inverse();
		worldInvB.Inverse();

		//worldInvB.cdump();
		

		// 各剛体のローカルから見たジョイント位置
		//LMatrix frameInA = worldInvA * jointOffset;
		//LMatrix frameInB = worldInvB * jointOffset;
		LMatrix frameInA = jointOffset * worldInvA;
		LMatrix frameInB = jointOffset * worldInvB;


		//frameInA.cdump();
		//frameInB.cdump();

		//btTransform		mmm = body_a_->getBtRigidBody()->getWorldTransform().inverse();
		//worldInvA.cdump();
		
		//D3DXMATRIX scale(
		//	1.0f, 0, 0, 0,  
		//	0, 1.0f, 0, 0,  
		//	0, 0, -1.0f, 0,  
		//	0, 0, 0, 1.0f);
		//frameInA = (LMatrix&)(scale * (D3DXMATRIX&)frameInA * scale);
		//frameInB = (LMatrix&)(scale * (D3DXMATRIX&)frameInB * scale);
		//

		//frameInA.cdump();

		// ジョイント作成
		mDofSpringJoint = LN_NEW Physics::DofSpringJoint();
		mDofSpringJoint->create(
			manager_->getPhysicsManager(),
			body_a_->getRigidBody(),
            body_b_->getRigidBody(),
			frameInA,
			frameInB );

		//body_a_->getRigidBody()->getBtRigidBody()->setActivationState(DISABLE_DEACTIVATION);
		//body_b_->getRigidBody()->getBtRigidBody()->setActivationState(DISABLE_DEACTIVATION);

		//_p(pmd_constraint_->szName);
		//pmd_constraint_->vec3PosLimitL.dump();
		//pmd_constraint_->vec3PosLimitU.dump();
		//pmd_constraint_->vec3RotLimitL.dump();
		//pmd_constraint_->vec3RotLimitU.dump();
		//return ;
#if 0
		btVector3   mRotation;
        btVector3   mPosition;
        btVector3   mPosLimitL;
        btVector3   mPosLimitU;
        btVector3   mRotLimitL;
        btVector3   mRotLimitU;
        btVector3   mSpringPos;
        btVector3   mSpringRot;

        mRotation.setValue( pmd_constraint_->vec3Rotation.x, pmd_constraint_->vec3Rotation.y, pmd_constraint_->vec3Rotation.z );
        mPosition.setValue( pmd_constraint_->vec3Position.x, pmd_constraint_->vec3Position.y, pmd_constraint_->vec3Position.z );
        mPosLimitL.setValue( pmd_constraint_->vec3PosLimitL.x, pmd_constraint_->vec3PosLimitL.y, pmd_constraint_->vec3PosLimitL.z );
        mPosLimitU.setValue( pmd_constraint_->vec3PosLimitU.x, pmd_constraint_->vec3PosLimitU.y, pmd_constraint_->vec3PosLimitU.z );
        mRotLimitL.setValue( pmd_constraint_->vec3RotLimitL.x, pmd_constraint_->vec3RotLimitL.y, pmd_constraint_->vec3RotLimitL.z );
        mRotLimitU.setValue( pmd_constraint_->vec3RotLimitU.x, pmd_constraint_->vec3RotLimitU.y, pmd_constraint_->vec3RotLimitU.z );
        mSpringPos.setValue( pmd_constraint_->vec3SpringPos.x, pmd_constraint_->vec3SpringPos.y, pmd_constraint_->vec3SpringPos.z );
        mSpringRot.setValue( pmd_constraint_->vec3SpringRot.x, pmd_constraint_->vec3SpringRot.y, pmd_constraint_->vec3SpringRot.z );


		// 各種制限パラメータのセット
		mDofSpringJoint->mBtDofSpringConstraint->setLinearLowerLimit( mPosLimitL );
	    mDofSpringJoint->mBtDofSpringConstraint->setLinearUpperLimit( mPosLimitU );

	    mDofSpringJoint->mBtDofSpringConstraint->setAngularLowerLimit( mRotLimitL );
	    mDofSpringJoint->mBtDofSpringConstraint->setAngularUpperLimit( mRotLimitU );

		    // 0 : translation X
	    if ( mSpringPos.x() != 0.0f )
	    {
		    mDofSpringJoint->mBtDofSpringConstraint->enableSpring( 0, true );
		    mDofSpringJoint->mBtDofSpringConstraint->setStiffness( 0, mSpringPos.x() );
	    }

		    // 1 : translation Y
	    if ( mSpringPos.y() != 0.0f )
	    {
		    mDofSpringJoint->mBtDofSpringConstraint->enableSpring( 1, true );
		    mDofSpringJoint->mBtDofSpringConstraint->setStiffness( 1, mSpringPos.y() );
	    }

		    // 2 : translation Z
	    if ( mSpringPos.z() != 0.0f )
	    {
		    mDofSpringJoint->mBtDofSpringConstraint->enableSpring( 2, true );
		    mDofSpringJoint->mBtDofSpringConstraint->setStiffness( 2, mSpringPos.z() );
	    }

		    // 3 : rotation X (3rd Euler rotational around new position of X axis, range [-PI+epsilon, PI-epsilon] )
		    // 4 : rotation Y (2nd Euler rotational around new position of Y axis, range [-PI/2+epsilon, PI/2-epsilon] )
		    // 5 : rotation Z (1st Euler rotational around Z axis, range [-PI+epsilon, PI-epsilon] )
	    mDofSpringJoint->mBtDofSpringConstraint->enableSpring( 3, true );	mDofSpringJoint->mBtDofSpringConstraint->setStiffness( 3, mSpringRot.x() );
	    mDofSpringJoint->mBtDofSpringConstraint->enableSpring( 4, true );	mDofSpringJoint->mBtDofSpringConstraint->setStiffness( 4, mSpringRot.y() );
	    mDofSpringJoint->mBtDofSpringConstraint->enableSpring( 5, true );	mDofSpringJoint->mBtDofSpringConstraint->setStiffness( 5, mSpringRot.z() );

		mDofSpringJoint->mBtDofSpringConstraint->calculateTransforms();
		mDofSpringJoint->mBtDofSpringConstraint->setEquilibriumPoint();

#else
		//return ;
		// 各種制限パラメータのセット
	    mDofSpringJoint->setLinearLowerLimit(
			LVector3(
				pmd_constraint_->vec3PosLimitL.X, 
				pmd_constraint_->vec3PosLimitL.Y, 
				pmd_constraint_->vec3PosLimitL.Z ));
	    mDofSpringJoint->setLinearUpperLimit(
			LVector3(
				pmd_constraint_->vec3PosLimitU.X, 
				pmd_constraint_->vec3PosLimitU.Y, 
				pmd_constraint_->vec3PosLimitU.Z ));
	    mDofSpringJoint->setAngularLowerLimit( 
			LVector3(
				pmd_constraint_->vec3RotLimitL.X, 
				pmd_constraint_->vec3RotLimitL.Y, 
				pmd_constraint_->vec3RotLimitL.Z ));
	    mDofSpringJoint->setAngularUpperLimit(
			LVector3(
				pmd_constraint_->vec3RotLimitU.X, 
				pmd_constraint_->vec3RotLimitU.Y, 
				pmd_constraint_->vec3RotLimitU.Z ));
		
		const LVector3& springPos = pmd_constraint_->vec3SpringPos;
		
		// 0 : translation X
	    if ( springPos.X != 0.0f )
	    {
		    mDofSpringJoint->enableSpring( 0, true );
		    mDofSpringJoint->setStiffness( 0, springPos.X );
	    }

		// 1 : translation Y
	    if ( springPos.Y != 0.0f )
	    {
		    mDofSpringJoint->enableSpring( 1, true );
		    mDofSpringJoint->setStiffness( 1, springPos.Y );
	    }

		// 2 : translation Z
	    if ( springPos.Z != 0.0f )
	    {
		    mDofSpringJoint->enableSpring( 2, true );
		    mDofSpringJoint->setStiffness( 2, springPos.Z );
	    }

		// 3 : rotation X (3rd Euler rotational around new position of X axis, range [-PI+epsilon, PI-epsilon] )
		// 4 : rotation Y (2nd Euler rotational around new position of Y axis, range [-PI/2+epsilon, PI/2-epsilon] )
		// 5 : rotation Z (1st Euler rotational around Z axis, range [-PI+epsilon, PI-epsilon] )
	    mDofSpringJoint->enableSpring( 3, true );	mDofSpringJoint->setStiffness( 3, pmd_constraint_->vec3SpringRot.X );
	    mDofSpringJoint->enableSpring( 4, true );	mDofSpringJoint->setStiffness( 4, pmd_constraint_->vec3SpringRot.Y );
	    mDofSpringJoint->enableSpring( 5, true );	mDofSpringJoint->setStiffness( 5, pmd_constraint_->vec3SpringRot.Z );

		//mDofSpringJoint->setDamping(3, 0.01f);
		//mDofSpringJoint->setDamping(4, 0.01f);
		//mDofSpringJoint->setDamping(5, 0.01f);
      //mDofSpringJoint->setParam(BT_CONSTRAINT_STOP_CFM, 1.0e-5f, 5);
#if 0
		LVector3(
				pmd_constraint_->vec3PosLimitL.x, 
				pmd_constraint_->vec3PosLimitL.y, 
				pmd_constraint_->vec3PosLimitL.z ).dump();
		LVector3(
				pmd_constraint_->vec3PosLimitU.x, 
				pmd_constraint_->vec3PosLimitU.y, 
				pmd_constraint_->vec3PosLimitU.z ).dump();
		LVector3(
				pmd_constraint_->vec3RotLimitL.x, 
				pmd_constraint_->vec3RotLimitL.y, 
				pmd_constraint_->vec3RotLimitL.z ).dump();
		LVector3(
				pmd_constraint_->vec3RotLimitU.x, 
				pmd_constraint_->vec3RotLimitU.y, 
				pmd_constraint_->vec3RotLimitU.z ).dump();
		pmd_constraint_->vec3SpringPos.dump();
		pmd_constraint_->vec3SpringRot.dump();
#endif


		mDofSpringJoint->setEquilibriumPoint();
#endif

#if 0
        btVector3   mRotation;
        btVector3   mPosition;
        btVector3   mPosLimitL;
        btVector3   mPosLimitU;
        btVector3   mRotLimitL;
        btVector3   mRotLimitU;
        btVector3   mSpringPos;
        btVector3   mSpringRot;

        mRotation.setValue( pmd_constraint_->vec3Rotation.x, pmd_constraint_->vec3Rotation.y, pmd_constraint_->vec3Rotation.z );
        mPosition.setValue( pmd_constraint_->vec3Position.x, pmd_constraint_->vec3Position.y, pmd_constraint_->vec3Position.z );
        mPosLimitL.setValue( pmd_constraint_->vec3PosLimitL.x, pmd_constraint_->vec3PosLimitL.y, pmd_constraint_->vec3PosLimitL.z );
        mPosLimitU.setValue( pmd_constraint_->vec3PosLimitU.x, pmd_constraint_->vec3PosLimitU.y, pmd_constraint_->vec3PosLimitU.z );
        mRotLimitL.setValue( pmd_constraint_->vec3RotLimitL.x, pmd_constraint_->vec3RotLimitL.y, pmd_constraint_->vec3RotLimitL.z );
        mRotLimitU.setValue( pmd_constraint_->vec3RotLimitU.x, pmd_constraint_->vec3RotLimitU.y, pmd_constraint_->vec3RotLimitU.z );
        mSpringPos.setValue( pmd_constraint_->vec3SpringPos.x, pmd_constraint_->vec3SpringPos.y, pmd_constraint_->vec3SpringPos.z );
        mSpringRot.setValue( pmd_constraint_->vec3SpringRot.x, pmd_constraint_->vec3SpringRot.y, pmd_constraint_->vec3SpringRot.z );

#if 0
        //_p(pmd_constraint_->szName);
        //_p(pmd_constraint_->ulRigidA);
        //_p(pmd_constraint_->ulRigidB);

		LMatrix frameInA, frameInB;
		btTransform btFrameInA, btFrameInB;
		LMatrix jointPos;
		jointPos.rotation( mRotation.x(), mRotation.y(), mRotation.z(), LN_ROTORDER_YXZ);
		jointPos.translation( mPosition.x(), mPosition.y(), mPosition.z() );

		btTransformToLMatrix( &frameInA, &body_a_->getBtRigidBody()->getWorldTransform() );
		frameInA.inverse();
		frameInA = jointPos * /*model.Transform **/ frameInA;

		btTransformToLMatrix( &frameInB, &body_b_->getBtRigidBody()->getWorldTransform() );
		frameInB.inverse();
		frameInB = jointPos * /*model.Transform **/ frameInB;

		btTransform bttrRigidAInvTransform;
		btTransform bttrRigidBInvTransform;

		LMatrixTobtTransform( &bttrRigidAInvTransform, frameInA );
		LMatrixTobtTransform( &bttrRigidBInvTransform, frameInB );
#else
        // 回転行列作成
	    btMatrix3x3	btmRotationMat;
	    btmRotationMat.setEulerZYX( mRotation.x(), mRotation.y(), mRotation.z() );
		

	    // コンストレイントのトランスフォームを作成
	    btTransform bttrTransform;
	    bttrTransform.setIdentity();
	    bttrTransform.setOrigin( mPosition );
	    bttrTransform.setBasis( btmRotationMat );

	    // 剛体A,Bから見たコンストレイントのトランスフォームを作成 
	    btTransform		bttrRigidAInvTransform = body_a_->getBtRigidBody()->getWorldTransform().inverse(),
					    bttrRigidBInvTransform = body_b_->getBtRigidBody()->getWorldTransform().inverse();

		//※ PMD に格納されているジョイントの位置・回転は「モデル内のローカル座標」
		//   これを「剛体のローカル座標」に変換する必要がある

	    bttrRigidAInvTransform = bttrRigidAInvTransform * bttrTransform;
	    bttrRigidBInvTransform = bttrRigidBInvTransform * bttrTransform;
#endif

	    mBtDofConstraint = new btGeneric6DofSpringConstraint(
            *body_a_->getBtRigidBody(),
            *body_b_->getBtRigidBody(),
		    bttrRigidAInvTransform,
            bttrRigidBInvTransform,
            true );

	    // 各種制限パラメータのセット
	    mBtDofConstraint->setLinearLowerLimit( mPosLimitL );
	    mBtDofConstraint->setLinearUpperLimit( mPosLimitU );

	    mBtDofConstraint->setAngularLowerLimit( mRotLimitL );
	    mBtDofConstraint->setAngularUpperLimit( mRotLimitU );

		    // 0 : translation X
	    if ( mSpringPos.x() != 0.0f )
	    {
		    mBtDofConstraint->enableSpring( 0, true );
		    mBtDofConstraint->setStiffness( 0, mSpringPos.x() );
	    }

		    // 1 : translation Y
	    if ( mSpringPos.y() != 0.0f )
	    {
		    mBtDofConstraint->enableSpring( 1, true );
		    mBtDofConstraint->setStiffness( 1, mSpringPos.y() );
	    }

		    // 2 : translation Z
	    if ( mSpringPos.z() != 0.0f )
	    {
		    mBtDofConstraint->enableSpring( 2, true );
		    mBtDofConstraint->setStiffness( 2, mSpringPos.z() );
	    }

		    // 3 : rotation X (3rd Euler rotational around new position of X axis, range [-PI+epsilon, PI-epsilon] )
		    // 4 : rotation Y (2nd Euler rotational around new position of Y axis, range [-PI/2+epsilon, PI/2-epsilon] )
		    // 5 : rotation Z (1st Euler rotational around Z axis, range [-PI+epsilon, PI-epsilon] )
	    mBtDofConstraint->enableSpring( 3, true );	mBtDofConstraint->setStiffness( 3, mSpringRot.x() );
	    mBtDofConstraint->enableSpring( 4, true );	mBtDofConstraint->setStiffness( 4, mSpringRot.y() );
	    mBtDofConstraint->enableSpring( 5, true );	mBtDofConstraint->setStiffness( 5, mSpringRot.z() );

		mBtDofConstraint->calculateTransforms();
		mBtDofConstraint->setEquilibriumPoint();

	    // シミュレーションワールドに追加
        Physics::JointBase::create( manager_->getPhysicsManager(), mBtDofConstraint );
    
#endif
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================