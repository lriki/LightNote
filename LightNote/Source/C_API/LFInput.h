//==============================================================================
// LFInput 
//------------------------------------------------------------------------------
///**
//  @file       LFInput.h
//  @brief      LFInput
//  @defgroup   group_lib_input Input
//  @{
//*/
//==============================================================================

#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

extern "C" {

//==============================================================================
// LNInput
//------------------------------------------------------------------------------
///**
//  @defgroup   group_input LNInput
//  @brief      入力処理
//  @{
//*/
//==============================================================================

	//----------------------------------------------------------------------
	///**
	//  @brief      ボタンが現在押されているかを判定する
	//  @param[in]	button		: 仮想ボタン番号 (0～19)
	//  @param[out]	state		: 状態を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNInput_IsPress(int button, LNBool* state);

	//----------------------------------------------------------------------
	///**
	//  @brief      仮想ゲームコントローラのボタンが新しく押されたかを判定する
	//  @param[in]  button		: 仮想ボタン番号 (0～19)
	//  @param[out]	state		: 状態を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNInput_IsTrigger(int button, LNBool* state);

	//----------------------------------------------------------------------
	///**
	//  @brief      仮想ゲームコントローラのボタンが離されたかを判定する
	//  @param[in]  button		: 仮想ボタン番号 (0～19)
	//  @param[out]	state		: 状態を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNInput_IsOffTrigger(int button, LNBool* state);

	//----------------------------------------------------------------------
	///**
	//  @brief      仮想ゲームコントローラのボタンが新しく押された瞬間とリピート状態を判定する
	//  @param[in]  button		: 仮想ボタン番号 (0～19)
	//  @param[out]	state		: 状態を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNInput_IsRepeat(int button, LNBool* state);

	//----------------------------------------------------------------------
	///**
	//  @brief      仮想ゲームコントローラの指定ボタンの入力をアナログ値(0.0～1.0)で取得する
	//  @param[in]  button		: 仮想ボタン番号 (0～19)
	//  @param[out]	state		: アナログ値を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNInput_GetPower(int button, float* state);

	//----------------------------------------------------------------------
	///**
	//  @brief      仮想ゲームコントローラの軸の状態(スティックの左右と上下)の取得
	//  @param[in]  axis		: 軸番号 (0～1)
	//  @param[out]	state		: 軸の状態を格納する2Dベクトル
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNInput_GetAxisState(int axis, LNVector2* state);

/** @} */	// defgroup

//==============================================================================
// LNVirtualPad
//------------------------------------------------------------------------------
///**
//  @defgroup   group_virtualpad LNVirtualPad
//  @brief      仮想ゲームコントローラ
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNVirtualPad);

	//----------------------------------------------------------------------
	///**
	//  @brief      仮想ゲームコントローラハンドルを取得する
	//  @param[in]	index		: 取得する仮想ゲームコントローラの番号 (0～3)
	//  @param[out]	virtualPad	: 仮想ゲームコントローラハンドルを格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVirtualPad_GetVirtualPad(int index, LNHandle* virtualPad);
	/*DeclOpt
	Cpp_MethodOverride = true;
	CS_MethodOverride = true;
	Ruby_MethodOverride = true;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief      仮想ゲームコントローラのボタンが現在押されているかを判定する
	//  @param[in]	virtualPad	: 仮想ゲームコントローラハンドル
	//  @param[in]	button		: 仮想ボタン番号 (0～19)
	//  @param[out]	state		: 状態を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVirtualPad_IsPress(LNHandle virtualPad, int button, LNBool* state);

	//----------------------------------------------------------------------
	///**
	//  @brief      仮想ゲームコントローラのボタンが新しく押されたかを判定する
	//  @param[in]	virtualPad	: 仮想ゲームコントローラハンドル
	//  @param[in]  button		: 仮想ボタン番号 (0～19)
	//  @param[out]	state		: 状態を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVirtualPad_IsTrigger(LNHandle virtualPad, int button, LNBool* state);

	//----------------------------------------------------------------------
	///**
	//  @brief      仮想ゲームコントローラのボタンが離されたかを判定する
	//  @param[in]	virtualPad	: 仮想ゲームコントローラハンドル
	//  @param[in]  button		: 仮想ボタン番号 (0～19)
	//  @param[out]	state		: 状態を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVirtualPad_IsOffTrigger(LNHandle virtualPad, int button, LNBool* state);

	//----------------------------------------------------------------------
	///**
	//  @brief      仮想ゲームコントローラのボタンが新しく押された瞬間とリピート状態を判定する
	//  @param[in]	virtualPad	: 仮想ゲームコントローラハンドル
	//  @param[in]  button		: 仮想ボタン番号 (0～19)
	//  @param[out]	state		: 状態を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVirtualPad_IsRepeat(LNHandle virtualPad, int button, LNBool* state);

	//----------------------------------------------------------------------
	///**
	//  @brief      仮想ゲームコントローラの指定ボタンの入力をアナログ値(0.0～1.0)で取得する
	//  @param[in]	virtualPad	: 仮想ゲームコントローラハンドル
	//  @param[in]  button		: 仮想ボタン番号 (0～19)
	//  @param[out]	state		: アナログ値を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVirtualPad_GetPower(LNHandle virtualPad, int button, float* state);

	//----------------------------------------------------------------------
	///**
	//  @brief      仮想ゲームコントローラの軸の状態(スティックの左右と上下)の取得
	//  @param[in]	virtualPad	: 仮想ゲームコントローラハンドル
	//  @param[in]  axis		: 軸番号 (0～1)
	//  @param[out]	state		: 軸の状態を格納する2Dベクトル
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVirtualPad_GetAxisState(LNHandle virtualPad, int axis, LNVector2* state);

	//----------------------------------------------------------------------
	///**
	//  @brief      仮想ゲームコントローラの新しく押されたデバイスボタン番号の取得
	//  @param[in]	virtualPad		: 仮想ゲームコントローラハンドル
	//  @param[out]	deviceButton	: デバイスボタン番号を格納する構造体
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	キーボード、マウス、ジョイパッド(スティックを含む)の
	//				何らかのボタンが押された場合、そのボタンを識別するための
	//				番号を返します。(デバイスボタン)<br>
	//				この番号は主にキーコンフィグで使用します。<br>
	//				この関数は isTrigger() 同様に動作し、ボタンが押された瞬間だけ
	//				ボタン番号を返します。<br>
	//				それ以外は 0 を返します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVirtualPad_GetPressedDeviceButton(LNHandle virtualPad, int* deviceButton);

	//----------------------------------------------------------------------
	///**
	//  @brief      デバイスボタン番号に対応するデバイスの種類を取得する
	//  @param[in]  deviceButton	: デバイスボタン番号
	//  @param[out]	type			: デバイスボタンの種類を格納する構造体
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVirtualPad_GetDeviceButtonType(int deviceButton, LNDeviceButtonType* type);

	//----------------------------------------------------------------------
	///**
	//  @brief      仮想ゲームコントローラのボタンにデバイスボタンを割り当てる
	//  @param[in]	virtualPad		: ゲームコントローラハンドル
	//  @param[in]  virtualButton	: 割り当て先の仮想ボタン番号
	//  @param[in]  deviceButton	: 割り当てるデバイスボタン番号
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details	この関数はキーコンフィグ用の関数です。<br>
	//              デバイスボタン(キーボードのキーや、ジョイパッドのボタン)を仮想ボタンに割り当てます。<br>
	//              <br>
	//              基本的に LNVirtualPad_GetPressedDeviceButton() とセットで使い、LNVirtualPad_GetDeviceButtonType()から
	//              受け取ったデバイスボタン番号をこの関数の deviceButton に渡します。<br>
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVirtualPad_AttachVirtualButton(LNHandle virtualPad, int virtualButton, int deviceButton);

	//----------------------------------------------------------------------
	///**
	//  @brief      仮想ゲームコントローラのデバイスボタンの割り当てを解除する
	//  @param[in]	virtualPad		: 仮想ゲームコントローラハンドル
	//  @param[in]  virtualButton	: 解除対象の仮想ボタン番号
	//  @param[in]  deviceButton	: 解除するデバイスボタン番号
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details    解除対象の仮想ボタンに deviceButton で示されるデバイスボタンが割り当てられている場合、解除します。<br>
	//				割り当てられていない場合は何もしません。<br>
	//				deviceButton に 0 を渡すと、virtualButton の全ての割り当てを解除します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVirtualPad_DetachVirtualButton(LNHandle virtualPad, int virtualButton, int deviceButton);

/** @} */	// defgroup

} // extern "C"

/** @} */ // defgroup

