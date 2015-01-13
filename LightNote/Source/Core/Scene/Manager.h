//==============================================================================
// Manager 
//------------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//*/
//==============================================================================

#pragma once

#include "../Base/STLUtils.h"
#include "../Math/Vector2.h"
#include "../Threading/EventFlag.h"
#include "../System/Interface.h"
#include "../Game/GameTime.h"
#include "../Graphics/Common/ProfilerRenderer.h"
#include "Common.h"
#include "Pane.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// Manager
//------------------------------------------------------------------------------
///**
//  @brief		シーンの管理クラス
//*/
//==============================================================================
class Manager
    : public Base::ReferenceObject
	, public System::IEventListener
{
public:

	/// 初期化データ
	struct ConfigData
	{
		System::Manager*	SystemManager;
        FileIO::Manager*	FileManager;
        Graphics::Manager*  GraphicsManager;
        Effect::Manager*    EffectManager;      ///< デフォルトのシーンに設定される EffectManage
        Physics::Manager*   PhysicsManager;     ///< デバッグ描画用
        LVector2            VirtualViewSize;    ///< カメラの2Dプロジェクション、3D→2Dの座標変換、シェーダのVIEWPIXELSIZE
        lnU32               ShaderCacheSize;
		float				AutoUpdateTimeScale;
        bool                EnableDrawThread;

		Base::Profiler*		Profiler;
    };

public:
    Manager();
	virtual ~Manager();

public:

	/// 初期化
    void initialize( const ConfigData& configData );

	/// 終了処理
    void finalize();

	/// デフォルトのシーンを取得する
	SceneGraph* getDefaultSceneGraph() { return mDefaultScene; }
	Pane* getDefaultPane() { return mDefaultPane; }

	/// デフォルトの Pane を取得する
	//Pane* getDefaultPane() { return mDefaultPane; }

	// TODO: この辺デフォルトシーンのみの更新だけど、AutoUpdate 用意して、全更新でいいと思う

	/// 物理直前更新
	void updateBeforePhysics();

	/// デフォルトシーンのフレーム更新
    void updateDefaultScene( const Game::GameTime& gameTime );
	
	/// 物理直後更新
	void updateAfterPhysics();

	/// [描画前同期タスク] 視錐台更新、描画コマンド構築など
    void updateDefaultSceneRenderContent();

	/// [描画タスク] デフォルトのシーンを描画する
    void renderDefaultScene(Graphics::Texture* renderTarget);

	void executeGC();

	/// メッセージ処理 (Core::Manager から呼ばれる)
    virtual bool onEvent( const System::EventArgs& e );

	/// フレーム更新の後処理。
	/// メインスレッドから、描画スキップの有無に関係なく必ず毎フレームよばれる。
	void postFrameUpdate();

public:
	Graphics::Manager*	getGraphicsManager() { return mGraphicsManager; }
	void setBaseViewSize( const LVector2& size );
	void addImageFilter( ImageFilter* filter );

private:
	System::Manager*			mSystemManager;
    Graphics::Manager*			mGraphicsManager;
    Effect::Manager*			mEffectManager;     ///< マルチコア最適化の処理のため。時間があるとき、ちゃんと TaskManager で実装したい
    SceneGraph*					mDefaultScene;
	Pane*						mDefaultPane;
	DrawingLayer*				mDefault3DLayer;
	DrawingLayer*				mDefault2DLayer;
	float						mAutoUpdateTimeScale;

	ImageFilterArray			mImageFilterArray;	///< 別スレッドでイテレートされることはないはずなので普通の vector

	Graphics::ProfilerRenderer	mProfilerRenderer;
};

} // namespace Scene
} // namespace Core
} // namespace LNote
