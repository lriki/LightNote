//==============================================================================
// Environment 
//==============================================================================

#include "stdafx.h"
#include "../Base/StringUtils.h"
#include "Win32/Win32Window.h"
#include "GLFW/GLFWWindow.h"
#include "Environment.h"

namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// ■ Environment
//==============================================================================

//---------------------------------------------------------------------
// ● システム時刻を取得する (ミリ秒)
//---------------------------------------------------------------------
lnU32 Environment::getSystemTime()
{
#if defined(LNOTE_WIN32)

    return ::timeGetTime();

#else

    timespec ts;
    clock_gettime( CLOCK_MONOTONIC, &ts );
    return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
	//return ( (uint64_t) ts.tv_sec * (uint64_t) 1000000000 + (uint64_t) ts.tv_nsec ) / 100000;
	//return (uint64_t)ts.tv_sec * (uint64_t)10000 + (uint64_t)ts.tv_nsec;

#endif
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void Environment::getCurrentDirectory(char* path)
{
#ifdef LNOTE_WIN32
	::GetCurrentDirectoryA(LN_MAX_PATH, path);
#else

#ifdef LNOTE_UNICODE
    char buf[ LN_MAX_PATH ];
    ::getcwd( buf_, LN_MAX_PATH );
    Base::StringUtil::convMultiToWide( buf_, LN_MAX_PATH, buf );
#else
    ::getcwd( buf_, LN_MAX_PATH );
#endif

#endif
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void Environment::getCurrentDirectory(wchar_t* path)
{
#ifdef LNOTE_WIN32
	::GetCurrentDirectoryW(LN_MAX_PATH, path);
#else
	mbstowcs を使って変換
#endif
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void Environment::setCurrentDirectory(const char* path)
{
#ifdef _WIN32
	::SetCurrentDirectoryA(path);
#else
	int chdir(const char *path);
#endif
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void Environment::setCurrentDirectory(const wchar_t* path)
{
#ifdef _WIN32
	::SetCurrentDirectoryW(path);
#else
	int chdir(const char *path);
#endif
}

//---------------------------------------------------------------------
// ● メッセージボックスの表示 (エラー用)
//---------------------------------------------------------------------
void Environment::showMessageBox( Window* parentWindow, const lnChar* message_, const lnChar* title_ )
{
#if defined(LNOTE_WIN32)
	HWND hwnd = NULL;// = ( parentWindow ) ? parentWindow->getWindowHandle() : NULL;
	if ( dynamic_cast<Win32Window*>( parentWindow ) != NULL ) {
		hwnd = ((Win32Window*)parentWindow)->getWindowHandle();
	}
	else if ( dynamic_cast<GLFWWindow*>(parentWindow) != NULL ) {
		hwnd = ((GLFWWindow*)parentWindow)->getWindowHandle();
	}
	::MessageBox( hwnd, message_, title_, MB_OK | MB_ICONERROR );
#endif // LNOTE_WIN32

#if defined(LNOTE_X11)
    Display* display = ::XOpenDisplay( NULL );
    int      screen  = DefaultScreen( display );

	// ダイアログウィンドウを作る
    Window dialog = XCreateSimpleWindow(
        display,
        RootWindow( display, screen ),
        0, 0, 200, 200,                     //ウインドウのサイズ
        1,                                  // ウィンドウの境界線の幅
        BlackPixel( display, screen ),      //枠の設定
        WhitePixel( display, screen ) );    //背景色の色番号

	//----------------------------------------------------------
	// ボタン

	// ボタン用の色を作る
    Colormap cmap;
	XColor c0, c1;
    cmap = DefaultColormap( display, 0 );
    XAllocNamedColor( display, cmap, "rgb:20/20/ff", &c1, &c0 );

	// OK ボタン作成 (ウィンドウをウインドウの中に作成してボタンにする)
    Window button = XCreateSimpleWindow(
        display,
        dialog,
        100, 100, 40, 20,
        1,
        c1.pixel,
        WhitePixel( display, screen ) );

	// マップして表示
    ::XMapWindow( display, button );

	// イベントマスクを登録
    ::XSelectInput( display, button, ButtonPressMask | ButtonReleaseMask );

	//----------------------------------------------------------
	//

	// ウィンドウの内容を記憶
    ::XSetWindowAttributes attr;
    attr.backing_store = WhenMapped;
    ::XChangeWindowAttributes( display, dialog, CWBackingStore, &attr ); 

	// ウインドウ名の設定
    ::XStoreName( display, dialog, "Error" );
    ::XSetIconName( display, dialog, "Error" );

	// イベントマスクを登録
    ::XSelectInput( display, dialog, ExposureMask ); 

	// グラフィックコンテキストを取得
    GC gc = XCreateGC( display, RootWindow( display, screen ), 0, 0 ); 
	
    XSetForeground( display, gc, BlackPixel( display, DefaultScreen( display ) ) );

	// フォントセットを生成する
    char** miss;
    char*  def;
    int    n_miss;
    XFontSet fonst_set = ::XCreateFontSet(
        display,
        "r14",
        &miss, &n_miss, &def );



	// マップして表示
    ::XMapWindow( display, dialog );
    ::XFlush( display );



    XEvent event;
    while ( 1 )
    {
        ::XNextEvent( display, &event );

    	switch ( event.type )
        {
    		case Expose:
			{
                // ボタンの文字を描画
                ::XDrawString( display, button, gc, 10, 15, "OK", 2 );

                // メッセージ文字列の描画
#ifdef LNOTE_UNICODE
                ::XwcDrawText( display, dialog, fonst_set, gc, 8, 32, message_, wcslen( message_ ) );
#else
                ::XmbDrawString( display, dialog, fonst_set, gc, 8, 32, message_, strlen( message_ ) );
#endif

				XFlush( display );
                break;
			}
			// 画面上でマウスのボタンが押された時
    		case ButtonPress:
            {
    			if ( event.xany.window == button )
                {
                    // ボーダーラインを太くする
                    ::XSetWindowBorderWidth( display, event.xany.window, 2 );
    			}
    			XFlush( display );
    			break;
            }
            // 画面上でマウスのボタンが離された時
    		case ButtonRelease:
            {
    			if ( event.xany.window == button )
                {
                    // ボーダーラインを細くする
                    ::XSetWindowBorderWidth( display, event.xany.window, 1 );
    			}
    			XFlush( display );
    	        goto EXIT;
    			break;
            }
    		default:
    			break;
    	}
    }

EXIT:

    ::XDestroyWindow( display, button );
    ::XDestroyWindow( display, dialog );

#endif // LNOTE_X11
}
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Environment::showMessageBox( Window* parentWindow, const wchar_t* message, const wchar_t* title )
{
#if defined(LNOTE_WIN32)
	HWND hwnd = NULL;
	if ( dynamic_cast<Win32Window*>( parentWindow ) != NULL ) {
		hwnd = ((Win32Window*)parentWindow)->getWindowHandle();
	}
	else if ( dynamic_cast<GLFWWindow*>(parentWindow) != NULL ) {
		hwnd = ((GLFWWindow*)parentWindow)->getWindowHandle();
	}
	::MessageBoxW(hwnd, message, title, MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
#endif // LNOTE_WIN32
}

//---------------------------------------------------------------------
// 
//---------------------------------------------------------------------
float Environment::getPhysicalPixelSize()
{
#if defined(LNOTE_WIN32)
    HDC hdc = ::GetDC(NULL);
    return ((float)::GetDeviceCaps(hdc, HORZSIZE)) / ((float)::GetDeviceCaps(hdc, HORZRES));
#endif
}

#if defined(LNOTE_WIN32)
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
lnRefString getDxDiagInfoString( IDxDiagContainer* c, const wchar_t* prop_name )
{
    HRESULT hr;  
    VARIANT var;
    lnRefString out;

    VariantInit( &var );
    hr = c->GetProp( prop_name, &var );
    if ( SUCCEEDED(hr) && var.vt == VT_BSTR && SysStringLen( var.bstrVal ) != 0 )
    {
        out = var.bstrVal;
    }
    VariantClear( &var );
    return out;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
bool getDxDiagInfoBool( IDxDiagContainer* c, const wchar_t* prop_name )
{
    HRESULT hr;  
    VARIANT var;
    bool out = false;

    VariantInit( &var );
    hr = c->GetProp( prop_name, &var );
    if( SUCCEEDED(hr) && var.vt == VT_BOOL )
    {	
        out = ( var.boolVal != FALSE );
    }
    VariantClear( &var );
    return out;
}

#endif

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
bool Environment::outputSystemInfo()
{
#if defined(LNOTE_WIN32)
    HRESULT hr;   
    IDxDiagProvider* pDxDiagProvider = NULL;
    IDxDiagContainer* pDxDiagRoot = NULL;
    IDxDiagContainer* pDxChild = NULL;

    bool co_initialized = false;
    hr = ::CoInitialize( NULL );
    if( SUCCEEDED( hr ) ) co_initialized = true;

    hr = CoCreateInstance( CLSID_DxDiagProvider,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_IDxDiagProvider,
                          (LPVOID*) &pDxDiagProvider );
    if( SUCCEEDED(hr) )
    {
        DXDIAG_INIT_PARAMS dxDiagInitParam;
        ZeroMemory( &dxDiagInitParam, sizeof(DXDIAG_INIT_PARAMS) );
        dxDiagInitParam.dwSize                  = sizeof(DXDIAG_INIT_PARAMS);
        dxDiagInitParam.dwDxDiagHeaderVersion   = DXDIAG_DX9_SDK_VERSION;
        dxDiagInitParam.bAllowWHQLChecks        = false;
        dxDiagInitParam.pReserved               = NULL;

        hr = pDxDiagProvider->Initialize( &dxDiagInitParam );
        if( SUCCEEDED(hr) )
        {
            // ルート コンテナにする IDxDiagContainer オブジェクトを作成し、初期化する。
            hr = pDxDiagProvider->GetRootContainer( &pDxDiagRoot );
            if( SUCCEEDED(hr) )
            {
                // DirectX診断ツールのシステムタブの情報を取得する
                hr = pDxDiagRoot->GetChildContainer( L"DxDiag_SystemInfo", &pDxChild );
                if( SUCCEEDED(hr) )
                {
                    Base::Logger::println( Base::Logger::LEVEL_INFO, _T("-------- SystemInfo --------") );
                    
                    // OSバージョン
                    Base::Logger::println(
                        Base::Logger::LEVEL_INFO,
                        _T("    OS              : %s"),
                        getDxDiagInfoString( pDxChild, L"szOSExLocalized" ).c_str() );

                    // CPU
                    Base::Logger::println(
                        Base::Logger::LEVEL_INFO,
                        _T("    CPU             : %s"),
                        getDxDiagInfoString( pDxChild, L"szProcessorEnglish" ).c_str() );

                    // SystemMemoryの容量
                    Base::Logger::println(
                        Base::Logger::LEVEL_INFO,
                        _T("    SystemMemory    : %s"),
                        getDxDiagInfoString( pDxChild, L"szPhysicalMemoryEnglish" ).c_str() );

                    // DirectXランタイムのバージョン
                    Base::Logger::println(
                        Base::Logger::LEVEL_INFO,
                        _T("    DirectX         : %s"),
                        getDxDiagInfoString( pDxChild, L"szDirectXVersionLongEnglish" ).c_str() );

                    // DirectXMode
                    Base::Logger::println(
                        Base::Logger::LEVEL_INFO,
                        _T("    DirectXMode     : %s"),
                        getDxDiagInfoBool( pDxChild, L"bIsD3DDebugRuntime" ) ? "DebugRuntime" : "RetailRuntime" );

                    pDxChild->Release();
                }

                // ディスプレイタブの情報を取得する
                hr = pDxDiagRoot->GetChildContainer( L"DxDiag_DisplayDevices", &pDxChild );
                if( SUCCEEDED(hr) )
                {
                    DWORD DisplayCnt;
                    WCHAR wszContainer[100];
                    IDxDiagContainer* pDxDisplay = NULL;
               
                    //ディスプレイアダプタの数を取得する。
                    pDxChild->GetNumberOfChildContainers( &DisplayCnt );
                    for( DWORD i=0; i < DisplayCnt; i++ )
                    {
                        Base::Logger::println( Base::Logger::LEVEL_INFO, _T("-------- Display Adapter : %d --------"), i );

                        //ディスプレイの列挙
                        hr = pDxChild->EnumChildContainerNames( i, wszContainer, 100 );
                        if( SUCCEEDED( hr ) )
                        {
                            hr = pDxChild->GetChildContainer( wszContainer, &pDxDisplay );
                            if( SUCCEEDED( hr ) )
                            {
                                // チップの種類
                                Base::Logger::println(
                                    Base::Logger::LEVEL_INFO,
                                    _T("    Chip            : %s"),
                                    getDxDiagInfoString( pDxDisplay, L"szChipType" ).c_str() );

                                // VRAM
                                Base::Logger::println(
                                    Base::Logger::LEVEL_INFO,
                                    _T("    VRAM            : %s"),
                                    getDxDiagInfoString( pDxDisplay, L"szDisplayMemoryEnglish" ).c_str() );

                                // DDI
                                Base::Logger::println(
                                    Base::Logger::LEVEL_INFO,
                                    _T("    DDI             : %s"),
                                    getDxDiagInfoString( pDxDisplay, L"szDDIVersionEnglish" ).c_str() );

                                // DirectDrawアクセラレータ
                                Base::Logger::println(
                                    Base::Logger::LEVEL_INFO,
                                    _T("    DDAcceleration  : %s"),
                                    getDxDiagInfoBool( pDxDisplay, L"bDDAccelerationEnabled" ) ? _T( "Enable" ) : _T( "Disable" ) );

                                // Direct3Dアクセラレータ
                                Base::Logger::println(
                                    Base::Logger::LEVEL_INFO,
                                    _T("    3DAcceleration  : %s"),
                                    getDxDiagInfoBool( pDxDisplay, L"b3DAccelerationEnabled" ) ? _T( "Enable" ) : _T( "Disable" ) );

                                // AGPアクセラレータ
                                Base::Logger::println(
                                    Base::Logger::LEVEL_INFO,
                                    _T("    AGP             : %s"),
                                    getDxDiagInfoBool( pDxDisplay, L"bAGPEnabled" ) ? _T( "Enable" ) : _T( "Disable" ) );
                            
                                pDxDisplay->Release();
                            }
                        }
                    }
                    pDxChild->Release();
                }
                pDxDiagRoot->Release();
            }
        }
        pDxDiagProvider->Release();
    }

    if ( co_initialized )
        CoUninitialize();
#endif
    return true;
}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	HWND Environment::getWindowHandle( Window* window )
	{
		HWND hwnd = NULL;
		if ( window != NULL )
		{
			if (dynamic_cast<Win32WindowBase*>(window) != NULL) {
				hwnd = ((Win32WindowBase*)window)->getWindowHandle();
			}
			else if (dynamic_cast<GLFWWindow*>(window) != NULL) {
				hwnd = ((GLFWWindow*)window)->getWindowHandle();
			}
		}
		return hwnd;
	}

//==============================================================================
// MessageBox
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void MessageBox::showError( Window* ownerWindow, const wchar_t* text )
	{
		::MessageBoxW( Environment::getWindowHandle( ownerWindow ), text, L"Error", MB_OK | MB_ICONERROR );
	}


} // namespace System
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================