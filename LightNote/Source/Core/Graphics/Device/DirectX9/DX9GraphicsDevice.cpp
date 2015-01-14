//==============================================================================
// DX9GraphicsDevice 
//==============================================================================

#include "stdafx.h"

#include <algorithm>
#include "../../../System/Environment.h"
#include "../../../System/Manager.h"
#include "../../../FileIO/Manager.h"
#include "../../../Resource/LNResource.h"
#include "DX9Module.h"
#include "DX9Canvas.h"
#include "DX9Renderer.h"
#include "DX9IndexBuffer.h"
#include "DX9VertexBuffer.h"
#include "DX9Texture.h"
#include "DX9Shader.h"
#include "DX9GraphicsDevice.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ DX9GraphicsDevice
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9GraphicsDevice::DX9GraphicsDevice()
		: mDefaultCanvas			( NULL )
		, mRenderer					( NULL )
		, mDirect3D                 ( NULL )
        , mDxDevice                 ( NULL )
		, mD3DXEffectPool			( NULL )
        , mCurrentMinFilter         ( D3DTEXF_POINT )
        , mCurrentMagFilter         ( D3DTEXF_POINT )
        , mCurrentMipFilter         ( D3DTEXF_NONE )
        , mBestMinFilter            ( D3DTEXF_POINT )
        , mBestMagFilter            ( D3DTEXF_POINT )
        , mBestMipFilter            ( D3DTEXF_NONE )
        , mMSAAQualityWindowed      ( 0 )
        , mMSAAQualityFullScreen    ( 0 )
        , mDeviceType               ( D3DDEVTYPE_HAL )
		, mEnableFPUPreserve		( NULL )
		, mIsFullScreen				( false )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9GraphicsDevice::~DX9GraphicsDevice()
    {
		SAFE_RELEASE( mD3DXEffectPool );
        SAFE_RELEASE( mDxDevice );
        SAFE_RELEASE( mDirect3D );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9GraphicsDevice::initialize( const ConfigData& configData )
	{
        LN_LOG_INIT_BEGIN;

		GraphicsDeviceBase::initialize( configData );
        mEnableFPUPreserve  = configData.EnableFPUPreserve;

		// DLL 読み込み
		DX9Module::getInstance()->initialize();

        // Direct3D 作成
        this->mDirect3D = DX9Module::getInstance()->Direct3DCreate9( D3D_SDK_VERSION );
		LN_THROW(mDirect3D, Base::DirectXException, Resource::String::ERR_FailedDirect3DInit);

		// デバイスの性能チェック
		_checkDeviceInformation();

		// デフォルトのキャンバス (コンストラクタでリストに入れられる)
		this->mDefaultCanvas = LN_NEW DX9Canvas();
		this->mDefaultCanvas->create(this, mSystemManager->getMainWindow(), configData.BackbufferSize, configData.BackbufferResizeMode);


		// 基本的に変化のないプレゼンテーションパラメータの設定
		// ( PresentationInterval は D3DPRESENT_INTERVAL_IMMEDIATE 以外の場合、
		//   DirectShow を使って動画再生した時に画面が表示されなくなることがある )
		ZeroMemory( &mPresentParameters, sizeof( D3DPRESENT_PARAMETERS ) );
		mPresentParameters.BackBufferCount				= 1;
		mPresentParameters.EnableAutoDepthStencil		= FALSE;//TRUE;//					// Canvas で作るのでこれはいらない
		mPresentParameters.AutoDepthStencilFormat		= D3DFMT_D24S8;//D3DFMT_UNKNOWN;//
		mPresentParameters.SwapEffect					= D3DSWAPEFFECT_DISCARD;	// マルチサンプリングするにはコレ
		mPresentParameters.PresentationInterval			= ( configData.EnableVSyncWait ) ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;//D3DPRESENT_INTERVAL_IMMEDIATE(アダプタの更新間隔無視); // 	//D3DPRESENT_INTERVAL_DEFAULT(アダプタの更新間隔に合わせる);	// 画面の更新間隔
		mPresentParameters.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
		mPresentParameters.hDeviceWindow				= System::Environment::getWindowHandle( mSystemManager->getMainWindow() );
		//mPresentParameters.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

		// デバイス作成
		_resetDevice( configData.FullScreen/*, mSystemManager->getMainWindow()->getSize()*/ );
		
		// ID3DXEffectPool
		HRESULT hr;
		LN_COMCALL( DX9Module::getInstance()->D3DXCreateEffectPool( &mD3DXEffectPool ) );

		// 描画クラスの作成
        this->mRenderer = LN_NEW DX9Renderer();
		this->mRenderer->initialize( this );
		
		this->mDefaultCanvas->postInitialize();
        LN_LOG_INIT_END;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9GraphicsDevice::finalize()
	{
        LN_LOG_FIN_BEGIN;

		LN_SAFE_RELEASE( this->mRenderer );
		LN_SAFE_RELEASE( this->mDefaultCanvas );

		GraphicsDeviceBase::finalize();

        LN_LOG_FIN_END;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IRenderer* DX9GraphicsDevice::getRenderer() 
	{ 
		return mRenderer; 
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ICanvas* DX9GraphicsDevice::getDefaultCanvas() 
	{ 
		return mDefaultCanvas; 
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IVertexBuffer* DX9GraphicsDevice::createVertexBuffer( LNVertexElemenst* elements, lnU32 vertexCount, const void* data, bool isDynamic )
    {
		LRefPtr<DX9VertexBuffer> vb( LN_NEW DX9VertexBuffer() );
		vb->create( this, elements, vertexCount, data, isDynamic );
		vb->addRef();
		return vb;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    IIndexBuffer* DX9GraphicsDevice::createIndexBuffer( lnU32 indexCount, const void* data, bool isDynamic, bool is16bit )
    {
		LRefPtr<DX9IndexBuffer> ib( LN_NEW DX9IndexBuffer() );
		ib->create( this, indexCount, data, isDynamic, is16bit );
		ib->addRef();
		return ib;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Texture* DX9GraphicsDevice::createTexture( lnU32 width, lnU32 height, lnU32 levels, SurfaceFormat format )
    {
		LRefPtr<DX9Texture> texture( LN_NEW DX9Texture() );
		texture->create( this, width, height, levels, format );
		texture->addRef();
		return texture;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Texture* DX9GraphicsDevice::createTexture( FileIO::Stream* stream, lnU32 colorKey, lnU32 levels, SurfaceFormat format, lnSharingKey registerKey )
    {
		if ( !mEnableTextureCache ) {
			registerKey = lnNullKey;
		}

		// キャッシュ検索
		Texture* t = static_cast<DX9Texture*>( findTextureCache( registerKey ) );
		if ( t ) {
			return t;
		}

		// 一時メモリにロード
		LRefPtr<Base::ReferenceBuffer> tmpBuffer( LN_NEW Base::ReferenceBuffer() );
		tmpBuffer->reserve( stream->getSize() );
		stream->read( tmpBuffer->getPointer(), stream->getSize() );

		// Texture 作成
        LRefPtr<DX9Texture> texture( LN_NEW DX9Texture() );
		texture->create( this, tmpBuffer->getPointer(), tmpBuffer->getSize(), colorKey, levels, format );

		// キャッシュ管理に追加
		if ( !registerKey.isEmpty() ) {
			mTextureCache.registerCachedObject( registerKey, texture );
		}

		texture->addRef();
		return texture;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Texture* DX9GraphicsDevice::createTexture( const lnChar* filePath, lnU32 colorKey, lnU32 levels, SurfaceFormat format, lnSharingKey registerKey )
    {
		LRefPtr<FileIO::InFile> file( mFileManager->createInFile( filePath ) );
		return createTexture( file, colorKey, levels, format, registerKey );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Texture* DX9GraphicsDevice::createRenderTarget( lnU32 width, lnU32 height, lnU32 mipLevels, SurfaceFormat format )
    {
		LRefPtr<DX9RenderTargetTexture> texture( LN_NEW DX9RenderTargetTexture() );
		texture->create( this, width, height, mipLevels, format );
		texture->addRef();
		return texture;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Texture* DX9GraphicsDevice::createDepthBuffer( lnU32 width, lnU32 height, SurfaceFormat format )
    {
		LRefPtr<DX9DepthBuffer> depth( LN_NEW DX9DepthBuffer() );
		depth->create( this, width, height, format );
		depth->addRef();
		return depth;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
  //  IShader* DX9GraphicsDevice::createShader( const void* data, lnU32 size, lnSharingKey registerKey )
  //  {
		//// キャッシュ検索
		//IShader* ts = findShaderCache( registerKey );
		//if ( ts ) {
		//	return ts;
		//}

		//// Shader 作成
  //      LRefPtr<DX9Shader> shader( LN_NEW DX9Shader() );
  //      shader->create( this, (const char*)data, size );

		//// エラーでなければキャッシュ管理に追加
  //      if ( shader->getCompileResult() != LN_SHADERRESULT_ERROR ) {
		//	if ( !registerKey.isEmpty() ) {
		//		mShaderCache.registerCachedObject( registerKey, shader );
		//	}
  //      }

		//shader->addRef();
		//return shader;
  //  }

#if 0
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IShader* DX9GraphicsDevice::createShader( const lnChar* filePath, lnSharingKey registerKey )
    {
		// キャッシュ検索
		IShader* ts = static_cast<DX9Shader*>( findShaderCache( registerKey ) );
		if ( ts ) {
			return ts;
		}

		// Shader 作成
        LRefPtr<DX9Shader> shader( LN_NEW DX9Shader() );
        shader->create( this, filePath );

		// エラーでなければキャッシュ管理に追加
        if ( shader->getCompileResult() != LN_SHADERRESULT_ERROR ) {
			if ( !registerKey.isEmpty() ) {
				mShaderCache.registerCachedObject( registerKey, shader );
			}
        }

		shader->addRef();
		return shader;
    }
#endif
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IShader* DX9GraphicsDevice::createShader(FileIO::Stream* stream, lnSharingKey registerKey, lnString* errors)
    {
		// キャッシュ検索
		IShader* ts = static_cast<DX9Shader*>( findShaderCache( registerKey ) );
		if ( ts ) {
			return ts;
		}

		// Shader 作成
        LRefPtr<DX9Shader> shader( LN_NEW DX9Shader() );
		shader->create(this, stream, errors);

		// エラーでなければキャッシュ管理に追加
        if ( shader->getCompileResult() != LN_SHADERRESULT_ERROR ) {
			if ( !registerKey.isEmpty() ) {
				mShaderCache.registerCachedObject( registerKey, shader );
			}
        }

		shader->addRef();
		return shader;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9GraphicsDevice::pauseDevice()
    {
		// ここでバックバッファに戻しておく
		this->mRenderer->setRenderTarget( 0, this->getDefaultCanvas()->getBackendBuffer(), true );
		this->mRenderer->setDepthBuffer( NULL, true );

		this->mRenderer->pauseDevice();

		mDefaultCanvas->onPrePauseDevice();
		GraphicsDeviceBase::pauseDevice();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9GraphicsDevice::resumeDevice()
    {
		GraphicsDeviceBase::resumeDevice();	// DefaultCanvas の持っているバックバッファの復帰が先
		mDefaultCanvas->onPostResumeDevice();

		// RenderTarget[0] がデフォルトバックバッファだった場合、
		// mDefaultCanvas->onPostResumeDevice() で、バックバッファテクスチャのサイズが更新された後で
		// resumeDevice() しないと、古いサイズのままで setViewport() がよばれてしまう。
		this->mRenderer->resumeDevice();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	DeviceState DX9GraphicsDevice::getDeviceState()
	{
		if ( isDeviceLost() ) return DeviceState_Lost;
		return DeviceState_Enabled;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9GraphicsDevice::resetDevice()
	{
		pauseDevice();
		_resetDevice( mIsFullScreen/*, mSystemManager->getMainWindow()->getSize()*/ );
		resumeDevice();
		mIsDeviceLost.setFalse();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//void DX9GraphicsDevice::testDeviceLost()
	//{
	//	if ( mIsDeviceLost.isTrue() ) {
	//		pauseDevice();
	//		_resetDevice( mIsFullScreen, mSystemManager->getMainWindow()->getSize() );
	//		resumeDevice();
	//		mIsDeviceLost.setFalse();
	//	}
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9GraphicsDevice::_checkDeviceInformation()
    {
        // 使えるデバイスの種類を調べる
		HRESULT hr = mDirect3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &mDxCaps );
        if ( SUCCEEDED( hr ) ) {
			mDeviceType = D3DDEVTYPE_HAL;
		}
		else {
			hr = mDirect3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, &mDxCaps );
			if ( SUCCEEDED( hr ) ) {
				mDeviceType = D3DDEVTYPE_REF;
			}
			else {
				hr = mDirect3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_SW, &mDxCaps );
				if ( SUCCEEDED( hr ) ) {
					mDeviceType = D3DDEVTYPE_SW;
				}
				else {
					LN_THROW_Com(hr, Resource::String::ERR_NotFoundValidGraphicsDevice);
				}
			}
		}

        // ディスプレイの情報を取得
		LN_COMCALL( mDirect3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &mDxDisplayMode ) );

        // アンチエイリアスする場合の品質
		// ウィンドウモードのときは？
		hr = mDirect3D->CheckDeviceMultiSampleType(
			D3DADAPTER_DEFAULT,  mDeviceType, 
            mDxDisplayMode.Format, TRUE,
			D3DMULTISAMPLE_NONMASKABLE,
			&mMSAAQualityWindowed );
		if ( FAILED( hr ) )
		{
			mMSAAQualityWindowed = 0;
		}
		// フルスクリーンのときは？
		hr = mDirect3D->CheckDeviceMultiSampleType(
			D3DADAPTER_DEFAULT,  mDeviceType, 
			mDxDisplayMode.Format, FALSE,
			D3DMULTISAMPLE_NONMASKABLE,
			&mMSAAQualityFullScreen );
		if ( FAILED( hr ) )
		{
			mMSAAQualityFullScreen = 0;
		}

        // 一番いいっぽいテクスチャフィルタ取得 ( あとでもっと追加しておく )
		DWORD tfcaps = mDxCaps.TextureFilterCaps;
		// 縮小
		if ( tfcaps & D3DPTFILTERCAPS_MIPFLINEAR )
		{
			mBestMipFilter = D3DTEXF_LINEAR;
		}
		else if ( tfcaps & D3DPTFILTERCAPS_MIPFPOINT )
		{
			mBestMipFilter = D3DTEXF_POINT;
		}
		// 拡大
		if ( tfcaps & D3DPTFILTERCAPS_MAGFANISOTROPIC )
		{
			mBestMagFilter = D3DTEXF_ANISOTROPIC;
		}
		else if ( tfcaps & D3DPTFILTERCAPS_MAGFLINEAR )
		{
			mBestMagFilter = D3DTEXF_LINEAR;
		}
		// ミップマップ
		if ( tfcaps & D3DPTFILTERCAPS_MINFANISOTROPIC )
		{
			mBestMinFilter = D3DTEXF_ANISOTROPIC;
		}
		else if ( tfcaps & D3DPTFILTERCAPS_MINFLINEAR )
		{
			mBestMinFilter = D3DTEXF_LINEAR;
		}

        LN_LOG_WRITE( "グラフィックデバイスの情報を取得します..." );
        LN_LOG_WRITE( "    スクリーンの幅             : %u", mDxDisplayMode.Width );
        LN_LOG_WRITE( "    スクリーンの高さ           : %u", mDxDisplayMode.Height );
        LN_LOG_WRITE( "    リフレッシュレート         : %u", mDxDisplayMode.RefreshRate );
        LN_LOG_WRITE( "    ディスプレイフォーマット   : %s", DX9Module::getDxFormatString( mDxDisplayMode.Format ) );
        LN_LOG_WRITE( "    テクスチャ" );
        LN_LOG_WRITE( "        最大横幅               : %u", mDxCaps.MaxTextureWidth );
        LN_LOG_WRITE( "        最大縦幅               : %u", mDxCaps.MaxTextureHeight );
        LN_LOG_WRITE( "        2 の累乗制限           : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_POW2 ? "あり" : "なし" );
        LN_LOG_WRITE( "        2 の累乗制限の限定解除 : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL ? "あり" : "なし" );
        LN_LOG_WRITE( "        正方形制限             : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY ? "あり" : "なし" );
        LN_LOG_WRITE( "        動的テクスチャ         : %s", mDxCaps.Caps2 & D3DCAPS2_DYNAMICTEXTURES ? "○" : "×" );
        LN_LOG_WRITE( "    キューブテクスチャ         : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP ? "○" : "×" );
        LN_LOG_WRITE( "        2 の累乗制限           : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP_POW2 ? "あり" : "なし" );
        LN_LOG_WRITE( "    ボリュームテクスチャ       : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP ? "○" : "×" );
        LN_LOG_WRITE( "        2 の累乗制限           : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP_POW2 ? "あり" : "なし" );
        LN_LOG_WRITE( "        ﾃﾞｨﾒﾝｼﾞｮﾝ最大値        : %u", mDxCaps.MaxVolumeExtent );
        LN_LOG_WRITE( "    異方性の次数               : %u", mDxCaps.MaxAnisotropy );
        LN_LOG_WRITE( "    異方性フィルタ拡大         : %s", mDxCaps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC ? "○" : "×" );
        LN_LOG_WRITE( "    異方性フィルタ縮小         : %s", mDxCaps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC ? "○" : "×" );
        LN_LOG_WRITE( "    最大プリミティブ数         : %u", mDxCaps.MaxPrimitiveCount );
        LN_LOG_WRITE( "    最大インデックス数         : %u", mDxCaps.MaxVertexIndex );
        LN_LOG_WRITE( "    頂点シェーダバージョン     : %u.%u", D3DSHADER_VERSION_MAJOR(mDxCaps.VertexShaderVersion), D3DSHADER_VERSION_MINOR(mDxCaps.VertexShaderVersion) );
        LN_LOG_WRITE( "    ピクセルシェーダバージョン : %u.%u", D3DSHADER_VERSION_MAJOR(mDxCaps.PixelShaderVersion), D3DSHADER_VERSION_MINOR(mDxCaps.PixelShaderVersion) );
        LN_LOG_WRITE( "    マルチレンダーターゲット数 : %u", mDxCaps.NumSimultaneousRTs );
        LN_LOG_WRITE( "    MSAA レベル(Window)        : %u", mMSAAQualityWindowed );
        LN_LOG_WRITE( "    MSAA レベル(FullScreen)    : %u", mMSAAQualityFullScreen );

        // シェーダのバージョンチェック
		LN_THROW_InvalidOperation((mDxCaps.VertexShaderVersion >= D3DVS_VERSION(2, 0)), Resource::String::ERR_InvalidVertexShaderVersion);
		LN_THROW_InvalidOperation((mDxCaps.PixelShaderVersion >= D3DPS_VERSION(2, 0)), Resource::String::ERR_InvalidPixelShaderVersion);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9GraphicsDevice::_resetDevice( bool fullscreen/*, const LSize& backbufferSize*/ )
    {
        HRESULT hr;

        // プレゼンテーションパラメータ設定
        mIsFullScreen = ( canFullScreen() ) ? fullscreen : false;
		if (mDefaultCanvas->getBackbufferResizeMode() == BackbufferResizeMode_Scaling)
			_setPresentParameters(mDefaultCanvas->getRequestedBackbufferSize());
		else
			_setPresentParameters(mSystemManager->getMainWindow()->getSize());

        // まだデバイスが作成されていなければ新規作成
        if ( !mDxDevice )
        {
            // D3Dデバイスの生成
            DWORD fpu_precision = ( mEnableFPUPreserve ) ? D3DCREATE_FPU_PRESERVE : 0;
            LN_COMCALL(
                mDirect3D->CreateDevice(
				    D3DADAPTER_DEFAULT,
				    mDeviceType,
				    mPresentParameters.hDeviceWindow,
				    fpu_precision | D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,//D3DCREATE_SOFTWARE_VERTEXPROCESSING, | D3DCREATE_MULTITHREADED
				    &mPresentParameters,
				    &mDxDevice ) );
        }
        // 既に作成されている場合はリセット
        else
        {
            // デバイスのリセット (作成したスレッド以外でリセットしようとすると失敗する)
            LN_COMCALL( mDxDevice->Reset( &mPresentParameters ) );
        }

        mIsDeviceLost.setFalse();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9GraphicsDevice::_setPresentParameters( const LSize& backbufferSize )
	{
		// フルスクリーンモードの場合
		if ( mIsFullScreen == true )
		{
            mPresentParameters.Windowed = TRUE;
			mPresentParameters.BackBufferWidth  = mDxDisplayMode.Width; 
			mPresentParameters.BackBufferHeight = mDxDisplayMode.Height;
			mPresentParameters.BackBufferFormat = D3DFMT_UNKNOWN;//D3DFMT_X8B8G8R8;//
			if ( false )//( mMSAAQualityWindowed > 0 )
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
				mPresentParameters.MultiSampleQuality = mMSAAQualityWindowed - 1;
			}
			else
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
				mPresentParameters.MultiSampleQuality = 0;
			}
            /*
			mPresentParameters.Windowed = FALSE;
			mPresentParameters.BackBufferWidth  = mDxDisplayMode.Width; 
            mPresentParameters.BackBufferHeight = mDxDisplayMode.Height;
            mPresentParameters.BackBufferFormat = mDxDisplayMode.Format;
			// 品質レベルが 0 より大きい場合
			if ( false )//( mMSAAQualityFullScreen > 0 )
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
				mPresentParameters.MultiSampleQuality = mMSAAQualityFullScreen - 1;
			}
			else
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
				mPresentParameters.MultiSampleQuality = 0;
			}
            */
		}
		// ウィンドウモードの場合
		else
		{
			mPresentParameters.Windowed = TRUE;
			mPresentParameters.BackBufferWidth  = std::max( 1, backbufferSize.w ); 
			mPresentParameters.BackBufferHeight = std::max(1, backbufferSize.h);
			mPresentParameters.BackBufferFormat = D3DFMT_X8R8G8B8;//D3DFMT_UNKNOWN;//D3DFMT_X8R8G8B8;//
			if ( false )//( mMSAAQualityWindowed > 0 )
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
				mPresentParameters.MultiSampleQuality = mMSAAQualityWindowed - 1;
			}
			else
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
				mPresentParameters.MultiSampleQuality = 0;
			}
		}
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote



//==============================================================================
//
//==============================================================================