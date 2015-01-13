//==============================================================================
// SceneShader 
//------------------------------------------------------------------------------
///**
//  @file       SceneShader.h
//  @brief      SceneShader
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Interface.h"
#include "../../Graphics/DeviceObjects.h"
#include "../Common.h"
#include "MMETypes.h"
#include "SceneShaderManager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
class SceneShaderTechnique;
class NodeRenderParam;
class DrawCommandContainer;

//==============================================================================
// SceneShader
//------------------------------------------------------------------------------
///**
//  @brief      シーン用シェーダのクラス
//*/
//==============================================================================
class SceneShader
    : public Base::ReferenceObject
	, public Base::ICacheObject
{
	LN_CACHE_OBJECT_DECL;
	LN_TYPE_INFO_ACCESS_DECL;
public:

    struct InitData
    {
        const void*     ShaderData;
        lnU32           ShaderDataSize;
        LRefTString     ShaderName;

    };

public:
    SceneShader( SceneShaderManager* manager );
    virtual ~SceneShader();

public:

	/// 初期化
	//LNRESULT initialize( const InitData& init_data_ );

	/// 作成
	void create(const lnChar* filePath, lnString* errors);

	/// 作成 TODO: ユーティリティ関数化
	void create( int dataResourceID );

	/// 再読み込み
    LNRESULT reload();

	/// リソースの再構築
    LNRESULT refreshResource( const LVector2& view_size_ );

	/// Graphics::IShader の取得
    Graphics::IShader* getCoreShader() { return mShader; }

	/// エラー文字列の設定 (SceneShaderTechnique クラスからも呼ばれる)
    void setErrorMessage( LRefTString str_ ) { mErrorMessage = str_; }

	/// エフェクトファイルの使用目的の取得
    MMEScriptClass getScriptClass() const { return mScriptClass; }

	/// エフェクトファイルの実行タイミングの取得
    MMEScriptOrder getScriptOrder() const { return mScriptOrder; }

	/// ポストエフェクトか、ポストエフェクトの場合 true
    bool isPostEffect() const { return ( mScriptOrder == MME_SCRORDER_preprocess || mScriptOrder == MME_SCRORDER_postprocess ); }

	/// ScriptExternal を持つエフェクト
    bool hasScriptExternal();



public: // ▼ 内部用

	/// SceneShaderManager 取得
    SceneShaderManager* getSceneShaderManager() { return mManager; }

	const FileIO::PathNameW& getFilePath() const { return mFilePath; }

	/// 設定する必要があるライトの数の取得
    lnU32 getRequiredLightNum() const { return mRequiredLightNum; }

	/// 変数名に対応する MMEShaderVariable を検索する (MMETechniqueで使う。このポインタはシェーダがあるうちは不変なので、MMETechnique で使うなら initialize() で取得しておいてもOK)
    MMEShaderVariable* findShaderVariable( const lnChar* name_ );

    

	/// Scene 単位で必要なパラメータを設定する
    void updateSceneParam( const MMESceneParam& param_, MMESceneBase* scene );

	/// Camera 単位で必要なパラメータを設定する
    void updateCameraParam( const LNCameraSceneParam& param_ );

	/// Node 単位で必要なパラメータを設定する (ノードごとに影響するライトは異なるため、ライトもこの中)
    LNRESULT updateNodeParam( const MMESceneObjectBase* param_ );// const NodeRenderParam& param_ );

	/// Subset 単位で必要なパラメータを設定する
    void updateSubsetParam( const LNSubsetRenderParam& param_ );

	/// "OffscreenOwner" を設定して、対象となる CONTROLOBJECT を更新する
    void updateOffscreenOwner( const MMESceneObjectBase* obj );

	/// CONTROLOBJECT を要求している変数の値をすべて更新する
	//void updateControlObjectVariables();

	/// オフスクリーンRT DefaultEffect の "self" で識別されるノードの取得 (onSettingShader() で設定)
    MMESceneObjectBase* getSelfSceneNode() { return mSelfSceneNode; }

	/// 共有可能なシェーダの場合 true
    bool isShareable() const { return mShareable; }

	/// SceneNode::setShader() での先頭で呼ばれる
	//void onSettingShader( MMESceneObjectBase* node_ );


	/// 情報出力
    void dumpInfo( FILE* stream_ = NULL );


	/// ★条件に一致するテクニックを取得する (見つからなければ NULLを返す)
	SceneShaderTechniqueArray* findTechnique( MMDPass pass, bool UseTexture, bool UseSphereMap, bool UseToon );

private:

	/// パラメータなどを実際に設定していく
    LNRESULT _build();

	/// 解放
    void _release();

	/// リソース解放
    void _releaseResource();

	/// シェーダ変数のセマンティクスを調べて対応する要求項目を返す
    MMEVariableRequest _checkVariableRequest( Graphics::IShaderVariable* var_, MMEShaderVariable* sv_ );

private:

	typedef std::vector<SceneShaderTechniqueArray>          TechniqueGroupArray;
    typedef std::vector< OffscreenScene* >          OffscreenSceneList;

private:

    SceneShaderManager*     mManager;
    Graphics::IShader*      mShader;                ///< シェーダプログラム本体
    //wchar_t					mFileFullPath[LN_MAX_PATH];
	FileIO::PathNameW		mFilePath;

    MMEScriptOutput         mScriptOutput;          ///< STANDARDSGLOBAL の ScriptOutput (常に "color")
    MMEScriptClass          mScriptClass;           ///< エフェクトファイルの使用目的 (デフォルトは "object")
    MMEScriptOrder          mScriptOrder;           ///< エフェクトファイルの実行タイミング (デフォルトは "standard")

    MMEShaderVariableArray  mShaderVariableArray;   ///< シェーダプログラム内の変数一覧
    MMEShaderVariableMap    mShaderVariableMap;     ///< 変数名とMMEShaderVariableの対応表 (主にテクニックのスクリプトで使う)
    MMEShaderVariableArray  mControlObjectVarArray; ///< CONTROLOBJECT を要求している変数リスト
    SceneShaderTechniqueArray   mTechniqueList;         ///< テクニックの一覧
	TechniqueGroupArray		mTechniqueGroupArray;
    OffscreenSceneList      mOffscreenSceneList;

    lnU32                   mRequiredLightNum;
    lnU32                   mWorldMatrixCalcMask;   ///< MMEWorldMatrixCalcFlags の組み合わせ (もしかしたら使わないかも)

    MMESceneObjectBase*              mSelfSceneNode;

    LRefTString             mErrorMessage;
    bool                    mIsError;
    bool                    mShareable;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================