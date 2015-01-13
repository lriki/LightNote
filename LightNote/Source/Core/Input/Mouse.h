//==============================================================================
// Mouse 
//------------------------------------------------------------------------------
///**
//  @file       Mouse.h
//  @brief      Mouse
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "InputDeviceBase.h"

namespace LNote
{
namespace Core
{
namespace Input
{

//==============================================================================
// ■ Mouse
//------------------------------------------------------------------------------
///**
//  @brief      マウスからの入力を扱うクラス
//  @par
//              このクラスのインスタンスは Engine::Input::Manager から取得します。<br><br>
//              isPress() 等の入力の判定で範囲外の値を渡したときの戻り値は常に false です。
//*/
//==============================================================================
class Mouse
    : public Base::ReferenceObject
{
public:
	Mouse( Manager* manager );
	virtual ~Mouse();

public:

	/// ボタンが現在押されているかを判定する
	bool isPress( LNMouseButton button );

	/// ボタンが新しく押された瞬間を判定する
	bool isOnTrigger( LNMouseButton button );

	/// ボタンが離された瞬間を判定する
	bool isOffTrigger( LNMouseButton button );

	/// ボタンが新しく押された瞬間とリピート状態を判定する
	bool isRepeat( LNMouseButton button );

	/// マウス座標の設定
    void setPoint( const LVector2& point ) { mInputDevice->setMousePoint( point ); }

	/// マウス座標の取得
	const LVector2& getPoint() { return mPosition; }

	/// 1 フレーム前からの移動量の取得
	const LVector2& getMoveDistance() { return mDistance; }

	/// 1 フレームのホイールの回転量の取得
	///		正の値は前(奥)、負の値は後ろ(手前)へ回された事を意味します。
	///		この関数は回した回数を返します。
	int getWheelState() { return mWheelState; }

	/// 位置を中央に固定するかの設定
    void setEnableCenterFix( bool flag ) { mIsCenterFix = flag; }

	/// リピート間隔の設定
	void setRepeatInterval( lnU32 first, lnU32 interval );

private:

	/// フレーム更新 (Manager から呼ばれる)
	void update();

private:

	/// 何からかのマウス操作メッセージがある場合に呼ぶ ( Manager から呼ばれる )
	void _onOperatedMouse( bool is_client );

	/// マウスカーソルの表示状態を設定する ( setCursorMode() _postMoveCursor() から呼ばれる )
	void _showCursor( bool flag );

	/// マウスカーソルの操作後に呼んで、現在の設定に応じてマウスカーソルの表示・非表示を切り替える ( クライアント領域内でのマウス移動の場合、true を渡す )
	void _postMoveCursor( bool is_client );

private:

	static const int MAX_BUTTON_NUM = LN_MAX_MOUSE_BUTTONS;	///< マウスボタンの最大数
	static const int MOUSE_CURSOR_COUNT_MAX = 100;			///< マウスを動かさない時、消えるまでのフレーム数

private:

	Manager*			mManager;							///< 管理クラス
	InputDeviceBase*	mInputDevice;						///< デバイスクラス
	lnS32				mButtonStatus[ MAX_BUTTON_NUM ];	///< ボタンの状態
	lnS32				mFirstRepeatInterval;				///< リピート開始までの待ちフレーム数
	lnS32				mRemRepeatInterval;					///< リピート中の1回の待ちフレーム数
	bool				mIsCenterFix;

	LVector2	        mPosition;							///< 現在位置
	LVector2	        mLastPosition;						///< 1 フレーム前の位置
	LVector2			mDistance;							///< 1 フレーム前からの移動量
	long				mWheelState;						///< ホイールの状態
	//LNMouseCursorMode	        mMouseCursorMode;					///< マウスカーソルの挙動
	int					mMouseVisibleCount;					///< マウスカーソルを一定時間動かない場合に消す処理に使うカウンタ
	bool				mIsSystemMouseShow;					///< システムのマウスカーソル表示中は true

	friend class Manager;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Input
} // namespace Core

} // namespace LNote

//==============================================================================
//
//==============================================================================