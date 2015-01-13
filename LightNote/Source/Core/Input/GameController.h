//==============================================================================
// GameController 
//------------------------------------------------------------------------------
//**
//  @file       GameController.h
//  @brief      GameController
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <vector>
#include "InputDeviceBase.h"

namespace LNote
{
namespace Core
{
namespace Input
{

//==============================================================================
// ■ GameController
//------------------------------------------------------------------------------
///**
//  @brief      仮想ゲームコントローラのクラス
//*/
//==============================================================================
class GameController
    : public Base::ReferenceObject
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
	/// コンストラクタ (ジョイパッドがない場合でも、番号を渡す)
	GameController( Manager* manager, int joypad_id );
	virtual ~GameController();

public:
	
	/// ボタンが現在押されているかを判定する
	bool isPress( LNGameControllerButton button );

	/// ボタンが新しく押された瞬間を判定する
	bool isOnTrigger( LNGameControllerButton button );

	/// キーが離された瞬間を判定する
	bool isOffTrigger( LNGameControllerButton button );

	/// ボタンが新しく押された瞬間とリピート状態を判定する
	bool isRepeat( LNGameControllerButton button );

	/// 指定ボタンの入力をアナログ値(0.0～1.0)で取得
	float getPower( LNGameControllerButton button );

	/// 軸の状態の取得 (0 ～ LN_MAX_JOYPAD_AXIS / 2 - 1)
	const LVector2& getAxisState( int axis );

	/// 新しく押された仮想ボタンの取得
	///		押されたキー(仮想キーコード)、ボタン、軸に対応する仮想ボタンを返す。
	int getPressedButton();

	//----------------------------------------------------------------------
	///**
	//  @brief      ボタン配置 (キーコンフィグ) を設定する
	//  @param[in]  target		: 変更するボタン
	//  @param[in]  v_button    : 新しく設定する仮想ボタン
	//  @param[in]  duplication : true の場合、重複を許可する
	//  @par
	//              仮想ボタンをゲームコントローラのボタンにアタッチします。<br>
	//              <br>
	//              基本的に getPressedButton() とセットで使い、getPressedButton()から
	//              受け取った仮想ボタン番号をこの関数の v_button に渡します。<br>
	//              <br>
	//              duplicationが false の場合、ln_button_ 以外のボタンに
	//              同じキーが割り当てられている場合、値を交換します。
	//*/
	//----------------------------------------------------------------------
	void attachVirtualButton( LNGameControllerAttachTarget target, int v_button, bool duplication );

	//----------------------------------------------------------------------
	///**
	//  @brief      ボタン配置 (キーコンフィグ) を解除する
	//  @param[in]  ln_button : 変更するボタン
	//  @param[in]  v_button  : 新しく設定する仮想ボタン
	//  @par
	//              v_button が ln_button_ にアタッチされている場合、アタッチを解除します。<br>
	//              v_button に LN_VBUTTON_NULL を渡すと、すべてのアタッチを解除します。
	//*/
	//----------------------------------------------------------------------
	void detachVirtualButton( LNGameControllerAttachTarget target, int v_button );

	//----------------------------------------------------------------------
	///**
	//  @brief      入力を受け取るデバイスの指定
	//  @param[in]  flags_ : LNInputAttachFlags のひとつ以上のフラグの組み合わせ
	//  @par
	//              デフォルトは LN_INPUTATTACH_ALL です。<br>
	//              <br>
	//              例えば LN_INPUTATTACH_MOUSE | LN_INPUTATTACH_KEYBOARD と設定した場合、
	//              このゲームコントローラにジョイパッドが接続されていてもそれからの入力は一切受け取りません。
	//*/
	//----------------------------------------------------------------------
    void setDeviceAttachFlag( lnU8 flags ) { mDeviceAttachFlag = flags; }

public:

	/// リピート間隔の設定
    void setRepeatInterval( lnS32 first, lnS32 interval ) { mFirstRepeatInterval = first; mRemRepeatInterval = interval; }

private:

	/// フレーム更新 (Manager から呼ばれる)
	void update();

private:

    typedef std::vector< int >  AttachedVButtons;

private:

    Manager*	        mManager;							    ///< 管理クラス
    InputDeviceBase*	mInputDevice;
	lnS32			    mButtonStatus[ LN_MAX_BUTTONS ];	    ///< ボタンの状態
	float			    mButtonPower[ LN_MAX_BUTTONS ];
    AttachedVButtons    mAttachedVButtons[ LN_MAX_AT_TARGETS ];

    int					mJoypadID;							    ///< 割り当てられているジョイパッドの番号
    LVector2	        mAxisState[ LN_MAX_JOYPAD_AXIS / 2 ];   ///< 軸の状態

    lnU32				mDeviceAttachFlag;

    lnS32				mFirstRepeatInterval;				    ///< リピート開始までの待ちフレーム数
	lnS32				mRemRepeatInterval;					    ///< リピート中の1回の待ちフレーム数
    bool				mPressedTriggerFlag;				    ///< getPressedButton() で使われるフラグ

	friend class Manager;
    /*
	Manager*			mManager;							///< 管理クラス
	lnS8					mButtonStatus[ LN_MAX_BUTTONS ];	///< ボタンの状態
	LVector2	        mAxisState[ MAX_AXES ];				///< 軸の状態
	lnS32					mFirstRepeatInterval;				///< リピート開始までの待ちフレーム数
	lnS32					mRemRepeatInterval;					///< リピート中の1回の待ちフレーム数
	
	//float				mAxisInvalidRange;					///< 軸の無効範囲
	//lnU8					mInputEnableFlags;					///< 実際に使用するデバイスのフラグ
	bool				mAssignedkeyboard;					///< キーボードが割り当てられているフラグ
	bool				mPressedTriggerFlag;				///< getPressedButton() で使われるフラグ
	//bool                mEnableNormalize;                   ///< true の場合、軸を正規化する


    ConfigData mConfigData;
	
	
    */
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