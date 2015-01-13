//==============================================================================
// SceneGraph 
//------------------------------------------------------------------------------
///**
//  @file       SceneGraph.h
//  @brief      SceneGraph
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <algorithm>
#include "../Base/ReferenceObjectArray.h"
#include "../Base/STLUtils.h"
#include "../Math/Vector2.h"
#include "../System/Interface.h"
#include "../Game/GameTime.h"
#include "../Graphics/Manager.h"
#include "../Geometry/Geometry.h"
#include "Common.h"
#include "DrawCommand.h"
#include "SceneNode/Viewport.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// SceneGraph
//------------------------------------------------------------------------------
///**
//  @brief      シーングラフ
//*/
//==============================================================================
class SceneGraph
    : Base::ReferenceObject
{
public:

    struct InitData
    {
        Base::LogFile*      LogFile;
        FileIO::Manager*      FileManager;
        Graphics::Manager*  GraphicsManager;
        Effect::Manager*    EffectManager;
        Physics::Manager*   PhysicsManager;
        LVector2            VirtualViewSize;
        lnU32               ShaderCacheSize;
    };

public:
    SceneGraph( Manager* manager_ );
    virtual ~SceneGraph();
    LN_REFOBJ_METHODS;

public:

	/// 初期化
    LNRESULT initialize( const InitData& init_data_ );

	/// 終了処理
    void finalize();

	/// SceneNode 初期化時に設定されるレンダリングステートの設定
	void setNodeDefaultRenderState(const Graphics::LRenderState& state_) { mNodeDefaultRenderState = state_; }

	/// SceneNode 初期化時に設定されるレンダリングステートの取得
	const Graphics::LRenderState& getNodeDefaultRenderState() { return mNodeDefaultRenderState; }

	/// シェーダ管理クラスの取得
    SceneShaderManager* getSceneShaderManager() const { return mSceneShaderManager; }

	/// デフォルトで作成されるノードの取得
    Viewport* getDefaultNode( LNDefaultSceneNode node_idx_ ) { return mDefaultSceneNodes[ node_idx_ ]; }

	/// デフォルトで作成されるカメラの取得 (0:3D 1:2D)
    Camera* getDefaultCamera( int type_ = 0 );

	/// デフォルトで作成されるライトの取得
    Light* getDefaultLight() { return mDefaultLight; }

    
	/// デフォルトで作成されるシェーダプログラムの取得
    SceneShader* getDefaultShader( LNDefaultSceneShader shader_idx_ ) { return mDefaultShaderArray[ shader_idx_ ]; }

	/// メインカメラの取得 (シェーダのテクスチャ作成時等、SceneGraph 以下のビューサイズの基準となる)
	Camera* getMainCamera() { return mMainCamera; }

	/// 物理直前更新
	void updateBeforePhysics();

	//----------------------------------------------------------------------
	///**
	//  @brief      フレーム更新 (makeDrawCommand() の前に一度呼ぶ) 
	//
	//  @note       
	//              [同期フェーズ]
	//              シーン内で、描画ノード、カメラに依存しない
	//              グローバルなパラメータを最新の状態に更新する。<br>
	//              具体的には、<br>
	//                  - デフォルトライトの状態を全シェーダにセット
	//                  - 時間、マウス情報を全シェーダにセット
	//                  - すべてのノードのワールド行列の階層更新
	//              <br>
	//              複数カメラを使うとき (複数プレイヤー用のビューなど)、
	//              1フレーム中に重複して発生する処理をまとめた感じ。
	//              この関数は 1 フレームに 1 度だけ呼び出せば OK。
	//*/
	//----------------------------------------------------------------------
	void updateFrame(float elapsedTime/* const Game::GameTime& gameTime */);

	/// 物理直後更新
	void updateAfterPhysics();

	/// [同期フェーズ][描画が必要な時]
    void updateAllContext();

	/// フレーム更新の後処理。
	/// メインスレッドから、描画スキップの有無に関係なく必ず毎フレームよばれる。
	void postFrameUpdate();


	SceneGraphContext* getSceneGraphContext() { return mSceneGraphContext; }

public:
	void setBaseViewSize( const LVector2& size );
    void addCamera( Camera* camera );
    void addLight( Light* light );
    bool onEvent( const System::EventArgs& e );

	Manager* getManager() { return mManager; }
    FileIO::Manager* getFileManager() { return mFileManager; }
    Graphics::Manager* getGraphicsManager() { return mGraphicsManager; }
	//Graphics::GeometryRenderer* getGeometryRenderer() { return mGeometryRenderer; }
    Effect::Manager* getEffectManager() { return mEffectManager; }
    Physics::Manager* getPhysicsManager() { return mPhysicsManager; }

    void addNode( SceneNode* node_ );
    void onDeleteNode( SceneNode* node_ );//removeNode( SceneNode* node_ );

	/// (Camera のコンストラクタから呼ばれる)
	//void addCamera( Camera* camera_ ) { mCameraList.push_back( camera_ ); }

	/// (Camera のデストラクタから呼ばれる)
	//void removeCamera( Camera* camera_ ) { Base::STLUtil::remove( mCameraList, camera_ ); }

	/// (Light のコンストラクタから呼ばれる)
	//void addLight( Light* light_ ) { mLightList.push_back( light_ ); }

	/// (Light のデストラクタから呼ばれる)
	//void removeLight( Light* light_ ) { Base::STLUtil::remove( mLightList, light_ ); }

	/// (Viewport のコンストラクタから呼ばれる)
    void addViewport( Viewport* vp_ ) { mViewportList.push_back( vp_ ); }

	/// (Viewport のデストラクタから呼ばれる)
    void removeViewport( Viewport* vp_ ) { mViewportList.remove( vp_ ); }

    

	/// SceneNode の名前を変更する直前に呼び出す (SceneNode の setName() から呼ばれる)
    void onSetNodeName( SceneNode* node_, const lnChar* old_name_, const lnChar* new_name_ );

	/// 有効ライトの中からライトを列挙する 
	//      ref_pos_ : 基準位置
	//      makeDrawCommand() 内の SceneNode::updateParamFromCameraAndLight() 経由で呼ばれる。
	//      ライトは out_->size() の数だけ格納する。あらかじめ resize しておくこと。
    void enumLights( LightArray* out_, const LVector3& ref_pos_ );

	/// SceneShader のデストラクタから呼ばれる
    ViewportList& getViewportList() { return mViewportList; }

	/// SceneNode 一覧の取得
	SceneNodeArray& getSceneNodeArray() { return mSceneNodeList2.getObjectArray(); }

private:

    struct ActiveLightData
    {
        Light*      ActiveLight;
        lnFloat     DistanceToCamera;   ///< アクティブなカメラからの距離の2乗
        lnFloat     Distance;           ///< ソート用の基準距離 (ソート直前に設定する。アクティブなカメラからの距離の2乗)
    };

    typedef std::vector< ActiveLightData > ActiveLightDataArray;
    

	/// ライト列挙用の比較関数
    static bool _cmpLights( const ActiveLightData& l_, const ActiveLightData& r_ );

private:

	typedef Base::ReferenceObjectArray<SceneNode*>	SceneNodeObjectArray;
	typedef Base::ReferenceObjectArray<Camera*>	CameraObjectArray;
	typedef Base::ReferenceObjectArray<Light*>	LightObjectArray;

    Base::LogFile*          mLogFile;
    Manager*                mManager;
    FileIO::Manager*          mFileManager;
    Graphics::Manager*      mGraphicsManager;
    Graphics::GeometryRenderer* mGeometryRenderer;
    Effect::Manager*        mEffectManager;
    Physics::Manager*       mPhysicsManager;
	//LVector2                mVirtualViewSize;
	Graphics::LRenderState			mNodeDefaultRenderState;
    SceneShaderManager*     mSceneShaderManager;
	//Game::GameTime			mLastUpdateTime;
	double					mTotalTime;		// context
	float					mElapsedTime;	// context

	SceneNodeObjectArray	mSceneNodeList2;
	//SceneNodeList           mNodeRegistOrderList;
	//SceneNodeArray          mSceneNodeList;     ///< 作成されている SceneObject のポインタリスト
    SceneNodeNameMap        mNodeNameMap;       ///< 作成されている SceneObject のうち、名前を持つものの対応表
	//SceneNodeArray          mSceneNodeAdditionalList;

	//CameraList              mCameraList;
	CameraObjectArray		mCameraObjectArray;
	LightObjectArray		mLightObjectArray;
	//LightList               mLightList;
    ViewportList            mViewportList;

    Viewport*               mDefaultSceneNodes[ LN_MAX_DEFAULT_NODES ];
    Camera*                 mDefaultCamera;
    Camera*                 mDefault2DCamera;
    Light*                  mDefaultLight;
    SceneShader*            mDefaultShaderArray[ LN_MAX_DEFAULT_SHADERS ];
	Camera*					mMainCamera;
	//Pane*                   mDefault3DPane;
	//Pane*                   mDefault2DPane;
	//Pane*					mDefaultPane;
    Camera*                 mActiveCamera;
    Camera*                 mActiveViewFrustum;
    ActiveLightDataArray    mActiveLightDataArray;  ///< 有効なライトの一覧 (makeDrawCommand() で作られる)

	//EffectBatchProxy*       mEffect3DBatchProxy;
	//EffectBatchProxy*       mEffect2DBatchProxy;
	//PhysicsDebugDrawProxy*  mPhysicsDebugDrawProxy;

    SceneGraphContext*      mSceneGraphContext;
    
    friend class SceneGraphContext;
};

