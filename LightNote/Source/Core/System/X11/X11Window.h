//=============================================================================
//【 Win32Window 】
//-----------------------------------------------------------------------------
///**
//  @file       Win32Window.h
//  @brief      Win32Window
//  @author     Riki
//*/
//=============================================================================

#pragma once

#include <string>
#include "../Common.h"
#include "../Interface.h"
#include "../Common/WindowBase.h"

namespace LNote
{
namespace Core
{
namespace System
{
class X11WindowManager;

//=============================================================================
// ■ X11Window
//-----------------------------------------------------------------------------
///**
//  @brief 
//*/
//=============================================================================
class X11Window
	: public WindowBase
{
public:

	/// 初期化データ
	struct ConfigData
	{
		const lnChar*	        TitleText;		///< ウィンドウタイトルの文字列
		int				        Width;			///< クライアント領域の幅
		int				        Height;			///< クライアント領域の高さ
		bool			        Windowed;		///< ウィンドウモードで初期化する場合 true
    };

public:
	X11Window( X11WindowManager* manager );
	virtual ~X11Window();

public:

	/// 初期化
    void initialize( const ConfigData& configData );

public:
    virtual void					finalize();
    virtual const Geometry::Size&	getSize();
    virtual void					setVisible( bool flag );
	virtual void					setEnableFullScreen( bool flag_ );
    virtual bool					isFullScreen() { return mFullScreen; }
	virtual void					setWindowTextFooter( const lnChar* format, ... );
    virtual void					captureMouse();
    virtual void					releaseMouseCapture();

private:

	lnString    				mTitleText;			///< ウィンドウタイトルの文字列
    Geometry::Size				mClientSize;        ///< クライアント領域の大きさ
    int                         mOrginalWidth;      ///< initialize() または setSize() で設定されたクライアント領域の幅
    int                         mOrginalHeight;     ///< initialize() または setSize() で設定されたクライアント領域の高さ
	//HWND				        mWindowHandle;		///< ウィンドウハンドル
	//RECT				        mWindowRect;		///< ウィンドウ領域
    //HACCEL                      mAccelerators;      ///< アクセラレータ (Alt+Enter の警告音対策のために使ってる)
    bool                        mFullScreen;        ///< フルスクリーンモード ( 用のスタイル ) の場合 true
};

} // namespace System
} // namespace Core
} // namespace LNote

//=============================================================================
//
//=============================================================================