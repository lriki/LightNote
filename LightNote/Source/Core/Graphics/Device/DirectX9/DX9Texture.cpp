//==============================================================================
// DX9Texture 
//==============================================================================

#include "stdafx.h"
#include "../../../Base/Unicode.h"
#include "../../Manager.h"
#include "../../Common/Font/FreeTypeFontRenderer.h"
#include "../../Common/Font/FreeTypeFont.h"
#include "DX9Module.h"
#include "DX9GraphicsDevice.h"
#include "DX9Texture.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ DX9TextureBase
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9TextureBase::DX9TextureBase()
		: mSurfaceFormat	( SurfaceFormat_Unknown )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9TextureBase::~DX9TextureBase()
    {
    }

//==============================================================================
// ■ DX9Texture
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9Texture::DX9Texture()
		: mGraphicsDevice   ( NULL )
        , mDxTexture		( NULL )
        , mDxSurface		( NULL )
        , mFont				( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9Texture::~DX9Texture()
    {	
		SAFE_RELEASE( this->mDxSurface );
		SAFE_RELEASE( this->mDxTexture );
        LN_SAFE_RELEASE( this->mFont );
        LN_SAFE_RELEASE( this->mGraphicsDevice );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9Texture::create( DX9GraphicsDevice* device, lnU32 width, lnU32 height, lnU32 levels, SurfaceFormat format )
    {
		LN_REFOBJ_SET( this->mGraphicsDevice, device );
        D3DFORMAT dx_fmt = DX9Module::convertLNFormatToDxFormat( format );

        // サイズ格納
        UINT w = width;
        UINT h = height;
        UINT miplevels = levels;
        this->mSize.X = static_cast<lnFloat>( width );
        this->mSize.Y = static_cast<lnFloat>( height );

        // 実際に作成されるべきテクスチャの情報を取得する
		HRESULT hr;
		LN_COMCALL( DX9Module::getInstance()->D3DXCheckTextureRequirements(
			this->mGraphicsDevice->getIDirect3DDevice9(),
            &w, &h,
			&miplevels,        // 0 の場合は最大数が格納される
            0,
            &dx_fmt,    
            D3DPOOL_MANAGED ) );

        this->mRealSize.X = static_cast< lnFloat >( w );
        this->mRealSize.Y = static_cast< lnFloat >( h );
        this->mRealSizeInv.X = 1.0f / this->mRealSize.X;
        this->mRealSizeInv.Y = 1.0f / this->mRealSize.Y;

		// テクスチャ作成
		// 3つめの引数 ( ミップマップ ) は、使わない場合は 1 にしておく( 0 にすると可能な限り全部作られる )
		LN_COMCALL( this->mGraphicsDevice->getIDirect3DDevice9()->CreateTexture(
			w, h, miplevels, 0,
			dx_fmt, D3DPOOL_MANAGED, &this->mDxTexture, NULL ) );

		// テクスチャのサーフェイスを取得しておく
		LN_COMCALL( this->mDxTexture->GetSurfaceLevel( 0, &mDxSurface ) );

		// サーフェイスフォーマットの取得
		D3DSURFACE_DESC desc;
		mDxSurface->GetDesc(&desc);
		mSurfaceFormat = DX9Module::convertFormatDxToLN(desc.Format);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9Texture::create( DX9GraphicsDevice* device, const void* data, lnU32 size, lnU32 color_key, lnU32 levels, SurfaceFormat format )
	{
		LN_REFOBJ_SET( this->mGraphicsDevice, device );

		// テクスチャ情報
		D3DXIMAGE_INFO image_info;
        HRESULT hr;
        LN_COMCALL( DX9Module::getInstance()->D3DXGetImageInfoFromFileInMemory( data, size, &image_info ) );

        // テクスチャのフォーマットを決める
        D3DFORMAT dxFormat = ( format == SurfaceFormat_Unknown ) ? image_info.Format : DX9Module::getInstance()->convertLNFormatToDxFormat( format );
        
        LN_COMCALL( DX9Module::getInstance()->D3DXCreateTextureFromFileInMemoryEx(
			this->mGraphicsDevice->getIDirect3DDevice9(), 
            data, size,
            //512, 512, // ノート OK
			//D3DX_DEFAULT_NONPOW2, // ノート NG  デスクトップ OK
            //D3DX_DEFAULT_NONPOW2,
            //D3DX_DEFAULT, // ノート OK  デスクトップ NG
            //D3DX_DEFAULT,
            //0, 0,       // ノート OK  デスクトップ NG
            //300, 300,       // デスクトップ OK
            image_info.Width,
            image_info.Height,
			levels,
            0,
			dxFormat,
			D3DPOOL_MANAGED,
            D3DX_FILTER_NONE,
            D3DX_DEFAULT,
			color_key,
			NULL,
			NULL,
			&mDxTexture ) );

		// ここの時点で mDxTexture の参照カウントは「 1 」

		// ファイルのイメージの幅と高さを記憶
		this->mSize.X = static_cast<lnFloat>( image_info.Width );
        this->mSize.Y = static_cast<lnFloat>( image_info.Height );

        UINT miplevels = levels;
        
		// 実際のテクスチャの大きさを取得
		LN_COMCALL( DX9Module::getInstance()->D3DXCheckTextureRequirements(
			this->mGraphicsDevice->getIDirect3DDevice9(),
            &image_info.Width, &image_info.Height,
			&miplevels, 0, NULL, D3DPOOL_MANAGED ) );

        this->mRealSize.X = static_cast< lnFloat >( image_info.Width );
        this->mRealSize.Y = static_cast< lnFloat >( image_info.Height );
        this->mRealSizeInv.X = 1.0f / this->mRealSize.X;
        this->mRealSizeInv.Y = 1.0f / this->mRealSize.Y;

		// テクスチャのサーフェイスを取得する
		// ( ここでテクスチャのインターフェイスの参照カウントもひとつ増えてるみたい )
		LN_COMCALL( mDxTexture->GetSurfaceLevel( 0, &this->mDxSurface ) );
    
		// ここの時点で mDxSurface の参照カウントは「 3 」
		// ここの時点で mDxTexture の参照カウントは「 2 」

		// サーフェイスフォーマットの取得
		D3DSURFACE_DESC desc;
		mDxSurface->GetDesc(&desc);
		mSurfaceFormat = DX9Module::convertFormatDxToLN(desc.Format);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9Texture::clear(const LColor& color)
	{
		LN_THROW_InvalidOperation(mSurfaceFormat == SurfaceFormat_A8R8G8B8 || mSurfaceFormat == SurfaceFormat_X8R8G8B8);

		D3DLOCKED_RECT locked_rect;
		HRESULT hr = this->mDxTexture->LockRect( 0, &locked_rect, NULL, 0 );
		lnU32* buf = (lnU32*)locked_rect.pBits;

		lnU32 c = color.toU32ARGB();

        int m = static_cast< int >( this->mRealSize.X * this->mRealSize.Y );
        for ( int i = 0; i < m; ++i ) {
            buf[ i ] = c;
        }

		this->mDxTexture->UnlockRect( 0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	lnU32* DX9Texture::lock()
	{
		D3DLOCKED_RECT locked_rect;
		HRESULT hr = this->mDxTexture->LockRect( 0, &locked_rect, NULL, 0 );
		return (lnU32*)locked_rect.pBits;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9Texture::unlock()
	{
		this->mDxTexture->UnlockRect( 0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9Texture::drawText( const lnChar* text, const Geometry::Rect& rect, TextAlign align, int len )
    {
#if 1
		int w = static_cast<int>( mRealSize.X );
		int h = static_cast<int>( mRealSize.Y );
		lnByte* data = (lnByte*)lock();

		try
		{
			Base::UnicodeString str;
			str.setDecodeBytes( text, len );

			Bitmap bmp;
			bmp.create( 
				data, 
				w * h * sizeof(lnU32),
				w, h,
				LN_BITMAPFORMAT_A8R8G8B8,
				false);

			Font* font = ( mFont ) ? mFont : mGraphicsDevice->getManager()->getFontManager()->getDefaultFont();
			FreeTypeFontRenderer fr( static_cast<FreeTypeFont*>(font), &bmp );
			fr.drawText(str, len, rect, align);

		}
		catch (...)
		{
			unlock();
			throw;
		}
		unlock();
#else
        LNRESULT lr;

        Graphics::BitmapTextRenderer tr( LN_GRAPHICSAPI_DIRECTX9 );

        if ( mFont )
        {
            tr.setFont( mFont );
        }
        else
        {
            tr.setFont( mGraphicsDevice->getManager()->getDefaultFont() );
        }

        tr.setTargetBuffer(
            lock(), 
            static_cast< lnU32 >( mRealSize.x ),
            static_cast< lnU32 >( mRealSize.y ) );

        lr = tr.drawText( text_, rect_, align_, len_ );
        
         unlock();

        return lr;
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9Texture::blt( int x, int y, Texture* src_texture, const Geometry::Rect& src_rect )
    {
#if 1
		LN_PRINT_NOT_IMPL_FUNCTION;
#else
        // フォーマットチェックが必要


        lnU32* dest = lock();
        lnU32* src  = src_texture_->lock();

        const LVector2& src_size = src_texture_->getRealSize();

        Util::bitblt(
            x_, y_,
            static_cast< int >( mRealSize.x ),
            static_cast< int >( mRealSize.y ),
            dest,
            static_cast< int >( mRealSize.x ),
            static_cast< int >( mRealSize.y ),
            src_rect_.x, src_rect_.y,
            src_rect_.w, src_rect_.h,
            src,
            static_cast< int >( src_size.x ),
            static_cast< int >( src_size.y ),
            true );

        unlock();
        src_texture_->unlock();
        return LN_OK;
#endif
    }


//==============================================================================
// ■ DX9RenderTargetTexture
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9RenderTargetTexture::DX9RenderTargetTexture()
		: mGraphicsDevice   ( NULL )
        , mDxTexture        ( NULL )
	    , mDxSurface        ( NULL )
        , mMipLevel         ( 1 )
        , mFormat           ( SurfaceFormat_A8R8G8B8 )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9RenderTargetTexture::~DX9RenderTargetTexture()
    {
        _dispose();
		LN_SAFE_RELEASE( this->mGraphicsDevice );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9RenderTargetTexture::create( DX9GraphicsDevice* device, lnU32 width, lnU32 height, lnU32 mipLevel, SurfaceFormat format )
	{
		LN_REFOBJ_SET( this->mGraphicsDevice, device );
        this->mSize.Set( static_cast<lnFloat>( width ), static_cast<lnFloat>( height ) );
	    this->mMipLevel = mipLevel;
        this->mFormat = format;
		this->_create();
		GraphicsResource::registerResource( this->mGraphicsDevice );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9RenderTargetTexture::_create()
    {
		HRESULT hr;
		IDirect3DDevice9* dx_device = this->mGraphicsDevice->getIDirect3DDevice9();

        D3DFORMAT dx_fmt = DX9Module::convertLNFormatToDxFormat( mFormat );
        switch ( dx_fmt )
        {
            case D3DFMT_A8B8G8R8: dx_fmt = D3DFMT_A8R8G8B8; break;
            case D3DFMT_X8B8G8R8: dx_fmt = D3DFMT_X8R8G8B8; break;
        }

        // サイズ格納
        UINT w = static_cast< UINT >( mSize.X );
        UINT h = static_cast< UINT >( mSize.Y );
        UINT levels = mMipLevel;
       /*
        // 実際に作成されるべきテクスチャの情報を取得する
        LN_DXCALL_R(
            GraphicsUtil::D3DXCheckTextureRequirements(
				dx_device,
                &w, &h,
				&levels,        // 0 の場合は最大数が格納される
                D3DUSAGE_RENDERTARGET,
                &dx_fmt,    
                D3DPOOL_MANAGED ),
            "Texture::initialize" );
            

        mSize.dump();

        // 実際の値格納
        mRealSize.x = static_cast< lnFloat >( w );
        mRealSize.y = static_cast< lnFloat >( h );
        mMipLevel = levels;
        mFormat = GraphicsUtil::convertFormatDxToLN( dx_fmt );

        mRealSize.dump();
        */

        LN_COMCALL(
            dx_device->CreateTexture(
				w, h,
				levels,
				D3DUSAGE_RENDERTARGET,	// レンダリングターゲットとして作成
                dx_fmt,
				D3DPOOL_DEFAULT,		// レンダリングターゲットは D3DPOOL_DEFAULT
				&mDxTexture,
				NULL ) );
            //"w=%d h=%d levels=%d dx_fmt=%d\n\nミップマップレベルが 1 以外でテクスチャサイズが 2 の累乗ではない場合、このエラーが発生することがあります。",
            //w, h, levels, dx_fmt );

        // サイズチェック
        D3DSURFACE_DESC desc;
        mDxTexture->GetLevelDesc( 0, &desc );
        //_p( desc.Width );
        //_p( desc.Height );
        mRealSize.X = static_cast< lnFloat >( desc.Width );
        mRealSize.Y = static_cast< lnFloat >( desc.Height );
        mRealSizeInv.X = 1.0f / mRealSize.X;
        mRealSizeInv.Y = 1.0f / mRealSize.Y;

		// レンダリングターゲットのサーフェイスを取得する
		LN_COMCALL( mDxTexture->GetSurfaceLevel( 0, &mDxSurface ) );

		// ちなみに D3DFMT_R8G8B8 はレンダリングターゲットには使えない

		// サーフェイスフォーマットの取得
		mSurfaceFormat = DX9Module::convertFormatDxToLN(desc.Format);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9RenderTargetTexture::_dispose()
    {
        SAFE_RELEASE( this->mDxSurface );
		SAFE_RELEASE( this->mDxTexture );
    }

//==============================================================================
// ■ DX9DepthBuffer
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9DepthBuffer::DX9DepthBuffer()
		: mGraphicsDevice	( NULL )
		, mDxSurface        ( NULL )
        , mFormat           ( SurfaceFormat_D24S8 )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9DepthBuffer::~DX9DepthBuffer()
    {
        _dispose();
		LN_SAFE_RELEASE( this->mGraphicsDevice );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9DepthBuffer::create( DX9GraphicsDevice* device, lnU32 width, lnU32 height, SurfaceFormat format )
    {
		LN_REFOBJ_SET( this->mGraphicsDevice, device );
        this->mSize.Set( static_cast<lnFloat>( width ), static_cast<lnFloat>( height ) );
        this->mFormat = format;
		this->_create();
		GraphicsResource::registerResource( this->mGraphicsDevice );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9DepthBuffer::_create()
    {
        HRESULT hr;
		IDirect3DDevice9* dx_device = this->mGraphicsDevice->getIDirect3DDevice9();

		LN_COMCALL(
		    dx_device->CreateDepthStencilSurface(
                static_cast< UINT >( mSize.X ),
				static_cast< UINT >( mSize.Y ),
				DX9Module::convertLNFormatToDxFormat( this->mFormat ),
				D3DMULTISAMPLE_NONE,	// 高度な補間方法の設定
				0,						// 画像の品質レベルの設定
				TRUE,					// 深度バッファの制御方法の指定
				&this->mDxSurface,
				NULL ) );

		// サーフェイスフォーマットの取得
		D3DSURFACE_DESC desc;
		mDxSurface->GetDesc(&desc);
		mSurfaceFormat = DX9Module::convertFormatDxToLN(desc.Format);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9DepthBuffer::_dispose()
    {
        SAFE_RELEASE( this->mDxSurface );
    }

//==============================================================================
// ■ DX9SystemCreatedTexture
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9SystemCreatedTexture::DX9SystemCreatedTexture()
		: mDxTexture	( NULL )
		, mDxSurface	( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9SystemCreatedTexture::~DX9SystemCreatedTexture()
    {
        SAFE_RELEASE( mDxTexture );
        SAFE_RELEASE( mDxSurface );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9SystemCreatedTexture::create( IDirect3DTexture9* texture, IDirect3DSurface9* surface )
	{

		SAFE_ADDREF( texture );
        SAFE_ADDREF( surface );
        SAFE_RELEASE( mDxTexture );
        SAFE_RELEASE( mDxSurface );
        mDxTexture = texture;
        mDxSurface = surface;

		if ( mDxSurface )
        {
            D3DSURFACE_DESC desc;
            mDxSurface->GetDesc( &desc );

            mRealSize.X = mSize.X = static_cast< lnFloat >( desc.Width );
            mRealSize.Y = mSize.Y = static_cast< lnFloat >( desc.Height );
            mRealSizeInv.X = 1.0f / mRealSize.X;
            mRealSizeInv.Y = 1.0f / mRealSize.Y;

			// サーフェイスフォーマットの取得
			mSurfaceFormat = DX9Module::convertFormatDxToLN(desc.Format);
        }
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================