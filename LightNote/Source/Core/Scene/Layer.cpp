//==============================================================================
// Layer 
//==============================================================================

#include "stdafx.h"
#include "SceneNode/Camera.h"
#include "Layer.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// LayerBase
//==============================================================================

	LN_TYPE_INFO_ACCESS_IMPL(LayerBase);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LayerBase::LayerBase()
		: mParentPane(NULL)
		, mPriority(0)
	{
	}


//==============================================================================
// DrawingLayerContext
//==============================================================================
class DrawingLayer::DrawingLayerContext
	: public LayerContextBase
{
public:

	DrawingLayerContext(LayerBase* owner)
		: LayerContextBase(owner)
	{
	}

public:

	virtual void postRender(ContextDrawParam* param)
	{
		mCamera->getCameraContext()->render(param);
	}

public:

	Camera* mCamera;
};

//==============================================================================
// DrawingLayer
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	DrawingLayer::DrawingLayer()
		: mContext(NULL)
		, mCamera(NULL)
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	DrawingLayer::~DrawingLayer()
	{
		LN_SAFE_RELEASE(mContext);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingLayer::create(Camera* camera)
	{
		LN_ERR2_ASSERT(camera->getParentLayer() == NULL)

		mContext = LN_NEW DrawingLayerContext(this);
		mContext->mCamera = camera;

		mCamera = camera;
		mCamera->setParentLayer(this);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingLayer::updateFrame(float elapsedTime)
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingLayer::updateContext()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingLayer::onUpdateViewSize(const LVector2& viewSize)
	{
		mCamera->onUpdateViewSize(viewSize);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LayerContextBase* DrawingLayer::getContext()
	{
		return mContext;
	}


} // namespace Scene
} // namespace Core
} // namespace LNote
