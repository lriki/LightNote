//==============================================================================
// GLTexture 
//------------------------------------------------------------------------------
///**
//  @file       GLTexture.h
//  @brief      GLTexture
//  @author     Riki
//*/
//==============================================================================

#pragma once

#ifdef LNOTE_FOR_200
#include <glfw/include/GLFW/glfw3.h>
#include "../../Interface.h"
#include "GLResource.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ GLTextureBase
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class GLTextureBase
    : public GLResource
    , public ITexture
{
public:
	GLTextureBase();
	virtual ~GLTextureBase();
	LN_REFOBJ_METHODS;

public:
	/// GLテクスチャの取得
	virtual GLuint getGLTexture() = 0;

public:
	// override ITexture
	virtual SurfaceFormat getSurfaceFormat() const { LN_THROW_NotImpl(0); return mSurfaceFormat; }

private:
	SurfaceFormat	mSurfaceFormat;
};

//==============================================================================
// ■ GLTexture
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class GLTexture
    : public GLTextureBase
{
public:
	GLTexture();
	virtual ~GLTexture();

public:

private:
};

//==============================================================================
// ■ GLRenderTargetTexture
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class GLRenderTargetTexture
    : public GLTextureBase
{
public:
	GLRenderTargetTexture();
	virtual ~GLRenderTargetTexture();

public:

private:
};

//==============================================================================
// ■ GLDepthBuffer
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class GLDepthBuffer
    : public GLTextureBase
{
public:
	GLDepthBuffer();
	virtual ~GLDepthBuffer();

public:

private:
};

//==============================================================================
// ■ GLBackbufferRenderTarget
//------------------------------------------------------------------------------
///**
//  @brief		バックバッファのダミークラス
//	@note
//				GLRenderer::setRenderTarget() に渡すと、バックバッファをターゲットにする。
//				glFramebufferTexture2D() に 0 を渡すとバックバッファを指すことを利用している。
//*/
//==============================================================================
class GLBackbufferRenderTarget
    : public GLTextureBase
{
public:
	GLBackbufferRenderTarget() {}
	virtual ~GLBackbufferRenderTarget() {}

public:

	/// サイズ設定 (GL のバックバッファ == ウィンドウサイズ)
	void setSize( int width, int height );

public:
	// ITexture
	virtual LNTextureType	getType() const { return LN_TEXTURETYPE_RENDERTARGET; }
	virtual const LVector2& getSize() const { return mSize; }
	virtual const LVector2& getRealSize( bool isReciprocal ) const { return ( isReciprocal ) ? mRealSizeInv : mRealSize; }
	virtual void			clear(const LColor& color) { LN_THROW_InvalidCall(0); }
	virtual lnU32*			lock() { LN_THROW_InvalidCall( 0 ); return NULL; }
	virtual void			unlock() { LN_THROW_InvalidCall( 0 ); }
	virtual void			setFont( IFont* font ) { LN_THROW_InvalidCall( 0 ); }
	virtual IFont*			getFont() const { LN_THROW_InvalidCall( 0 ); }
	virtual void			drawText( const lnChar* text, const LRect& rect, TextAlign align, int length ) { LN_THROW_InvalidCall( 0 ); }
	virtual void			blt( int x, int y, ITexture* srcTexture, const LRect& srcRect ) { LN_THROW_InvalidCall( 0 ); }
	virtual const lnChar*	getFilename() const { return NULL; }

public:
	// GLTextureBase
	virtual GLuint getGLTexture() { return 0; } // glFramebufferTexture2D() の texture に 0 を渡すとバックバッファをターゲットするの OPenGL の仕様
	
protected:
	// GLResource
	virtual void onCommitGLResource() {}
	virtual void onDisposeGLResource() {}

private:
	LVector2	mSize;
    LVector2	mRealSize;
    LVector2	mRealSizeInv;
};

