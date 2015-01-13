//==============================================================================
// Manager 
//------------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <deque>
#include "../Threading/Thread.h"
#include "../Threading/Mutex.h"
#include "../Threading/EventFlag.h"
#include "Common.h"
#include "Interface.h"

#if defined(LNOTE_ANDROID)
struct android_app;
#endif

namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// ■ Manager クラス
//------------------------------------------------------------------------------
///**
//  @brief		システム (OS 周り) 管理クラス
//*/
//==============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

	/// 初期化データ
	struct SettingData
	{
		WindowSystemAPI		SystemAPI;
        bool			    UseConsole;         ///< [win32] コンソールを割り当てる場合 true
        bool                UseThread;          ///< [win32] メッセージ処理に専用のスレッドを使う場合は true
        const lnChar*	    WindowTitleText;    ///< [win32] ウィンドウタイトルの文字列
        lnU32               WindowWidth;        ///< ウィンドウのクライアント領域の幅
        lnU32               WindowHeight;       ///< ウィンドウのクライアント領域の高さ
        bool			    Windowed;		    ///< [win32] ウィンドウモードで初期化する場合 true
		bool				WindowResizeEnabled;///< リサイズ可能か
        void*               WindowHandle;       ///< [win32] ユーザー定義のウィンドウハンドル (windows の場合は HWND、X11 は Window*。ただし、X11 は未対応)
#if defined(LNOTE_ANDROID)
        android_app*        AndroidApp;
#endif
    };

public:
    Manager();
	virtual ~Manager();
    LN_REFOBJ_METHODS;

public:

	/// 初期化
    void initialize( const SettingData& setting_data );

	/// 終了処理
    void finalize();

	/// メインウィンドウを取得する
	Window* getMainWindow();

	/// ウィンドウマネージャの取得
	WindowManagerBase* getWindowManager() { return mWindowManager; }

	/// メッセージ処理
	bool processMessage();

public:

	void postEventFromWindowThread( const EventArgs* args );

private:

	class WndProcThread
		: public Threading::Thread
	{
	public:
		WndProcThread( Manager* manager ) : mManager( manager ) {}
		virtual ~WndProcThread() {}
		virtual void run();

	public:
		void waitFinishInitialize() { mInitFinished.wait(); }
		void requestEnd() { mEndRequested.setTrue(); }

	private:
		Manager*				mManager;
		Threading::EventFlag	mInitFinished;
		Threading::EventFlag	mEndRequested;
	};

private:

	friend class WndProcThread;
	WindowManagerBase*		mWindowManager;
	WndProcThread*			mWndProcThread;
	Threading::Mutex		mEventQueueLock;
	std::deque<EventArgs>*	mPrimaryEventQueue;
	std::deque<EventArgs>*	mSecondaryEventQueue;
	bool					mEndRequested;

	/*	複数スレッドがアクセスするイベントキューは、processMessage() 中に
		ウィンドウサイズが変更された等でイベントが発生すると、
		postEventFromWindowThread() でデッドロックする可能性がある。
		簡単に対応しようとするとかなり頻繁にロックを掛ける必要があるため、
		パフォーマンスが少しだけ心配。
		そもそも、deque 自体がスレッドセーフではないわけで…。
		ということで、入力専用、出力専用のダブルバッファリングにしてしまう。
	*/
};

} // namespace System
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================