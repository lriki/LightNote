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
// �� Box
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

	/// �쐬 (size �͊e�ӂ̒���)
	void create( Manager* manager, const LVector3& size, float mass, lnU16 group = 0xffff, lnU16 mask = 0xffff );

	/// �쐬
	void create( Manager* manager, const LVector3& size, const ConfigData& configData );

	/// �f�o�b�O�p�̌`��`��
    virtual void drawDebugShape( IDebugRenderer* renderer );

private:

    LVector3    mHarfSize;
};

//==============================================================================
// �� Capsule
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

	/// �쐬
	void create( Manager* manager, lnFloat radius, lnFloat length, lnFloat mass, lnU16 group = 0xffff, lnU16 mask = 0xffff );

	/// �쐬
	void create( Manager* manager, lnFloat radius, lnFloat length, const ConfigData& configData );

protected:
    virtual void drawDebugShape( IDebugRenderer* renderer );
};

//==============================================================================
// �� Sphere
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

	/// �쐬 (radius �͔��a)
	void create( Manager* manager, lnFloat radius, lnFloat mass, lnU16 group = 0xffff, lnU16 mask = 0xffff );

	/// �쐬 (radius �͔��a)
	void create( Manager* manager, lnFloat radius, const ConfigData& configData );

protected:
    virtual void drawDebugShape( IDebugRenderer* renderer );
};


#if 0
//==============================================================================
// �� TriMesh
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