//==============================================================================
// ■ GLBackbufferDepth
//------------------------------------------------------------------------------
///**
//  @brief		バックバッファのダミークラス
//	@note
//				GLRenderer::setDepthBuffer() に渡すと、バックバッファをターゲットにする。
//*/
//==============================================================================
class GLBackbufferDepth
    : public GLTextureBase
{
public:
	GLBackbufferDepth() {}
	virtual ~GLBackbufferDepth() {}
	
public:

	/// サイズ設定 (GL のバックバッファ == ウィンドウサイズ)
	void setSize( int width, int height );

public:
	// ITexture
	virtual LNTextureType	getType() const { return LN_TEXTURETYPE_RENDERTARGET; }
	virtual const LVector2& getSize() const { return mSize; }
	virtual const LVector2& getRealSize( bool isReciprocal ) const { return ( isReciprocal ) ? mRealSizeInv : mRealSize; }
	virtual void			clear(const LColor& color) { LN_THROW_InvalidCall(0); }
	virtual lnU32*			lock() { LN_THROW_InvalidCall( 0 ); return NULL; }
	virtual void			unlock() { LN_THROW_InvalidCall( 0 ); }
	virtual void			setFont( IFont* font ) { LN_THROW_InvalidCall( 0 ); }
	virtual IFont*			getFont() const { LN_THROW_InvalidCall( 0 ); }
	virtual void			drawText( const lnChar* text, const LRect& rect, TextAlign align, int length ) { LN_THROW_InvalidCall( 0 ); }
	virtual void			blt( int x, int y, ITexture* srcTexture, const LRect& srcRect ) { LN_THROW_InvalidCall( 0 ); }
	virtual const lnChar*	getFilename() const { return NULL; }

public:
	// GLTextureBase
	virtual GLuint getGLTexture() { return 0; }

protected:
	// GLResource
	virtual void onCommitGLResource() {}
	virtual void onDisposeGLResource() {}

private:
	LVector2	mSize;
    LVector2	mRealSize;
    LVector2	mRealSizeInv;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote


#else
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Interface.h"
#include "GLUtil.h"
#include "GLGraphicsDevice.h"

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



//==============================================================================
// ■ TextureBase クラス
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class TextureBase
    : public GLResource//public Base::ReferenceObject
    , public ITexture
{
public:

	/// コンストラクタ
    TextureBase() {}

	/// デストラクタ
    virtual ~TextureBase() {}

    LN_REFOBJ_METHODS;

public:

	/// ファイルから作成したときのファイル名
    virtual const lnChar* getFilename() const { return NULL; }

	/// GLテクスチャの取得
    virtual GLuint getGLTexture() = 0;

 //   /// デバイスリセット直前のコールバック
 //   virtual void onLostDevice() = 0;

	///// デバイスリセット直後のコールバック
 //   virtual void onResetDevice() = 0;

	// 実装つぶし
    virtual void* getIDirect3DTexture9() { return NULL; }
    virtual void* getIDirect3DSurface9() { return NULL; }
};

//==============================================================================
// ■ Texture クラス
//------------------------------------------------------------------------------
///**
//  @brief		普通のテクスチャのクラス
//*/
//==============================================================================
class Texture
    : public TextureBase
    , public TextureCache::CachedObject
{
public:
    
	Texture( GraphicsDevice* device_ );

	virtual ~Texture();

    LN_CACHED_REFOBJ_METHODS;

public:

	/// テクスチャ作成
	void initialize( lnU32 width_, lnU32 height_, lnU32 levels_, LNSurfaceFormat format_ );

	/// テクスチャ作成 (ファイルから)
	void initialize( const lnChar* filename_, lnU32 color_key_, lnU32 levels_, LNSurfaceFormat format_ );

    

	/// 種類の取得
	virtual LNTextureType getType() const { return LN_TEXTURETYPE_NORMAL; }

	/// サイズの取得
    virtual const LVector2& getSize() const { return mSize; }

	/// 実際のサイズの取得
    virtual const LVector2& getRealSize( bool is_reciprocal_ ) const { return ( is_reciprocal_ ) ? mRealSizeInv : mRealSize; }

	/// クリア
	virtual void clear( const LColorF& color_ );

	/// ロック
	virtual lnU32* lock();

	/// アンロック
	virtual void unlock();

	/// drawText() での文字列描画に使用するフォントの設定
    virtual void setFont( IFont* font_ ) { LN_SAFE_RELEASE( mFont ); mFont = font_; LN_SAFE_ADDREF( mFont ); }

	/// drawText() での文字列描画に使用するフォントの取得
    virtual IFont* getFont() const { return mFont; }

	/// 文字列の描画
    virtual LNRESULT drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_, int len_ );

	/// テクスチャのブロック転送 (現在、普通のテクスチャ (RT、深度バッファではない) にのみ有効)
    virtual LNRESULT blt( int x_, int y_, ITexture* src_texture_, const Geometry::Rect& src_rect_ );

	/// ファイルから作成したときのファイル名
    virtual const lnChar* getFilename() const { return mFilename.c_str(); }

public:

	/// GLテクスチャの取得
    virtual GLuint getGLTexture() { return mGLTexture; }

 //   /// デバイスリセット直前のコールバック
 //   virtual void onLostDevice();

	///// デバイスリセット直後のコールバック
	//virtual void onResetDevice();

    virtual void onCommitGLResource();
    virtual void onDisposeGLResource();
    void onLostDevice() { GLResource::onLostDevice(); }
    void onResetDevice() { GLResource::onResetDevice(); }

private:


private:

    GraphicsDevice*     mGraphicsDevice;    ///< 管理クラス
    GLuint              mGLTexture;
    lnU32*              mMemTexture;

    lnU32               mMipLevel;


    LVector2            mSize;              ///< 作成時に要求したサイズ
    LVector2            mRealSize;          ///< 実際のテクスチャのサイズ
    LVector2            mRealSizeInv;       ///< 実際のテクスチャサイズの逆数
    LNSurfaceFormat	    mFormat;			///< サーフェイスフォーマット

	IFont*              mFont;

    LRefTString         mFilename;
    lnU32               mColorKey;

	//lnSharingKey mSharingKey;
};


