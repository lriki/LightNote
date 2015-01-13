//==============================================================================
// ModelFace 
//------------------------------------------------------------------------------
///**
//  @file       ModelFace.h
//  @brief      ModelFace
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../../Base/RefString.h"
#include "../../../Animation/AnimationController.h"
#include "Common.h"
#include "PMDTypes.h"
//#include "Model.h"
#include "Core/ModelFaceCore.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ModelFace
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelMorph
	: public Animation::IAnimationApplyTarget2
{
public:
    ModelMorph();
    virtual ~ModelMorph();

public:
	void create( Model2* ownerModel, ModelMorphCore* core );
	const lnRefStringW&	getName() const { return mModelMorphCore->Name; }

protected:
	// Animation::IAnimationApplyTarget
	virtual const lnRefStringW& getAnimationTargetName() const { return getName(); }
	virtual void onUpdateAnimationValue( Animation::ValueType type, void* value ) {}

private:
	Model2*			mOwnerModel;
    ModelMorphCore*	mModelMorphCore;
};

//==============================================================================
// ■ ModelFace
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelFace
	: public Base::ReferenceObject
	, public Animation::IAnimationApplyTarget
{
public:
    ModelFace( Model* parentModel, ModelFaceCore* core );
    virtual ~ModelFace();

public:
	const lnRefString&	getName() const { return mModelFaceCore->getName(); }
	ModelFaceCore*		getFaceCore() { return mModelFaceCore; }

public:
	//virtual const lnRefString& getAnimationTargetName() const { return mModelFaceCore->getName(); }
	virtual const Animation::lnRefKeyString& getAnimationTargetName() const { return Animation::lnRefKeyString(); }
	virtual void onUpdateAnimationValue( Animation::ValueType type, void* value );

private:
	Model*			mParentModel;
    ModelFaceCore*	mModelFaceCore;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================