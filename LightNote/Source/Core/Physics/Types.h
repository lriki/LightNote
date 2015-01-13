//==============================================================================
// Types 
//------------------------------------------------------------------------------
///**
//  @file       Types.h
//  @brief      Types
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <vector>

class btGeneric6DofSpringConstraint;

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

struct LNTriangleMeshBodyInfo
{
	lnU32           VertexCount;    ///< ���_��
	const LVector3* VertexTable;	///< ���_�f�[�^�e�[�u��
	lnU32           IndexCount;	    ///< �C���f�b�N�X��
    lnU32           IndexStride;    ///< �C���f�b�N�X�̃X�g���C�h (2 or 4)
	const void*     IndexTable;	    ///< �C���f�b�N�X�f�[�^�e�[�u��
};

namespace Core
{
namespace Physics
{
class IDebugRenderer;
class BodyBase;
class RigidBody;
class CollisionBody;
class Box;
class Plane;
class JointBase;
class TriMesh;
class DebugDrawer;

enum BodyType
{
	BodyType_Unknown = 0,
	BodyType_RigidBody,
	BodyType_SoftBody,
	BodyType_CollisionBody,
	BodyType_CharacterObject,
	BodyType_Joint,

	BodyType_Max,
};

/// �ڐG�_���
struct ContactInfo
{
	LVector3	Normal;		///< �ڐG�_�@��
	LVector3	Position;	///< �ڐG�_�ʒu
	BodyBase*	TargetBody;	///< �ڐG�{�f�B
};

typedef std::vector<ContactInfo>	ContactInfoArray;


} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================