//==============================================================================
// ■ RenderTargetTexture クラス
//------------------------------------------------------------------------------
///**
//  @brief		レンダーターゲットテクスチャのクラス
//*/
//==============================================================================
class RenderTargetTexture
    : public TextureBase
{
public:

	/// コンストラクタ
	RenderTargetTexture( GraphicsDevice* device_ );

	/// デストラクタ
	virtual ~RenderTargetTexture();

    LN_REFOBJ_METHODS;

public:

	/// レンダーターゲットテクスチャの作成
	void initialize( lnU32 width_, lnU32 height_, lnU32 levels_, LNSurfaceFormat format_ );

	/// 種類の取得
	virtual LNTextureType getType() const { return LN_TEXTURETYPE_RENDERTARGET; }

	/// サイズの取得
    virtual const LVector2& getSize() const { return mSize; }

	/// 実際のサイズの取得
	virtual const LVector2& getRealSize( bool is_reciprocal_ ) const { return ( is_reciprocal_ ) ? mRealSizeInv : mRealSize; }

	/// クリア ( レンダリングターゲットではサポートしてません )
    void clear( const LColorF& color_ ) { }

	/// ロック
    lnU32* lock() { return NULL; }

	/// アンロック
    void unlock() { }

	/// drawText() での文字列描画に使用するフォントの設定
    virtual void setFont( IFont* font_ ) { }

	/// drawText() での文字列描画に使用するフォントの取得
    virtual IFont* getFont() const { return NULL; }

	/// 文字列の描画
    virtual LNRESULT drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_, int len_ ) { return LN_ERR_INVALIDCALL; }

	/// テクスチャのブロック転送
    virtual LNRESULT blt( int x_, int y_, ITexture* src_texture_, const Geometry::Rect& src_rect_ ) { return LN_ERR_INVALIDCALL; }

public:

	/// GLテクスチャの取得
    virtual GLuint getGLTexture() { return mGLTexture; }

 //   /// デバイスリセット直前のコールバック
	//virtual void onLostDevice();

	///// デバイスリセット直後のコールバック
	//virtual void onResetDevice();



    virtual void onCommitGLResource();
    virtual void onDisposeGLResource();
    void onLostDevice() { GLResource::onLostDevice(); }
    void onResetDevice() { GLResource::onResetDevice(); }



private:

    GraphicsDevice*     mGraphicsDevice;    ///< 管理クラス

    GLuint              mGLTexture;

    LVector2            mSize;              ///< 作成時に要求したサイズ
    LVector2            mRealSize;          ///< 実際のテクスチャのサイズ
    LVector2            mRealSizeInv;       ///< 実際のテクスチャサイズの逆数
    lnU32                 mMipLevel;			///< ミップマップレベル
	LNSurfaceFormat	    mFormat;			///< サーフェイスフォーマット

};

//==============================================================================
// ■ DepthBuffer クラス
//------------------------------------------------------------------------------
///**
//  @brief		深度バッファのクラス
//*/
//==============================================================================
class DepthBuffer
    : public TextureBase
{
public:

	/// コンストラクタ
	DepthBuffer( GraphicsDevice* device_ );

	/// デストラクタ
	virtual ~DepthBuffer();

    LN_REFOBJ_METHODS;

public:

	/// 深度バッファの作成
	void initialize( lnU32 width_, lnU32 height_, LNSurfaceFormat format_ );

	/// 種類の取得
	virtual LNTextureType getType() const { return LN_TEXTURETYPE_DEPTH; }

	/// サイズの取得
    virtual const LVector2& getSize() const { return mSize; }

	/// 実際のサイズの取得
	virtual const LVector2& getRealSize( bool is_reciprocal_ ) const { return ( is_reciprocal_ ) ? mRealSizeInv : mRealSize; }

	/// クリア ( 深度バッファではサポートしてません )
    void clear( const LColorF& color_ ) { }

	/// ロック
    lnU32* lock() { return NULL; }

	/// アンロック
    void unlock() { }

	/// drawText() での文字列描画に使用するフォントの設定
    virtual void setFont( IFont* font_ ) { }

	/// drawText() での文字列描画に使用するフォントの取得
    virtual IFont* getFont() const { return NULL; }

	/// 文字列の描画
    virtual LNRESULT drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_, int len_ ) { return LN_ERR_INVALIDCALL; }

	/// テクスチャのブロック転送
    virtual LNRESULT blt( int x_, int y_, ITexture* src_texture_, const Geometry::Rect& src_rect_ ) { return LN_ERR_INVALIDCALL; }

