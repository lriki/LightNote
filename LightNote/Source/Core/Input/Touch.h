//==============================================================================
// Touch 
//------------------------------------------------------------------------------
///**
//  @file       Touch.h
//  @brief      Touch
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
// ■ Touch
//------------------------------------------------------------------------------
///**
//  @brief        
//*/
//==============================================================================
class Touch
    : public Base::ReferenceObject
{
public:
	Touch( Manager* manager );
	virtual ~Touch();

public:

	/// ボタンが現在押されているかを判定する
	bool isPress( lnU32 id );

	/// ボタンが新しく押された瞬間を判定する
	bool isOnTrigger( lnU32 id );

	/// ボタンが離された瞬間を判定する
	bool isOffTrigger( lnU32 id );

	/// ボタンが新しく押された瞬間とリピート状態を判定する
	bool isRepeat( lnU32 id );

	/// タッチ座標の取得 (マウスと同じく、仮想スクリーンへの変換が必要な場合はその変換後。タッチされていない場合は最後にタッチされたときの座標)
	const LVector2& getPoint( lnU32 id ) { return mPosition[id]; }

	/// 1 フレーム前からの移動量の取得
	const LVector2& getMoveDistance( lnU32 id ) { return mDistance[id]; }

	/// リピート間隔の設定
	void setRepeatInterval( lnU32 first, lnU32 interval );

private:

	/// フレーム更新 (Manager から呼ばれる)
	void update();

private:

	Manager*			    mManager;							///< 管理クラス
    InputDeviceBase*		mInputDevice;						///< デバイスクラス
	lnS32					mPressCounts[LN_MAX_TOUCH_DATA];	///< 押下中のフレームカウント
	lnS32					mFirstRepeatInterval;				///< リピート開始までの待ちフレーム数
	lnS32					mRemRepeatInterval;					///< リピート中の1回の待ちフレーム数
   
    LNTouchDeviceState      mTouchState[LN_MAX_TOUCH_DATA];
    
	LVector2	            mPosition[LN_MAX_TOUCH_DATA];		///< 位置
	LVector2	            mLastPosition[LN_MAX_TOUCH_DATA];	///< 1 フレーム前の位置
	LVector2	            mDistance[LN_MAX_TOUCH_DATA];		///< 1 フレーム前からの移動量

	friend class Manager;
};

} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================