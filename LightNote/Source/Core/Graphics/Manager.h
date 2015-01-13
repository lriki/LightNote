//==============================================================================
// Manager 
//------------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "Interface.h"
#include "Common/Font/FontManagerBase.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ Manager
//------------------------------------------------------------------------------
///**
//  @brief		グラフィックス機能の管理クラス
//*/
//==============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

	/// 初期化データ
	struct ConfigData
	{
        FileIO::Manager*				FileIOManager;
        Physics::Manager*				PhysicsManager;
        System::Manager*				SystemManager;
		LSize							BackbufferSize;
		Graphics::BackbufferResizeMode	BackbufferResizeMode;
		bool							EnableFPUPreserve;  ///< 浮動小数点の演算精度を落とさないようにする場合は true
        LNFontNameList					FontFilenameList;
		LNGraphicsAPI					GraphicsAPI;
		lnU32							TextureCacheSize;
		lnU32							ShaderCacheSize;
		lnU32							MaxSprite2DCount;
		lnU32							MaxSprite3DCount;
    };

public:
	
    Manager();
    virtual ~Manager();

public:

	/// 初期化
    void initialize( const ConfigData& configData );

	/// 終了処理
    void finalize();

	/// IGraphicsDevice の取得
    IGraphicsDevice* getGraphicsDevice();

	/// 形状描画クラスの取得
    GeometryRenderer* getGeometryRenderer() { return mGeometryRenderer; }

	/// 3Dスプライト描画クラスの取得
    SpriteRenderer* getSprite3DRenderer() { return mSprite3DRenderer; }

	/// 2Dスプライト描画クラスの取得
    SpriteRenderer* getSprite2DRenderer() { return mSprite2DRenderer; }

	/// ダミー用テクスチャの取得 (32x32、不透明の白)
    Texture* getDummyTexture() { return mDummyTexture; }

	/// フォント管理クラスの取得
    FontManagerBase* getFontManager() { return mFontManager; }

	/// モデル管理クラスの取得
    ModelManager* getModelManager() { return mModelManager; }

public:
	FileIO::Manager*	getFileIOManager() { return mFileIOManager; }

private:

	FileIO::Manager*	mFileIOManager;
	GraphicsDeviceBase*	mGraphicsDevice;
	GeometryRenderer*	mGeometryRenderer;
    SpriteRenderer*		mSprite3DRenderer;
    SpriteRenderer*		mSprite2DRenderer;
    Texture*			mDummyTexture;
	FontManagerBase*	mFontManager;
    ModelManager*		mModelManager;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
