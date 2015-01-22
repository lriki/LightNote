//==============================================================================
// LFBase 
//------------------------------------------------------------------------------
///**
//  @file       LFBase.h
//  @brief      LFBase
//	@defgroup   group_lib_base Base
//	@{
//*/
//==============================================================================

#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

extern "C" {

//==============================================================================
// LNSize
//------------------------------------------------------------------------------
///**
//  @defgroup   group_size LNSize
//  @brief      オブジェクトの横幅と縦幅の情報
//  @{
//*/
//==============================================================================

/** @} */	// defgroup

//==============================================================================
// LNRect
//------------------------------------------------------------------------------
///**
//  @defgroup   group_rect LNRect
//  @brief      矩形を表す
//  @{
//*/
//==============================================================================

	//----------------------------------------------------------------------
	///**
	//  @brief      指定した x y 座標の点が矩形に含まれているかを判別する
	//  @param[in]  rect	: 処理の基になる Rect
	//  @param[in]  x		: X 座標
	//  @param[in]	y		: Y 座標
	//  @param[out]	result	: 結果を格納する変数
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNRect_Contains(const LNRect* rect, int x, int y, LNBool* result);

/** @} */	// defgroup

//==============================================================================
// LNException
//------------------------------------------------------------------------------
///**
//  @defgroup   group_exception LNException
//  @brief      例外処理
//  @{
//*/
//==============================================================================

	//----------------------------------------------------------------------
	///**
	//  @brief      例外発生時にコールバックされる関数を登録する
	//  @param[in]	handler		: 
	//*/
	//----------------------------------------------------------------------
    LNOTEAPI void LNException_SetHandler( LNExceptionCallback handler );
	/*DeclOpt
	HSP_Enabled = false;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief      例外を発生させる
	//  @param[in]	errCode		: 
	//  @param[in]	message		: 
	//  @param[in]	file		: 
	//  @param[in]	line		: 
	//  @param[in]	func		: 
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNException_Raise(LNResult errCode, const LNChar* message, const LNChar* file, int line, const LNChar* func);

	//----------------------------------------------------------------------
	///**
	//  @brief      最後に発生した例外のエラーコードを取得する
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNException_GetLastErrorCode();

	//----------------------------------------------------------------------
	///**
	//  @brief      最後に発生した例外に対して、デフォルトの処理を行う (メッセージボックスを表示し、LNException_SetHandled(true))
	//*/
	//----------------------------------------------------------------------
    LNOTEAPI void LNException_ProcDefault();

/** @} */	// defgroup

//==============================================================================
// LNObject
//------------------------------------------------------------------------------
///**
//  @defgroup   group_object LNObject
//  @brief      ベースオブジェクト
//  @{
//*/
//==============================================================================

	//----------------------------------------------------------------------
	///**
	//  @brief		オブジェクトの解放
	//  @param[in]	hadnleObject	: オブジェクトハンドル
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	指定されたオブジェクトの参照を解放します。<br>
	//				Create～という名前の付く関数でオブジェクトを生成した場合は
	//				基本的にこの関数でオブジェクトを解放する必要があります。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNObject_Release(LNHandle hadnleObject);

	//----------------------------------------------------------------------
	///**
	//  @brief		オブジェクトの参照カウントをインクリメントする
	//  @param[in]	hadnleObject	: オブジェクトハンドル
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	この関数は「参照カウント」という概念について十分に理解した上で使用してください。
	//				参照カウントは Create 時に 1 となり、LNObject_Release でデクリメントされます。
	//				Release 漏れがあるとメモリリークの原因になります。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNObject_AddRef(LNHandle hadnleObject);

	//----------------------------------------------------------------------
	///**
	//  @brief		オブジェクトの参照カウント取得する
	//  @param[in]	hadnleObject	: オブジェクトハンドル
	//  @param[out]	count			: 参照カウントを格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNObject_GetRefCount(LNHandle hadnleObject, int* count);

/** @} */	// group_object

//==============================================================================
// LNObjectList
//------------------------------------------------------------------------------
///**
//  @defgroup   group_object_list LNObjectList
//  @brief      オブジェクトリスト
//  @{
//*/
//==============================================================================

	//----------------------------------------------------------------------
	///**
	//  @brief		オブジェクトリストに格納されているオブジェクトの数を取得する
	//  @param[in]	objectList	: オブジェクトリストハンドル
	//  @param[out]	count		: 要素の数を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNObjectList_GetCount(LNHandle objectList, int* count);

	//----------------------------------------------------------------------
	///**
	//  @brief		オブジェクトリストの指定したインデックスにオブジェクトを設定する
	//  @param[in]	objectList	: オブジェクトリストハンドル
	//  @param[in]	index		: インデックス(要素番号)
	//  @param[in]	item		: 設定するオブジェクト
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNObjectList_SetAt(LNHandle objectList, int index, LNHandle item);

	//----------------------------------------------------------------------
	///**
	//  @brief		オブジェクトリストの指定したインデックスのオブジェクトを取得する
	//  @param[in]	objectList	: オブジェクトリストハンドル
	//  @param[in]	index		: インデックス(要素番号)
	//  @param[out]	outItem		: オブジェクトを格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNObjectList_GetAt(LNHandle objectList, int index, LNHandle* outItem);

	//----------------------------------------------------------------------
	///**
	//  @brief		オブジェクトリストの末尾にオブジェクトを追加する
	//  @param[in]	objectList	: オブジェクトリストハンドル
	//  @param[in]	item		: 追加するオブジェクト
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNObjectList_Add(LNHandle objectList, LNHandle item);

	//----------------------------------------------------------------------
	///**
	//  @brief		オブジェクトリストから全てのオブジェクトを削除する
	//  @param[in]	objectList	: オブジェクトリストハンドル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNObjectList_Clear(LNHandle objectList);

	//----------------------------------------------------------------------
	///**
	//  @brief		オブジェクトリストの指定したインデックスの位置にオブジェクトを挿入する
	//  @param[in]	objectList	: オブジェクトリストハンドル
	//  @param[in]	index		: item を挿入するインデックス
	//  @param[in]	item		: 挿入するオブジェクト
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNObjectList_Insert(LNHandle objectList, int index, LNHandle item);

	//----------------------------------------------------------------------
	///**
	//  @brief		オブジェクトリスト内で指定したハンドルと一致する最初のオブジェクトを削除する
	//  @param[in]	objectList	: オブジェクトリストハンドル
	//  @param[in]	item		: リストから削除するオブジェクト
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNObjectList_Remove(LNHandle objectList, LNHandle item);

	//----------------------------------------------------------------------
	///**
	//  @brief		オブジェクトリスト内で指定したハンドルと一致する全てのオブジェクトを削除する
	//  @param[in]	objectList	: オブジェクトリストハンドル
	//  @param[in]	item		: リストから削除するオブジェクト
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNObjectList_RemoveAll(LNHandle objectList, LNHandle item);

	//----------------------------------------------------------------------
	///**
	//  @brief		オブジェクトリストの指定したインデックスにあるオブジェクトを削除する
	//  @param[in]	objectList	: オブジェクトリストハンドル
	//  @param[in]	index		: 削除するオブジェクトのインデックス番号
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNObjectList_RemoveAt(LNHandle objectList, int index);

/** @} */	// group_object

//==============================================================================
// LNIntTable
//------------------------------------------------------------------------------
///**
//  @defgroup   group_inttable LNIntTable
//  @brief      整数型の多次元配列クラス
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNIntTable);

	//----------------------------------------------------------------------
	///**
	//	@brief      整数型の多次元配列を作成する
	//  @param[out]	intTable	: 作成されたテーブルハンドルを格納する変数
	//  @param[in]  xSize		: X 要素数
	//  @param[in]  ySize		: Y 要素数
	//  @param[in]  zSize		: Z 要素数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNIntTable_Create(LNHandle* intTable, int xSize, int ySize LNOTE_DEFAULT_ARG(1), int zSize LNOTE_DEFAULT_ARG(1));

	//----------------------------------------------------------------------
	///**
	//	@brief      整数型の多次元配列を作成する
	//  @param[out]	intTable	: 作成されたテーブルハンドルを格納する変数
	//  @param[in]  xSize		: X 要素数
	//  @param[in]  ySize		: Y 要素数
	//  @param[in]  zSize		: Z 要素数
	//  @param[in]  srcData		: XXXX
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNIntTable_CreateFromSrcData(LNHandle* intTable, int xSize, int ySize, int zSize, const int* srcData);

	//----------------------------------------------------------------------
	///**
	//	@brief      整数型テーブルの指定したインデックスに値を設定する
	//  @param[in]	intTable	: 整数型テーブルハンドル
	//  @param[in]  xSize		: X 要素インデックス
	//  @param[in]  ySize		: Y 要素インデックス
	//  @param[in]  zSize		: Z 要素インデックス
	//  @param[in]  value		: 設定する整数値
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	変更前のデータは保持されます。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNIntTable_SetValue(LNHandle intTable, int xSize, int ySize, int zSize, int value);

	//----------------------------------------------------------------------
	///**
	//	@brief		整数型テーブルの指定したインデックスに値を設定する
	//  @param[in]	intTable	: 整数型テーブルハンドル
	//  @param[in]	xSize		: X 要素インデックス
	//  @param[in]	ySize		: Y 要素インデックス
	//  @param[in]	zSize		: Z 要素インデックス
	//  @param[out]	value		: 値を格納する整数型変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	変更前のデータは保持されます。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNIntTable_GetValue(LNHandle intTable, int xSize, int ySize, int zSize, int* value);

	//----------------------------------------------------------------------
	///**
	//	@brief      整数型テーブルの要素数を変更する
	//  @param[in]	intTable	: 整数型テーブルハンドル
	//  @param[in]  xSize		: X 要素数
	//  @param[in]  ySize		: Y 要素数
	//  @param[in]  zSize		: Z 要素数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	変更前のデータは保持されます。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNIntTable_Resize(LNHandle intTable, int xSize, int ySize, int zSize);

	//----------------------------------------------------------------------
	///**
	//	@brief      整数型テーブルの X 要素数を取得する
	//  @param[in]	intTable	: 整数型テーブルハンドル
	//  @param[out]	xSize		: X 要素数を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNIntTable_GetXSize(LNHandle intTable, int* xSize);

	//----------------------------------------------------------------------
	///**
	//	@brief      整数型テーブルの Y 要素数を取得する
	//  @param[in]	intTable	: 整数型テーブルハンドル
	//  @param[out]	ySize		: Y 要素数を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNIntTable_GetYSize(LNHandle intTable, int* ySize);

	//----------------------------------------------------------------------
	///**
	//	@brief      整数型テーブルの Z 要素数を取得する
	//  @param[in]	intTable	: 整数型テーブルハンドル
	//  @param[out]	zSize		: Z 要素数を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNIntTable_GetZSize(LNHandle intTable, int* zSize);


/** @} */	// defgroup

	LNOTEINTERNALAPI void* LNObject_GetTypeUserData(LNHandle hadnleObject);
	LNOTEINTERNALAPI void* LNObject_GetInternalObject(LNHandle hadnleObject);

} // extern "C"

/** @} */	// defgroup
