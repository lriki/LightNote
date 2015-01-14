//==============================================================================
// BulletUtil 
//------------------------------------------------------------------------------
///**
//  @file       BulletUtil.h
//  @brief      BulletUtil
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
#include <LinearMath/btTransform.h>
#include "../Math/LMath.h"

namespace LNote
{
namespace Core
{
namespace Physics
{

//==============================================================================
// Å° BulletUtil
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class BulletUtil
{
public:

	static LVector3 btVector3ToLNVector3( const btVector3& v )
	{
		return LVector3( v.getX(), v.getY(), v.getZ() );
	}

	static btVector3 LNVector3ToBtVector3( const LVector3& v )
	{
		return btVector3( v.X, v.Y, v.Z );
	}

	static LQuaternion btQuaternionToLNQuaternion( const btQuaternion& q )
	{
		return LQuaternion( q.getX(), q.getY(), q.getZ(), q.getW() );
	}

	static btQuaternion LNQuaternionToBtQuaternion( const LQuaternion& q )
	{
		return btQuaternion( q.X, q.Y, q.Z, q.W );
	}


	static void dumpBtVector3( const btVector3& v )
	{
		printf( "Vector3\n%f, %f, %f\n", v.x(), v.y(), v.z() );
	}

	static void dumpBtTransform( const btTransform& t )
	{
		printf( "btTransform\n%f, %f, %f\n%f, %f, %f\n%f, %f, %f\n%f, %f, %f\n",
			t.getBasis().getRow(0).x(), t.getBasis().getRow(0).y(), t.getBasis().getRow(0).z(),
			t.getBasis().getRow(1).x(), t.getBasis().getRow(1).y(), t.getBasis().getRow(1).z(),
			t.getBasis().getRow(2).x(), t.getBasis().getRow(2).y(), t.getBasis().getRow(2).z(),
			t.getOrigin().x(), t.getOrigin().y(), t.getOrigin().z());
	}
};

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================