//==============================================================================
// ModelFace 
//==============================================================================

#include "stdafx.h"
#include "Core/ModelCore.h"
#include "Model.h"
#include "ModelFace.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ModelMorph
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelMorph::ModelMorph()
		: mOwnerModel		( NULL )
		, mModelMorphCore	( NULL )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelMorph::~ModelMorph()
	{
		LN_SAFE_RELEASE( mModelMorphCore );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelMorph::create( Model2* ownerModel, ModelMorphCore* core )
	{
		mOwnerModel = ownerModel;
		LN_REFOBJ_SET( mModelMorphCore, core );
	}

//==============================================================================
// ■ ModelFace
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelFace::ModelFace( Model* parentModel, ModelFaceCore* core )
        : mParentModel      ( parentModel )
        , mModelFaceCore	( core ) 
    {
		mModelFaceCore->addRef();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelFace::~ModelFace()
    {
		LN_SAFE_RELEASE( mModelFaceCore );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelFace::onUpdateAnimationValue( Animation::ValueType type, void* value )
	{
		if ( type == Animation::ValueType_Float )
		{
			IVertexBuffer* vb = mParentModel->getModelCore()->getVertexBuffer();
			Vertex::PMDVertex* v = (Vertex::PMDVertex*)vb->lock();
			mModelFaceCore->blendFace( v, *((lnFloat*)value) );
			vb->unlock();
		}
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================