//==============================================================================
// Win32WindowManager 
//------------------------------------------------------------------------------
///**
//  @file       Win32WindowManager.h
//  @brief      Win32WindowManager
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Common.h"
#include "../Common/WindowManagerBase.h"
#include "../Common/EventListenerList.h"

namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// ■ Win32WindowManager
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class Win32WindowManager
    : public WindowManagerBase
{
public:

	/// 初期化データ
	struct SettingData
	{
        bool		    UseConsole;     ///< コンソールを割り当てる場合 true
        void*           WindowHandle;   ///< ユーザー定義のウィンドウハンドル
        const lnChar*	TitleText;		///< ウィンドウタイトルの文字列
		int				Width;			///< クライアント領域の幅
		int			    Height;			///< クライアント領域の高さ
		bool		    Windowed;		///< ウィンドウモードで初期化する場合 true
		bool			Resizable;
    };

public:
    Win32WindowManager( Manager* manager );
	virtual ~Win32WindowManager();

public:

	/// 前初期化 (call by Main Thread)
	void preInitialize( const SettingData& setting_data );

	/// イベントリスナーのアタッチ (通常、WindowManager をキャストしないと使えない。必ずデタッチすること)
	void attachWin32MessageListener( IWin32MessageListener* listener, int priority ) { mWin32MessageListenerList.attach( listener, priority ); }

	/// イベントリスナーのデタッチ
	void detachWin32MessageListener( IWin32MessageListener* listener ) { mWin32MessageListenerList.detach( listener ); }

public:
    virtual void				initialize();
	virtual void				finalize();
    virtual void				processMessage();
	virtual Window*				getMainWindow() const;
	virtual WindowBaseArray&	getWindowBaseArray() { return mWindowArray; }

private:
	LRESULT sendWndMsgToAttachedListener( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, bool* handled );

private:
	typedef EventListenerList<IWin32MessageListener*> Win32MessageListenerList;

	SettingData					mSettingData;
	Win32WindowBase*			mMainWindow;
	bool						mConsoleAlloced;
	bool						mSystemMouseShown;
	Win32MessageListenerList	mWin32MessageListenerList;
};

//==============================================================================
// ■ IWin32MessageListener
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class IWin32MessageListener
{
public:

	//----------------------------------------------------------------------
	///**
	//	@brief      メッセージ処理
	//
	//  @param[out] handled	: メッセージの処理済みを伝える場合は true を返すようにする
	//
	//  @par
	//              この関数は LNote が windows のメッセージ処理を始める前に呼ばれます。<br>
	//              LNote が処理するべきメッセージ含めて全てのメッセージが送られて来ます。<br>
	//              <br>
	//              この関数を使ってメッセージを処理した後、呼び出し側で
	//              DefWindowProc() や他の処理を呼び出す必要がない場合 (WndProc() 終了後、何もしてほしくないとき) は
	//              handled_ に true を返してください。<br>
	//              この関数が終了した後すぐにプロシージャ関数を抜けるようになります。
	//*/	
	//----------------------------------------------------------------------
    virtual LRESULT WndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, bool* handled ) = 0;

};

} // namespace System
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================