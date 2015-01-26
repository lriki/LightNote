//==============================================================================
// LFBase 
//------------------------------------------------------------------------------
/*
	■HSPでの例外(エラー)処理
		①[HSP] .hs で定義している LNException_SetHandler でラベルを登録する。
		        登録するWinメッセージは WIN32_MESSAGE_FOR_HSP_EXCEPTION
		②[C++] LN_FUNC_TRY_BEGIN～END で囲まれた領域内で例外が発生すると、
		        _LNException_SendToWin32Message() が呼ばれる (内部関数)
		③[C++] _LNException_SendToWin32Message() 内で WIN32_MESSAGE_FOR_HSP_EXCEPTION を send
		④[HSP] 登録したラベルがコールされる
		⑤[HSP] デフォルトでは LNException_ProcDefault() → end とする。
				こうしないと、メッセージボックス表示後にアプリが終了しない。

	■[2013/12/08] 公開する関数に try を張ることについて
		try を張った関数のオーバーヘッドは以下の通り。
			VC++	… 関数の入り口で必ず発生
			GCC		… catch したときのみ発生

		VC++ では try を書かないこと以外回避できないけど、
		それは現時点でのライブラリの方針的にNG。
		関数ライブラリとしても、正常系部分に異常系をガリガリ書かせたくない。

		そうなると try の数を減らす必要があるわけだけど、
		実際に例外を throw するのはほとんどが Create 系で、
		フレーム更新ごとに毎回呼ばれるものはほぼ投げない。setPosition() 等。

		というところで、基本的には以下のようなルールで実装する。
			・Create 等必要なタイミングでのみ呼ばれるものは try を張る
			・フレーム更新ごとに呼ばれるものは基本的に try を張らない。
			  NULL チェックはすべて assert で処理する。
			  (NULL を渡すことは、関数の使い方が間違っているもの)
*/
//==============================================================================

#pragma once

#include "stdafx.h"
#include "../Core/Base/ReferenceList.h"
#include "../Core/Geometry/DrawingShapes.h"
#include "../Core/Base/Table.h"
#include "LFInternal.h"
#include "LFBase.h"

//==============================================================================
// LNRect
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNRect_Contains(const LNRect* rect, int x, int y, LNBool* result)
	{
		Core::Geometry::Point<int> pt( x, y );
		*result = LNOTE_BOOL_TO_LNBOOL(((LRect*)rect)->contains(pt));
	}


