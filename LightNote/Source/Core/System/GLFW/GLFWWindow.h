//==============================================================================
// GLFWWindow 
//------------------------------------------------------------------------------
///**
//  @file       GLFWWindow.h
//  @brief      GLFWWindow
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <string>

#ifdef LNOTE_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <glfw/include/GLFW/glfw3.h>
#endif
#include <glfw/include/GLFW/glfw3native.h>
#include "../../System/Common.h"
#include "../Common.h"
#include "../Interface.h"
#include "../Common/WindowBase.h"

namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// ■ GLFWWindow
//------------------------------------------------------------------------------
///**
//  @brief 
//*/
//==============================================================================
class GLFWWindow
	: public WindowBase
{
public:

	/// 初期化データ
	struct SettingData
	{
		const lnChar*	        TitleText;		///< ウィンドウタイトルの文字列
		int				        Width;			///< クライアント領域の幅
		int				        Height;			///< クライアント領域の高さ
		bool			        Windowed;		///< ウィンドウモードで初期化する場合 true
		bool					Resizable;
    };

public:
	GLFWWindow( GLFWWindowManager* manager );
	virtual ~GLFWWindow();

public:

	/// 初期化
    void initialize( const SettingData& setting_data );

public:
    virtual void					finalize();
    virtual const Geometry::Size&	getSize();
    virtual void					setVisible( bool flag );
	virtual void					setEnableFullScreen( bool flag_ );
    virtual bool					isFullScreen() { return mFullScreen; }
    virtual bool					isActive() { return mIsActive; }
	virtual void					setWindowTextFooter( const lnChar* format, ... );
    virtual void					captureMouse();
    virtual void					releaseMouseCapture();

public:
	GLFWwindow* getGLFWWindow() { return mGLFWWindow; }
#ifdef LNOTE_WIN32
	HWND getWindowHandle() { return glfwGetWin32Window( mGLFWWindow ); }
#endif

private:

	/// ウィンドウサイズを変更する
    void _resize( bool fullscreen );

	static void window_close_callback( GLFWwindow* window );
	static void window_focus_callback( GLFWwindow* window, int focused );	// ウィンドウアクティブ化/非アクティブ化
	static void window_key_callback( GLFWwindow* window, int key, int scancode, int action, int mods );

private:
	GLFWWindowManager*	mWindowManager;
	GLFWwindow*			mGLFWWindow;
	int					mLastMouseX;
	int					mLastMouseY;
    bool                mIsActive;

	lnString    		mTitleText;			///< ウィンドウタイトルの文字列    
    Geometry::Size		mClientSize;        ///< クライアント領域の大きさ
    int					mOrginalWidth;      ///< initialize() または setSize() で設定されたクライアント領域の幅
    int					mOrginalHeight;     ///< initialize() または setSize() で設定されたクライアント領域の高さ
    bool				mFullScreen;        ///< フルスクリーンモード ( 用のスタイル ) の場合 true
};


} // namespace System
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================