//==============================================================================
// Win32Window 
//------------------------------------------------------------------------------
///**
//  @file       Win32Window.h
//  @brief      Win32Window
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <string>
#include "../Common.h"
#include "../Interface.h"
#include "../Common/WindowBase.h"

#if defined(LNOTE_WIN32)

namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// ■ Win32WindowBase
//------------------------------------------------------------------------------
///**
//  @brief      ライブラリで必要なウィンドウメッセージを処理するプロシージャを持った基底
//*/
//==============================================================================
class Win32WindowBase
    : public WindowBase
{
public:
    Win32WindowBase( Win32WindowManager* manager );
    virtual ~Win32WindowBase();

public:

	/// ダミーウィンドウかを判定する
    virtual bool isDummy() const = 0;

	/// プロシージャ関数 (通常ウィンドウの場合はシステムから呼ばれ、ダミーの場合はWindowManagerから呼ばれる)
    virtual LRESULT WndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

	/// ウィンドウハンドルの取得
    virtual HWND getWindowHandle() = 0;

public:
    virtual void finalize() {}
    virtual bool isActive() { return mIsActive; }

protected:

    Win32WindowManager*	mWindowManager;
	int					mLastMouseX;
	int					mLastMouseY;
    bool                mIsActive;
};

//==============================================================================
// ■ Win32Window
//------------------------------------------------------------------------------
///**
//  @brief 
//*/
//==============================================================================
class Win32Window
	: public Win32WindowBase
{
public:

	/// 初期化データ
	struct SettingData
	{
		const lnChar*	        TitleText;		///< ウィンドウタイトルの文字列
		int				        Width;			///< クライアント領域の幅
		int				        Height;			///< クライアント領域の高さ
		bool			        Windowed;		///< ウィンドウモードで初期化する場合 true
        const lnChar*	        WinClassName;   ///< ウィンドウクラスの名前 または NULL ( NULL の場合、L"_LNote_" が使われる )
        int                     IconResourceID; ///< タイトルバーのアイコン識別子 (IDI_ICON1 等)
		bool					Resizable;
    };

public:

	static const lnChar*	WINDOW_CLASS_NAME;
	static const lnChar*	PROP_WINPROC;
	static const DWORD		WINDOWED_STYLE;
	static const DWORD		FULLSCREEN_STYLE;

public:
	Win32Window( Win32WindowManager* manager );
	virtual ~Win32Window();

public:

	/// 初期化
    void initialize( const SettingData& setting_data );

public:
    virtual void					finalize();
    virtual const Geometry::Size&	getSize();
    virtual void					setVisible( bool flag );
	virtual void					setEnableFullScreen( bool flag_ );
    virtual bool					isFullScreen() { return mFullScreen; }
	virtual void					setWindowTextFooter( const lnChar* format, ... );
    virtual void					captureMouse();
    virtual void					releaseMouseCapture();
    virtual HWND					getWindowHandle() { return mWindowHandle; }

	virtual bool	isDummy() const { return false; }
	virtual LRESULT WndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

private:

	/// ウィンドウサイズを変更する
    void _resize( bool fullscreen );

	/// 呼び出し元プロシージャ関数 (ウィンドウを作成したスレッド内で呼ばれる)
	static LRESULT CALLBACK _staticMsgProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

private:

	lnString    				mTitleText;			///< ウィンドウタイトルの文字列    
    Geometry::Size				mClientSize;        ///< クライアント領域の大きさ
    int                         mOrginalWidth;      ///< initialize() または setSize() で設定されたクライアント領域の幅
    int                         mOrginalHeight;     ///< initialize() または setSize() で設定されたクライアント領域の高さ
	HWND				        mWindowHandle;		///< ウィンドウハンドル
	RECT				        mWindowRect;		///< ウィンドウ領域
    HACCEL                      mAccelerators;      ///< アクセラレータ (Alt+Enter の警告音対策のために使ってる)
	DWORD						mWindowedStyle;		///< ウィンドウモード時のスタイル
    bool                        mFullScreen;        ///< フルスクリーンモード ( 用のスタイル ) の場合 true
};

//==============================================================================
// ■ Win32WindowHost
//------------------------------------------------------------------------------
///**
//  @brief      ウィンドウハンドルをラップするクラス
//*/
//==============================================================================
class Win32WindowHost
	: public Win32WindowBase
{
public:
	Win32WindowHost( Win32WindowManager* manager );
	virtual ~Win32WindowHost();

public:

	/// 初期化
	void initialize( HWND hwnd );

public:
	virtual const Geometry::Size& getSize();
    virtual void setVisible( bool flag_ ) {}
    virtual void setEnableFullScreen( bool flag_ ) {}
    virtual bool isFullScreen() { return false; }
    virtual bool isActive() { return mIsActive; }
    virtual void setWindowTextFooter( const lnChar* format, ... ) {}
    virtual HWND getWindowHandle() { return mWindowHandle; }
    virtual void captureMouse() { ::SetCapture( mWindowHandle ); }
    virtual void releaseMouseCapture() { ::ReleaseCapture(); }

    virtual bool isDummy() const { return true; }

private:

    HWND			mWindowHandle;
    Geometry::Size	mClientSize;
};


} // namespace System
} // namespace Core
} // namespace LNote

#endif // LNOTE_WIN32

//==============================================================================
//
//==============================================================================