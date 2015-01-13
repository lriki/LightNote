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
	lnU32           VertexCount;    ///< 頂点数
	const LVector3* VertexTable;	///< 頂点データテーブル
	lnU32           IndexCount;	    ///< インデックス数
    lnU32           IndexStride;    ///< インデックスのストライド (2 or 4)
	const void*     IndexTable;	    ///< インデックスデータテーブル
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

/// 接触点情報
struct ContactInfo
{
	LVector3	Normal;		///< 接触点法線
	LVector3	Position;	///< 接触点位置
	BodyBase*	TargetBody;	///< 接触ボディ
};

typedef std::vector<ContactInfo>	ContactInfoArray;


} // namespace Physics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================