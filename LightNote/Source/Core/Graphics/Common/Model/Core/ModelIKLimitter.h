//==============================================================================
// ModelIKLimitter
//------------------------------------------------------------------------------
///**
//  @file       ModelIKLimitter.h
//  @brief      ModelIKLimitter
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
// ModelIKLimitter
//------------------------------------------------------------------------------
///**
//  @brief		MMDX スタイルの IK リミッタ
//*/
//==============================================================================
class ModelIKLimitter2
	: public Base::ReferenceObject
{
public:
	ModelIKLimitter2();
    virtual ~ModelIKLimitter2() {}

public:
	void setRotationLimits( const LVector3& min, const LVector3& max );
	void setRotationRestitution( bool x, bool y, bool z, const LVector3& restitution );
	float adjustRotation( float value, int index );

	// ボーンへの制限適用(受け取った SQTTransform を一度角度に分解し、制限を加えて戻す)
	void adjustRotation( LSQTTransform* localTransform );	

	void setAxisLimits( bool x, bool y, bool z );
	void adjustAxisLimits( LVector3* rot );

public:

	LVector3	RotationMin;	///< 最小回転
	LVector3	RotationMax;	///< 最大回転
	bool		Mirror[3];		///< 角度の反射調整機能使用フラグ
	LVector3	Restitution;	///< 角度の反射調整の反発係数

	bool		AxisLimits[3];	///< X,Y,Z 軸に対応。true の場合、角度を 0 にする

	bool		EnableRotationLimit;
	bool		EnableAxisLimit;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