//==============================================================================
// LNException
//==============================================================================

	static const uint32_t WIN32_MESSAGE_FOR_HSP_EXCEPTION = 0xA000;
	//static bool gHandled = false;
	bool gEnableWrappedException = false;

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void LNException_SetHandler( LNExceptionCallback handler )
	{
		FuncLibManager::ExceptionCallback = handler;
	}

	//----------------------------------------------------------------------
	//
	////----------------------------------------------------------------------
	//void LNException_SetHandled( LNBool handled )
	//{
	//	gHandled = (handled != LN_FALSE);
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNException_Raise(LNResult errCode, const LNChar* message, const LNChar* file, int line, const LNChar* func)
	{
		LN_FUNC_TRY_BEGIN;
		Core::Base::ExceptionManager::raise( (LNote::ResultCode)errCode, message, file, line, func );
		LN_FUNC_TRY_END;		// わざと throw して、ここで処理を行う
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNException_GetLastErrorCode()
	{
		Base::Exception* e = Core::Base::ExceptionManager::getLastException();
		if (e != NULL) {
			return (LNResult)e->getErrorCode();
		}
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void LNException_ProcDefault()
	{
		Base::Exception* e = Core::Base::ExceptionManager::getLastException();
		if (e != NULL) {
			FuncLibManager::procException(e);
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void _LNException_SendToWin32Message(LNResult errCode)
	{
#ifdef LNOTE_WIN32
		// HSP 用。ランタイムではなく DLL としてリンクする場合は
		// この方法以外、ラベルをコールバックする方法がない。
		if ( FuncLibManager::ConfigData.WindowHandle )
			::SendMessage( (HWND)FuncLibManager::ConfigData.WindowHandle, WIN32_MESSAGE_FOR_HSP_EXCEPTION, 0, 0 );
#endif
		//return LNOTE_BOOL_TO_LNBOOL(gHandled);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNException_SetEnableWrappedException( LNBool f )
	{
		gEnableWrappedException = (f != LN_FALSE);
	}

//==============================================================================
// LFObject
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNObject_Release(LNHandle hadnleObject)
	{
		if (hadnleObject == NULL) return ::LN_ERR_ARGUMENT;

		FuncLibManager::releaseObject(TO_INDEX(hadnleObject));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNObject_AddRef(LNHandle hadnleObject)
	{
		if (hadnleObject == NULL) return ::LN_ERR_ARGUMENT;

		FuncLibManager::addRefObject(TO_INDEX(hadnleObject));
		//if (FuncLibManager::getReferenceObject(TO_INDEX(hadnleObject))) {
			//FuncLibManager::getReferenceObject(TO_INDEX(hadnleObject))->addRef();
		//}
		//else if (FuncLibManager::getReferenceObject(TO_INDEX(hadnleObject))) {
		//	FuncLibManager::getReferenceObject(TO_INDEX(hadnleObject))->addRef();
		//}
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNObject_GetRefCount(LNHandle hadnleObject, int* count)
	{
		if (hadnleObject == NULL) return ::LN_ERR_ARGUMENT;
		if (count == NULL) return ::LN_ERR_ARGUMENT;


		*count = FuncLibManager::getRefCountRefObject(TO_INDEX(hadnleObject));
		//if (FuncLibManager::getReferenceObject(TO_INDEX(hadnleObject))) {
			//*count = FuncLibManager::getReferenceObject(TO_INDEX(hadnleObject))->getRefCount();
		//}
		//else if (FuncLibManager::getReferenceObject(TO_INDEX(hadnleObject))) {
		//	*count = FuncLibManager::getReferenceObject(TO_INDEX(hadnleObject))->getRefCount();
		//}
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LN_INTERNAL_API void* LNObject_GetTypeUserData(LNHandle hadnleObject)
	{
		if (hadnleObject == NULL) return NULL;

		//if (FuncLibManager::getReferenceObject(TO_INDEX(hadnleObject))) {
			return FuncLibManager::getReferenceObject(TO_INDEX(hadnleObject))->getTypeUserData();
		//}
		//else if (FuncLibManager::getReferenceObject(TO_INDEX(hadnleObject))) {
		//	return FuncLibManager::getReferenceObject(TO_INDEX(hadnleObject))->getTypeUserData();
		//}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LN_INTERNAL_API void* LNObject_GetInternalObject(LNHandle hadnleObject)
	{
		return FuncLibManager::getReferenceObject(TO_INDEX(hadnleObject));
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LN_INTERNAL_API void LNObject_SetUserData(LN_HANDLE(LNObject) hadnleObject, void* data)
	{
		FuncLibManager::getObjectEntry(hadnleObject)->UserData = data;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LN_INTERNAL_API void* LNObject_GetUserData(LN_HANDLE(LNObject) hadnleObject)
	{
		return FuncLibManager::getObjectEntry(hadnleObject)->UserData;
	}

//==============================================================================
// LNObjectList
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNObjectList_GetCount(LNHandle objectList, int* count)
	{
		LN_CHECK_ARG_HANDLE(objectList);

		*count = TO_REFOBJ(Base::ReferenceList<Base::ReferenceObject*>, objectList)->getCount();

		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNObjectList_SetAt(LNHandle objectList, int index, LNHandle item)
	{
		LN_CHECK_ARG_HANDLE(objectList);

		LN_FUNC_TRY_BEGIN;
		Base::ReferenceObject* obj = FuncLibManager::getReferenceObject(TO_INDEX(item));
		TO_REFOBJ(Base::ReferenceList<Base::ReferenceObject*>, objectList)->setAt(index, obj);
		LN_FUNC_TRY_END_RETURN;

		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNObjectList_GetAt(LNHandle objectList, int index, LNHandle* outItem)
	{
		LN_CHECK_ARG_HANDLE(objectList);

		LN_FUNC_TRY_BEGIN;
		Base::ReferenceObject* obj = TO_REFOBJ(Base::ReferenceList<Base::ReferenceObject*>, objectList)->getAt(index);
		*outItem = FuncLibManager::getInternalObjectHandle(obj);
		LN_FUNC_TRY_END_RETURN;

		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNObjectList_Add(LNHandle objectList, LNHandle item)
	{
		LN_CHECK_ARG_HANDLE(objectList);

		TO_REFOBJ(Base::ReferenceList<Base::ReferenceObject*>, objectList)->add(FuncLibManager::getReferenceObject(item));

		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNObjectList_Clear(LNHandle objectList)
	{
		LN_CHECK_ARG_HANDLE(objectList);

		TO_REFOBJ(Base::ReferenceList<Base::ReferenceObject*>, objectList)->clear();

		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNObjectList_Insert(LNHandle objectList, int index, LNHandle item)
	{
		LN_CHECK_ARG_HANDLE(objectList);

		TO_REFOBJ(Base::ReferenceList<Base::ReferenceObject*>, objectList)->insert(index, FuncLibManager::getReferenceObject(item));

		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNObjectList_Remove(LNHandle objectList, LNHandle item)
	{
		LN_CHECK_ARG_HANDLE(objectList);

		TO_REFOBJ(Base::ReferenceList<Base::ReferenceObject*>, objectList)->remove(FuncLibManager::getReferenceObject(item));

		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNObjectList_RemoveAll(LNHandle objectList, LNHandle item)
	{
		LN_CHECK_ARG_HANDLE(objectList);

		TO_REFOBJ(Base::ReferenceList<Base::ReferenceObject*>, objectList)->removeAll(FuncLibManager::getReferenceObject(item));

		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNObjectList_RemoveAt(LNHandle objectList, int index)
	{
		LN_CHECK_ARG_HANDLE(objectList);

		TO_REFOBJ(Base::ReferenceList<Base::ReferenceObject*>, objectList)->removeAt(index);

		return ::LN_OK;
	}

//==============================================================================
// LNIntTable
//==============================================================================

LNOTE_TYPE_INFO_IMPL(Base::IntTable, LNIntTable);

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
LNOTEAPI LNResult LNIntTable_Create(LNHandle* intTable, int xSize, int ySize, int zSize)
{
	LN_FUNC_TRY_BEGIN;
	LRefPtr<Base::IntTable> obj(LN_NEW Base::IntTable(xSize, ySize, zSize));
	obj.addRef();
	*intTable = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
	LN_FUNC_TRY_END_RETURN;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
LNOTEAPI LNResult LNIntTable_CreateFromSrcData(LNHandle* intTable, int xSize, int ySize, int zSize, const int* srcData)
{
	LN_FUNC_TRY_BEGIN;
	LRefPtr<Base::IntTable> obj(LN_NEW Base::IntTable(xSize, ySize, zSize, srcData));
	obj.addRef();
	*intTable = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
	LN_FUNC_TRY_END_RETURN;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
LNOTEAPI LNResult LNIntTable_SetValue(LNHandle intTable, int xSize, int ySize, int zSize, int value)
{
	LN_CHECK_ARG_HANDLE(intTable);
	TO_REFOBJ(Base::IntTable, intTable)->setValue(xSize, ySize, zSize, value);
	return LN_OK;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
LNOTEAPI LNResult LNIntTable_GetValue(LNHandle intTable, int xSize, int ySize, int zSize, int* value)
{
	LN_CHECK_ARG_HANDLE(intTable);
	LN_CHECK_ARG(value != NULL);
	*value = TO_REFOBJ(Base::IntTable, intTable)->getValue(xSize, ySize, zSize);
	return LN_OK;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
LNOTEAPI LNResult LNIntTable_Resize(LNHandle intTable, int xSize, int ySize, int zSize)
{
	LN_CHECK_ARG_HANDLE(intTable);
	TO_REFOBJ(Base::IntTable, intTable)->resize(xSize, ySize, zSize);
	return LN_OK;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
LNOTEAPI LNResult LNIntTable_GetXSize(LNHandle intTable, int* xSize)
{
	LN_CHECK_ARG_HANDLE(intTable);
	*xSize = TO_REFOBJ(Base::IntTable, intTable)->getXSize();
	return LN_OK;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
LNOTEAPI LNResult LNIntTable_GetYSize(LNHandle intTable, int* ySize)
{
	LN_CHECK_ARG_HANDLE(intTable);
	*ySize = TO_REFOBJ(Base::IntTable, intTable)->getYSize();
	return LN_OK;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
LNOTEAPI LNResult LNIntTable_GetZSize(LNHandle intTable, int* zSize)
{
	LN_CHECK_ARG_HANDLE(intTable);
	*zSize = TO_REFOBJ(Base::IntTable, intTable)->getZSize();
	return LN_OK;
}


#if 0
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
	LNOTEAPI void LNException_SetHandler(LNExceptionCallback handler);
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

#endif