//==============================================================================
// Manager 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Threading/Thread.h"
#include "../Threading/EventFlag.h"
#include "../Threading/Mutex.h"
#include "Environment.h"
#include "Manager.h"

#if defined(LNOTE_WIN32)
    #include "Win32/SystemUtil.h"
    #include "Win32/Win32WindowManager.h"
    #include "GLFW/GLFWWindowManager.h"
#elif defined(LNOTE_X11)
    #include "X11/X11SystemUtil.h"
    #include "X11/X11WindowManager.h"
#elif defined(LNOTE_ANDROID)
    #include "../../AndroidMain/native_app_glue.h"
    #include <android/native_window.h>
    #include "Android/AndroidWindow.h"
    #include "Android/AndroidApplication.h"
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// ■ Manager
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Manager::Manager()
        : mWindowManager		( NULL )
		, mWndProcThread		( NULL )
		, mPrimaryEventQueue	( NULL )
		, mSecondaryEventQueue	( NULL )
        , mEndRequested			( true )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Manager::~Manager()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initialize( const SettingData& setting_data )
    {
        LN_LOG_INIT_BEGIN;

		mPrimaryEventQueue = LN_NEW std::deque<EventArgs>();
		mSecondaryEventQueue = LN_NEW std::deque<EventArgs>();

#if defined(LNOTE_WIN32)
		if ( setting_data.SystemAPI == WindowSystemAPI_GLFW )
		{
			GLFWWindowManager::SettingData setting;
			setting.UseConsole		= setting_data.UseConsole;
			setting.TitleText		= setting_data.WindowTitleText;
			setting.Width			= setting_data.WindowWidth;
			setting.Height			= setting_data.WindowHeight;
			setting.Windowed		= setting_data.Windowed;
			setting.Resizable		= setting_data.WindowResizeEnabled;
			GLFWWindowManager* window_manager = LN_NEW GLFWWindowManager( this );
			window_manager->preInitialize( setting );
			mWindowManager = window_manager;
		}
		else //if ( setting_data.SystemAPI == WindowSystemAPI_WinAPI )
		{
			Win32WindowManager::SettingData setting;
			setting.UseConsole		= setting_data.UseConsole;
			setting.WindowHandle	= setting_data.WindowHandle;
			setting.TitleText		= setting_data.WindowTitleText;
			setting.Width			= setting_data.WindowWidth;
			setting.Height			= setting_data.WindowHeight;
			setting.Windowed		= setting_data.Windowed;
			setting.Resizable		= setting_data.WindowResizeEnabled;
			Win32WindowManager* window_manager = LN_NEW Win32WindowManager( this );
			window_manager->preInitialize( setting );
			mWindowManager = window_manager;
		}
#elif defined(LNOTE_X11)
        X11WindowManager::ConfigData data;
        data.TitleText      = setting_data.WindowTitleText;
        data.Width          = setting_data.WindowWidth;
        data.Height         = setting_data.WindowHeight;
        data.Windowed       = setting_data.Windowed;
        X11WindowManager* window_manager = LN_NEW X11WindowManager( this );
		window_manager->preInitialize( data );
		mWindowManager = window_manager;
#elif defined(LNOTE_ANDROID)
        AndroidApplication::InitData data;
        data.SystemManager  = this;
        data.Width          = init_data_.WindowWidth;
        data.Height         = init_data_.WindowHeight;
        data.AndroidApp     = init_data_.AndroidApp;
        AndroidApplication* app = LN_NEW AndroidApplication();
        app->initialize( data );
        mApplication = app;
#endif

		if (setting_data.UseThread && setting_data.WindowHandle == NULL)
		{
			LN_LOG_WRITE( L"    Threaded : %d", setting_data.UseThread );
			mWndProcThread = LN_NEW WndProcThread( this );
			mWndProcThread->start();
			mWndProcThread->waitFinishInitialize();
		}
		else
		{
			mWindowManager->initialize();
		}

		mWindowManager->getMainWindow()->setVisible( true );
		mWindowManager->setMouseCursorVisibleState( false, 1000 );

		
        mEndRequested = false;

        LN_LOG_INIT_END;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::finalize()
    {
        LN_LOG_FIN_BEGIN;

		if ( mWndProcThread )
		{
			mWndProcThread->requestEnd();
			mWndProcThread->wait();
			SAFE_DELETE( mWndProcThread );
		}
		else
		{
			if ( mWindowManager ) mWindowManager->finalize();
		}

		LN_SAFE_RELEASE( mWindowManager );

		SAFE_DELETE( mPrimaryEventQueue );
		SAFE_DELETE( mSecondaryEventQueue );

        LN_LOG_FIN_END;
    }

	//----------------------------------------------------------------------
	// ● ウィンドウを取得する
	//----------------------------------------------------------------------
    Window* Manager::getMainWindow()
    {
		return (mWindowManager) ? mWindowManager->getMainWindow() : NULL;
    }

	//----------------------------------------------------------------------
	// ● メッセージ処理
	//----------------------------------------------------------------------
    bool Manager::processMessage()
    {
		if ( !mWndProcThread )
		{
			mWindowManager->processMessage();
		}

		while ( !mPrimaryEventQueue->empty() )
		{
			const EventArgs& e = mPrimaryEventQueue->front();
			mWindowManager->sendEventToAllListener( e );

			switch ( e.Type )
			{
				case LN_EVENT_CLOSE:
				{
					mEndRequested = true;
					break;
				}
			}

			mPrimaryEventQueue->pop_front();
		}

		// ロックしてスワップ
		Threading::ScopedLock lock( mEventQueueLock );
		std::deque<EventArgs>* t = mPrimaryEventQueue;
		mPrimaryEventQueue = mSecondaryEventQueue;
		mSecondaryEventQueue = t;

        return !mEndRequested;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::postEventFromWindowThread( const EventArgs* args )
	{
		LN_ERR2_ASSERT( args->StructSize <= sizeof(EventArgs) );

		Threading::ScopedLock lock( mEventQueueLock );
		mSecondaryEventQueue->push_back( *args );
	}

//==============================================================================
// ■ Manager::WndProcThread
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::WndProcThread::run()
	{
		mManager->mWindowManager->initialize();
		mInitFinished.setTrue();
		while ( !mEndRequested.isTrue() )
        {
			mManager->mWindowManager->processMessage();
			Threading::Thread::sleep( 16 );
		}
		mManager->mWindowManager->finalize();
	}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace System
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================