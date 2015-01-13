//==============================================================================
// DX9Texture 
//------------------------------------------------------------------------------
///**
//  @file       DX9Texture.h
//  @brief      DX9Texture
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Interface.h"
#include "../../Device/Texture.h"
#include "../GraphicsResource.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ DX9TextureBase
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class DX9TextureBase
    : public GraphicsResource
    , public Texture
	, public Base::ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:
	DX9TextureBase();
	virtual ~DX9TextureBase();

public:
	virtual IDirect3DTexture9* getIDirect3DTexture9() = 0;
    virtual IDirect3DSurface9* getIDirect3DSurface9() = 0;

public:
	// override ITexture
	virtual SurfaceFormat getSurfaceFormat() const { return mSurfaceFormat; }

protected:
	SurfaceFormat	mSurfaceFormat;
};

//==============================================================================
// ■ DX9Texture
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class DX9Texture
    : public DX9TextureBase
{
public:
	DX9Texture();
	virtual ~DX9Texture();

public:
	/// テクスチャ作成 (level : ミップマップレベル ( 0 で全て作成 ))
	void create( DX9GraphicsDevice* device, lnU32 width, lnU32 height, lnU32 levels, SurfaceFormat format );

	/// テクスチャ作成 (メモリに展開された画像データから)
	void create( DX9GraphicsDevice* device, const void* data, lnU32 size, lnU32 color_key, lnU32 levels, SurfaceFormat format );

public:
	// ITexture
	virtual LNTextureType	getType() const { return LN_TEXTURETYPE_NORMAL; }
    virtual const LVector2& getSize() const { return mSize; }
    virtual const LVector2& getRealSize( bool is_reciprocal ) const { return ( is_reciprocal ) ? mRealSizeInv : mRealSize; }
	virtual void			clear( const LColor& color );
	virtual lnU32*			lock();
	virtual void			unlock();
    virtual void			setFont( Font* font ) { LN_REFOBJ_SET( mFont, font ); }
    virtual Font*			getFont() const { return mFont; }
    virtual void			drawText( const lnChar* text, const Geometry::Rect& rect, TextAlign align, int len );
    virtual void			blt( int x, int y, Texture* src_texture, const Geometry::Rect& src_rect );
    virtual const lnChar*	getFilename() const { return mFilename.c_str(); }

public:
	// DX9TextureBase
	virtual IDirect3DTexture9* getIDirect3DTexture9() { return mDxTexture; }
	virtual IDirect3DSurface9* getIDirect3DSurface9() { return mDxSurface; }

public:
	// GraphicsResource
    virtual void onPauseDevice() { }
	virtual void onResumeDevice() { }

private:
	DX9GraphicsDevice*	mGraphicsDevice;    ///< 管理クラス
	IDirect3DTexture9*	mDxTexture;		    ///< Direct3DTexture 本体
	IDirect3DSurface9*	mDxSurface;		    ///< テクスチャのサーフェイス

    LVector2            mSize;                   ///< 作成時に要求したサイズ
    LVector2            mRealSize;               ///< 実際のテクスチャのサイズ
    LVector2            mRealSizeInv;            ///< 実際のテクスチャサイズの逆数

    LRefTString         mFilename;
	Font*				mFont;

	//lnSharingKey mSharingKey;
};

//==============================================================================
// ■ DX9RenderTargetTexture
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class DX9RenderTargetTexture
    : public DX9TextureBase
{
public:
	DX9RenderTargetTexture();
	virtual ~DX9RenderTargetTexture();

public:

	/// 作成
	void create( DX9GraphicsDevice* device, lnU32 width, lnU32 height, lnU32 mipLevel, SurfaceFormat format );
	
public:
	// ITexture
	virtual LNTextureType	getType() const { return LN_TEXTURETYPE_RENDERTARGET; }
	virtual const LVector2& getSize() const { return mSize; }
	virtual const LVector2& getRealSize( bool isReciprocal ) const { return ( isReciprocal ) ? mRealSizeInv : mRealSize; }
	virtual void			clear(const LColor& color) { LN_THROW_InvalidOperation(0); }
	virtual lnU32*			lock() { LN_THROW_InvalidOperation( 0 ); return NULL; }
	virtual void			unlock() { LN_THROW_InvalidOperation( 0 ); }
	virtual void			setFont( Font* font ) { LN_THROW_InvalidOperation( 0 ); }
	virtual Font*			getFont() const { LN_THROW_InvalidOperation( 0 ); }
	virtual void			drawText( const lnChar* text, const LRect& rect, TextAlign align, int length ) { LN_THROW_InvalidOperation( 0 ); }
	virtual void			blt( int x, int y, Texture* srcTexture, const LRect& srcRect ) { LN_THROW_InvalidOperation( 0 ); }
	virtual const lnChar*	getFilename() const { return NULL; }

public:
	// GraphicsResource
	virtual void onPauseDevice() { _dispose(); }
	virtual void onResumeDevice() { _create(); }

public:
	// DX9TextureBase
	virtual IDirect3DTexture9* getIDirect3DTexture9() { return mDxTexture; }
	virtual IDirect3DSurface9* getIDirect3DSurface9() { return mDxSurface; }

private:
	void _create();
	void _dispose();

private:
	DX9GraphicsDevice*	mGraphicsDevice;
	IDirect3DTexture9*	mDxTexture;
	IDirect3DSurface9*	mDxSurface;
    LVector2            mSize;              ///< 作成時に要求したサイズ
    LVector2            mRealSize;          ///< 実際のテクスチャのサイズ
    LVector2            mRealSizeInv;       ///< 実際のテクスチャサイズの逆数
    lnU32				mMipLevel;			///< ミップマップレベル
	SurfaceFormat	    mFormat;			///< サーフェイスフォーマット
};

