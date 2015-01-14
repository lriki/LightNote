//==============================================================================
// Shapes 
//==============================================================================

#include "stdafx.h"
#include <btBulletDynamicsCommon.h>
#include "../BulletUtil.h"
#include "Shapes.h"

namespace LNote
{
namespace Core
{
namespace Physics
{

//==============================================================================
// ■ Box
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Box::Box()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Box::~Box()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Box::create( Manager* manager, const LVector3& size, float mass, lnU16 group, lnU16 mask )
    {
		// 形状の作成
		mHarfSize = size * 0.5f;
		btCollisionShape* boxShape = new btBoxShape( BulletUtil::LNVector3ToBtVector3( mHarfSize ) );

		// 基底の初期化
		RigidBodyInitArgs args;
		args.Shape = boxShape;

		ConfigData data;
		data.Mass = mass;
		data.Group = group;
		data.GroupMask = mask;
        RigidBody::initialize( manager, args, data );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Box::create( Manager* manager, const LVector3& size, const ConfigData& configData )
	{
		// 形状の作成
		mHarfSize = size * 0.5f;
		btCollisionShape* boxShape = new btBoxShape( BulletUtil::LNVector3ToBtVector3( mHarfSize ) );

		RigidBodyInitArgs args;
		args.Shape = boxShape;
		RigidBody::initialize( manager, args, configData );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Box::drawDebugShape( IDebugRenderer* renderer )
    {
        LVector3 bbmin( -mHarfSize.X, -mHarfSize.Y, -mHarfSize.Z );
        LVector3 bbmax(  mHarfSize.X,  mHarfSize.Y,  mHarfSize.Z );

        renderer->setMatrix( this->mWorldMatrix );
        renderer->drawBox( bbmin, bbmax, LColorF( 0, 0, 1, 0.5f ) );
    }

//==============================================================================
// ■ Capsule
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Capsule::Capsule()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Capsule::~Capsule()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Capsule::create( Manager* manager, lnFloat radius, lnFloat length, lnFloat mass, lnU16 group, lnU16 mask )
    {
		// 形状の作成
		btCollisionShape* shape = new btCapsuleShape( radius, length );

		// 基底の初期化
		RigidBodyInitArgs args;
		args.Shape = shape;

		ConfigData data;
		data.Mass = mass;
		data.Group = group;
		data.GroupMask = mask;
        RigidBody::initialize( manager, args, data );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Capsule::create( Manager* manager, lnFloat radius, lnFloat length, const ConfigData& configData )
	{
		// 形状の作成
		btCollisionShape* shape = new btCapsuleShape( radius, length );

		// 基底の初期化
		RigidBodyInitArgs args;
		args.Shape = shape;
        RigidBody::initialize( manager, args, configData );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Capsule::drawDebugShape( IDebugRenderer* renderer )
    {
    }

//==============================================================================
// ■ Sphere
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Sphere::Sphere()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Sphere::~Sphere()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sphere::create( Manager* manager, lnFloat radius, lnFloat mass, lnU16 group, lnU16 mask )
    {
		// 形状の作成
		btCollisionShape* shape = new btSphereShape( radius );

		// 基底の初期化
		RigidBodyInitArgs args;
		args.Shape = shape;

		ConfigData data;
		data.Mass = mass;
		data.Group = group;
		data.GroupMask = mask;
        RigidBody::initialize( manager, args, data );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sphere::create( Manager* manager, lnFloat radius, const ConfigData& configData )
	{
		// 形状の作成
		btCollisionShape* shape = new btSphereShape( radius );

		// 基底の初期化
		RigidBodyInitArgs args;
		args.Shape = shape;
        RigidBody::initialize( manager, args, configData );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sphere::drawDebugShape( IDebugRenderer* renderer )
    {
    }

#if 0
//==============================================================================
// ■ TriMesh
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    TriMesh::TriMesh( Manager* manager_ )
        : RigidBody         ( manager_ )
        , mBtTriMeshData    ( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    TriMesh::~TriMesh()
    {
        SAFE_DELETE( mBtTriMeshData );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void TriMesh::initialize( const RigidBodyInitArgs& args_, btTriangleIndexVertexArray* bt_tri_mesh_data_ )
    {
        RigidBody::initialize( args_ );
        mBtTriMeshData = bt_tri_mesh_data_;
    }
#endif

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================