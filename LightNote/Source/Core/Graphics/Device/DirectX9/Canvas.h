//=============================================================================
//【 Canvas 】
//-----------------------------------------------------------------------------
///**
//  @file       Canvas.h
//  @brief      Canvas
//  @author     Riki
//*/
//=============================================================================

#pragma once

#include "../../Interface.h"
#include "../CanvasBase.h"
#include "Texture.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace DirectX9
{
class GraphicsDevice;

//=============================================================================
// ■ CanvasBase
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class CanvasBase
    : public Graphics::CanvasBase
{
public:
    CanvasBase( GraphicsDevice* device_ );
    virtual ~CanvasBase();

public:

	/// デバイスリセット直前 GraphicsDevice から呼ばれる
    virtual LNRESULT onLostDevice() = 0;

	/// デバイスリセット後に GraphicsDevice から呼ばれる
    virtual LNRESULT onResetDevice() = 0;

protected:

    GraphicsDevice* mGraphicsDevice;
};

//=============================================================================
// ■ DefaultCanvas
//-----------------------------------------------------------------------------
///**
//  @brief		DX9 のデフォルトで作成されるスワップチェインを使ったキャンバス
//*/
//=============================================================================
class DefaultCanvas
    : public CanvasBase
	, public System::IEventListener
{
public:

    DefaultCanvas( GraphicsDevice* device );

    virtual ~DefaultCanvas();

public:

	/// 初期化
    void initialize( System::IWindow* window, bool fixedBackbufferSize );

	/// GraphicsDevice 初期化後呼び出し
	void postInitialize();

	/// キャンバスサイズを取得する
    virtual const Geometry::Size& getSize() { return mBackbufferSize; }

	/// キャンバスサイズを変更する
    virtual LNRESULT resize( lnU32 width_, lnU32 height_ );

	/// バックバッファの取得
    virtual ITexture* getBackbuffer() { return mBackbuffer; }

	/// バックバッファのレンダリングターゲット、深度バッファを設定する
    virtual LNRESULT activate();

	/// バックバッファの内容をウィンドウへ転送する
    virtual LNRESULT present();

	/// 描画先のウィンドウサイズを設定する
    virtual void setWindowSize( const Geometry::Size& size_ );

	/// キャンバス→ウィンドウへ転送する際の座標変換行列の取得
    virtual const LMatrix& getScreenTransform() const { return mScreenTransformMatrix; }

	/// present() の転送先ウィンドウのウィンドウハンドルの設定
	virtual void setTargetWindowHandle(HWND hwnd) { mTargetWindowHandle = hwnd; }


public:

	/// デバイスリセット直前 GraphicsDevice から呼ばれる
    virtual LNRESULT onLostDevice();

	/// デバイスリセット後に GraphicsDevice から呼ばれる
    virtual LNRESULT onResetDevice();

	virtual bool onEvent( const System::EventArgs& e );

private:

	void _refreshResource();
	void _releaseResource();

private:
	
    IDirect3DDevice9*       mDxDevice;
	System::IWindow*		mTargetWindow;
    SystemCreatedTexture*   mDefaultBackbuffer;
    ITexture*				mBackbuffer;
    ITexture*               mDepthBuffer;

    Geometry::Size			mWindowSize;
    Geometry::Size			mBackbufferSize;
    LColorF                  mBackgroundColor;
    lnU32					mBackgroundColorCode;

    RECT                    mViewDestRect;           ///< 転送先矩形
    LMatrix                 mScreenTransformMatrix;

	HWND					mTargetWindowHandle;

	bool					mFixedBackbufferSize;
	bool					mModified;
};

} // namespace DirectX9
} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//
//=============================================================================