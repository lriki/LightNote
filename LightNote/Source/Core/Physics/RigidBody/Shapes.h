//==============================================================================
// Shapes 
//------------------------------------------------------------------------------
///**
//  @file       Shapes.h
//  @brief      Shapes
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "RigidBody.h"

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
// ■ Box
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class Box
    : public RigidBody
{
public:
    Box();
    virtual ~Box();

public:

	/// 作成 (size は各辺の長さ)
	void create( Manager* manager, const LVector3& size, float mass, lnU16 group = 0xffff, lnU16 mask = 0xffff );

	/// 作成
	void create( Manager* manager, const LVector3& size, const ConfigData& configData );

	/// デバッグ用の形状描画
    virtual void drawDebugShape( IDebugRenderer* renderer );

private:

    LVector3    mHarfSize;
};

//==============================================================================
// ■ Capsule
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class Capsule
    : public RigidBody
{
public:
    Capsule();
    virtual ~Capsule();

public:

	/// 作成
	void create( Manager* manager, lnFloat radius, lnFloat length, lnFloat mass, lnU16 group = 0xffff, lnU16 mask = 0xffff );

	/// 作成
	void create( Manager* manager, lnFloat radius, lnFloat length, const ConfigData& configData );

protected:
    virtual void drawDebugShape( IDebugRenderer* renderer );
};

//==============================================================================
// ■ Sphere
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class Sphere
    : public RigidBody
{
public:
    Sphere();
    virtual ~Sphere();

public:

	/// 作成 (radius は半径)
	void create( Manager* manager, lnFloat radius, lnFloat mass, lnU16 group = 0xffff, lnU16 mask = 0xffff );

	/// 作成 (radius は半径)
	void create( Manager* manager, lnFloat radius, const ConfigData& configData );

protected:
    virtual void drawDebugShape( IDebugRenderer* renderer );
};


#if 0
//==============================================================================
// ■ TriMesh
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class TriMesh
    : public RigidBody
{
public:
    TriMesh( Manager* manager_ );

protected:
    virtual ~TriMesh();

public:

    void initialize( const RigidBodyInitArgs& args_, btTriangleIndexVertexArray* bt_tri_mesh_data_ );

	//virtual LNRESULT drawDebugShape( IDebugRenderer* renderer_ );

private:

    btTriangleIndexVertexArray* mBtTriMeshData;
};
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================