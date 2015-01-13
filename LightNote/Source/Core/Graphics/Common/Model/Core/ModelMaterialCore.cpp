//==============================================================================
// ModelMaterialCore 
//==============================================================================

#pragma once

#include "stdafx.h"
#include "ModelMaterialCore.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ModelMaterialCore
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelMaterialCore::postInitialize()
	{
		mOriginalMaterials = Materials;
	}


} // namespace Graphics
} // namespace Core
} // namespace LNote
