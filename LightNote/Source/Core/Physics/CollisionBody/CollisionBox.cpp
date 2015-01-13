//==============================================================================
// CollisionBox 
//==============================================================================

#pragma once

#include "stdafx.h"
#include <btBulletDynamicsCommon.h>
#include "../../Math/Vector3.h"
#include "CollisionBox.h"

namespace LNote
{
namespace Core
{
namespace Physics
{

//==============================================================================
// ¡ CollisionBox
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
		btBoxShape* shape = new btBoxShape( btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f) );

		btCollisionObject* obj = new btCollisionObject();
		obj->setCollisionShape( shape );
		obj->getWorldTransform().setIdentity();

		// Õ“ËŒŸo‚Í‚·‚é‚ªA‘¼‚Ì„‘Ì‚É‰e‹¿‚ð—^‚¦‚È‚¢
		obj->setCollisionFlags( obj->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE );

		CollisionBody::create( manager, obj, group, mask );
	}

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================