//==============================================================================
// ■ SceneGraphContext
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class SceneGraphContext
	: public MMESceneBase//
{
public:

    SceneGraphContext();

public:

    void initialize( SceneGraph* scene_ );

    void updateContext();

    void selectLights( MMELightArray* out, const LVector3& ref_pos );

	void render( ContextDrawParam& param );

	//void OnSetActivePaneContext( PaneContext* pane_context_, bool is_3d_ );

protected:
    virtual MMELightBase* getDefaultMMELight();
	virtual MMESceneObjectArray& getMMESceneObjectArray() { return mMMESceneObjectArray; }
	virtual MMESceneObjectBase* findMMESceneObject( const lnChar* name );

private:

    static bool _cmpLights( const LightContext* l_, const LightContext* r_ );

private:

    SceneGraph*             mSceneGraph;
    LVector2                mVirtualViewSize;
	MMESceneObjectArray		mMMESceneObjectArray;
	SceneNodeNameMap		mSceneNodeNameMap;
    CameraContextArray      mCameraContextArray;    // カメラやライトの AdditionalList を Scene に作れば必要ないけど…
    LightContextArray       mLightContextArray;
    LightContextArray       mActiveLightContextArray;   ///< OnSetActivePaneContext() での視錐台カリングで格納
    LViewFrustum            mActiveViewFrustum;         ///< OnSetActivePaneContext() での視錐台カリングで格納
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================