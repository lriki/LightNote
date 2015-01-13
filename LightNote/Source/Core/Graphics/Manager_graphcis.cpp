//==============================================================================
// Manager 
//==============================================================================

#include "stdafx.h"
#include "../FileIO/Manager.h"
#include "../System/Manager.h"
#include "../Physics/Manager.h"
#ifdef LNOTE_WIN32
	#include "Device/DirectX9/DX9GraphicsDevice.h"
#endif
#include "Device/OpenGL/GLGraphicsDevice.h"
#include "Common/GeometryRenderer.h"
#include "Common/SpriteRenderer.h"
#include "Common/Font/FreeTypeFontManager.h"
#include "Common/Model/ModelManager.h"
#include "DeviceObjects.h"
#include "Manager.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ Manager
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Manager::Manager()
		: mFileIOManager	( NULL )
		, mGraphicsDevice	( NULL )
		, mGeometryRenderer	( NULL )
        , mSprite3DRenderer ( NULL )
        , mSprite2DRenderer ( NULL )
		, mDummyTexture		( NULL )
		, mFontManager		( NULL )
		, mModelManager		( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Manager::~Manager()
    {
        LN_SAFE_RELEASE( mFileIOManager );
        LN_SAFE_RELEASE( mGraphicsDevice );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initialize( const ConfigData& configData )
    {
        LN_LOG_INIT_BEGIN;

		LN_REFOBJ_SET( mFileIOManager, configData.FileIOManager );

		if ( configData.GraphicsAPI == LN_GRAPHICSAPI_DIRECTX9 )
		{
			DX9GraphicsDevice::ConfigData data;
            data.Manager				= this;
            data.SystemManager			= configData.SystemManager;
            data.FileManager			= mFileIOManager;
			data.TextureCacheSize		= configData.TextureCacheSize;
			data.ShaderCacheSize		= configData.ShaderCacheSize;
			data.BackbufferSize			= configData.BackbufferSize;
			data.BackbufferResizeMode	= configData.BackbufferResizeMode;
			data.FullScreen				= false;
            data.EnableVSyncWait		= true;
			data.EnableFPUPreserve		= configData.EnableFPUPreserve;
            DX9GraphicsDevice* device = LN_NEW DX9GraphicsDevice();
            device->initialize( data );
            mGraphicsDevice = device;
		}
		else if ( configData.GraphicsAPI == LN_GRAPHICSAPI_DIRECTX11 )
		{
		}
		else if ( configData.GraphicsAPI == LN_GRAPHICSAPI_OPENGL )
		{
			//GLGraphicsDevice::ConfigData data;
   //         data.Manager			= this;
   //         data.SystemManager		= configData.SystemManager;
			//data.FileManager		= mFileIOManager;
			//data.TextureCacheSize	= configData.TextureCacheSize;
			//data.ShaderCacheSize	= configData.ShaderCacheSize;
			//data.BackbufferSize		= configData.BackbufferSize;
   //         data.EnableVSyncWait	= true;
   //         GLGraphicsDevice* device = LN_NEW GLGraphicsDevice();
   //         device->initialize( data );
   //         mGraphicsDevice = device;
			//return;
		}

		// 形状描画クラス
		mGeometryRenderer = LN_NEW GeometryRenderer();
		mGeometryRenderer->initialize( this );

		// 3Dスプライト描画クラス
        mSprite3DRenderer = LN_NEW SpriteRenderer( this );
		mSprite3DRenderer->initialize(mGraphicsDevice, configData.MaxSprite2DCount, true);

        // 2Dスプライト描画クラス
        mSprite2DRenderer = LN_NEW SpriteRenderer( this );
		mSprite2DRenderer->initialize(mGraphicsDevice, configData.MaxSprite3DCount, false);

		// ダミーテクスチャ
        mDummyTexture = mGraphicsDevice->createTexture( 32, 32, 1, SurfaceFormat_A8R8G8B8 );
		mDummyTexture->clear(LColor::White);

		// フォント管理
        FreeTypeFontManager* fm = LN_NEW FreeTypeFontManager();
        fm->initialize( mFileIOManager );
        mFontManager = fm;

		// モデル管理クラス
        ModelManager::InitData mo_data;
		mo_data.GraphicsManager		= this;
        mo_data.GraphicsDevice      = mGraphicsDevice;
        mo_data.FileManager         = mFileIOManager;
        mo_data.PhysicsManager      = configData.PhysicsManager;
        mo_data.ModelCoreCacheSize  = 32;//TODO
        mModelManager = LN_NEW ModelManager();
		mModelManager->initialize( mo_data );

        LN_LOG_INIT_END;
    }


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::finalize()
    {
        LN_LOG_FIN_BEGIN;

		if ( mModelManager ) {
			mModelManager->finalize();
            LN_SAFE_RELEASE( mModelManager );
		}

		if ( mFontManager ) {
            mFontManager->finalize();
            LN_SAFE_RELEASE( mFontManager );
        }

		LN_SAFE_RELEASE( mDummyTexture );

        LN_SAFE_RELEASE( mSprite2DRenderer );
        LN_SAFE_RELEASE( mSprite3DRenderer );
        LN_SAFE_RELEASE( mGeometryRenderer );
		
		if ( mGraphicsDevice ) {
			mGraphicsDevice->finalize();
		}

        LN_LOG_FIN_END;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    IGraphicsDevice* Manager::getGraphicsDevice()
    {
        return mGraphicsDevice;
    }
	
} // namespace Graphics
} // namespace Core
} // namespace LNote
