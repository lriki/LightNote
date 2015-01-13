//==============================================================================
// CharacterObject 
//==============================================================================

#include "stdafx.h"
#include <btBulletDynamicsCommon.h>
#include "CharacterObject.h"

namespace LNote
{
namespace Core
{
namespace Physics
{

//==============================================================================
// CharacterObject
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    CharacterObject::CharacterObject()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    CharacterObject::~CharacterObject()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void CharacterObject::createCapsule( Manager* manager, lnFloat radius, lnFloat height )
    {
		// Œ`ó‚Ìì¬
		btCollisionShape* shape = new btCapsuleShape( radius, height );

		float mass = 1.0f;
		btVector3 localInertia( 0.0f, 0.0f, 0.0f );
		shape->calculateLocalInertia( mass, localInertia );

		btTransform transform;
		transform.setIdentity();

		btDefaultMotionState* motionState = new btDefaultMotionState( transform );
		btRigidBody::btRigidBodyConstructionInfo BodyInfo( mass, motionState, shape, localInertia );
		btRigidBody* rigidBody = new btRigidBody( BodyInfo );
		//rigidBody->setCollisionFlags( rigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK );
		rigidBody->setAngularFactor( 0.0f );			// ‰ñ“]‚³‚¹‚È‚¢
		rigidBody->setFriction( 0.0f );
		rigidBody->setRestitution( FLT_MIN );
		rigidBody->setDamping( 0.1f, 0.1f );


		// Šî’ê‚Ì‰Šú‰»
		RigidBodyInitArgs args;
		args.Shape = shape;
		args.RigidBody = rigidBody;

		ConfigData data;
        RigidBody::initialize( manager, args, data );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void CharacterObject::preUpdate()
	{
		//if ( abs( mMoveVelocity.x ) > FLT_MIN || abs( mMoveVelocity.y ) > FLT_MIN || abs( mMoveVelocity.z ) > FLT_MIN )
		{
			mBtRigidBody->activate( true );
			mBtRigidBody->setLinearVelocity( btVector3( mMoveVelocity.x, mMoveVelocity.y, mMoveVelocity.z ) );
		}

		mMoveVelocity = LVector3::ZERO;
	}

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================