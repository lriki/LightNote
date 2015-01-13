﻿//=============================================================================
//【 Environment 】
//=============================================================================

#include "stdafx.h"
#include "../../Base/StringUtil.h"
#include "../Environment.h"

namespace LNote
{
namespace Core
{
namespace System
{

//=============================================================================
// ■ Environment
//=============================================================================

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	lnU32 Environment::getSystemTime()
	{
	    timespec ts;
	    clock_gettime( CLOCK_MONOTONIC, &ts );
	    return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
	    //return ( (uint64_t) ts.tv_sec * (uint64_t) 1000000000 + (uint64_t) ts.tv_nsec ) / 100000;
	    //return (uint64_t)ts.tv_sec * (uint64_t)10000 + (uint64_t)ts.tv_nsec;
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void Environment::getCurrentDirectory( lnChar* buf_ )
	{
#ifdef LNOTE_UNICODE
	    char buf[ LN_MAX_PATH ];
	    ::getcwd( buf_, LN_MAX_PATH );
	    Base::StringUtil::convMultiToWide( buf_, LN_MAX_PATH, buf );
#else
	    ::getcwd( buf_, LN_MAX_PATH );
#endif
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void Environment::showMessageBox( IWindow* parentWindow, const lnChar* message_, const lnChar* title_ )
	{
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
	}
		
	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void Environment::showMessageBox( IWindow* parentWindow, const wchar_t* message, const wchar_t* title )
	{
		printf("Environment::showMessageBox  not impl\n");
	}

	//---------------------------------------------------------------------
	// 
	//---------------------------------------------------------------------
	float Environment::getPhysicalPixelSize()
	{
		printf("Environment::getPhysicalPixelSize  not impl\n");
	}
	
	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	bool Environment::outputSystemInfo()
	{
		return true;
	}

} // namespace System
} // namespace Core
} // namespace LNote

//=============================================================================
//
//=============================================================================