public:

	/// GLテクスチャの取得
    virtual GLuint getGLTexture() { return mGLTexture; }

 //   /// デバイスリセット直前のコールバック
	//virtual void onLostDevice();

	///// デバイスリセット直後のコールバック
	//virtual void onResetDevice();

    virtual void onCommitGLResource();
    virtual void onDisposeGLResource();
    void onLostDevice() { GLResource::onLostDevice(); }
    void onResetDevice() { GLResource::onResetDevice(); }

private:

	/// glTexImage2D() で設定しなおし
	//void _refresh();

private:

    GraphicsDevice*     mGraphicsDevice;    ///< 管理クラス
	GLuint              mGLTexture;
    LVector2            mSize;              ///< 作成時に要求したサイズ
    LVector2            mRealSize;          ///< 実際のテクスチャのサイズ
    LVector2            mRealSizeInv;       ///< 実際のテクスチャサイズの逆数
	LNSurfaceFormat	    mFormat;			///< サーフェイスフォーマット
};

#if 0
//==============================================================================
// ■ SystemCreatedTexture クラス
//------------------------------------------------------------------------------
///**
//  @brief		DirectX 側で自動的に作成されたテクスチャを扱うクラス
//
//  @note
//              とりあえずデフォルトのスワップチェインの処理で使ってる。
//*/
//==============================================================================
class SystemCreatedTexture
    : public DX9TextureBase
{
public:

	/// コンストラクタ
	SystemCreatedTexture();

	/// デストラクタ
	virtual ~SystemCreatedTexture();

    LN_REFOBJ_METHODS;

public:

	/// テクスチャ、サーフェイスのセット (texture_ は NULL でもOK。surface_ は必ず。デバイスリセット時は両方 NULL で参照カウントをデクリメント)
	LNRESULT setTextureAndSurface( IDirect3DTexture9* texture_, IDirect3DSurface9* surface_ );

	/// 種類の取得
	virtual LNTextureType getType() const { return LN_TEXTURETYPE_RENDERTARGET; }

	/// サイズの取得
    virtual const LVector2& getSize() const { return mSize; }

	/// 実際のサイズの取得
	virtual const LVector2& getRealSize( bool is_reciprocal_ ) const { return ( is_reciprocal_ ) ? mRealSizeInv : mRealSize; }

	/// クリア
    void clear( const LColorF& color_ ) { printf( "レンダーターゲットテクスチャの clear() はサポートしていません。\n" ); }

	/// drawText() での文字列描画に使用するフォントの設定
    virtual void setFont( IFont* font_ ) { }

	/// drawText() での文字列描画に使用するフォントの取得
    virtual IFont* getFont() const { return NULL; }

	/// 文字列の描画
    virtual LNRESULT drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_, int len_ ) { return LN_ERR_INVALIDCALL; }

	/// テクスチャのブロック転送
    virtual LNRESULT blt( int x_, int y_, ITexture* src_texture_, const Geometry::Rect& src_rect_ ) { return LN_ERR_INVALIDCALL; }

	/// ロック
    virtual lnU32* lock() { return NULL; }

	/// アンロック
    virtual void unlock() {}

public:

	/// IDirect3DTexture9 ポインタの取得
    virtual IDirect3DTexture9* getDxTexture() { return mDxTexture; }

	/// テクスチャサーフェイスへのポインタ取得
	virtual IDirect3DSurface9* getDxSurface() { return mDxSurface; }

	/// デバイスリセット直前のコールバック
    virtual LNRESULT onLostDevice() { return LN_OK; }

	/// デバイスリセット直後のコールバック
    virtual LNRESULT onResetDevice() { return LN_OK; }

private:

    IDirect3DTexture9*  mDxTexture;
	IDirect3DSurface9*	mDxSurface;			///< テクスチャのサーフェイス

    LVector2            mSize;              ///< 作成時に要求したサイズ
    LVector2            mRealSize;          ///< 実際のテクスチャのサイズ
    LVector2            mRealSizeInv;       ///< 実際のテクスチャサイズの逆数
};

#endif

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