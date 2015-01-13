//==============================================================================
// WindowManagerBase 
//------------------------------------------------------------------------------
///**
//  @file       WindowManagerBase.h
//  @brief      WindowManagerBase
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Base/STLUtils.h"
#include "../Common.h"
#include "../Interface.h"
#include "MouseCursorVisibleCounter.h"

namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// ■ WindowManagerBase
//------------------------------------------------------------------------------
///**
//  @brief
//	@note
//				このクラスはウィンドウが作成されるスレッド上でのメッセージ処理を行う。
//				このクラスの仮想関数はウィンドウを作成するスレッドからのみ呼び出すこと。
//				processMessage() はOS毎のメッセージを処理し、ライブラリのイベント形式に変換して
//				Manager に post する。
//
//				ワーカースレッドから呼び出すのは、派生の initialize() と、processMessage()、finalize()。
//				
//				また、スレッドを使用する場合初期化データは一度保持する必要があるため、
//				preInitialize() でデータセット→ initialize() で初期化実行という2段構成になっている。
//
//				processMessage() のエラー処理は？
//					assert、throw 以外のエラー (ファイルが存在しない等) は、
//					基本的にハンドル先でメッセージボックスを出す等して対応するべき。
//					呼び出し側に伝えるべきエラーがちょっと見つからないので、とりあえずこのまま。
//*/
//==============================================================================
class WindowManagerBase
    : public Base::ReferenceObject
{
public:
    WindowManagerBase( Manager* manager );
	virtual ~WindowManagerBase();

public:

	/// 管理クラスの取得
	Manager* getManager() const { return mManager; }

	/// マウスカーソルの表示設定 (visible が false の場合、invisible_time(ms) だけ待ってから消す。0ですぐ消す)
    void setMouseCursorVisibleState( bool visible, int invisible_time );

	/// 全てのイベントが持つすべてのイベントリスナーにイベントを送信する (Manager::processMessage() から呼ばれる)
	bool sendEventToAllListener( const EventArgs& e );

public:

	/// 初期化
    virtual void initialize() = 0;

	/// 終了処理
    virtual void finalize();

	/// メッセージ処理
    virtual void processMessage() = 0;

	/// メインウィンドウの取得
	virtual Window* getMainWindow() const = 0;

private:
	friend class WindowBase;
	void addWindow( WindowBase* w ) { mWindowArray.push_back( w ); }
	void removeWindow( WindowBase* w ) { Base::STLUtils::remove( mWindowArray, w ); }

protected:

	Manager*		mManager;
	WindowBaseArray	mWindowArray;
	MouseCursorVisibleCounter	mMouseCursorVisibleCounter;
};

} // namespace System
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================