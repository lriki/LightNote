//==============================================================================
// DX9Renderer 
//------------------------------------------------------------------------------
///**
//  @file       DX9Renderer.h
//  @brief      DX9Renderer
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../RendererBase.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ DX9Renderer
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class DX9Renderer
    : public RendererBase
{
public:
	DX9Renderer();
	virtual ~DX9Renderer();

public:

	/// 初期化
    void initialize( DX9GraphicsDevice* device );

public:
	// IRenderer
	virtual void setRenderState( const LRenderState& state, bool reset );
    virtual void setRenderTarget( lnU32 index, Texture* texture, bool reset );
	virtual void setDepthBuffer( Texture* texture, bool reset );
	virtual void setViewport( const LRect& rect );
	virtual void setScissor( const LRect& rect );
	virtual void setVertexBuffer( IVertexBuffer* vertexBuffer, bool reset );
	virtual void setIndexBuffer( IIndexBuffer* indexBuffer, bool reset );

	virtual void clear( bool target, bool depth, const LColorF& color, lnFloat z );
	virtual void drawPrimitive( PrimitiveType primitive, lnU32 startVertex, lnU32 primitiveCount );
	virtual void drawPrimitiveIndexed( PrimitiveType primitive, lnU32 startIndex, lnU32 primitiveCount );
	virtual void beginScene();
	virtual void endScene();


public:
	// RendererBase
    virtual void pauseDevice();
    virtual void resumeDevice();

private:
	DX9GraphicsDevice*	mGraphicsDevice;

	IDirect3DDevice9*	mDxDevice;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================