//==============================================================================
// ModelPMXIKSolver 
//------------------------------------------------------------------------------
///**
//  @file       ModelPMXIKSolver.h
//  @brief      ModelPMXIKSolver
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "Common.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ModelPMXIKSolver
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelPMXIKSolver
{
public:
    ModelPMXIKSolver();
    virtual ~ModelPMXIKSolver();

public:
	void create( Model2* ownerModel ) { mOwnerModel = ownerModel; }

	///// エフェクタボーンの追加 (IsIK = true であるフレーム)
	//void addEffectorFrame( ModelFrame2* frame );

	/// 適用 (モーション適用後、ボーンのワールド行列が階層更新された後に呼ぶ)
    void updateIKFrames();

private:
	void solve( ModelIKCore2* ik, ModelFrame2* ikTarget, ModelFrame2* effector );
	void limitRotation( const ModelIKLink& ikLink, ModelFrame2* ikLinkFrame );

private:
	Model2*				mOwnerModel;
    ModelFrameArray2	IKBones;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
