//==============================================================================
// GLRenderer 
//------------------------------------------------------------------------------
///**
//  @file       GLRenderer.h
//  @brief      GLRenderer
//  @author     Riki
//*/
//==============================================================================

#pragma once

#ifdef LNOTE_FOR_200
#include <glfw/include/GLFW/glfw3.h>
#include "../RendererBase.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ GLRenderer
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class GLRenderer
    : public RendererBase
{
public:
	GLRenderer();
	virtual ~GLRenderer();

public:

	/// 初期化
	void initialize( GLGraphicsDevice* device );

	/// 変更を反映して描画できる状態にする (実際のフレームバッファの設定など)
	void commitChanges() { _refreshFramebuffer(); }

public:
	// IRenderer
	virtual void setRenderState( const LRenderState& state, bool reset );
    virtual void setRenderTarget( lnU32 index, ITexture* texture, bool reset );
	virtual void setDepthBuffer( ITexture* texture, bool reset );
	virtual void setViewport( const LRect& rect );
	virtual void setScissor( const LRect& rect );
	virtual void setVertexBuffer( IVertexBuffer* vertexBuffer, bool reset );
	virtual void setIndexBuffer( IIndexBuffer* indexBuffer, bool reset );

	virtual void clear( bool target, bool depth, const LColorF& color, lnFloat z );
	virtual void drawPrimitive( LNPrimitiveType primitive, lnU32 startVertex, lnU32 primitiveCount );
	virtual void drawPrimitiveIndexed( LNPrimitiveType primitive, lnU32 startIndex, lnU32 primitiveCount );
	virtual void beginScene();
	virtual void endScene();

public:
	// RendererBase
    virtual void pauseDevice();
    virtual void resumeDevice();

private:
	void _dispose();

	// 実際にカラー、深度バッファを設定する (カラー、深度をワンセットで設定する必要がある)
	void _refreshFramebuffer();

private:
	GLGraphicsDevice*	mGraphicsDevice;

    GLuint				mGLFramebuffer;
	
    LSize				mCurrentRenderTarget0Size;
    LSize				mCurrentRenderTarget0RealSize;
    LRect				mCurrentViewportRealRect;

};

} // namespace Graphics
} // namespace Core
} // namespace LNote

#else

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Interface.h"
#include "GLPlatformContext.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace OpenGL
{
class GraphicsDevice;
class VertexBuffer;
class IndexBuffer;

//==============================================================================
// ■ Renderer クラス
//------------------------------------------------------------------------------
///**
//  @brief     
//*/
//==============================================================================
class Renderer
    : public Base::ReferenceObject
    , public IRenderer
{
public:

	/// initialize() に渡す初期化データ
    struct InitData
    {
        Base::LogFile*              LogFile;
        OpenGL::GraphicsDevice*     GraphicsDevice;
        //OpenGL::PlatformContext*    PlatformContext;
    }; 

public:

    Renderer();

    virtual ~Renderer();

    LN_REFOBJ_METHODS;

public:

	/// 初期化
    void initialize( const InitData& init_data_ );

	/// レンダリングステートの設定
    virtual LNRESULT setRenderState( const LRenderState& state_ );
	virtual const LRenderState& getRenderState() { return mCurrentRenderState; }

	/// レンダリングターゲットの取得 (参照カウントに変化はナシ)
    virtual ITexture* getRenderTarget( lnU32 index_ ) { return mCurrentRenderTargets[ index_ ]; }

	// レンダリングターゲットの設定
    virtual LNRESULT setRenderTarget( lnU32 index_, ITexture* texture_, bool reset_ = false );

	/// 深度バッファの取得 (参照カウントに変化はナシ)
    virtual ITexture* getDepthBuffer() { return mCurrentDepthBuffer; }

	/// 深度バッファの設定
    virtual LNRESULT setDepthBuffer( ITexture* texture_ );

	/// ビューポート矩形の取得
    virtual const Geometry::Rect& getViewport() const;

	/// ビューポート矩形の設定
	virtual LNRESULT setViewport( const Geometry::Rect& viewport_rect_ );

	/// シザー矩形の取得
    virtual const Geometry::Rect& getScissor() const;

	/// シザー矩形の設定
	virtual LNRESULT setScissor( const Geometry::Rect& rect_ );

	/// 頂点バッファの設定
	virtual LNRESULT setVertexBuffer( IVertexBuffer* vertex_buffer_, bool reset_ = false );

	/// インデックスバッファの設定
	virtual LNRESULT setIndexBuffer( IIndexBuffer* index_buffer_, bool reset_ = false );

	/// 設定されているカラーバッファ、深度バッファをクリアする
	virtual LNRESULT clear( bool target_, bool depth_, lnU32 color_, lnFloat z_ = 1.0f );
	virtual void clear( bool target, bool depth, const LColorF& color, lnFloat z );

	/// プリミティブ描画
	virtual LNRESULT drawPrimitive( LNPrimitiveType primitive_, lnU32 start_vertex_, lnU32 primitive_count_ );

	/// プリミティブ描画 ( インデックス付き )
	virtual LNRESULT drawPrimitiveIndexed( LNPrimitiveType primitive_, lnU32 start_index_, lnU32 primitive_count_ );

	/// シーン開始
	virtual LNRESULT beginScene();

	/// シーン終了
	virtual LNRESULT endScene();

public: 

	/// GraphicsDevice::onLostDevice() から呼ばれる
    void onLostDevice();

	/// GraphicsDevice::onResetDevice() から呼ばれる
    void onResetDevice();

private:

	/// 後で変更しないレンダリングステートをデバイスに設定する
    void _initRenderState();

	/// 実際にレンダリングステートをデバイスに設定する
    LNRESULT _updateRenderState( bool reset_ );

	/// 実際にカラー、深度バッファを設定する
    void _refreshFramebuffer();

private:

    Base::LogFile*          mLogFile;
    GraphicsDevice*	        mGraphicsDevice;    ///< 参照カウントは増やさない
	//PlatformContext*        mPlatformContext;

    GLuint                  mGLFramebuffer;

    LRenderState            mCurrentRenderState;
    LRenderState            mRequestRenderState;

    ITexture*               mCurrentRenderTargets[ MaxMultiRenderTargets ];
    ITexture*               mCurrentDepthBuffer;
    Geometry::Size			mCurrentRenderTarget0Size;
    Geometry::Size			mCurrentRenderTarget0RealSize;


    Geometry::Rect			mCurrentViewportRect;
    Geometry::Rect			mCurrentViewportRealRect;

    VertexBuffer*           mCurrentVertexBuffer;
    IndexBuffer*            mCurrentIndexBuffer;

    Geometry::Rect			mCurrentScissorRect;
    bool                    mEnableScissorTest;
    bool                    mSceneBegan;
    bool                    mIsDeviceLost;

};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace OpenGL
} // namespace Graphics
} // namespace Core
} // namespace LNote

#endif

//==============================================================================
//
//==============================================================================