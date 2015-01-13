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
	void LNException_Raise( LNErrorCode errCode, const LNChar* message, const LNChar* file, int line, const LNChar* func )
	{
		LN_FUNC_TRY_BEGIN;
		Core::Base::ExceptionManager::raise( (LNote::ResultCode)errCode, message, file, line, func );
		LN_FUNC_TRY_END;		// わざと throw して、ここで処理を行う
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNErrorCode LNException_GetLastErrorCode()
	{
		Base::Exception* e = Core::Base::ExceptionManager::getLastException();
		if (e != NULL) {
			return (LNErrorCode)e->getErrorCode();
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
	void _LNException_SendToWin32Message( LNErrorCode errCode )
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
	LNOTEINTERNALAPI void* LNObject_GetTypeUserData(LNHandle hadnleObject)
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
	LNOTEINTERNALAPI void* LNObject_GetInternalObject(LNHandle hadnleObject)
	{
		return FuncLibManager::getReferenceObject(TO_INDEX(hadnleObject));
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
