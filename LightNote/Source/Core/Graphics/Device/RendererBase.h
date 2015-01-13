//==============================================================================
// RendererBase 
//------------------------------------------------------------------------------
///**
//  @file       RendererBase.h
//  @brief      RendererBase
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Interface.h"
#include "../DeviceObjects.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ RendererBase
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class RendererBase
    : /*public Base::ReferenceObject
    , */public IRenderer
{
public:
	RendererBase();
	virtual ~RendererBase();
    LN_REFOBJ_METHODS;

public:

	/// 作成
	//void initialize( System::IWindow* window, bool fixedAspect );

public:
	virtual void				setRenderState( const LRenderState& state, bool reset ) { mCurrentRenderState = state; }
	virtual const LRenderState& getRenderState() { return mCurrentRenderState; }
    virtual void				setRenderTarget( lnU32 index, Texture* texture, bool reset );
	virtual Texture*			getRenderTarget( lnU32 index ) { return mCurrentRenderTargets[index]; }
	virtual void				setDepthBuffer( Texture* texture, bool reset ) { LN_REFOBJ_SET( mCurrentDepthBuffer, texture ); }
	virtual Texture*			getDepthBuffer() { return mCurrentDepthBuffer; }
	virtual void				setViewport( const LRect& rect ) { mCurrentViewportRect = rect; }
	virtual const LRect&		getViewport() const { return mCurrentViewportRect; }
	virtual void				setScissor( const LRect& rect ) { mCurrentScissorRect = rect; }
	virtual const LRect&		getScissor() const { return mCurrentScissorRect; }
	virtual void				setVertexBuffer( IVertexBuffer* vertexBuffer, bool reset ) { LN_REFOBJ_SET( mCurrentVertexBuffer, vertexBuffer ); }
	virtual void				setIndexBuffer( IIndexBuffer* indexBuffer, bool reset ) { LN_REFOBJ_SET( mCurrentIndexBuffer, indexBuffer ); }

public:
	/// デバイスリセット前処理
    virtual void pauseDevice();

	/// デバイスリセット後処理
    virtual void resumeDevice();

protected:
	LRenderState	mCurrentRenderState;
	Texture*		mCurrentRenderTargets[MaxMultiRenderTargets];
    Texture*		mCurrentDepthBuffer;
	LRect			mCurrentViewportRect;
    LRect			mCurrentScissorRect;
    IVertexBuffer*	mCurrentVertexBuffer;
    IIndexBuffer*	mCurrentIndexBuffer;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================