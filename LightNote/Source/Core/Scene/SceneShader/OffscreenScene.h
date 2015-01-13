//==============================================================================
// OffscreenScene 
//------------------------------------------------------------------------------
///**
//  @file       OffscreenScene.h
//  @brief      OffscreenScene
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Common.h"
#include "../SceneNode/NodeRenderParam.h"

namespace LNote
{
namespace Core
{
namespace Scene
{
class SceneGraph;
class SceneShader;

//==============================================================================
// ■ OffscreenScene
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class OffscreenScene
    : public Base::ReferenceObject
{
public:
	OffscreenScene( SceneShaderManager* manager, SceneShader* shader );
    virtual ~OffscreenScene();

public:
   
	/// 初期化
    void initialize( 
		const LVector4 &	clearColor,
        lnFloat             clearDepth,
        lnRefString         description,
        lnRefString         defaultEffectStrings,
        Graphics::Texture*	renderTarget );

	/// レンダリングターゲットクリアする色の設定
    void setClearColor( const LVector4& color_ );

	/// レンダリングターゲットクリアする色の取得
    const LColorF& getClearColor() const { return mClearColor; }

	/// 深度ターゲットクリアするZ値の設定
    void setClearDepth( lnFloat depth_ ) { mClearDepth = depth_; }

	/// 深度ターゲットクリアするZ値の取得
    lnFloat getClearDepth() const { return mClearDepth; }

	/// エフェクトファイルの説明文の設定
    void setDescription( LRefTString desc_ ) { mDescription = desc_; }

	/// エフェクトファイルの説明文の取得
    LRefTString getDescription() const { return mDescription; }

	/// エフェクトファイルの割り当て方法の設定 (DefaultEffect アノテーション)
    void setDefaultEffectStrings( LRefTString effects_ ) { mDefaultEffectStrings = effects_; }

	/// エフェクトファイルの割り当て方法の取得
	//DefaultEffectStrings getDefaultEffectStrings() { return mDefaultEffectStrings; }
	
	LNPriorityParameter* tryGetPriorityParameter( MMESceneObjectBase* obj );

public:
	void setOffscreenSceneID( int id ) { mOffscreenSceneID = id; }
	int getOffscreenSceneID() const { return mOffscreenSceneID; }

	/// 描画
	void render( MMESceneBase* scene, ContextDrawParam& param, MMESceneObjectBase* renderRootNode );

private:

	/// エフェクトファイルの割り当て更新
	//      必要なシェーダがまだ Manager に登録されていない場合は新しく作成する。
	//      これだけなら初期化のときにすればいいと思ったけど、
	//      このクラスの initialzie() はシェーダの初期化から呼ばれる。
	//      ということは、Manager::createShader() が再帰的に呼ばれることになってしまい、
	//      なんか不安。(実際、createShader() では FileManager の排他処理を行ってる)
	//      そんなわけでちょっと面倒かもだけどシェーダを作り終わった後、
	//      初期化後処理っていう形で検索・作成をしていく。
	//
	//      [12/8/12] initialize() は SceneShader::refreshResource() から呼び出すようにしたので、
	//      別途これを呼ぶ必要は今のところなし。
    void _refreshEffects();

	// オフスクリーン RT 内で作成したシェーダを解放する
    void _releaseEffects();

private:

    enum EntryType
    {
        ENTRY_EFFECT = 0,   ///< エフェクトファイル
        ENTRY_NONE,         ///< エフェクトなし
        ENTRY_HIDE          ///< 非表示
    };

    struct DefaultEffectEntry
    {
        //EntryType       Type;
        LRefTString     ObjectName;     ///< (ワイルドカード指定可能 * ?)
        LRefTString     EffectName;     ///< (ワイルドカード指定不可)
        //SceneShader*    Shader;         ///< EffectName で作成したシェーダ
		LNPriorityParameter	PriorityParameter;
    };

	//struct SceneNodeEntry
	//{
	//    MMESceneObjectBase*		Node;				///< 描画するノード
	//    LNPriorityParameter		PriorityParameter;  ///< 描画コマンドに渡す
	//};

private:

    typedef std::vector< DefaultEffectEntry >   DefaultEffectEntryList;
	//typedef std::vector< SceneNodeEntry >       SceneNodeEntryList;
private:

	SceneShaderManager*     mManager;
    SceneShader*            mSceneShader;
    LColorF					mClearColor;
    lnFloat                 mClearDepth;
    lnRefString             mDescription;
    lnString				mDefaultEffectStrings;
    Graphics::Texture*		mRTTexture;
	Graphics::Texture*		mDepthTexture;
	int						mOffscreenSceneID;
	MMESceneObjectBase*		mOffscreenOwner;				

	//SceneNodeEntryList      mSceneNodeEntryArray;			///< このオフスクリーンRTで描画するノードの一覧
	DefaultEffectEntryList  mDefaultEffectEntryList;
    LRefTString             mErrorMessage;

	bool                    mIsThereSelf;					///< string DefaultEffect = ～ に "self=～" を持つかどうか
    bool                    mNeedRefreshAffectAllNodes;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================