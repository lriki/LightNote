//==============================================================================
// SceneShaderManager 
//------------------------------------------------------------------------------
///**
//  @file       SceneShaderManager.h
//  @brief      SceneShaderManager
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <algorithm>
#include "../../Base/Misc.h"
#include "../../Base/Cache.h"
#include "../../Base/ReferenceObjectArray.h"
#include "../../System/Interface.h"
#include "../../FileIO/Manager.h"
#include "../../Graphics/Interface.h"
#include "../../Graphics/DeviceObjects.h"
#include "../../Graphics/Manager.h"
#include "../../Game/GameTime.h"
#include "../Common.h"
#include "BaseClasses.h"
#include "MMETypes.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
class SceneGraph;
class SceneShader;
class OffscreenScene;
class DrawCommandContainer;
//typedef Base::CacheObjectManager< SceneShader >    SceneShaderCache;

//==============================================================================
// ■ SceneShaderManager クラス
//------------------------------------------------------------------------------
///**
//  @brief      シーン用シェーダのクラス
//*/
//==============================================================================
class SceneShaderManager
    : public Base::CacheManager
{
public:

	/// 初期化データ
    struct ConfigData
    {
        FileIO::Manager*		FileManager;
        Graphics::Manager*  GraphicsManager;
        lnU32               ShaderCacheSize;
        bool                UseSemantics;       ///< true の場合、シェーダ変数の検索にセマンティクス名を使う (false の場合は名前)
    };

	

public:
    SceneShaderManager(/* MMESceneBase* scene */);
    virtual ~SceneShaderManager();

public:

	/// 初期化
    void initialize( const ConfigData& configData );

	/// 終了処理
    void finalize();

	/// テクスチャ生成時に称するビューサイズ
	void setBaseViewSize( const LVector2& size ) { mBaseViewSize = size; }

	/// シェーダの作成
	//LNRESULT createSceneShader( SceneShader** shader_, const void* data_, lnU32 size_, const lnChar* name_ );

	/// シェーダの作成 (ファイル名指定・ファイル名をキーとして辞書登録)
    SceneShader* createSceneShader( const lnChar* fulePath, lnString* errors );

	/// シェーダの作成
	SceneShader* createSceneShader( int dataResourceID, const lnSharingKey& key );

	/// 解放が必要なシェーダがないかチェックする
    void updateShaderList();

	/// 全シェーダにシーン単位で設定するべきデータを設定する
	void updateAllShaderGlobalParam(double totalTime, float elapsedTime,/*const Game::GameTime& game_time_,*/ MMESceneBase* scene);

	/// 全ェーダにカメラ単位で設定するべきデータを設定する
    void updateAllShaderCameraParam( const LNCameraSceneParam& param_ );
  
	///// (SceneGraph::addNode() から呼ばれる)
	//void addSceneObjectToOffscreenScene( MMESceneObjectBase* obj );

	///// (SceneGraph::onDeleteNode() から呼ばれる)
	//void removeSceneObjectFromOffscreenScene( MMESceneObjectBase* obj );

	/// 全 OffscreenScene の描画
	void renderAllOffscreenScene( MMESceneBase* scene, ContextDrawParam& param, MMESceneObjectBase* renderRootNode );

public:

	/// メッセージ処理 (SceneGraph から呼ばれる)
    bool onEvent( const System::EventArgs& e );

	//MMESceneBase* getScene() { return mScene; }
	const LVector2& getBaseViewSize() const { return mBaseViewSize; }	// テクスチャ生成時に使用する

	/// セマンティクスの文字列とIDの対応表の取得
    const MMESemanticsMap& getSemanticsMap() { return mMMESemanticsMap; }

	/// アノテーションの文字列とIDの対応表
    const MMEAnnotationMap& getAnnotationMap() { return mMMEAnnotationMap; }

    LNRESULT createTexture(
        Graphics::Texture** texture_,
        MMEVariableRequest req_,
        //const lnChar* resource_name_,
		FileIO::PathNameW resourcePath,
        lnU32 width_,
        lnU32 height_,
        lnU32 depth_,
        lnU32 miplevels_,
        Graphics::SurfaceFormat format_ );

	/// GraphicsManager の取得
    Graphics::Manager* getGraphicsManager() { return mGraphicsManager; }

	/// グラフィックスデバイスの取得
    Graphics::IGraphicsDevice* getGraphicsDevice() { return mGraphicsDevice; }
    FileIO::Manager* getFileManager() const { return mFileManager; }

    

	/// SceneShader::updateNodeParam() で行列・ベクトル配列をセットするときに使うので公開
    Base::TempBuffer* getTempBuffer() { return &mTempBuffer; }

 
	void addMMETextureObject( MMETextureObject* tex );
	void removeMMETextureObject( MMETextureObject* tex );
	void addOffscreenScene( OffscreenScene* ofs );
	void removeOffscreenScene( OffscreenScene* ofs );
private:

	/// シェーダの作成
	LNRESULT _createSceneShader(SceneShader** shader_, const void* data_, lnU32 size_, const lnChar* name_, lnString* errors);

	/// シェーダの作成 (入力ストリームから)
	SceneShader* _createSceneShader(FileIO::Stream* stream, const lnChar* key, lnString* errors);

private:

    typedef std::vector< SceneShader* >     SceneShaderList;
	//typedef Base::ReferenceObjectArray< SceneShader* >		SceneShaderArray;
    typedef std::vector< OffscreenScene* >  OffscreenSceneArray;
	//typedef Base::ReferenceObjectArray< OffscreenScene* >		OffscreenSceneArray;

private:

	//MMESceneBase*				mScene;
    FileIO::Manager*				mFileManager;
    Graphics::Manager*          mGraphicsManager;
    Graphics::IGraphicsDevice*  mGraphicsDevice;
	LVector2					mBaseViewSize;

    MMESemanticsMap             mMMESemanticsMap;   ///< セマンティクスの文字列とIDの対応表 (セマンティクスを使わない場合は変数名) 
    MMEAnnotationMap            mMMEAnnotationMap;  ///< アノテーションの文字列とIDの対応表

	//SceneShaderArray			mSceneShaderArray;			///< 解放が ~SceneShader → ~OffscreenScene → ~SceneShader のように再帰的に呼ばれるため、普通の vector では Cache 内でデッドロックする
	SceneShaderList				mSceneShaderList;
	MMETextureObjectArray		mMMETextureObjectArray;		///< 生成・削除などで描画スレッドから参照することはないため、普通の vector で問題ない
    OffscreenSceneArray          mOffscreenSceneList;

    double                      mPlayStartTime;
    MMESceneParam               mMMESceneParam;     ///< Scene 単位のシェーダ設定パラメータ (updateAllShaderGlobalParam() で設定)
   
    Base::TempBuffer            mTempBuffer;        ///< 
	std::stack<int>				mOffscreenSceneIDStack;	///< (0～MaxOffscreenScenes - 1)
	
	Threading::Mutex			mShaderListMutex;
	Threading::Mutex			mObjectListMutex;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================