//==============================================================================
// ■ DX9DepthBuffer
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class DX9DepthBuffer
    : public DX9TextureBase
{
public:
	DX9DepthBuffer();
	virtual ~DX9DepthBuffer();

public:
	void create( DX9GraphicsDevice* device, lnU32 width, lnU32 height, SurfaceFormat format );
	
public:
	// ITexture
	virtual LNTextureType	getType() const { return LN_TEXTURETYPE_DEPTH; }
	virtual const LVector2& getSize() const { return mSize; }
	virtual const LVector2& getRealSize( bool isReciprocal ) const { return ( isReciprocal ) ? mRealSizeInv : mRealSize; }
	virtual void			clear(const LColor& color) { LN_THROW_InvalidOperation(0); }
	virtual lnU32*			lock() { LN_THROW_InvalidOperation( 0 ); return NULL; }
	virtual void			unlock() { LN_THROW_InvalidOperation( 0 ); }
	virtual void			setFont( Font* font ) { LN_THROW_InvalidOperation( 0 ); }
	virtual Font*			getFont() const { LN_THROW_InvalidOperation( 0 ); }
	virtual void			drawText( const lnChar* text, const LRect& rect, TextAlign align, int length ) { LN_THROW_InvalidOperation( 0 ); }
	virtual void			blt( int x, int y, Texture* srcTexture, const LRect& srcRect ) { LN_THROW_InvalidOperation( 0 ); }
	virtual const lnChar*	getFilename() const { return NULL; }

public:
	// GraphicsResource
	virtual void onPauseDevice() { _dispose(); }
	virtual void onResumeDevice() { _create(); }

public:
	// DX9TextureBase
	virtual IDirect3DTexture9* getIDirect3DTexture9() { return NULL; }
	virtual IDirect3DSurface9* getIDirect3DSurface9() { return mDxSurface; }

private:
	void _create();
	void _dispose();

private:
	DX9GraphicsDevice*	mGraphicsDevice;
	IDirect3DSurface9*	mDxSurface;			///< 深度バッファサーフェイス
    LVector2            mSize;              ///< 作成時に要求したサイズ
    LVector2            mRealSize;          ///< 実際のテクスチャのサイズ
    LVector2            mRealSizeInv;       ///< 実際のテクスチャサイズの逆数
	SurfaceFormat	    mFormat;			///< サーフェイスフォーマット
};

//==============================================================================
// ■ DX9SystemCreatedTexture
//------------------------------------------------------------------------------
///**
//  @brief		DirectX 側で自動的に作成されたテクスチャを扱うクラス
//  @note
//              とりあえずデフォルトのスワップチェインの処理で使っているため、
//				レンダリングターゲット固定。
//*/
//==============================================================================
class DX9SystemCreatedTexture
    : public DX9TextureBase
{
public:
	DX9SystemCreatedTexture();
	virtual ~DX9SystemCreatedTexture();

public:
	/// テクスチャ、サーフェイスのセット (texture_ は NULL でもOK。surface_ は必ず)
	void create( IDirect3DTexture9* texture, IDirect3DSurface9* surface );

public:
	// ITexture
	virtual LNTextureType	getType() const { return LN_TEXTURETYPE_RENDERTARGET; }
	virtual const LVector2& getSize() const { return mSize; }
	virtual const LVector2& getRealSize( bool isReciprocal = false ) const { return ( isReciprocal ) ? mRealSizeInv : mRealSize; }
	virtual void			clear(const LColor& color) { LN_THROW_InvalidOperation(0); }
	virtual lnU32*			lock() { LN_THROW_InvalidOperation( 0 ); return NULL; }
	virtual void			unlock() { LN_THROW_InvalidOperation( 0 ); }
	virtual void			setFont( Font* font ) { LN_THROW_InvalidOperation( 0 ); }
	virtual Font*			getFont() const { LN_THROW_InvalidOperation( 0 ); }
	virtual void			drawText( const lnChar* text, const LRect& rect, TextAlign align, int length ) { LN_THROW_InvalidOperation( 0 ); }
	virtual void			blt( int x, int y, Texture* srcTexture, const LRect& srcRect ) { LN_THROW_InvalidOperation( 0 ); }
	virtual const lnChar*	getFilename() const { return NULL; }

public:
	// GraphicsResource
	virtual void onPauseDevice() {}
	virtual void onResumeDevice() {}

public:
	// DX9TextureBase
	virtual IDirect3DTexture9* getIDirect3DTexture9() { return mDxTexture; }
	virtual IDirect3DSurface9* getIDirect3DSurface9() { return mDxSurface; }

private:
	IDirect3DTexture9*  mDxTexture;
	IDirect3DSurface9*	mDxSurface;			///< テクスチャのサーフェイス
    LVector2            mSize;              ///< 作成時に要求したサイズ
    LVector2            mRealSize;          ///< 実際のテクスチャのサイズ
    LVector2            mRealSizeInv;       ///< 実際のテクスチャサイズの逆数
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================