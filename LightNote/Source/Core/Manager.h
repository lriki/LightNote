//==============================================================================
// Manager 
//------------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//*/
//==============================================================================

#pragma once

#include "ConfigData.h"
#include "Base/Profiler.h"
#include "System/Interface.h"
#include "Game/FPSController.h"
#include "Game/GameScene.h"
#include "Task/ITaskExecuteHandler.h"
#include "Task/ParallelTask.h"

namespace LNote
{
namespace Core
{

//==============================================================================
// Manager
//------------------------------------------------------------------------------
///**
//  @brief		ライブラリ全体の管理クラス
//*/
//==============================================================================
class Manager
    : public Base::ReferenceObject
    , public System::IEventListener
    , public Task::ITaskExecuteHandler
{
public:
    Manager();
	virtual ~Manager();

public:

	/// 初期化
    void initialize( const ConfigData& configData );

	/// 終了処理
    void finalize();

	/// 共通機能の初期化
	void initializeBaseModule( const ConfigData& configData );

	/// システム管理クラスの初期化
    void initializeSystemManager( const ConfigData& configData );

	/// ファイル管理クラスの初期化
    void initializeFileManager( const ConfigData& configData );

	/// 入力管理クラスの初期化
    void initializeInputManager( const ConfigData& configData );

	/// オーディオ管理クラスの初期化
    void initializeAudioManager( const ConfigData& configData );

	/// 物理演算管理クラスの初期化
    void initializePhysicsManager( const ConfigData& configData );
    
	/// グラフィックス管理クラスの初期化
    void initializeGraphicsManager( const ConfigData& configData );

	/// エフェクト管理クラスの初期化
    void initializeEffectManager( const ConfigData& configData );

	/// GUI 管理クラスの初期化
    void initializeGUIManager( const ConfigData& configData );

	/// シーン管理クラスの初期化
    void initializeSceneManager( const ConfigData& configData );

	/// FPSController の作成
	void createFPSController( const ConfigData& configData );

	/// システム管理クラスの取得
    System::Manager* getSystemManager() { return mSystemManager; }

	/// ファイル管理クラスの取得
    FileIO::Manager* getFileManager() { return mFileManager; }

	/// 入力管理クラスの取得
    Input::Manager* getInputManager() { return mInputManager; }

	/// オーディオ管理クラスの取得
    Audio::Manager* getAudioManager() { return mAudioManager; }

	/// 物理演算管理クラスの取得
    Physics::Manager* getPhysicsManager() { return mPhysicsManager; }

	/// グラフィックス管理クラスの取得
    Graphics::Manager* getGraphicsManager() { return mGraphicsManager; }

	/// エフェクト管理クラスの取得
    Effect::Manager* getEffectManager() { return mEffectManager; }

	/// シーン管理クラスの取得
    Scene::Manager* getSceneManager() { return mSceneManager; }

	/// GUI 管理クラスの取得
    GUI::Manager* getGUIManager() { return mGUIManager; }

	/// ゲームシーン管理クラスの取得
	//Game::Manager* getGameManager() { return mGameManager; }
    

	/// ゲームタイムの取得
    const Game::GameTime& getGameTime() const { return mFPSController->getGameTime(); }

	/// FPS 操作クラスの取得
    Game::FPSController* getFPSController() { return mFPSController; }

	// ライブラリ全体に終了を通知する
    void exit() { mEndRequested = true; }

	bool isEndRequested() const { return mEndRequested; }

	/// フレーム更新
    bool update();

	/// エディタ、あるいは組み込み用のフレーム更新
    void updateAsLibrary();

	/// 時間がかかる処理の後に呼び出すことで、FPS 処理、ゲームタイムへの影響をリセットする
    void resetDelayTime();

	/// デバイスリセットが必要であればonLostDevice() → リセット onResetDevice() と呼び出す (同期フェーズで実行。Managed Wrapper で使うので public 化)
    void checkDeviceState();

	/// [テスト用]描画が完了するまで待つ
	void waitRenderEnd();

public:

	/// メッセージ処理コールバック
    virtual bool onEvent( const System::EventArgs& e );

	/// メインスレッドのフレーム待ち処理を行う
    virtual void OnFrameWait();

	/// 描画情報の構築を行う
    virtual void OnUpdateDrawContents();

private:

	/// デバイスリセット前処理
	//		_onLostDevice()、_onResetDevice() は Graphics 以外に発生を伝えるための処理。
    void _onLostDevice();

	/// デバイスリセット後処理
    void _onResetDevice();

	/// 同期フェーズの処理
    void _onUpdateSyncPhase();

	/// 描画 (描画スレッドから呼ばれる)
	void _render();

	/// 物理更新 (物理更新スレッドから呼ばれる)
	void _updatePhysics();

private:
    ConfigData				mConfigData;        ///< 初期化情報

    Task::Manager*			mTaskManager;
    System::Manager*		mSystemManager;
    FileIO::Manager*		mFileManager;
    Input::Manager*			mInputManager;
    Audio::Manager*			mAudioManager;
    Physics::Manager*		mPhysicsManager;
    Graphics::Manager*		mGraphicsManager;
    Effect::Manager*		mEffectManager;
    Scene::Manager*			mSceneManager;
    GUI::Manager*			mGUIManager;
    Game::Manager*			mGameManager;

    Game::FPSController*    mFPSController;
	Task::ParallelTask*		mRenderThread;			///< 描画スレッド
	Task::ParallelTask*		mPhysicsUpdateThread;	///< 別スレッド更新が有効なら NULl ではない

    LNUpdateMode			mUpdateMode;
    bool					mInitialized;
	bool					mBaseModuleInitialized;
    bool					mEndRequested;
	bool					mDelayedPhysics;
};

} // namespace Core
} // namespace LNote
