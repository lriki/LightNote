//==============================================================================
// Win32Window 
//==============================================================================

#include "stdafx.h"
#include "../../Base/StringUtils.h"
#include "../Manager.h"
#include "Win32WindowManager.h"
#include "SystemUtil.h"
#include "Win32Window.h"

#if defined(LNOTE_WIN32)

namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// ■ Win32WindowBase
//==============================================================================

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
Win32WindowBase::Win32WindowBase( Win32WindowManager* manager )
    : WindowBase		( manager )
	, mWindowManager	( manager )
    , mLastMouseX		( -1 )
    , mLastMouseY		( -1 )
    , mIsActive			( false )
{
	LN_SAFE_ADDREF( mWindowManager );
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
Win32WindowBase::~Win32WindowBase()
{
	LN_SAFE_RELEASE( mWindowManager );
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
LRESULT Win32WindowBase::WndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	// WM_SYSKEYDOWN 関係をむやみに postMessage() すると強制終了するので必要なものだけフィルタリング
    //if ( msg != WM_SYSKEYDOWN || (msg == WM_SYSKEYDOWN && wparam == VK_RETURN) )
    {
		// マウスキャプチャの処理
		switch( msg )
		{
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
				::SetCapture( hwnd );
				break;
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
				::ReleaseCapture();
				break;
		}

		switch( msg )
		{
			/////////////////// ウィンドウが閉じられようとしている
			case WM_CLOSE:
			{
				/*
				ここは DefWindowProc() を呼ばないで終了する。
				こうしないと、×ボタンが押された瞬間にウィンドウが破棄されて、
				終了直前の最後の描画時に DirectX の EndScene() で
				「描画先のウィンドウがありません」ということでエラーが出る。

				代わりに終了フラグだけを立てて、それをメインループで検出、
				その後の finalize() 呼び出しで DestroyWindow() を呼び出す。
				*/

				EventArgs e;
				e.Type			= LN_EVENT_CLOSE;
				e.Sender		= this;
				e.StructSize	= sizeof(EventArgs);
				mWindowManager->getManager()->postEventFromWindowThread( &e );
				return 0;
			}

			/////////////////// ウィンドウがアクティブ・非アクティブになった場合
			case WM_ACTIVATE:
			{
				bool active = ( ( wparam & 0xffff ) != 0 );
				if ( active != mIsActive )
				{
					mIsActive = active;

					EventArgs e;
					e.Type			= ( mIsActive ) ? LN_EVENT_APP_ACTIVATE : LN_EVENT_APP_DEACTIVATE;
					e.Sender		= this;
					e.StructSize	= sizeof(EventArgs);
					mWindowManager->getManager()->postEventFromWindowThread( &e );
				}
				return 0;
			}
			/////////////////////////////////////////////// ウィンドウサイズが変更された
			case WM_SIZE:
			{
				EventArgs e;
				e.Type			= LN_EVENT_WINDOW_SIZE_CHANGED;
				e.Sender		= this;
				e.StructSize	= sizeof(EventArgs);
				mWindowManager->getManager()->postEventFromWindowThread( &e );
				return 0;
			}
			/////////////////////////////////////////////// Alt + Enter 確認
			//case WM_SYSKEYDOWN:
			//{
			//	if ( wparam == VK_RETURN )
			//	{
			//		EventArgs e;
			//		e.Type			= LN_EVENT_ALTENTER;
			//		e.Sender		= this;
			//		e.StructSize	= sizeof(EventArgs);
			//		mWindowManager->getManager()->postEventFromWindowThread( &e );

			//		return MAKELONG( -1, MNC_EXECUTE );     // 第一引数はメニューの属性。今回はメニュー使ってないのでとりあえずこのまま
			//	}
			//	// ここは以降に DefWindowProc() を呼ぶ。
			//	// こうしないと Alt + F4 とかが無効になってそれで終了できなくなる。
			//	return 0;
			//}
			/////////////////////////////////////////////// Alt + Enter の警告音カット http://www26.atwiki.jp/gunsyu3/pages/11.html
			case WM_SYSCHAR:
			{
				return 0;
			}
			/////////////////////////////////////////////// マウスボタン入力
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			{
				MouseEventArgs e;
				e.Sender		= this;
				e.StructSize	= sizeof(MouseEventArgs);

				switch( msg )
				{
					case WM_LBUTTONDOWN:
						e.Type		= LN_EVENT_MOUSE_DOWN;
						e.Button	= LN_MOUSE_LEFT;
						break;
					case WM_LBUTTONUP:
						e.Type		= LN_EVENT_MOUSE_UP;
						e.Button	= LN_MOUSE_LEFT;
						break;
					case WM_RBUTTONDOWN:
						e.Type		= LN_EVENT_MOUSE_DOWN;
						e.Button	= LN_MOUSE_RIGHT;
						break;
					case WM_RBUTTONUP:
						e.Type		= LN_EVENT_MOUSE_UP;
						e.Button	= LN_MOUSE_RIGHT;
						break;
					case WM_MBUTTONDOWN:
						e.Type		= LN_EVENT_MOUSE_DOWN;
						e.Button	= LN_MOUSE_MIDDLE;
						break;
					case WM_MBUTTONUP:
						e.Type		= LN_EVENT_MOUSE_UP;
						e.Button	= LN_MOUSE_MIDDLE;
						break;
				}  

				e.X = LOWORD( lparam ); 
				e.Y = HIWORD( lparam );
				e.Delta = 0;
				e.MoveX = ( mLastMouseX >= 0 ) ? e.X - mLastMouseX : 0;
				e.MoveY = ( mLastMouseY >= 0 ) ? e.Y - mLastMouseY : 0;
				mWindowManager->getManager()->postEventFromWindowThread( (EventArgs*)&e );

				mLastMouseX = e.X;
				mLastMouseY = e.Y;
				return 0;
			}
			/////////////////////////////////////////////// マウス移動
			case WM_MOUSEMOVE:
			{
				MouseEventArgs e;
				e.Type			= LN_EVENT_MOUSE_MOVE;
				e.Sender		= this;
				e.StructSize	= sizeof(MouseEventArgs);
				e.Button		= LN_MOUSE_NONE;
				e.Delta			= 0;
				e.X				= static_cast< short >( LOWORD( lparam ) );     // 一度 short にキャストしないと、
				e.Y				= static_cast< short >( HIWORD( lparam ) );     // マイナス値になったとき 65535 とか値が入る
				e.MoveX			= ( mLastMouseX >= 0 ) ? e.X - mLastMouseX : 0;
				e.MoveY			= ( mLastMouseY >= 0 ) ? e.Y - mLastMouseY : 0;
				mWindowManager->getManager()->postEventFromWindowThread( (EventArgs*)&e );

				mLastMouseX = e.X;
				mLastMouseY = e.Y;
				return 0;
			}
			/////////////////////////////////////////////// クライアント領域外でマウスが移動した
			case WM_NCMOUSEMOVE:
			{
				// 念のためホントにクライアント領域外かチェック
				if ( wparam != HTCLIENT )
				{
					MouseEventArgs e;
					e.Type			= LN_EVENT_MOUSE_MOVE;
					e.Sender		= this;
					e.StructSize	= sizeof(MouseEventArgs);
					e.Button		= LN_MOUSE_NONE;
					e.Delta			= 0;
					e.X				= -1;
					e.Y				= -1;
					e.MoveX			= ( mLastMouseX >= 0 ) ? e.X - mLastMouseX : 0;
					e.MoveY			= ( mLastMouseY >= 0 ) ? e.Y - mLastMouseY : 0;
					mWindowManager->getManager()->postEventFromWindowThread( (EventArgs*)&e );

					mLastMouseX = e.X;
					mLastMouseY = e.Y;
					return 0;
				}
			}
			///////////////////////////////////////////// マウスホイールが操作された
			case WM_MOUSEWHEEL:
			{
				MouseEventArgs e;
				e.Type			= LN_EVENT_MOUSE_WHEEL;
				e.Sender		= this;
				e.StructSize	= sizeof(MouseEventArgs);
				e.Button		= LN_MOUSE_NONE;
				e.Delta			= GET_WHEEL_DELTA_WPARAM( wparam ) / WHEEL_DELTA;
				e.X				= static_cast< short >( LOWORD( lparam ) );
				e.Y				= static_cast< short >( HIWORD( lparam ) );
				e.MoveX			= ( mLastMouseX >= 0 ) ? e.X - mLastMouseX : 0;
				e.MoveY			= ( mLastMouseY >= 0 ) ? e.Y - mLastMouseY : 0;
				mWindowManager->getManager()->postEventFromWindowThread( (EventArgs*)&e );

				mLastMouseX = e.X;
				mLastMouseY = e.Y;
				return 0;
			}
			///////////////////////////////////////////// キー↓
			case WM_KEYDOWN:
			{
				KeyEventArgs e;
				e.Type			= LN_EVENT_KEY_DOWN;
				e.Sender		= this;
				e.StructSize	= sizeof(KeyEventArgs);
				e.KeyCode		= Win32::convertVirtualKeyCode( wparam );	// 仮想キーコード
				e.IsAlt			= ::GetKeyState( VK_MENU ) < 0;
				e.IsShift		= ::GetKeyState( VK_SHIFT ) < 0;
				e.IsControl		= ::GetKeyState( VK_CONTROL ) < 0;
				mWindowManager->getManager()->postEventFromWindowThread( (EventArgs*)&e );

				return 0;
			}
			///////////////////////////////////////////// キー↑
			case WM_KEYUP:
			{
				KeyEventArgs e;
				e.Type			= LN_EVENT_KEY_UP;
				e.Sender		= this;
				e.StructSize	= sizeof(KeyEventArgs);
				e.KeyCode		= Win32::convertVirtualKeyCode( wparam );	// 仮想キーコード
				e.IsAlt			= ::GetKeyState( VK_MENU ) < 0;
				e.IsShift		= ::GetKeyState( VK_SHIFT ) < 0;
				e.IsControl		= ::GetKeyState( VK_CONTROL ) < 0;
				mWindowManager->getManager()->postEventFromWindowThread( (EventArgs*)&e );

				return 0;
			}
			///////////////////////////////////////////// Alt + KeyDown
			case WM_SYSKEYDOWN:
			{
				KeyEventArgs e;
				e.Type = LN_EVENT_KEY_DOWN;
				e.Sender = this;
				e.StructSize = sizeof(KeyEventArgs);
				e.KeyCode = Win32::convertVirtualKeyCode(wparam);	// 仮想キーコード
				e.IsAlt = true;										// Alt on
				e.IsShift = ::GetKeyState(VK_SHIFT) < 0;
				e.IsControl = ::GetKeyState(VK_CONTROL) < 0;
				mWindowManager->getManager()->postEventFromWindowThread((EventArgs*)&e);
				
				break;	// WM_SYSKEYDOWNを捕まえた場合、必ずDefWindowProcに行くようにする
			}
			///////////////////////////////////////////// Alt + KeyUp
			case WM_SYSKEYUP:
			{
				KeyEventArgs e;
				e.Type = LN_EVENT_KEY_UP;
				e.Sender = this;
				e.StructSize = sizeof(KeyEventArgs);
				e.KeyCode = Win32::convertVirtualKeyCode(wparam);	// 仮想キーコード
				e.IsAlt = true;										// Alt on
				e.IsShift = ::GetKeyState(VK_SHIFT) < 0;
				e.IsControl = ::GetKeyState(VK_CONTROL) < 0;
				mWindowManager->getManager()->postEventFromWindowThread((EventArgs*)&e);

				break;	// WM_SYSKEYUPを捕まえた場合、必ずDefWindowProcに行くようにする
			}
			///////////////////////////////////////////// 文字入力
			case WM_CHAR:
			{	
				// 文字のみ送る
				if ( 0x20 <= wparam && wparam <= 0x7E )
				{
					KeyEventArgs e;
					e.Type			= LN_EVENT_KEY_CHAR;
					e.Sender		= this;
					e.StructSize	= sizeof(KeyEventArgs);
					e.KeyCode		= wparam;
					e.IsAlt			= ::GetKeyState( VK_MENU ) < 0;
					e.IsShift		= ::GetKeyState( VK_SHIFT ) < 0;
					e.IsControl		= ::GetKeyState( VK_CONTROL ) < 0;
					mWindowManager->getManager()->postEventFromWindowThread( (EventArgs*)&e );

					return 0;
				}
			}
		}
	}

    return ::DefWindowProc( hwnd, msg, wparam, lparam );
}

//==============================================================================
// ■ Win32Window
//==============================================================================

const lnChar*	Win32Window::WINDOW_CLASS_NAME	= _T( "_LNote_" );
const lnChar*	Win32Window::PROP_WINPROC		= _T( "_LNoteProp_" );
const DWORD		Win32Window::WINDOWED_STYLE		= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;// | WS_THICKFRAME;
const DWORD		Win32Window::FULLSCREEN_STYLE	= WS_POPUP;

static const int		MAX_WINDOW_TEXT = 256;			///< ウィンドウタイトル文字列の最大文字数
static const int		MAX_WINDOW_TEXT_FOOTER = 64;	///< ウィンドウタイトル文字列のフッタ情報の最大文字数

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
Win32Window::Win32Window( Win32WindowManager* manager )
	: Win32WindowBase  ( manager )
    , mOrginalWidth     ( 640 )
    , mOrginalHeight    ( 480 )
    , mWindowHandle     ( NULL )
    , mAccelerators     ( NULL )
	, mWindowedStyle	( WINDOWED_STYLE )
    , mFullScreen       ( false )
{
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
Win32Window::~Win32Window()
{
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Win32Window::initialize( const SettingData& setting_data )
{
    mTitleText      = setting_data.TitleText;
    mClientSize.w   = setting_data.Width;
    mClientSize.h   = setting_data.Height;
    mFullScreen     = false;
    mOrginalWidth   = mClientSize.w;
    mOrginalHeight  = mClientSize.h;
	mWindowedStyle = WINDOWED_STYLE | ((setting_data.Resizable) ? (WS_THICKFRAME | WS_MAXIMIZEBOX) : 0);

    HINSTANCE inst = (HINSTANCE)::GetModuleHandle( NULL );

	// ウィンドウアイコン
	DWORD dwExStyle = 0;
    HICON icon = NULL;
    if ( setting_data.IconResourceID ) {
        icon = ::LoadIcon( inst, MAKEINTRESOURCE( setting_data.IconResourceID ) );
    }
	else {
		dwExStyle = WS_EX_DLGMODALFRAME;	// アイコンの無いスタイル
	}

	// ウィンドウクラスの設定
	WNDCLASSEX	wcex = {
		sizeof( WNDCLASSEX ),			    // この構造体のサイズ
		NULL,							    // ウインドウのスタイル
		_staticMsgProc,					    // メッセージ処理関数
		0, 0,							    // 通常は使わないので常に0
		inst,				                // インスタンスハンドル
		icon,							    // アイコン
        ::LoadCursor( NULL, IDC_ARROW ),	// カーソルの形
		(HBRUSH)( COLOR_WINDOW + 1 ),	    // 背景色
		NULL,							    // メニューなし
        ( setting_data.WinClassName ) ? setting_data.WinClassName : WINDOW_CLASS_NAME,				// クラス名の指定
		NULL };							    // 小アイコン（なし）

	// ウィンドウクラスの登録
	if ( !::RegisterClassEx( &wcex ) )
	{
		LN_THROW(0, Base::Win32Exception, GetLastError());
	}

	// ウィンドウの作成
	mWindowHandle = ::CreateWindowEx(
		dwExStyle,
		( setting_data.WinClassName ) ? setting_data.WinClassName : WINDOW_CLASS_NAME,
		mTitleText.c_str(),
        ( setting_data.Windowed ) ? mWindowedStyle : FULLSCREEN_STYLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, inst, NULL);
	LN_THROW(mWindowHandle, Base::Win32Exception, GetLastError());

	// ウィンドウハンドルとこのクラスのポインタを関連付ける
	BOOL r = ::SetProp(mWindowHandle, PROP_WINPROC, this);
	LN_THROW((r != FALSE), Base::Win32Exception, GetLastError());

	// アクセラレータの作成
    ACCEL accels[ 1 ] =
    {
        { FALT | FVIRTKEY, VK_RETURN, 0 }
    };
	mAccelerators = ::CreateAcceleratorTable(accels, 1);
	LN_THROW(mAccelerators, Base::Win32Exception, GetLastError());

	// WM_PAINTが呼ばれないようにする
	::ValidateRect( mWindowHandle, 0 );

	// ウィンドウの大きさを変更する
    _resize( !setting_data.Windowed );
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Win32Window::finalize()
{
	if ( mWindowHandle )
	{
		::DestroyWindow( mWindowHandle );
		//UnregisterClass( WINDOW_CLASS_NAME, mInstanceHandle );
		mWindowHandle = NULL;
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
const Geometry::Size& Win32Window::getSize()
{
	// コンストラクタで mClientSize に格納しておいてもいいと思ったけど、
	// フルスクリーン化等でウィンドウサイズが変わった時の対応が面倒そうなのでこのまま。
    RECT rc;
    ::GetClientRect( mWindowHandle, &rc );
    mClientSize.set( rc.right, rc.bottom );
    return mClientSize;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Win32Window::setVisible( bool flag )
{
    ::ShowWindow( mWindowHandle, ( flag ) ? SW_SHOWNORMAL : SW_HIDE );
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Win32Window::setEnableFullScreen( bool flag )
{
	// フルスクリーンにする場合
	if ( flag )
	{
        mFullScreen = true;
		::SetWindowLong(mWindowHandle, GWL_STYLE, FULLSCREEN_STYLE);
		::SetWindowLong(mWindowHandle, GWL_EXSTYLE, 0);
        _resize( true );
	}
	// ウィンドウモードにする場合
	else
	{
        mFullScreen = false;
        _resize( false );
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Win32Window::setWindowTextFooter( const lnChar* format, ... )
{
    lnChar buf[ MAX_WINDOW_TEXT_FOOTER ];
	va_list args;
	int len;
	va_start( args, format );
	len = _vsctprintf( format, args ) + 1;
	_vstprintf_s( buf, len, format, args );
	va_end( args );

    lnChar text[ MAX_WINDOW_TEXT ] = { 0 };
    _stprintf_s( text, MAX_WINDOW_TEXT, _T( "%s%s" ), mTitleText.c_str(), buf );
    ::SetWindowText( mWindowHandle, text );
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Win32Window::_resize( bool fullscreen )
{
	BOOL res;
	int x, y, sw, sh;
	int win_w = mOrginalWidth;
	int win_h = mOrginalHeight;

	// フルスクリーンモードの場合
	if ( fullscreen )
	{
        /*
        // 一瞬塗りつぶす
        HDC hdc;
        PAINTSTRUCT ps;
        hdc = BeginPaint(mWindowHandle , &ps);

	    //SelectObject(hdc , CreateSolidBrush(RGB(0xFF , 0 , 0)));
	    PatBlt(hdc , 10 , 10 , 200 , 100 , PATCOPY);
	    //DeleteObject(
		   // SelectObject(
			  //  hdc , GetStockObject(WHITE_BRUSH)
		   // )
	    //);

	    EndPaint(mWindowHandle , &ps);
        */

        // 画面いっぱい
        mClientSize.w = ::GetSystemMetrics( SM_CXSCREEN );
		mClientSize.h = ::GetSystemMetrics( SM_CYSCREEN );

        ::SetForegroundWindow( mWindowHandle );

        x = ( ::GetSystemMetrics( SM_CXSCREEN ) - mClientSize.w ) / 2;
		y = ( ::GetSystemMetrics( SM_CYSCREEN ) - mClientSize.h ) / 2;

		//res = ::SetWindowPos( mWindowHandle, HWND_TOP, 0, 0, mWidth, mHeight, SWP_SHOWWINDOW );
        ::SetWindowPos( mWindowHandle, NULL, x, y, mClientSize.w, mClientSize.h, SWP_SHOWWINDOW );
	}
	// ウィンドウモードの場合は実際にウィンドウの位置を設定する
    else
    {
        mClientSize.w  = mOrginalWidth;
	    mClientSize.h = mOrginalHeight;

        // 新しいクライアント領域の矩形を作成
	    mWindowRect.left = 0;
	    mWindowRect.top = 0;
	    mWindowRect.right = win_w;
	    mWindowRect.bottom = win_h;

        // 念のため
        ::SetWindowLong( mWindowHandle, GWL_STYLE, mWindowedStyle );

        // 枠の大きさも含めたウィンドウサイズを取得
	    res = ::AdjustWindowRect( &mWindowRect, mWindowedStyle, FALSE );

        // 必要なウィンドウの幅と高さを計算
	    win_w = mWindowRect.right - mWindowRect.left;
	    win_h = mWindowRect.bottom - mWindowRect.top;

		// ディスプレイ全体のサイズを取得
		sw = ::GetSystemMetrics( SM_CXSCREEN );
		sh = ::GetSystemMetrics( SM_CYSCREEN );

		// ディスプレイの中央に表示できる座標を計算
		x = ( sw - win_w ) / 2;
		y = ( sh - win_h ) / 2;

		// 位置と大きさを変更 (SWP_SHOWWINDOW を指定しないと、フルスクリーンからの復帰時に何も描画されなくなった)
		res = ::SetWindowPos( mWindowHandle, NULL, x, y, win_w, win_h, SWP_SHOWWINDOW );
    }
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
LRESULT CALLBACK Win32Window::_staticMsgProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	//static int i = 0;
	//printf( "%d %4x\n", i++, msg_ );
    /*
    0x000C SPI_LANGDRIVER
    0x00AE ?
    0x007F SPI_SETMOUSEDOCKTHRESHOLD
    */

    Win32Window* window = (Win32Window*)::GetProp( hwnd, PROP_WINPROC );

    if ( window )
    {
        return window->WndProc( hwnd, msg, wparam, lparam );
    }
    else
    {
        return ::DefWindowProc( hwnd, msg, wparam, lparam );
    }
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
LRESULT Win32Window::WndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
    switch ( msg )
    {
        /////////////////// ウィンドウが破棄された場合
        case WM_DESTROY:
        {
		    ::PostQuitMessage( 0 );

            // ウィンドウハンドルとポインタの関連付けを解除
	        ::RemoveProp( hwnd, PROP_WINPROC );
		    return 0;
        }
        /////////////////// 上記以外は基底に回す
        default:
        {
            return Win32WindowBase::WndProc( hwnd, msg, wparam, lparam );
        }
    }
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Win32Window::captureMouse()
{
	::SetCapture( mWindowHandle );
}
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Win32Window::releaseMouseCapture()
{
	::ReleaseCapture();
}

//==============================================================================
// ■ Win32WindowHost
//==============================================================================

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
Win32WindowHost::Win32WindowHost( Win32WindowManager* manager )
	: Win32WindowBase	( manager )
	, mWindowHandle		( NULL )
{
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
Win32WindowHost::~Win32WindowHost()
{
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Win32WindowHost::initialize( HWND hwnd )
{
	mWindowHandle = hwnd;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
const Geometry::Size& Win32WindowHost::getSize()
{
	// コンストラクタで mClientSize に格納しておいてもいいと思ったけど、
	// フルスクリーン化等でウィンドウサイズが変わった時の対応が面倒そうなのでこのまま。
    RECT rc;
    ::GetClientRect( mWindowHandle, &rc );
    mClientSize.set( rc.right, rc.bottom );
    return mClientSize;
}

} // namespace System
} // namespace Core
} // namespace LNote

#endif // LNOTE_WIN32

//==============================================================================
//
//==============================================================================