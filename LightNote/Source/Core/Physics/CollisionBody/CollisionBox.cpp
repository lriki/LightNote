//==============================================================================
// CollisionBox 
//==============================================================================

#pragma once

#include "stdafx.h"
#include <btBulletDynamicsCommon.h>
#include "../../Math/LMath.h"
#include "CollisionBox.h"

namespace LNote
{
namespace Core
{
namespace Physics
{

//==============================================================================
// ■ CollisionBox
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    CollisionBox::CollisionBox()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    CollisionBox::~CollisionBox()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void CollisionBox::create( Manager* manager, const LVector3& size, lnU16 group, lnU16 mask )
	{
		btBoxShape* shape = new btBoxShape( btVector3(size.X * 0.5f, size.Y * 0.5f, size.Z * 0.5f) );

		btCollisionObject* obj = new btCollisionObject();
		obj->setCollisionShape( shape );
		obj->getWorldTransform().setIdentity();

		// 衝突検出はするが、他の剛体に影響を与えない
		obj->setCollisionFlags( obj->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE );

		CollisionBody::create( manager, obj, group, mask );
	}

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================