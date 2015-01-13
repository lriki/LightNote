//==============================================================================
// Common 
//------------------------------------------------------------------------------
///**
//  @file       Common.h
//  @brief      Common
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <vector>
#include "../../../Math/Vector2.h"
#include "../../../Math/Vector3.h"
#include "../../../Math/Vector4.h"
#include "../../../Math/Matrix.h"
#include "../../../Math/Quaternion.h"
#include "../../../Math/SQTTransform.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{
class ModelManager;

class Model;
class ModelFace;

typedef std::vector<ModelFace*>			ModelFaceArray;

class ModelCore2;
class ModelMaterialCore;
class ModelFrameCore;
class ModelIKCore;
class ModelIKCore2;
class ModelIKLimitter2;
class ModelMorphCore;
class ModelRigidBodyCore;
class ModelConstraintCore;
struct ModelIKLink;

class Model2;
class ModelFrame2;

class ModelAnimationController2;

typedef std::vector<int>					ModelFrameCoreIndexArray;
typedef std::vector<ModelFrameCore*>		ModelFrameCoreArray;
typedef std::vector<ModelIKCore*>			ModelIKCoreArray;
typedef std::vector<ModelIKCore2*>			ModelIKCoreArray2;
typedef std::vector<ModelMorphCore*>		ModelMorphCoreArray;
typedef std::vector<ModelRigidBodyCore*>	ModelRigidBodyCoreArray;
typedef std::vector<ModelConstraintCore*>	ModelConstraintCoreArray;

typedef std::vector<ModelFrame2*>			ModelFrameArray2;

enum ModelFormat
{
	ModelFormat_Unknown = 0,
	ModelFormat_PMD,
	ModelFormat_PMX,
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================