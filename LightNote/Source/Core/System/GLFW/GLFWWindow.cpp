//==============================================================================
// GLFWWindow 
//==============================================================================

#include "stdafx.h"
#include "../../Base/StringUtils.h"
#include "../Manager.h"
#include "GLFWWindowManager.h"
#include "GLFWWindow.h"

namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// ■ GLFWWindow
//==============================================================================

	static const int		MAX_WINDOW_TEXT = 256;			///< ウィンドウタイトル文字列の最大文字数
	static const int		MAX_WINDOW_TEXT_FOOTER = 64;	///< ウィンドウタイトル文字列のフッタ情報の最大文字数

	static lnS16 GLFWKeyToLNKey[GLFW_KEY_LAST] = { 0 };
	static bool mInitedKeyTable = false;
	static void initKeyTable()
	{
		if ( mInitedKeyTable ) return;

		GLFWKeyToLNKey[LN_KEY_UNKNOWN] = GLFW_KEY_UNKNOWN;

		GLFWKeyToLNKey[GLFW_KEY_A] = LN_KEY_A;
		GLFWKeyToLNKey[GLFW_KEY_B] = LN_KEY_B;
		GLFWKeyToLNKey[GLFW_KEY_C] = LN_KEY_C;
		GLFWKeyToLNKey[GLFW_KEY_B] = LN_KEY_D;
		GLFWKeyToLNKey[GLFW_KEY_E] = LN_KEY_E;
		GLFWKeyToLNKey[GLFW_KEY_F] = LN_KEY_F;
		GLFWKeyToLNKey[GLFW_KEY_G] = LN_KEY_G;
		GLFWKeyToLNKey[GLFW_KEY_H] = LN_KEY_H;
		GLFWKeyToLNKey[GLFW_KEY_I] = LN_KEY_I;
		GLFWKeyToLNKey[GLFW_KEY_J] = LN_KEY_J;
		GLFWKeyToLNKey[GLFW_KEY_K] = LN_KEY_K;
		GLFWKeyToLNKey[GLFW_KEY_L] = LN_KEY_L;
		GLFWKeyToLNKey[GLFW_KEY_M] = LN_KEY_M;
		GLFWKeyToLNKey[GLFW_KEY_N] = LN_KEY_N;
		GLFWKeyToLNKey[GLFW_KEY_O] = LN_KEY_O;
		GLFWKeyToLNKey[GLFW_KEY_P] = LN_KEY_P;
		GLFWKeyToLNKey[GLFW_KEY_Q] = LN_KEY_Q;
		GLFWKeyToLNKey[GLFW_KEY_R] = LN_KEY_R;
		GLFWKeyToLNKey[GLFW_KEY_S] = LN_KEY_S;
		GLFWKeyToLNKey[GLFW_KEY_T] = LN_KEY_T;
		GLFWKeyToLNKey[GLFW_KEY_U] = LN_KEY_U;
		GLFWKeyToLNKey[GLFW_KEY_V] = LN_KEY_V;
		GLFWKeyToLNKey[GLFW_KEY_W] = LN_KEY_W;
		GLFWKeyToLNKey[GLFW_KEY_X] = LN_KEY_X;
		GLFWKeyToLNKey[GLFW_KEY_Y] = LN_KEY_Y;
		GLFWKeyToLNKey[GLFW_KEY_Z] = LN_KEY_Z;

		GLFWKeyToLNKey[GLFW_KEY_0] = LN_KEY_0;
		GLFWKeyToLNKey[GLFW_KEY_1] = LN_KEY_1;
		GLFWKeyToLNKey[GLFW_KEY_2] = LN_KEY_2;
		GLFWKeyToLNKey[GLFW_KEY_3] = LN_KEY_3;
		GLFWKeyToLNKey[GLFW_KEY_4] = LN_KEY_4;
		GLFWKeyToLNKey[GLFW_KEY_5] = LN_KEY_5;
		GLFWKeyToLNKey[GLFW_KEY_6] = LN_KEY_6;
		GLFWKeyToLNKey[GLFW_KEY_7] = LN_KEY_7;
		GLFWKeyToLNKey[GLFW_KEY_8] = LN_KEY_8;
		GLFWKeyToLNKey[GLFW_KEY_9] = LN_KEY_9;

		GLFWKeyToLNKey[GLFW_KEY_F1] = LN_KEY_F1;
		GLFWKeyToLNKey[GLFW_KEY_F2] = LN_KEY_F2;
		GLFWKeyToLNKey[GLFW_KEY_F3] = LN_KEY_F3;
		GLFWKeyToLNKey[GLFW_KEY_F4] = LN_KEY_F4;
		GLFWKeyToLNKey[GLFW_KEY_F5] = LN_KEY_F5;
		GLFWKeyToLNKey[GLFW_KEY_F6] = LN_KEY_F6;
		GLFWKeyToLNKey[GLFW_KEY_F7] = LN_KEY_F7;
		GLFWKeyToLNKey[GLFW_KEY_F8] = LN_KEY_F8;
		GLFWKeyToLNKey[GLFW_KEY_F9] = LN_KEY_F9;
		GLFWKeyToLNKey[GLFW_KEY_F10] = LN_KEY_F10;
		GLFWKeyToLNKey[GLFW_KEY_F11] = LN_KEY_F11;
		GLFWKeyToLNKey[GLFW_KEY_F12] = LN_KEY_F12;

		GLFWKeyToLNKey[GLFW_KEY_SPACE] = LN_KEY_SPACE;
		GLFWKeyToLNKey[GLFW_KEY_ESCAPE] = LN_KEY_ESCAPE;
		GLFWKeyToLNKey[GLFW_KEY_UP] = LN_KEY_UP;
		GLFWKeyToLNKey[GLFW_KEY_DOWN] = LN_KEY_DOWN;
		GLFWKeyToLNKey[GLFW_KEY_LEFT] = LN_KEY_LEFT;
		GLFWKeyToLNKey[GLFW_KEY_RIGHT] = LN_KEY_RIGHT;
		GLFWKeyToLNKey[GLFW_KEY_LEFT_SHIFT] = LN_KEY_LSHIFT;
		GLFWKeyToLNKey[GLFW_KEY_RIGHT_SHIFT] = LN_KEY_RSHIFT;
		GLFWKeyToLNKey[GLFW_KEY_LEFT_CONTROL] = LN_KEY_LCTRL;
		GLFWKeyToLNKey[GLFW_KEY_RIGHT_CONTROL] = LN_KEY_RCTRL;
		GLFWKeyToLNKey[GLFW_KEY_LEFT_ALT] = LN_KEY_LALT;
		GLFWKeyToLNKey[GLFW_KEY_RIGHT_ALT] = LN_KEY_RALT;
		GLFWKeyToLNKey[GLFW_KEY_TAB] = LN_KEY_TAB;
		GLFWKeyToLNKey[GLFW_KEY_ENTER] = LN_KEY_ENTER;
		GLFWKeyToLNKey[GLFW_KEY_BACKSPACE] = LN_KEY_BACKSPACE;
		GLFWKeyToLNKey[GLFW_KEY_INSERT] = LN_KEY_INSERT;
		GLFWKeyToLNKey[GLFW_KEY_DELETE] = LN_KEY_DELETE;
		GLFWKeyToLNKey[GLFW_KEY_PAGE_UP] = LN_KEY_PAGEUP;
		GLFWKeyToLNKey[GLFW_KEY_PAGE_DOWN] = LN_KEY_PAGEDOWN;
		GLFWKeyToLNKey[GLFW_KEY_HOME] = LN_KEY_HOME;
		GLFWKeyToLNKey[GLFW_KEY_END] = LN_KEY_END;

		GLFWKeyToLNKey[GLFW_KEY_SEMICOLON] = LN_KEY_COLON;
		GLFWKeyToLNKey[GLFW_KEY_EQUAL] = LN_KEY_SEMICOLON;
		GLFWKeyToLNKey[GLFW_KEY_COMMA] = LN_KEY_COMMA;
		GLFWKeyToLNKey[GLFW_KEY_PERIOD] = LN_KEY_PERIOD;
		GLFWKeyToLNKey[GLFW_KEY_SLASH] = LN_KEY_SLASH;
		GLFWKeyToLNKey[GLFW_KEY_MINUS] = LN_KEY_MINUS;
		GLFWKeyToLNKey[GLFW_KEY_BACKSLASH] = LN_KEY_BACKSLASH;
		GLFWKeyToLNKey[GLFW_KEY_WORLD_2] = LN_KEY_YEN;
		GLFWKeyToLNKey[GLFW_KEY_APOSTROPHE] = LN_KEY_CARET;
		GLFWKeyToLNKey[GLFW_KEY_LEFT_BRACKET] = LN_KEY_LBRACKET;
		GLFWKeyToLNKey[GLFW_KEY_RIGHT_BRACKET] = LN_KEY_RBRACKET;

		mInitedKeyTable = true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	GLFWWindow::GLFWWindow( GLFWWindowManager* manager )
		: WindowBase		( manager )
		, mWindowManager	( manager )
		, mLastMouseX		( -1 )
		, mLastMouseY		( -1 )
		, mIsActive			( false )
		, mOrginalWidth     ( 640 )
		, mOrginalHeight    ( 480 )
		, mFullScreen       ( false )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	GLFWWindow::~GLFWWindow()
	{
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::initialize( const SettingData& setting_data )
	{
		mTitleText      = setting_data.TitleText;
		mClientSize.w   = setting_data.Width;
		mClientSize.h   = setting_data.Height;
		mFullScreen     = !setting_data.Windowed;
		mOrginalWidth   = mClientSize.w;
		mOrginalHeight  = mClientSize.h;

		initKeyTable();

		
		
		// OpenGL Version 3.2 を選択
		// http://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20120908
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE );
		glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE );

		// とりあえず生成時固定。GLFW は実行中に変更したければ、
		// glfwCreateWindow() で新しいウィンドウを開きなおす必要がある。
		if ( mFullScreen )
		{
			glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
			glfwWindowHint( GLFW_DECORATED, GL_FALSE );
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* vidmode = glfwGetVideoMode( monitor );
			mGLFWWindow = glfwCreateWindow( vidmode->width, vidmode->height, mTitleText.c_str(), NULL/*glfwGetPrimaryMonitor()*/, NULL );
		}
		else
		{
			glfwWindowHint( GLFW_RESIZABLE, (setting_data.Resizable) ? GL_TRUE : GL_FALSE );
			glfwWindowHint( GLFW_DECORATED, GL_TRUE );
			mGLFWWindow = glfwCreateWindow( mClientSize.w, mClientSize.h, mTitleText.c_str(), NULL, NULL );
		}
		LN_THROW_SystemCall( mGLFWWindow );

		mIsActive = true;

		glfwSetWindowUserPointer( mGLFWWindow, this );
		glfwSetWindowCloseCallback( mGLFWWindow, window_close_callback );
		glfwSetWindowFocusCallback( mGLFWWindow, window_focus_callback );
		glfwSetKeyCallback( mGLFWWindow, window_key_callback );
		
		glfwMakeContextCurrent( mGLFWWindow );
		glewInit();
		//GLuint mProgram = glCreateProgram();// LN_CHECK_GLERROR();
		// 初回クリア (しておかないと、背景が透明なままになる)
		glClear( GL_COLOR_BUFFER_BIT );
        glfwSwapBuffers( mGLFWWindow );

		//printf( "GL_EXTENSIONS: %s\n", glGetString( GL_EXTENSIONS ) );
  //  printf( "GL_VERSION: %s\n", glGetString( GL_VERSION ) );
  //  printf( "GL_REBDER: %s\n", glGetString( GL_RENDERER ) );
  //  printf( "GL_VENDOR: %s\n", glGetString( GL_VENDOR ) );

		// ウィンドウの大きさを変更する
		//_resize( !setting_data.Windowed );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::finalize()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	const Geometry::Size& GLFWWindow::getSize()
	{
		int w = 0;
		int h = 0;
		glfwGetWindowSize( mGLFWWindow, &w, &h );
		mClientSize.set( w, h );
		return mClientSize;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::setVisible( bool flag )
	{
		if ( flag ) {
			glfwShowWindow( mGLFWWindow );
		}
		else {
			glfwHideWindow( mGLFWWindow );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::setEnableFullScreen( bool flag )
	{
		LN_THROW_NotImpl( 0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::setWindowTextFooter( const lnChar* format, ... )
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
		
		glfwSetWindowTitle( mGLFWWindow, text );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::_resize( bool fullscreen )
	{
		LN_THROW_NotImpl( 0 );

		//int x, y, sw, sh;
		//int win_w = mOrginalWidth;
		//int win_h = mOrginalHeight;

		
			

			//glfwWindowHint( GLFW_DECORATED , GL_TRUE );
			//glfwGetWindowAttrib
		// フルスクリーンモードの場合
		if ( fullscreen )
		{
			//glfwGetDesktopMode();
			
		}
		// ウィンドウモードの場合は実際にウィンドウの位置を設定する
		else
		{
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::captureMouse()
	{
		//glfwSetInputMode
	}
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::releaseMouseCapture()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::window_close_callback( GLFWwindow* glfw_window )
	{
		GLFWWindow* thisWindow = (GLFWWindow*)glfwGetWindowUserPointer( glfw_window );

		EventArgs e;
		e.Type			= LN_EVENT_CLOSE;
		e.Sender		= thisWindow;
		e.StructSize	= sizeof(EventArgs);
		thisWindow->mWindowManager->getManager()->postEventFromWindowThread( &e );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::window_focus_callback( GLFWwindow* glfw_window, int focused )
	{
		GLFWWindow* thisWindow = (GLFWWindow*)glfwGetWindowUserPointer( glfw_window );
		thisWindow->mIsActive = (focused != 0);

		EventArgs e;
		e.Type			= ( thisWindow->mIsActive ) ? LN_EVENT_APP_ACTIVATE : LN_EVENT_APP_DEACTIVATE;
		e.Sender		= thisWindow;
		e.StructSize	= sizeof(EventArgs);
		thisWindow->mWindowManager->getManager()->postEventFromWindowThread( &e );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::window_key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
	{
		GLFWWindow* thisWindow = (GLFWWindow*)glfwGetWindowUserPointer( window );

		if ( action == GLFW_PRESS || action == GLFW_REPEAT )
		{
			KeyEventArgs e;
			e.Type			= LN_EVENT_KEY_DOWN;
			e.Sender		= thisWindow;
			e.StructSize	= sizeof(KeyEventArgs);
			e.KeyCode		= GLFWKeyToLNKey[key];
			e.IsAlt			= (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT);
			e.IsShift		= (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT);
			e.IsControl		= (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL);
			thisWindow->mWindowManager->getManager()->postEventFromWindowThread( (EventArgs*)&e );
		}
		else if ( action == GLFW_RELEASE )
		{
			KeyEventArgs e;
			e.Type			= LN_EVENT_KEY_UP;
			e.Sender		= thisWindow;
			e.StructSize	= sizeof(KeyEventArgs);
			e.KeyCode		= GLFWKeyToLNKey[key];
			e.IsAlt			= (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT);
			e.IsShift		= (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT);
			e.IsControl		= (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL);
			thisWindow->mWindowManager->getManager()->postEventFromWindowThread( (EventArgs*)&e );
		}
	}

} // namespace System
} // namespace Core
} // namespace LNote


//==============================================================================
//
//==============================================================================