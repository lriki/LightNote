//==============================================================================
// ModelMaterialCore 
//------------------------------------------------------------------------------
///**
//  @file       ModelMaterialCore.h
//  @brief      ModelMaterialCore
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Common.h"
#include "../../Common.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{
typedef std::vector<LMaterial>			MaterialArray;
typedef std::vector<LNMeshAttribute>	MeshAttributeArray;

//==============================================================================
// ModelMaterialCore
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelMaterialCore
    : public Base::ReferenceObject
{
public:
	ModelMaterialCore() {}
	virtual ~ModelMaterialCore() {}

public:
	void postInitialize();

	ModelMaterialCore* copy() const
	{
		ModelMaterialCore* m = LN_NEW ModelMaterialCore();
		m->Materials = Materials;
		m->Attributes = Attributes;
		return m;
	}

public:
	MaterialArray		Materials;
	MeshAttributeArray	Attributes;

private:
	MaterialArray		mOriginalMaterials;		///< 材質モーフ用オリジナルマテリアル
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
