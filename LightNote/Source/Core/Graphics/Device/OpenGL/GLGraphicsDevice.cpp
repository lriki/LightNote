//==============================================================================
// GLGraphicsDevice 
//==============================================================================

#include "stdafx.h"

#ifdef LNOTE_FOR_200
#include "../../../System/Manager.h"
#include "../../../FileIO/Manager.h"
//#include "LNGL/LNGL.h"
//#include "GLPlatformContext.h"
#include "GLRenderer.h"
#include "GLCanvas.h"
#include "GLVertexBuffer.h"
#include "GLIndexBuffer.h"
#include "GLShader.h"
#include "GLGraphicsDevice.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ GLGraphicsDevice
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    GLGraphicsDevice::GLGraphicsDevice()
		: mPlatformContext	( NULL )
		, mRenderer			( NULL )
		, mDefaultCanvas	( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    GLGraphicsDevice::~GLGraphicsDevice()
    {
        //LNGL::finalize();
		if ( mPlatformContext ) {
			mPlatformContext->finalize();
		}
		SAFE_DELETE( mPlatformContext );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GLGraphicsDevice::initialize( const ConfigData& configData )
	{
		GraphicsDeviceBase::initialize( configData );

		//LNGL::InitData lngl_data;
  //      lngl_data.LogFile       = NULL;
  //      lngl_data.SystemManager = configData.SystemManager;
  //      LNGL::initialize( lngl_data );

		mPlatformContext = LN_NEW GLPlatformContext();
		mPlatformContext->initialize( configData.SystemManager );

		mDefaultCanvas = LN_NEW GLCanvas();
		mDefaultCanvas->create(this, configData.SystemManager->getMainWindow(), configData.BackbufferSize, BackbufferResizeMode_Scaling);	// TODO: ResizeMode

		mRenderer = LN_NEW GLRenderer();
		mRenderer->initialize( this );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GLGraphicsDevice::finalize()
	{
		LN_SAFE_RELEASE( mRenderer );
		LN_SAFE_RELEASE( mDefaultCanvas );
		GraphicsDeviceBase::finalize();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IRenderer* GLGraphicsDevice::getRenderer() 
	{ 
		return mRenderer;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ICanvas* GLGraphicsDevice::getDefaultCanvas()
	{
		return mDefaultCanvas;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IVertexBuffer* GLGraphicsDevice::createVertexBuffer( LNVertexElemenst* elements, lnU32 vertexCount, const void* data, bool isDynamic )
    {
		LRefPtr<GLVertexBuffer> vb( LN_NEW GLVertexBuffer() );
		vb->create( this, elements, vertexCount, data, isDynamic );
		vb->addRef();
		return vb;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    IIndexBuffer* GLGraphicsDevice::createIndexBuffer( lnU32 indexCount, const void* data, bool isDynamic, bool is16bit )
    {
		LRefPtr<GLIndexBuffer> ib( LN_NEW GLIndexBuffer() );
		ib->create( this, indexCount, data, isDynamic, is16bit );
		ib->addRef();
		return ib;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ITexture* GLGraphicsDevice::createTexture( lnU32 width, lnU32 height, lnU32 levels, Graphics::SurfaceFormat format )
    {
		return NULL;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ITexture* GLGraphicsDevice::createTexture( FileIO::Stream* stream, lnU32 colorKey, lnU32 levels, Graphics::SurfaceFormat format, lnSharingKey key )
    {
		return NULL;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ITexture* GLGraphicsDevice::createTexture( const lnChar* filePath, lnU32 colorKey, lnU32 levels, Graphics::SurfaceFormat format, lnSharingKey key )
    {
		return NULL;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ITexture* GLGraphicsDevice::createRenderTarget( lnU32 width, lnU32 height, lnU32 mipLevels, Graphics::SurfaceFormat format )
    {
		return NULL;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ITexture* GLGraphicsDevice::createDepthBuffer( lnU32 width, lnU32 height, Graphics::SurfaceFormat format )
    {
		return NULL;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IShader* GLGraphicsDevice::createShader( FileIO::Stream* stream, lnSharingKey registerKey )
	{ 
		// キャッシュ検索
		IShader* ts = static_cast<GLShader*>( findShaderCache( registerKey ) );
		if ( ts ) {
			return ts;
		}

		// Shader 作成
        LRefPtr<GLShader> shader( LN_NEW GLShader() );
        shader->create( this, stream );

		// エラーでなければキャッシュ管理に追加
        if ( shader->getCompileResult() != LN_SHADERRESULT_ERROR ) {
			if ( !registerKey.isEmpty() ) {
				mShaderCache.registerCachedObject( registerKey, shader );
			}
        }

		shader.addRef();
		return shader;
	}

	////---------------------------------------------------------------------
	////
 //   //---------------------------------------------------------------------
 //   IShader* GLGraphicsDevice::createShader( const void* data, lnU32 size, lnSharingKey registerKey )
 //   {
	//	return NULL;
 //   }

	////---------------------------------------------------------------------
	////
 //   //---------------------------------------------------------------------
	//IShader* GLGraphicsDevice::createShader( const lnChar* filePath, lnSharingKey registerKey )
 //   {
	//	// キャッシュ検索
	//	IShader* ts = static_cast<GLShader*>( findShaderCache( registerKey ) );
	//	if ( ts ) {
	//		return ts;
	//	}

	//	// Shader 作成
 //       LRefPtr<GLShader> shader( LN_NEW GLShader() );
 //       shader->create( this, filePath );

	//	// エラーでなければキャッシュ管理に追加
 //       if ( shader->getCompileResult() != LN_SHADERRESULT_ERROR ) {
	//		if ( !registerKey.isEmpty() ) {
	//			mShaderCache.registerCachedObject( registerKey, shader );
	//		}
 //       }

	//	shader->addRef();
	//	return shader;
 //   }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GLGraphicsDevice::pauseDevice()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GLGraphicsDevice::resumeDevice()
    {
    }

	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
	//void GLGraphicsDevice::testDeviceLost()
	//{
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLGraphicsDevice::attachCurrentThread()
	{
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote

#else
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../../System/Manager.h"
#include "../../../FileIO/Manager.h"
#include "LNGL/LNGL.h"
#include "GLRenderer.h"
#include "GLCanvas.h"
#include "GLVertexBuffer.h"
#include "GLIndexBuffer.h"
#include "GLTexture.h"
#include "GLShaderManager.h"
#include "GLShader.h"
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
// ■ GraphicsDevice クラス
//==============================================================================

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    GraphicsDevice::GraphicsDevice()
        : mLogFile              ( NULL )
        , mSystemManager        ( NULL )
        , mFileManager          ( NULL )
        , mRenderer             ( NULL )
        , mShaderManager        ( NULL )
        //, mPlatformContext      ( NULL )
        , mDefaultCanvas        ( NULL )
        , mCurrentShaderrPass   ( NULL )
        , mIsDeviceLost         ( false )
    {
    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    GraphicsDevice::~GraphicsDevice()
    {
        /*SAFE_DELETE( mPlatformContext );*/

        LNGL::finalize();
    }

	//----------------------------------------------------------------------
	// ● 初期化
	//----------------------------------------------------------------------
    void GraphicsDevice::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        LNGL::InitData lngl_data;
        lngl_data.LogFile       = mLogFile;
        lngl_data.SystemManager = init_data_.SystemManager;
        LNGL::initialize(lngl_data);
        


        mManager        = init_data_.Manager;
        mFileManager    = init_data_.FileManager;


        
        //-----------------------------------------------------
        //


        // キャッシュサイズを設定
        TextureCache::initialize( init_data_.TextureCacheSize );

        // デフォルトのキャンバス (コンストラクタでリストに入れられる)
        DefaultCanvas::InitData c_data;
        //c_data.PlatformContext  = mPlatformContext;
        c_data.Window           = init_data_.SystemManager->getMainWindow();
        c_data.CanvasSize       = init_data_.CanvasSize;
        mDefaultCanvas = LN_NEW DefaultCanvas( this );
        mDefaultCanvas->initialize( c_data );

        // 描画クラスの作成
        Renderer::InitData rend_data;
        rend_data.LogFile           = mLogFile;
        rend_data.GraphicsDevice    = this;
        //rend_data.PlatformContext   = mPlatformContext;
        mRenderer = LN_NEW Renderer();
        mRenderer->initialize( rend_data );

        // シェーダ管理クラス
        ShaderManager::InitData sm_data;
        sm_data.LogFile             = mLogFile;
        sm_data.GraphicsDevice      = this;
        sm_data.ShaderCacheSize     = init_data_.ShaderCacheSize;
        mShaderManager = LN_NEW ShaderManager();
        mShaderManager->initialize( sm_data );



        

        //mIsDeviceLost = true;   // リセットを実行させるため、一度 true にする
        //onResetDevice();

        LN_LOG_INIT_END;
    }


	//----------------------------------------------------------------------
	// ● 終了処理
	//----------------------------------------------------------------------
    void GraphicsDevice::finalize()
    {
        LN_LOG_FIN_BEGIN;

        pauseDevice();

        LN_SAFE_RELEASE( mCurrentShaderrPass );

        
        LN_SAFE_RELEASE( mRenderer );

        // ~Renderer() → onLostDevice() で使用しているのでRenderer の後で開放する
        LN_SAFE_RELEASE( mDefaultCanvas );  

        if ( mShaderManager )
        {
            mShaderManager->finalize();
            LN_SAFE_RELEASE( mShaderManager );
        }

        TextureCache::finalize();

        LN_LOG_FIN_END;
    }

	//----------------------------------------------------------------------
	// ● 描画クラスの取得
	//----------------------------------------------------------------------
    IRenderer* GraphicsDevice::getRenderer()
    {
        return mRenderer;
    }

	//----------------------------------------------------------------------
	// ● デフォルトのバックバッファを示すキャンバスの取得
	//----------------------------------------------------------------------
    ICanvas* GraphicsDevice::getDefaultCanvas()
    {
        return mDefaultCanvas;
    }

	//----------------------------------------------------------------------
	// ● 頂点バッファの作成
	//----------------------------------------------------------------------
    LNRESULT GraphicsDevice::createVertexBuffer( IVertexBuffer** obj_, LNVertexElemenst* elements_, lnU32 vertex_num_, const void* data_, bool is_dynamic_ )
    {
        *obj_ = NULL;
            
        VertexBuffer* vb = LN_NEW VertexBuffer();
        vb->initialize( this, elements_, vertex_num_, data_, is_dynamic_ );

        mVertexBufferList.push_back(vb);
        *obj_ = vb;
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● インデックスバッファの作成
	//----------------------------------------------------------------------
    LNRESULT GraphicsDevice::createIndexBuffer( IIndexBuffer** obj_, lnU32 index_num_, const lnU16* data_, bool is_dynamic_, bool is_16bit_ )
    {
        *obj_ = NULL;

        IndexBuffer* ib = LN_NEW IndexBuffer();
        ib->initialize( this, index_num_, data_, is_dynamic_, is_16bit_ );

        mIndexBufferList.push_back(ib);
        *obj_ = ib;
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● テクスチャの作成
	//----------------------------------------------------------------------
    LNRESULT GraphicsDevice::createTexture( ITexture** obj_, lnU32 width_, lnU32 height_, lnU32 levels_, LNSurfaceFormat format_ )
    {
        *obj_ = NULL;
        Texture* tex = LN_NEW Texture( this );
        tex->initialize( width_, height_, levels_, format_ );
        *obj_ = tex;
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● テクスチャの作成 ( メモリに展開された画像データから )
	//----------------------------------------------------------------------
    LNRESULT GraphicsDevice::createTexture( ITexture** obj_, const void* data_, lnU32 size_, lnU32 color_key_, lnU32 levels_, LNSurfaceFormat format_, lnSharingKey key_ )
    {
#if 0
        LNRESULT lr;
        *obj_ = NULL;

        // キャッシュを検索
        *obj_ = TextureCache::findCacheAddRef( key_ );
        if ( *obj_ )
        {
            return LN_OK;
        }
        
        // 新しく作る
        Texture* texture = LN_NEW Texture( this );
        LN_CALL_R( texture->initialize( data_, size_, color_key_, levels_, format_ ) );

        // キャッシュ管理クラスに追加
        TextureCache::registerCachedObject( key_, texture );
        
        *obj_ = texture;
        return LN_OK;
#endif
        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● テクスチャの作成 (入力ストリームから)
	//----------------------------------------------------------------------
    LNRESULT GraphicsDevice::createTexture( ITexture** obj_, FileIO::IInStream* stream_, lnU32 color_key_, lnU32 levels_, LNSurfaceFormat format_, lnSharingKey key_ )
    {
#if 0
        lnU32 size = stream_->getSize();
        *obj_ = NULL;

        lnByte* buf = mTempBuffer.lock( size );
        stream_->read( buf, size );

        LNRESULT lr = createTexture( obj_, buf, size, color_key_, levels_, format_, key_ );

        mTempBuffer.unlock();
        return lr;
#endif
        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }
    
	//----------------------------------------------------------------------
	// ● テクスチャの作成 (ファイルから)
	//----------------------------------------------------------------------
    void GraphicsDevice::createTexture( ITexture** obj_, const lnChar* filename_, lnU32 color_key_, lnU32 levels_, LNSurfaceFormat format_, lnSharingKey key_ )
    {
        *obj_ = NULL;
        Texture* tex = LN_NEW Texture( this );
        tex->initialize( filename_, color_key_, levels_, format_ );
        *obj_ = tex;

        mTextureList.push_back(tex);
    }

	//----------------------------------------------------------------------
	// ● レンダーターゲットテクスチャの作成
	//----------------------------------------------------------------------
    LNRESULT GraphicsDevice::createRenderTarget( ITexture** obj_, lnU32 width_, lnU32 height_, lnU32 levels_, LNSurfaceFormat format_ )
    {
        *obj_ = NULL;
        RenderTargetTexture* tex = LN_NEW RenderTargetTexture( this );
        tex->initialize( width_, height_, levels_, format_ );
        mRenderTargetList.push_back( tex );
        *obj_ = tex;
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● 深度バッファの作成
	//----------------------------------------------------------------------
    LNRESULT GraphicsDevice::createDepthBuffer( ITexture** obj_, lnU32 width_, lnU32 height_, LNSurfaceFormat format_ )
    {
        *obj_ = NULL;
        DepthBuffer* tex = LN_NEW DepthBuffer( this );
        tex->initialize( width_, height_, format_ );
        mDepthBufferList.push_back( tex );
        *obj_ = tex;
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● シェーダの作成
	//----------------------------------------------------------------------
    LNRESULT GraphicsDevice::createShader( IShader** obj_, const void* data_, lnU32 size_, lnSharingKey key_ )
    {
#if 0
        LNRESULT lr;
        *obj_ = NULL;

        // キャッシュを検索
        *obj_ = ShaderCache::findCacheAddRef( key_ );
        if ( *obj_ )
        {
            return LN_OK;
        }
        
        // 新しく作る
        Shader* shader = LN_NEW Shader( this );
        LN_CALL_R( shader->initialize( static_cast< const char* >( data_ ) ) );

        // キャッシュ管理クラスに追加
        ShaderCache::registerCachedObject( key_, shader );

        mShaderList.push_back( shader );
        
        *obj_ = shader;
        return LN_OK;
#endif
        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● シェーダの作成 ( 入力ストリームから )
	//----------------------------------------------------------------------
    LNRESULT GraphicsDevice::createShader( IShader** obj_, FileIO::IInStream* stream_, lnSharingKey key_ )
    {
#if 0
        lnU32 size = stream_->getSize();
        *obj_ = NULL;

        lnByte* buf = mTempBuffer.lock( size + 1 );

        // mTempBuffer に全部読み込む
        stream_->read( buf, size );

        // Cg に渡すときは終端 NULL が必要
        buf[ size ] = '\0';

        LNRESULT lr = createShader( obj_, buf, size, key_ );

        mTempBuffer.unlock();
        return lr;
#endif
        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GraphicsDevice::createShader( IShader** obj_, const lnChar* filename_, lnSharingKey key_ )
    {
        *obj_ = mShaderManager->createShader( filename_, key_ );
    }

	//----------------------------------------------------------------------
	// ● キャッシュのクリア
	//----------------------------------------------------------------------
    void GraphicsDevice::clearCache( bool texture_, bool shader_ )
    {
        if ( texture_ ) TextureCache::clear();
        if ( shader_  ) mShaderManager->clearCache();
    }
    
	//----------------------------------------------------------------------
	// ● デバイスリセット前処理
	//----------------------------------------------------------------------
    void GraphicsDevice::pauseDevice()
    {
        if (!mIsDeviceLost)
        {
            LN_LOG_WRITE(_T("GraphicsDevice::onLostDevice() begin"));

            ln_foreach(VertexBuffer* t, mVertexBufferList)
            {
                t->onLostDevice();
            }
            ln_foreach(IndexBuffer* t, mIndexBufferList)
            {
                t->onLostDevice();
            }
            ln_foreach(Texture* t, mTextureList)
            {
                t->onLostDevice();
            }
            ln_foreach(RenderTargetTexture* t, mRenderTargetList)
            {
                t->onLostDevice();
            }
            ln_foreach(DepthBuffer* t, mDepthBufferList)
            {
                t->onLostDevice();
            }          

            if (mShaderManager) mShaderManager->onLostDevice();
            if (mRenderer) mRenderer->onLostDevice();
            if (mDefaultCanvas) mDefaultCanvas->onLostDevice();
            mIsDeviceLost = true;

            LN_LOG_WRITE(_T("GraphicsDevice::onLostDevice() end"));
        }
    }

	//----------------------------------------------------------------------
	// ● デバイスリセット後処理
	//----------------------------------------------------------------------
    void GraphicsDevice::resumeDevice()
    {
        if (mIsDeviceLost)
        {
            LN_LOG_WRITE(_T("GraphicsDevice::onResetDevice() begin"));

            mDefaultCanvas->onResetDevice();
            mRenderer->onResetDevice();
            mShaderManager->onResetDevice();

            ln_foreach(DepthBuffer* t, mDepthBufferList)
            {
                t->onResetDevice();
            } 
            ln_foreach(RenderTargetTexture* t, mRenderTargetList)
            {
                t->onResetDevice();
            }
            ln_foreach(Texture* t, mTextureList)
            {
                t->onResetDevice();
            }
            ln_foreach(IndexBuffer* t, mIndexBufferList)
            {
                t->onResetDevice();
            }
            ln_foreach(VertexBuffer* t, mVertexBufferList)
            {
                t->onResetDevice();
            }
            
            // Renderer::setRenderTarget() で mDefaultCanvas を使ってるとかの都合で、
            // mDefaultCanvas、mRenderer の初期化が終わった後に設定しておく必要がある
            //mDefaultCanvas->activate();

            mIsDeviceLost = true;

            LN_LOG_WRITE(_T("GraphicsDevice::onResetDevice() end"));
        }
    }


	//----------------------------------------------------------------------
	// ● デバイスのリセット
	//----------------------------------------------------------------------
    LNRESULT GraphicsDevice::resetDevice( bool fullscreen_, const Geometry::Size& window_bb_size_ )
    {
#if 0
        HRESULT hr;
        LNRESULT lr;

        // プレゼンテーションパラメータ設定
        mIsFullScreen = ( canFullScreen() ) ? fullscreen_ : false;
		_setPresentParameters( window_bb_size_ );

        // まだデバイスが作成されていなければ新規作成
        if ( !mDxDevice )
        {
            // D3Dデバイスの生成
            DWORD fpu_precision = ( mEnableFPUPreserve ) ? D3DCREATE_FPU_PRESERVE : 0;
            LN_DXCALL_R(
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
            LN_DXCALL_R( _onLostDevice() );

            // デバイスのリセット
            LN_DXCALL_R( mDxDevice->Reset( &mPresentParameters ) );

            LN_DXCALL_R( _onResetDevice() );
        }

        return LN_OK;
#endif
        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GraphicsDevice::commitChangesGLResource()
    {
        ln_foreach(VertexBuffer* t, mVertexBufferList)
        {
            t->commitChanges();
        }
        ln_foreach(IndexBuffer* t, mIndexBufferList)
        {
            t->commitChanges();
        }
        ln_foreach(Texture* t, mTextureList)
        {
            t->commitChanges();
        }
        ln_foreach(RenderTargetTexture* t, mRenderTargetList)
        {
            t->commitChanges();
        }
        ln_foreach(DepthBuffer* t, mDepthBufferList)
        {
            t->commitChanges();
        }     
        mShaderManager->commitChangesGLResource();
    }

	//----------------------------------------------------------------------
	// ● デバイスの性能チェック
	//----------------------------------------------------------------------
    LNRESULT GraphicsDevice::_checkDeviceInformation()
    {

        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }

 
	//----------------------------------------------------------------------
	// ● デバイスリセット前処理
	//----------------------------------------------------------------------
    LNRESULT GraphicsDevice::_onLostDevice()
    {

        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● デバイスリセット後処理
	//----------------------------------------------------------------------
    LNRESULT GraphicsDevice::_onResetDevice()
    {
#if 0
        LNRESULT lr;

        ln_foreach( Shader* t, mShaderList )
        {
            LN_CALL_R( t->onResetDevice() );
        }

        ln_foreach( DepthBuffer* t, mDepthBufferList )
        {
            LN_CALL_R( t->onResetDevice() );
        }

        ln_foreach( RenderTargetTexture* t, mRenderTargetList )
        {
            LN_CALL_R( t->onResetDevice() );
        }
       
        ln_foreach( CanvasBase* t, mCanvasList )
        {
            LN_CALL_R( t->onResetDevice() );
        }

        // ビューポートサイズのリセットにバックバッファサイズを使っているため、
        // デフォルトキャンバスの onResetDevice() の後でないと
        // バックバッファサイズが古いままになってしまう
        LN_CALL_R( mRenderer->onResetDevice() );

        // TODO リセット後コールバック呼び出し
        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
#endif
        LN_PRINT_NOT_IMPL_FUNCTION;
        
        
        
        
        
        
        return LN_OK;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GraphicsDevice::setCurrentShaderrPass( ShaderPass* pass_ )
    {
        if ( pass_ != mCurrentShaderrPass )
        {
            LN_SAFE_RELEASE( mCurrentShaderrPass );
            mCurrentShaderrPass = pass_;
            LN_SAFE_ADDREF( mCurrentShaderrPass );
        }
    }


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