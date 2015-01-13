//==============================================================================
// Win32WindowManager 
//==============================================================================

#include "stdafx.h"
#include "Win32Window.h"
#include "Win32WindowManager.h"

namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// ■ Win32WindowManager
//==============================================================================

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
Win32WindowManager::Win32WindowManager( Manager* manager )
	: WindowManagerBase	( manager )
	, mMainWindow		( NULL )
	, mConsoleAlloced	( false )
	, mSystemMouseShown	( true )
{
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
Win32WindowManager::~Win32WindowManager()
{
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Win32WindowManager::preInitialize( const SettingData& setting_data )
{
	mSettingData = setting_data;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Win32WindowManager::initialize()
{
	// ユーザー定義のウィンドウハンドルがある場合はダミーウィンドウを作る
	if ( mSettingData.WindowHandle )
	{
		Win32WindowHost* window = LN_NEW Win32WindowHost( this );
		window->initialize( (HWND)mSettingData.WindowHandle );
		mMainWindow = window;
	}
	// ユーザー定義が無ければ通常のウィンドウ
	else
	{
		Win32Window::SettingData data;
		data.TitleText		= mSettingData.TitleText;
		data.Width			= mSettingData.Width;
		data.Height			= mSettingData.Height;
		data.Windowed		= mSettingData.Windowed;
		data.WinClassName	= NULL;
		data.IconResourceID	= NULL;
		data.Resizable		= mSettingData.Resizable;
		Win32Window* window = LN_NEW Win32Window( this );
		window->initialize( data );
		mMainWindow = window;
	}

	if ( mSettingData.UseConsole )
	{
		// コンソールアプリとして実行している場合はこの if に入ることはない
		if ( ::AllocConsole() )
		{
			freopen( "CON", "r", stdin );
			freopen( "CON", "w", stdout );
			freopen( "CON", "w", stderr );
			mConsoleAlloced = true;

			// 割り当てた後はコンソールのウィンドウが最前面になるので、
			// ゲームのウィンドウを最前面に移動しておく
			::SetForegroundWindow( mMainWindow->getWindowHandle() );
		}
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Win32WindowManager::finalize()
{
	// 割り当てたコンソールが残っていれば解放
	if ( mConsoleAlloced )
	{
		printf( "Enter キーを押すと終了します...\n" );
     
		getchar();
		::FreeConsole();
		mConsoleAlloced = false;

		// コンソールアプリとして実行している場合は終了時に「キーを押すと終了～」と
		// メッセージが出て一時停止するけど、AllocConsole() で割り当てた場合は停止しない。
		// そのため、一度メッセージを出して getchar() で停止してから終了する
	}


	if ( mMainWindow ) mMainWindow->finalize();
	LN_SAFE_RELEASE( mMainWindow );

	WindowManagerBase::finalize();
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Win32WindowManager::processMessage()
{
	// 通常ウィンドウの時のみ、メッセージ処理を行う
	// (ユーザー定義ウィンドウの時はユーザー責任で処理する)
	if ( !mMainWindow->isDummy() )
	{
		// 非アクティブの場合はクライアント領域外で移動したことにして、カーソルを表示する
		if ( mMainWindow->isActive() == false )
		{
			this->mMouseCursorVisibleCounter.onMoveCursor( false );
		}

		// 時間経過によるマウスカーソルの非表示処理
		bool mc_visible = this->mMouseCursorVisibleCounter.checkVisible();
		if ( mc_visible != mSystemMouseShown )
		{
			if ( mc_visible )
			{
				::ShowCursor( TRUE );
			}
			else
			{
				::ShowCursor( FALSE );
			}
			mSystemMouseShown = mc_visible;
		}

		// メッセージ処理
		MSG msg;
		while ( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if ( ::GetMessage( &msg, NULL, 0, 0 ) )
			{
				::TranslateMessage( &msg );

				bool handled = false;
				LRESULT r = sendWndMsgToAttachedListener( msg.hwnd, msg.message, msg.wParam, msg.lParam, &handled );
				if ( !handled ) 
				{
					switch ( msg.message )
					{
						case WM_MOUSEMOVE:
							this->mMouseCursorVisibleCounter.onMoveCursor( true );
							break;
						case WM_NCMOUSEMOVE:
							this->mMouseCursorVisibleCounter.onMoveCursor( false );
							break;
					}
					::DispatchMessage( &msg );
				}
			}
		}
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
Window* Win32WindowManager::getMainWindow() const 
{ 
	return mMainWindow; 
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
LRESULT Win32WindowManager::sendWndMsgToAttachedListener( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, bool* handled )
{
	LRESULT r = 0;
	Win32MessageListenerList::ListenerEntryList& list = mWin32MessageListenerList.getListenerEntryList();
	ln_foreach( Win32MessageListenerList::ListenerEntry& e, list )
	{
		r = e.EventListener->WndProc( hwnd, msg, wparam, lparam, handled );
		if ( *handled ) return r;
	}
	return r;
}

} // namespace System
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================