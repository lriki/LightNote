//==============================================================================
// DX9Canvas
//------------------------------------------------------------------------------
///**
//  @file       DX9Canvas.h
//  @brief      DX9Canvas
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../CanvasBase.h"
#include "DX9Texture.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// DX9Canvas
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class DX9Canvas
    : public CanvasBase
{
public:
	DX9Canvas();
	virtual ~DX9Canvas();

public:

	/// 作成
	void create(DX9GraphicsDevice* device, System::Window* window, const LSize& backbufferSize, BackbufferResizeMode resizeMode);

	/// この関数はデバイスが初期化された直後に呼ばれる
	void postInitialize();

	void onPrePauseDevice() { _releaseResource(); }
	void onPostResumeDevice() { _refreshResource(); }

public:
	// ICanvas
    virtual void resize( const LSize& size );
	virtual Texture* getBackbuffer() { return mBackbuffer; }
    virtual void activate();
    virtual void present();
	virtual Texture* getBackendBuffer() { return mBackendBuffer; }

public:
	// GraphicsResource
	virtual void onPauseDevice() { /*_releaseResource();*/ }
    virtual void onResumeDevice() { /*_refreshResource();*/ }

private:
	void _refreshResource();
	void _releaseResource();

private:
	HWND						mTargetWindowHandle;
	DX9GraphicsDevice*			mGraphicsDevice;
	DX9SystemCreatedTexture*	mBackendBuffer;
	Texture*					mBackbuffer;
	Texture*					mDepthBuffer;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
