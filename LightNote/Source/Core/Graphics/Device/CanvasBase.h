//==============================================================================
// CanvasBase 
//------------------------------------------------------------------------------
///**
//  @file       CanvasBase.h
//  @brief      CanvasBase
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Interface.h"
#include "GraphicsResource.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ CanvasBase
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class CanvasBase
    : /*public Base::ReferenceObject
	,*/ public GraphicsResource
    , public ICanvas
{
public:
	CanvasBase();
	virtual ~CanvasBase();
//    LN_REFOBJ_METHODS;

public:

	/// 作成
	void create(GraphicsDeviceBase* device, System::Window* window, const LSize& backbufferSize, BackbufferResizeMode resizeMode);
	
	bool isFixedAspect() const { return mRequestedBackbufferSize.w != 0 && mRequestedBackbufferSize.h != 0; }

	bool isLetterBox() const { return mBackbufferResizeMode == BackbufferResizeMode_ScalingWithLetterBox/* || mBackbufferResizeMode == BackbufferResizeMode_ResizeWithLetterBox*/; }

	const LSize& getRequestedBackbufferSize() const { return mRequestedBackbufferSize; }
	BackbufferResizeMode getBackbufferResizeMode() const { return mBackbufferResizeMode; }

public:
	virtual const LSize& getSize() { return mCurrentBackbufferSize; }
    virtual void resize( const LSize& size );
    virtual void present();
	virtual const LMatrix& getScreenTransform() const { return mScreenTransform; }

protected:
	void updateViewBoxMatrix(/*const LSize& newWindowSize, const const LSize& newBackbufferSize*/);

protected:
	System::Window*			mTargetWindow;
	LSize					mRequestedBackbufferSize;
	LSize					mCurrentBackbufferSize;
	LSize					mWindowSize;
	LRect					mViewDestRect;
	LMatrix					mScreenTransform;
	LMatrix					mBackbufferOffset;
	BackbufferResizeMode	mBackbufferResizeMode;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================