//==============================================================================
// ModelConstraintCore
//------------------------------------------------------------------------------
///**
//  @file       ModelConstraintCore.h
//  @brief      ModelConstraintCore
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

//==============================================================================
// ModelConstraintCore
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelConstraintCore
	: public Base::ReferenceObject
{
public:
	ModelConstraintCore() {}
	virtual ~ModelConstraintCore() {}

public:

	lnRefStringW	Name;

	int				RigidBodyAIndex;			///< 結合剛体 A
	int				RigidBodyBIndex;			///< 結合剛体 B

	LVector3	    Position;					///< 位置(モデル原点中心)
	LVector3	    Rotation;					///< 回転(radian)

	LVector3		PositionLimitLower;			///< 移動制限1
	LVector3		PositionLimitUpper;			///< 移動制限2

	LVector3		RotationLimitLower;			///< 回転制限1
	LVector3		RotationLimitUpper;			///< 回転制限2

	LVector3	    SpringPositionStiffness;	///< ばね移動
	LVector3	    SpringRotationStiffness;	///< ばね回転
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
