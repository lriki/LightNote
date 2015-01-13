//==============================================================================
// Manager 
//==============================================================================

#include "stdafx.h"
#include <locale.h>
#include "Resource/LocaleManager.h"
#include "Threading/Thread.h"
#include "Task/Manager.h"
#include "System/Manager.h"
#include "System/Environment.h"
#include "FileIO/Manager.h"
#include "Input/Manager.h"
#include "Audio/Manager.h"
#include "Physics/Manager.h"
#include "Graphics/Manager.h"
#include "Effect/Manager.h"
#include "Scene/Manager.h"
#include "Scene/SceneGraph.h"
#include "Scene/SceneNode/Camera.h"
#include "Scene/Pane.h"
#include "GUI/Manager.h"
#include "Game/GameSceneManager.h"
#include "Manager.h"

namespace LNote
{
namespace Core
{

//==============================================================================
// Manager
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Manager::Manager()
        : mTaskManager				( NULL )
        , mSystemManager			( NULL )
        , mFileManager				( NULL )
        , mInputManager				( NULL )
        , mAudioManager				( NULL )
        , mPhysicsManager			( NULL )
        , mGraphicsManager			( NULL )
        , mEffectManager			( NULL )
        , mSceneManager				( NULL )
        , mGUIManager				( NULL )
        , mGameManager				( NULL )
        , mFPSController			( NULL )
		, mRenderThread				( NULL )
		, mPhysicsUpdateThread		( NULL )
        , mUpdateMode				( LN_UPDATEMODE_ACTIVE_ONLY )
        , mInitialized				( false )
		, mBaseModuleInitialized	( false )
        , mEndRequested				( false )
		, mDelayedPhysics			( false )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Manager::~Manager()
    {
        finalize();

        SAFE_DELETE( mFPSController );
        LN_SAFE_RELEASE( mSceneManager );
        LN_SAFE_RELEASE( mGUIManager );
        LN_SAFE_RELEASE( mEffectManager );
		LN_SAFE_RELEASE( mPhysicsManager );
        LN_SAFE_RELEASE( mAudioManager );
        LN_SAFE_RELEASE( mFileManager );
        LN_SAFE_RELEASE( mInputManager );
        LN_SAFE_RELEASE( mSystemManager );
        LN_SAFE_RELEASE( mGraphicsManager );
        
//        Base::Error::finalize();
        Base::ExceptionManager::finalize();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::initialize( const ConfigData& configData )
    {
        if ( mInitialized ) {
			return;
		}

		// Main スレッド登録
		Base::Profiler::Instance.registerThread(
			Base::Profiler::MainThread_ID, _T("Main"));
		Base::Profiler::Instance.setBaseFrameRate(Base::Profiler::MainThread_ID, configData.FrameRate);
		Base::Profiler::Instance.registerSection(
			Base::Profiler::MainThread_ID, Base::Profiler::MainThread_UpdateSection, _T("Update"));
		Base::Profiler::Instance.registerSection(
			Base::Profiler::MainThread_ID, Base::Profiler::MainThread_PrepareRendering, _T("PreRender"));

		// Render スレッド登録
		Base::Profiler::Instance.registerThread(
			Base::Profiler::RenderThread_ID, Base::Profiler::RenderThread_Name);
		Base::Profiler::Instance.setBaseFrameRate(Base::Profiler::RenderThread_ID, configData.FrameRate);
		Base::Profiler::Instance.registerSection(
			Base::Profiler::RenderThread_ID, Base::Profiler::RenderThread_Section0, _T("Render"));
		Base::Profiler::Instance.registerSection(
			Base::Profiler::RenderThread_ID, Base::Profiler::RenderThread_Section_Present, _T("Present"));


		createFPSController( configData );

		initializeBaseModule( configData );
		initializeSystemManager( configData );
		initializeFileManager( configData );
		initializeInputManager( configData );
		initializeAudioManager( configData );
		initializeGraphicsManager( configData );
		initializePhysicsManager( configData );
		initializeEffectManager( configData );
		initializeGUIManager( configData );
		initializeSceneManager( configData );

		mFPSController->refreshSystemDelay( System::Environment::getSystemTime() );
		mInitialized = true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::finalize()
    {
		// 描画スレッド停止
		if ( mRenderThread != NULL )
        {
            mRenderThread->endRequest();
            mRenderThread->wait();
            SAFE_DELETE( mRenderThread );
        }
		// 物理更新スレッド停止
		if ( mPhysicsUpdateThread != NULL )
        {
			mPhysicsUpdateThread->endRequest();
            mPhysicsUpdateThread->wait();
			SAFE_DELETE( mPhysicsUpdateThread );
        }

		// ロードスレッドの終了は最初に待つ必要がある
		// (こうしないと、こうしないと、Graphics::finalize() 後にテクスチャ生成、等が起こる)
        if ( mFileManager ) {
            mFileManager->finalize();
        }

        if ( mSceneManager ) {
            mSceneManager->finalize();
        }
        if ( mEffectManager ) {
            mEffectManager->finalize();
        }
        if ( mGUIManager ) {
            mGUIManager->finalize();
        }
        if ( mPhysicsManager ) {
            mPhysicsManager->finalize();
        }
        if ( mAudioManager ) {
            mAudioManager->finalize();
        }
        if ( mSystemManager ) {
			if ( mSystemManager->getMainWindow() ) {
				mSystemManager->getMainWindow()->detachEventListener( this );
			}
            mSystemManager->finalize();
//			Base::Error2::setParentWindow( NULL );
        }
        if ( mGraphicsManager ) {
            mGraphicsManager->finalize();
        }
        if ( mInputManager ) {
            mInputManager->finalize();
        }

//        Base::Error::setErrorObserver( NULL );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::initializeBaseModule( const ConfigData& configData )
	{
		if ( !mBaseModuleInitialized )
		{
			mConfigData = configData;
			mUpdateMode = configData.UpdateMode;

			setlocale( LC_ALL, "" );
			Resource::LocaleManager::initialize();

//			Base::Error::initialize();
        
			if ( configData.EnableMakeLogFile )
			{
				Base::Logger::initialize( _T( "lnlog.txt" ), configData.LogSaveDir );
				System::Environment::outputSystemInfo();
#ifdef LNOTE_ENABLE_CRT_DEBUG
				if ( mConfigData.EnableCRTSetDebugFlags ) {
					_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
				}
#else
				Base::MemoryManager::attachLogger();
#endif
			}

			Base::ExceptionManager::initialize();
			mBaseModuleInitialized = true;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initializeSystemManager( const ConfigData& configData )
    {
		if ( mSystemManager == NULL )
		{
			System::Manager::SettingData data;
			data.SystemAPI				= configData.WindowSystemAPI;
			data.UseConsole				= configData.EnableConsole;
			data.UseThread				= ( mUpdateMode == LN_UPDATEMODE_THREADED );
			data.WindowTitleText		= configData.WindowTitle;
			data.WindowWidth			= configData.WindowWidth;
			data.WindowHeight			= configData.WindowHeight;
			data.Windowed				= !configData.EnableFullscreen;
			data.WindowResizeEnabled	= configData.WindowResizeEnabled;
			data.WindowHandle			= configData.WindowHandle;
#if defined(LNOTE_ANDROID)
			data.AndroidApp				= mConfigData.AndroidApp;
#endif
			mSystemManager  = LN_NEW System::Manager();
			mSystemManager->initialize( data );

			mSystemManager->getMainWindow()->attachEventListener( this, LN_DEFAULT_EL_PRIORITY_FRAMEWORK );

//			Base::Error2::setParentWindow( mSystemManager->getMainWindow() );
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initializeFileManager( const ConfigData& configData )
    {
		if ( mFileManager == NULL )
		{
			initializeBaseModule( configData );

			FileIO::Manager::ConfigData data;
			data.ArchiveList    = mConfigData.ArchiveList;
			data.AccessMode     = mConfigData.FileAccessMode;
			mFileManager = LN_NEW FileIO::Manager();
			mFileManager->initialize( data );
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initializeInputManager( const ConfigData& configData )
    {
        if ( mInputManager == NULL )
		{
			initializeSystemManager( configData );

			Input::Manager::ConfigData data;
			data.Window     = mSystemManager->getMainWindow();

			mInputManager = LN_NEW Input::Manager();
			mInputManager->initialize( data );
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initializeAudioManager( const ConfigData& configData )
    {
		if ( mAudioManager == NULL )
		{
			initializeBaseModule( configData );
			initializeFileManager( configData );

			// ユーザー定義のウィンドウハンドルが設定されていて、システム管理クラスが未初期化の場合
			if ( configData.WindowHandle && !mSystemManager )
			{
				initializeSystemManager( configData );
			}

			Audio::Manager::ConfigData data;
			data.FileIOManager          = mFileManager;
			data.AudioSourceCacheSize   = configData.SoundCacheSize;
			data.DMInitMode             = configData.DMInitMode;
			data.Window                 = ( mSystemManager ) ? mSystemManager->getMainWindow() : NULL;

			mAudioManager = LN_NEW Audio::Manager();
			mAudioManager->initialize( data );

			mAudioManager->setAutoPlayTypeSelectThreshold(configData.SoundPlayTypeThreshold);
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initializePhysicsManager( const ConfigData& configData )
    {
		if ( mPhysicsManager == NULL )
		{
			initializeBaseModule( configData );

			Physics::Manager::InitData data;
			data.LogFile            = NULL;

			mPhysicsManager = LN_NEW Physics::Manager();
			mPhysicsManager->initialize( data );

			// 更新スレッド
			if ( mConfigData.EnablePhysicsThread )
			{
				mPhysicsUpdateThread = LN_NEW Task::ParallelTask();
				mPhysicsUpdateThread->setUpdateMethod( 
					Base::createDelegate( this, &Manager::_updatePhysics ) );
				mPhysicsUpdateThread->start();
				mPhysicsUpdateThread->waitForInitialized();
			}
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initializeGraphicsManager( const ConfigData& configData )
    {
		if ( mGraphicsManager == NULL )
		{
			initializeBaseModule( configData );
			initializeFileManager( configData );
			initializeSystemManager( configData );
			initializePhysicsManager( configData );

			Graphics::Manager::ConfigData data;
			data.FileIOManager			= mFileManager;
			data.PhysicsManager			= mPhysicsManager;
			data.SystemManager			= mSystemManager;
			data.BackbufferSize			= configData.BackbufferSize;
			data.BackbufferResizeMode	= configData.BackbufferResizeMode;
			data.EnableFPUPreserve		= configData.EnableFPUPreserve;
			data.FontFilenameList		= configData.FontFilenameList;
			data.GraphicsAPI			= configData.GraphicsAPI;
			data.TextureCacheSize		= configData.TextureCacheSize;
			data.ShaderCacheSize		= configData.ShaderCacheSize;
			data.MaxSprite2DCount		= 1024;
			data.MaxSprite3DCount		= 1024;

			if (data.BackbufferSize.w <= 0 && data.BackbufferSize.h <= 0)
				data.BackbufferSize.set(configData.WindowWidth, configData.WindowHeight);

			mGraphicsManager = LN_NEW Graphics::Manager();
			mGraphicsManager->initialize( data );
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initializeEffectManager( const ConfigData& configData )
    {
		if ( mEffectManager == NULL )
		{
			initializeFileManager( configData );
			initializeAudioManager( configData );
			initializeGraphicsManager( configData );

			Effect::Manager::ConfigData data;
			data.FileManager        = mFileManager;
			data.GraphicsManager    = mGraphicsManager;
			data.AudioManager		= mAudioManager;

			mEffectManager = LN_NEW Effect::Manager();
			mEffectManager->initialize( data );
		}
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initializeGUIManager( const ConfigData& configData )
	{
        if ( mSceneManager == NULL )
		{
			initializeBaseModule( configData );
			initializeSystemManager( configData );
			initializeFileManager( configData );
			initializeGraphicsManager( configData );

			GUI::Manager::ConfigData data;
			data.FileManager = mFileManager;
			data.MainWindow = mSystemManager->getMainWindow();
			data.GraphicsManager = mGraphicsManager;

			mGUIManager = LN_NEW GUI::Manager();
			mGUIManager->initialize( data );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initializeSceneManager( const ConfigData& configData )
    {
        if ( mSceneManager == NULL )
		{
			initializeBaseModule( configData );
			initializeSystemManager( configData );
			initializeFileManager( configData );
			initializeGraphicsManager( configData );
			initializeEffectManager( configData );
			initializePhysicsManager( configData );

			float timeScale = configData.AutoUpdateTimeScale;
			if (timeScale == 0.0) {
				timeScale = configData.FrameRate;
			}

			Scene::Manager::ConfigData data;
			data.SystemManager			= mSystemManager;
			data.FileManager			= mFileManager;
			data.GraphicsManager		= mGraphicsManager;
			data.EffectManager			= mEffectManager;
			data.PhysicsManager			= mPhysicsManager;
			data.VirtualViewSize		= LVector2( (lnFloat)configData.WindowWidth, (lnFloat)configData.WindowHeight );
			data.ShaderCacheSize		= 32;
			data.AutoUpdateTimeScale	= timeScale;
			data.EnableDrawThread		= false;//true;
			data.Profiler				= &Base::Profiler::Instance;

			mSceneManager = LN_NEW Scene::Manager();
			mSceneManager->initialize( data );

			// TODO:
			if ( 1 )
			{
				// スレッドを開始して初期化が完了するまで待つ
				mRenderThread = LN_NEW Task::ParallelTask();
				mRenderThread->setUpdateMethod( Base::createDelegate( this, &Manager::_render ) );
				mRenderThread->start();
				mRenderThread->waitForInitialized();
			}
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::createFPSController( const ConfigData& configData )
	{
		mFPSController = LN_NEW Game::FPSController();
        mFPSController->setFrameRate( configData.FrameRate );
        mFPSController->setEnableFPSTest( true );
		mFPSController->setEnableFrameWait( configData.EnableFrameWait );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool Manager::update()
    {
		{
			Base::ScopedProfilerSection section(
				Base::Profiler::Instance, Base::Profiler::MainThread_ID, 0);

			//-----------------------------------------------------
			// フレーム更新
			//		この処理は基本的に描画の前に行っておく必要がある。
			//		特にSceneは追加したノードのワールド行列を先に更新する必要がある。

			// FileIO
			if (mFileManager) {
				mFileManager->broadcastAsyncIOProcFinished();
			}

			// Scene 物理前
			if (mSceneManager) {
				mSceneManager->updateBeforePhysics();
			}

			// Physics
			if (mPhysicsManager)
			{
				// 物理演算スレッド有効
				if (mPhysicsUpdateThread != NULL)
				{
					if (mDelayedPhysics)
					{
						// 遅延している
						//mPhysicsManager->updateBefore();	// あるいは無し
					}
					else
					{
						// 更新開始
						mPhysicsManager->updateBefore();
						mPhysicsUpdateThread->enableRunning();
					}
				}
				// 物理演算スレッド無効
				else
				{
					mPhysicsManager->updateBefore();
					mPhysicsManager->updateContents();
				}
			}

			// ▼ここからは物理スレッドとは非同期実行

#if 0
			// Physics
			if ( mPhysicsManager )
			{
				// 物理演算スレッド有効
				if ( mPhysicsUpdateThread != NULL )
				{
					mPhysicsUpdateThread->waitForTaskWait();
					mPhysicsManager->updateContents();		// 物理演算の同期
					mPhysicsUpdateThread->enableRunning();
				}
				// 物理演算スレッド無効
				else
				{
					_updatePhysics();
					mPhysicsManager->updateContents();
				}
			}
#endif


			// Scene
			if (mSceneManager) {
				mSceneManager->updateDefaultScene(mFPSController->getGameTime());
			}

			// GUI
			if (mGUIManager) {
				mGUIManager->updateFrame(mFPSController->getGameTime());
			}

			// ▲ここまで物理スレッドとは非同期実行

			// Physics
			if (mPhysicsManager)
			{
				// 物理演算スレッド有効
				if (mPhysicsUpdateThread != NULL)
				{
					// FPS維持優先。遅れていたら、次の更新をスキップする
					if (0)	// TODO:
					{
						// 処理完了済みである場合
						if (mPhysicsUpdateThread->isWaiting())
						{
							mPhysicsManager->updateContents();
							mDelayedPhysics = false;
						}
						// 遅延が発生している場合
						else
						{
							//mPhysicsManager->updateContents();	// あるいは無し
							mDelayedPhysics = true;
						}
					}
					else
					{
						// 完了するまで待つ
						mPhysicsUpdateThread->waitForTaskWait();
						mPhysicsManager->updateContents();
					}
				}
				// 物理演算スレッド無効
				else
				{
					_updatePhysics();
					mPhysicsManager->updateContents();
				}
			}

			// Scene 物理後
			if (mSceneManager) {
				mSceneManager->updateAfterPhysics();
			}





			//-----------------------------------------------------
			// 描画スレッド
			if (mRenderThread)
			{
				// FPS維持優先。描画が遅れていたら、次の描画をスキップする
				if (1)	// TODO:
				{
					// この if が false の場合はマスターの処理に遅延が発生しているという意味
					if (mRenderThread->isWaiting())
					{
						mRenderThread->waitForTaskWait();   // 止まるまで待つ(isWaiting() でチェックしているが念のため)
						OnUpdateDrawContents();				// 同期処理実行
						mRenderThread->enableRunning();     // 実行開始 (初期化後、初回の実行もここから)
					}
					else {
						// 遅延している場合は何もしない
					}
				}
				else
				{
					mRenderThread->waitForTaskWait();
					OnUpdateDrawContents();
					mRenderThread->enableRunning();
				}
			}

			if (mSceneManager != NULL)
			{
				// フレーム更新後処理は描画スキップにかかわらず必ず呼び出す
				mSceneManager->postFrameUpdate();
			}

		} // ProfilerSection
		
		// フレーム待ち処理
		if ( mFPSController ) {
            mFPSController->process();

			// 1 秒毎のタイミングでプロファイラに設定する
			if (mFPSController->isRoundSec())
			{
				Base::Profiler::Instance.MainFPS = mFPSController->getFPS();
				Base::Profiler::Instance.MainFPSCapacity = mFPSController->getCapacityFPS();
			}
        }

		// ウィンドウとバックバッファサイズをプロファイラへ
		if (mGraphicsManager != NULL)
		{
			Base::Profiler::Instance.WindowSize = mSystemManager->getMainWindow()->getSize();
			Base::Profiler::Instance.BackbufferSize = mGraphicsManager->getGraphicsDevice()->getDefaultCanvas()->getSize();

		}

#ifdef LNOTE_PUBLIC_ONLY

		//-----------------------------------------------------
		// System
		if ( (mConfigData.ClassUpdateFlag & LN_CLASS_SYSTEM) && mSystemManager )
        {
            // ウィンドウがアクティブのときだけ更新する場合でウィンドウが非アクティブの場合は、メッセージ処理だけをして待機する
            if ( mUpdateMode == LN_UPDATEMODE_ACTIVE_ONLY )
            {
                while ( !mSystemManager->getMainWindow()->isActive() )
		        {
			        if ( !mSystemManager->processMessage()/* || mEndRequested*/ )
                    {
                        return false;
                    }
					Threading::Thread::sleep(32);    // 無限ループの負荷軽減
		        }
            }

            // FPS 値をウィンドウタイトルに表示する場合
		    if ( mConfigData.EnableShowFPS && mFPSController )
		    {
			    // 1 秒毎のタイミングで設定する
			    if ( mFPSController->isRoundSec() ) {
                    mSystemManager->getMainWindow()->setWindowTextFooter( _T( " - fps:%.1f cap:%.1f" ), mFPSController->getFPS(), mFPSController->getCapacityFPS() );
                }
		    }

            bool r = mSystemManager->processMessage();

			// Input はメッセージ処理直後に更新した方が、より最新の状態で更新できる
			if ( mInputManager ) {
				mInputManager->update();
			}

			if ( !r ) {
				mEndRequested = true;
			}

            return ( mEndRequested ) ? false : r;
        }
        else
        {
            return !mEndRequested;
        }
#else
        bool r = false;

        //if ( (mConfigData.ClassUpdateFlag & LN_CLASS_EFFECT) && mEffectManager )
        //{
        //    mEffectManager->update();
        //}

		if ( mFileManager ) {
			mFileManager->broadcastAsyncIOProcFinished();
		}

        if ( mGameManager ) mGameManager->update();

        if ( mEffectManager )
        {
            mEffectManager->setGameTime( mFPSController->getGameTime() );
        }

        if ( (mConfigData.ClassUpdateFlag & LN_CLASS_SCENE) && mSceneManager )
        {

            mSceneManager->updateDefaultScene( mFPSController->getGameTime() );

            //mSceneManager->renderDefaultScene();
            /*
            Graphics::IRenderer* r = mGraphicsManager->getGraphicsDevice()->getRenderer();

            r->setRenderTarget( NULL, 0 );
            r->beginScene();
            mSceneManager->renderDefaultScene();
            r->endScene();
            mGraphicsManager->getGraphicsDevice()->getDefaultCanvas()->present();
            */
        }
        
        if ( mPhysicsManager )
        {
            //mPhysicsManager->update( mFPSController->getGameTime().getElapsedGameTime() );
        }
        
        if ( mGUIManager )
        {
            mGUIManager->update( mFPSController->getGameTime() );
        }

        if ( mTaskManager )
        {
            mTaskManager->execute();
        }

        

		

        if ( (mConfigData.ClassUpdateFlag & LN_CLASS_SYSTEM) && mSystemManager )
        {
            // ウィンドウがアクティブのときだけ更新する場合でウィンドウが非アクティブの場合は、メッセージ処理だけをして待機する
            if ( mUpdateMode == LN_UPDATEMODE_ACTIVE_ONLY )
            {
                while ( !mSystemManager->getMainWindow()->isActive() )
		        {
			        if ( !mSystemManager->processMessage() || mEndRequested )
                    {
                        return false;
                    }
                    Threading::sleep( 32 );    // 無限ループの負荷軽減
		        }
            }

            // FPS 値をウィンドウタイトルに表示する場合
		    if ( mConfigData.EnableShowFPS && mFPSController )
		    {
			    // 1 秒毎のタイミングで設定する
			    if ( mFPSController->isRoundSec() )
			    {
                    mSystemManager->getMainWindow()->setWindowTextFooter( _T( " - fps:%.1f cap:%.1f" ), mFPSController->getFPS(), mFPSController->getCapacityFPS() );
                }
		    }

            r = mSystemManager->processMessage();

			if ( (mConfigData.ClassUpdateFlag & LN_CLASS_INPUT) && mInputManager )
			{
				mInputManager->update();
			}

            return ( mEndRequested ) ? false : r;
        }
        else
        {
            return !mEndRequested;
        }
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::resetDelayTime()
    {
        mFPSController->refreshSystemDelay( System::Environment::getSystemTime() );
    }

	//----------------------------------------------------------------------
	// ● メッセージ処理コールバック
	//----------------------------------------------------------------------
    bool Manager::onEvent( const System::EventArgs& e )
    {
#ifdef LNOTE_PUBLIC_ONLY
		switch ( e.Type )
        {
            /////////////////////////////////////////////// Alt + Enter
    //        case LN_EVENT_ALTENTER:
    //        {
    //            System::Window* main_window = mSystemManager->getMainWindow();

    //            //mTaskManager->waitMasterTaskWait();
				////mGraphicsManager->getGraphicsDevice()->pauseDevice();
				////mGraphicsManager->getGraphicsDevice()->resumeDevice();

    //            if ( mConfigData.ToggleFullscreenEnabled )
    //            {
    //                bool b = main_window->isFullScreen();
    //                if ( b ) {
    //                    main_window->setEnableFullScreen( false );
    //                }
    //                else {
    //                    main_window->setEnableFullScreen( true );
    //                }

    //                //if ( mGraphicsManager ) {
    //                //    mGraphicsManager->getGraphicsDevice()->getDefaultCanvas()->setWindowSize( main_window->getSize() );
    //                //}
    //                return true;
    //            }
    //            break;
    //        }

			/////////////////////////////////////////////// キー押下
			case LN_EVENT_KEY_DOWN:
			{
				System::KeyEventArgs* keyArgs = (System::KeyEventArgs*)&e;
				for (int i = 0; i < AcceleratorKeyType_Max; i++)
				{
					AcceleratorKeyEntry& ac = mConfigData.AcceleratorKeys[i];
					if (keyArgs->KeyCode == ac.KeyCode &&
						keyArgs->IsShift == ac.IsShift &&
						keyArgs->IsControl == ac.IsCtrl &&
						keyArgs->IsAlt == ac.IsAlt)
					{
						switch (i)
						{
							//////////////////////////////// フルスクリーン切り替え
							case AcceleratorKeyType_ToggleFullscreen:
							{
								System::Window* main_window = mSystemManager->getMainWindow();
								if (mConfigData.ToggleFullscreenEnabled)
								{
									bool b = main_window->isFullScreen();
									if (b) {
										main_window->setEnableFullScreen(false);
									}
									else {
										main_window->setEnableFullScreen(true);
									}
								}
								return true;
							}
							//////////////////////////////// デバッグ表示切替
							case AcceleratorKeyType_ShowDebugStatistics:
							{
								Base::Profiler::Instance.setEnabled(!Base::Profiler::Instance.isEnabled());
								return true;
							}
						}
					}
				}
				//if (keyArgs->KeyCode == LN_KEY_F3) {
				//	Base::Profiler::Instance.setEnabled(!Base::Profiler::Instance.isEnabled());
				//	return true;
				//}
				//if (keyArgs->KeyCode == LN_KEY_ENTER && keyArgs->IsAlt) {
				//	System::Window* main_window = mSystemManager->getMainWindow();
				//	if (mConfigData.ToggleFullscreenEnabled)
				//	{
				//		bool b = main_window->isFullScreen();
				//		if (b) {
				//			main_window->setEnableFullScreen(false);
				//		}
				//		else {
				//			main_window->setEnableFullScreen(true);
				//		}
				//		return true;
				//	}
				//	return true;
				//}
				
			}

            /////////////////////////////////////////////// ウィンドウがアクティブになった
            case LN_EVENT_APP_ACTIVATE:
            {
#ifdef LNOTE_PUBLIC_ONLY
#else
                if ( mFPSController ) {
                    mFPSController->refreshSystemDelay( System::Environment::getSystemTime() );
                }
#endif
                break;
            }

            /////////////////////////////////////////////// Android の初期化処理
            //case LNEV_ANDROID_INIT_WINDOW:
            //{
            //    
            //    return true;
            //}
#if 0
            /////////////////// Android APP_CMD_PAUSE (デバイスロスト)
            case LN_EVENT_ANDROID_PAUSED:
            {
                _onLostDevice();
                mGraphicsManager->getGraphicsDevice()->pauseDevice();
                return true;
            }

            /////////////////// Android APP_CMD_RESUME (デバイスリセット)
            case LN_EVENT_ANDROID_RESUMED:
            {
				mGraphicsManager->getGraphicsDevice()->resumeDevice();
                _onResetDevice();
                return true;
            }
#endif
        }

		return false;
#else
        LNRESULT lr;

        if ( mInputManager )
        {
            lr = mInputManager->onEvent( e );
            if ( LN_EVENT_HANDLED( lr ) )
            {
                return true;
            }
        }

        if ( mGUIManager )
        {
            if ( mGUIManager->doEvent( e ) ) return true;
        }

        if ( mSceneManager )
        {
            lr = mSceneManager->onEvent( e );
            if ( LN_EVENT_HANDLED( lr ) )
            {
                return true;
            }
        }

        switch ( e.Type )
        {
            /////////////////////////////////////////////// Alt + Enter
            case LN_EVENT_ALTENTER:
            {
                System::IWindow* main_window = mSystemManager->getMainWindow();

                mTaskManager->waitMasterTaskWait();
				mGraphicsManager->getGraphicsDevice()->pauseDevice();
				mGraphicsManager->getGraphicsDevice()->resumeDevice();

                if ( mConfigData.ForbidAutoFullscreen )
                {
                    bool b = main_window->isFullScreen();
                    if ( b )
                    {
                        main_window->setEnableFullScreen( false );
                    }
                    else
                    {
                        main_window->setEnableFullScreen( true );
                    }

                    if ( mGraphicsManager )
                    {
                        mGraphicsManager->getGraphicsDevice()->getDefaultCanvas()->setWindowSize( main_window->getSize() );
                           
                        /*
                        if ( b )
                        {
                            mGraphicsManager->getGraphicsDevice()->getDefaultCanvas()->setEnableFullScreen( false );
                        }
                        else
                        {
                            mGraphicsManager->getGraphicsDevice()->getDefaultCanvas()->setEnableFullScreen( true );
                        }
                        */
                    }

                    //LNEvent ev;
                    //ev.Type = LNEV_DEVICE_LOST;
                    //mSystemManager->postEvent( ev );

                    //ev.Type = LNEV_DEVICE_RESET;
                    //mSystemManager->postEvent( ev );

                    return true;
                }
                break;
            }

            /////////////////////////////////////////////// ウィンドウがアクティブになった
            case LN_EVENT_APP_ACTIVATE:
            {
                if ( mFPSController )
                {
                    mFPSController->refreshSystemDelay( System::Environment::getSystemTime() );
                }
                break;
            }

            /////////////////////////////////////////////// Android の初期化処理
            //case LNEV_ANDROID_INIT_WINDOW:
            //{
            //    
            //    return true;
            //}

            /////////////////// Android APP_CMD_PAUSE (デバイスロスト)
            case LN_EVENT_ANDROID_PAUSED:
            {
                _onLostDevice();
                mGraphicsManager->getGraphicsDevice()->pauseDevice();
                return true;
            }

            /////////////////// Android APP_CMD_RESUME (デバイスリセット)
            case LN_EVENT_ANDROID_RESUMED:
            {
				mGraphicsManager->getGraphicsDevice()->resumeDevice();
                _onResetDevice();
                return true;
            }
        }

        return false;
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::OnFrameWait()
    {
#ifdef LNOTE_PUBLIC_ONLY
#else
        if ( (mConfigData.ClassUpdateFlag & LN_CLASS_FRAMEWORK) && mFPSController )
        {
            mFPSController->process();
        }
        else
        {
            Threading::sleep(16);  // 念のため
        }
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::OnUpdateDrawContents()
    {
		// 描画情報同期のパフォーマンス測定
		Base::ScopedProfilerSection section(
			Base::Profiler::Instance, Base::Profiler::MainThread_ID, Base::Profiler::MainThread_PrepareRendering);


#ifdef LNOTE_PUBLIC_ONLY

		// デバイスロストの確認
        checkDeviceState();

		// 描画情報更新
		if ( mSceneManager ) {
			mSceneManager->updateDefaultSceneRenderContent();
			mSceneManager->executeGC();
		}

		// Effect
		if ( mEffectManager ) {
			mEffectManager->updateRenderContents();
		}

		// GUI
		if ( mGUIManager ) {
			mGUIManager->updateRenderContents();
		}

		// デバイスロストの確認
        checkDeviceState();

		//printf("----\n");
		//Base::Profiler t = Base::Profiler::Instance;
		//for (int i = 0; i < t.getRegisterdThreadCount(); ++i)
		//{
		//	_p(t.getRegisterdThread(i).Sections[0].ElapsedTime);
		//}
		//_p(.getRegisterdThread(0).Sections[0].ElapsedTime);

#else
		// デバイスロストの確認
        checkDeviceState();

		if ( mPhysicsManager ) mPhysicsManager->updateContents();

        // エフェクト描画リスト更新
        mEffectManager->updateDrawContents();

        // 描画情報更新
        mSceneManager->updateDefaultSceneRenderContent();

        

        mGraphicsManager->getDebugText()->commitText();
#endif
    }
	

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
#ifdef LNOTE_PUBLIC_ONLY
#else
    void Manager::_onLostDevice()
    {
		// LNEV_DEVICE_LOST
		//mSystemManager->sendEvent(System::createEvent<System::Event>(LNEV_DEVICE_LOST));

        //if ( mGraphicsManager )
        //{
        //    mGraphicsManager->getGraphicsDevice()->onLostDevice();
        //}
    }
#endif

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
#ifdef LNOTE_PUBLIC_ONLY
#else
    void Manager::_onResetDevice()
    {
		

        //if ( mGraphicsManager )
        //{
        //    mGraphicsManager->getGraphicsDevice()->onResetDevice();
        //}
        if ( mInputManager && mGraphicsManager )
        {
            LMatrix m = mGraphicsManager->getGraphicsDevice()->getDefaultCanvas()->getScreenTransform();
            mInputManager->setMouseTransform( m );
        }

		// LNEV_DEVICE_RESET
		//mSystemManager->sendEvent(System::createEvent<System::Event>(LNEV_DEVICE_RESET));
    }
#endif


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::_onUpdateSyncPhase()
    {
        
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::_render()
	{
		if ( mSceneManager )
		{
			// デフォルトキャンバスを描画先にする
			Graphics::ICanvas* canvas = mGraphicsManager->getGraphicsDevice()->getDefaultCanvas();
			canvas->activate();

			// 描画パフォーマンス測定区間
			{
				Base::ScopedProfilerSection section(
					Base::Profiler::Instance,
					Base::Profiler::RenderThread_ID,
					Base::Profiler::RenderThread_Section0);

				// 描画開始
				Graphics::IRenderer* renderer = mGraphicsManager->getGraphicsDevice()->getRenderer();
				renderer->beginScene();

				// デフォルトシーンを描画
				mSceneManager->renderDefaultScene(canvas->getBackbuffer());

				// 描画終了
				renderer->endScene();

				//::Sleep(10);
			}

			// present パフォーマンス測定区間
			{
				//Base::ScopedProfilerSection section(
				//	Base::Profiler::Instance,
				//	Base::Profiler::RenderThread_ID,
				//	Base::Profiler::RenderThread_Section_Present);

				canvas->present();
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::_updatePhysics() 
	{  
		// TODO: 
		mPhysicsManager->update( mFPSController->getGameTime().getElapsedTime() ); 
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::checkDeviceState()
    {
		if ( mGraphicsManager ) {
			
			if ( mGraphicsManager->getGraphicsDevice()->getDeviceState() == Graphics::DeviceState_Lost ) {

				if ( mEffectManager ) mEffectManager->OnPauseDevice();

				mGraphicsManager->getGraphicsDevice()->resetDevice();

				if ( mEffectManager ) mEffectManager->OnResumeDevice();
			}
			//mGraphicsManager->getGraphicsDevice()->testDeviceLost();
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::waitRenderEnd()
	{
		mRenderThread->waitForTaskWait();
	}

} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================
