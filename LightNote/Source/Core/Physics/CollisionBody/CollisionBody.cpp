//==============================================================================
// CollisionBody 
//==============================================================================

#pragma once

#include "stdafx.h"
#include <btBulletDynamicsCommon.h>
#include "CollisionBody.h"

namespace LNote
{
namespace Core
{
namespace Physics
{

//==============================================================================
// ¡ CollisionBody
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    CollisionBody::CollisionBody()
		: mBtCollisionObject	( NULL )
		, mCollisionEnable		( false )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    CollisionBody::~CollisionBody()
	{
		if ( mBtCollisionObject )
		{
			btCollisionShape* shape = mBtCollisionObject->getCollisionShape();
			SAFE_DELETE( shape );

			mManager->getDynamicsWorld()->removeCollisionObject( mBtCollisionObject );
			SAFE_DELETE( mBtCollisionObject );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void CollisionBody::create( Manager* manager, btCollisionObject* obj, lnU16 group, lnU16 mask )
	{
		BodyBase::initialize( manager, NULL, obj );
		mBtCollisionObject = obj;

		mCollisionEnable = true;
		mManager->addCollisionBody( this );
	}

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================