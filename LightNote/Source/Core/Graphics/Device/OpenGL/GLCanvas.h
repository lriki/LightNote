//==============================================================================
// GLCanvas 
//------------------------------------------------------------------------------
///**
//  @file       GLCanvas.h
//  @brief      GLCanvas
//  @author     Riki
//*/
//==============================================================================

#pragma once

#ifdef LNOTE_FOR_200
#include <glfw/include/GLFW/glfw3.h>
#include "../CanvasBase.h"
#include "GLTexture.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ GLCanvas
//------------------------------------------------------------------------------
///**
//  @brief
//	@note
//				GLCanvas はひとつしか作成できない。
//*/
//==============================================================================
class GLCanvas
    : public CanvasBase
{
public:
	GLCanvas();
	virtual ~GLCanvas();

public:

	/// 作成
	void create(GLGraphicsDevice* device, System::Window* window, const LSize& backbufferSize, BackbufferResizeMode resizeMode);

public:
	// ICanvas
    virtual void resize( const LSize& size );
	virtual ITexture* getBackbuffer() { return mBackbufferTarget; }
    virtual void activate();
    virtual void present();
	virtual ITexture* getBackendBuffer() { return mBackendBufferTarget; }

public:
	// GraphicsResource
	virtual void onPauseDevice() { _releaseResource(); }
    virtual void onResumeDevice() { _refreshResource(); }

private:
	void _refreshResource();
	void _releaseResource();
	
private:
	GLGraphicsDevice*			mGraphicsDevice;
	GLRenderTargetTexture*		mBackbufferTarget;
	GLRenderTargetTexture*		mBackbufferDepth;
	GLBackbufferRenderTarget*	mBackendBufferTarget;
	GLBackbufferDepth*			mBackendBufferDepth;
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
#include "GLTexture.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System { class IWindow; }

namespace Graphics
{
namespace OpenGL
{
class GraphicsDevice;

//==============================================================================
// ■ DefaultCanvas クラス
//------------------------------------------------------------------------------
///**
//  @brief
//
//  @note
//              OpenGL のキャンバスは常に1つ。
//*/
//==============================================================================
class DefaultCanvas
    : public Base::ReferenceObject
    , public ICanvas
{
public:

    struct InitData
    {
        //OpenGL::PlatformContext*    PlatformContext;
        System::IWindow*            Window;
        Geometry::Size				CanvasSize;
    };

public:

    DefaultCanvas( GraphicsDevice* device_ );

    virtual ~DefaultCanvas();
    
    LN_REFOBJ_METHODS;

public:

	/// 初期化
    void initialize( const InitData& init_data_ );//System::Window* window_ );

	/// キャンバスサイズを取得する
    virtual const Geometry::Size& getSize() { return mCanvasSize; }

	/// キャンバスサイズを変更する
    virtual LNRESULT resize( lnU32 width_, lnU32 height_ );

	/// 仮想バックバッファの取得
    virtual ITexture* getBackbuffer() { LN_PRINT_NOT_IMPL_FUNCTION; return NULL; }

	/// 仮想バックバッファのレンダリングターゲット、深度バッファを設定する
    virtual LNRESULT activate();

	/// バックバッファの内容をウィンドウへ転送する
    virtual LNRESULT present();

	/// 描画先のウィンドウサイズを設定する
    virtual void setWindowSize( const Geometry::Size& size_ );

	/// キャンバス→ウィンドウへ転送する際の座標変換行列の取得
    virtual const LMatrix& getScreenTransform() const { return mScreenTransformMatrix; }

#if defined(LNOTE_WIN32)
	/// present() の転送先ウィンドウのウィンドウハンドルの設定
	virtual void setTargetWindowHandle(HWND hwnd) { }
#endif

public:

    const Geometry::Size& getWindowSize() { return mWindowSize; }

	/// デバイスリセット直前 GraphicsDevice から呼ばれる
    void onLostDevice();

	/// デバイスリセット後に GraphicsDevice から呼ばれる (EGLDisplay、EGLConfig 初期化済みであること)
    void onResetDevice();

private:

    GraphicsDevice*         mGraphicsDevice;
	//PlatformContext*        mPlatformContext;
    System::IWindow*        mWindow;


	//EGLDisplay              mEGLDisplay;
	//EGLSurface              mEGLSurface;

    Geometry::Size			mWindowSize;
    Geometry::Size			mCanvasSize;
    LColorF                  mBackgroundColor;
    lnU32                     mBackgroundColorCode;

    ITexture*               mCanvasBuffer;
    ITexture*               mCanvasDepthBuffer;

	//RECT                    mViewDestRect;           ///< 転送先矩形
    LMatrix                 mScreenTransformMatrix;

    LMatrix                 mTransformMatrix;
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