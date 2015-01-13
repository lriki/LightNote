//==============================================================================
// ModelRigidBody 
//------------------------------------------------------------------------------
///**
//  @file       ModelRigidBody.h
//  @brief      ModelRigidBody
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Common.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{
enum CollisionShapeType2
{
    CollisionShapeType_Sphere = 0,		///< 球
	CollisionShapeType_Box,				///< 箱
	CollisionShapeType_Capsule,			///< カプセル    
};

enum RigidBodyType2
{
	RigidBodyType_ControlledByBone = 0,	///< Bone追従
	RigidBodyType_Physics,				///< 物理演算
	RigidBodyType_PhysicsAlignment,		///< 物理演算(Bone位置合せ)
}; 

//==============================================================================
// ModelRigidBodyCore
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelRigidBodyCore
	: public Base::ReferenceObject
{
public:
	ModelRigidBodyCore() {}
	virtual ~ModelRigidBodyCore() {}

public:

	/// 形状データ
    struct CollisionShapeData
    {
        CollisionShapeType2 Type;
        union
        {
            struct
            {
                lnFloat Radius;
            } Sphere;

            struct
            {
                lnFloat Width;
                lnFloat Height;
	            lnFloat Depth;
            } Box;

            struct
            {
                lnFloat Radius;
                lnFloat Height;
            } Capsule;
        };
    };

public:

	lnRefStringW		Name;

	RigidBodyType2		RigidBodyType;
	LMatrix				BoneOffset;			///< ボーン行列 → 剛体行列変換用 (グローバル行列ではない)
	LMatrix				InvBoneOffset;		///< 剛体行列 → ボーン行列変換用

    CollisionShapeData	ColShapeData;

    lnU16				Group;				///< 衝突グループ
	lnU16				GroupMask;			///< 衝突グループマスク
    lnFloat				Mass;				///< 質量
	lnFloat				LinearDamping;		///< 移動減
	lnFloat				AngularDamping;		///< 回転減
	lnFloat				Restitution;		///< 反発力
	lnFloat				Friction;			///< 摩擦力

	int					RelatedFrameIndex;	///< 関連ボーン
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
