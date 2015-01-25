
#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

extern "C" {

/**
	@brief	
*/
LN_MODULE(Base)

//==============================================================================
/**
	@brief	矩形を表します。
*/
LN_STRUCT_CLASS(LNRect)

	/**
		@brief		矩形の各要素を設定します。
		@param[in]	rect	: 処理の基になる Rect
		@param[in]	x		: 左上 X 座標
		@param[in]	y		: 左上 Y 座標
		@param[in]	width	: 幅
		@param[in]	height	: 高さ
	*/
	LN_INSTANCE_API
	LN_STRUCT_CONSTRUCTOR
	void LNRect_Set(const LNRect* rect, int x, int y, int width, int height);

	/**
		@brief		指定した x y 座標の点が矩形に含まれているかを判別する
		@param[in]	rect	: 処理の基になる Rect
		@param[in]	x		: X 座標
		@param[in]	y		: Y 座標
		@param[out]	result	: 結果を格納する変数
	*/
	LN_INSTANCE_API void LNRect_Contains(const LNRect* rect, int x, int y, LNBool* result);

LN_CLASS_END



//==============================================================================
/**
	@brief	例外処理
*/
LN_EXTENSION_CLASS(LNException)

	/**
		@brief		例外発生時にコールバックされる関数を登録する
		@param[in]	handler		: 
	*/
    LN_STATIC_API void LNException_SetHandler( LNExceptionCallback handler );
	/*Option
		@disable[hsp]
	Option*/

	/**
		@brief		例外を発生させる
		@param[in]	errCode		: 
		@param[in]	message		: 
		@param[in]	file		: 
		@param[in]	line		: 
		@param[in]	func		: 
	*/
	LN_STATIC_API void LNException_Raise(LNResult errCode, const LNChar* message, const LNChar* file, int line, const LNChar* func);

	/**
		@brief		最後に発生した例外のエラーコードを取得する
	*/
	LN_STATIC_API LNResult LNException_GetLastErrorCode();

	/**
		@brief		最後に発生した例外に対して、デフォルトの処理を行う (メッセージボックスを表示し、LNException_SetHandled(true))
	*/
	LN_STATIC_API void LNException_ProcDefault();

LN_CLASS_END


//==============================================================================
/**
	@brief	全てのオブジェクトのベースオブジェクトです。
*/
LN_EXTENSION_CLASS(LNObject)

	/**
		@brief		オブジェクトの解放
		@param[in]	hadnleObject	: オブジェクトハンドル
		@details	指定されたオブジェクトの参照を解放します。<br>
					Create～という名前の付く関数でオブジェクトを生成した場合は
					基本的にこの関数でオブジェクトを解放する必要があります。
	*/
	LN_INSTANCE_API LNResult LNObject_Release(LN_HANDLE(LNObject) hadnleObject);

	/**
		@brief		オブジェクトの参照カウントをインクリメントする
		@param[in]	hadnleObject	: オブジェクトハンドル
		@details	この関数は「参照カウント」という概念について十分に理解した上で使用してください。
					参照カウントは Create 時に 1 となり、LNObject_Release でデクリメントされます。
					Release 漏れがあるとメモリリークの原因になります。
	*/
	LN_INSTANCE_API LNResult LNObject_AddRef(LN_HANDLE(LNObject) hadnleObject);

	/**
		@brief		オブジェクトの参照カウント取得する
		@param[in]	hadnleObject	: オブジェクトハンドル
		@param[out]	count			: 参照カウントを格納する変数
	*/
	LN_INSTANCE_API LNResult LNObject_GetRefCount(LN_HANDLE(LNObject) hadnleObject, int* count);

	LN_INTERNAL_API void*	LNObject_GetTypeUserData(LN_HANDLE(LNObject) hadnleObject);
	LN_INTERNAL_API void*	LNObject_GetInternalObject(LN_HANDLE(LNObject) hadnleObject);
	LN_INTERNAL_API void	LNObject_SetUserData(LN_HANDLE(LNObject) hadnleObject, void* data);
	LN_INTERNAL_API void*	LNObject_GetUserData(LN_HANDLE(LNObject) hadnleObject);
	
LN_CLASS_END


//==============================================================================
/**
	@brief	オブジェクトの可変長配列です。
*/
LN_EXTENSION_CLASS(LNObjectList)

	/**
		@brief		オブジェクトリストに格納されているオブジェクトの数を取得する
		@param[in]	objectList	: オブジェクトリストハンドル
		@param[out]	count		: 要素の数を格納する変数
	*/
	LN_INSTANCE_API LNResult LNObjectList_GetCount(LN_HANDLE(LNObjectList) objectList, int* count);

	/**
		@brief		オブジェクトリストの指定したインデックスにオブジェクトを設定する
		@param[in]	objectList	: オブジェクトリストハンドル
		@param[in]	index		: インデックス(要素番号)
		@param[in]	item		: 設定するオブジェクト
	*/
	LN_INSTANCE_API LNResult LNObjectList_SetAt(LN_HANDLE(LNObjectList) objectList, int index, LN_HANDLE(LNObject) item);

	/**
		@brief		オブジェクトリストの指定したインデックスのオブジェクトを取得する
		@param[in]	objectList	: オブジェクトリストハンドル
		@param[in]	index		: インデックス(要素番号)
		@param[out]	outItem		: オブジェクトを格納する変数
	*/
	LN_INSTANCE_API LNResult LNObjectList_GetAt(LN_HANDLE(LNObjectList) objectList, int index, LN_HANDLE(LNObject)* outItem);

	/**
		@brief		オブジェクトリストの末尾にオブジェクトを追加する
		@param[in]	objectList	: オブジェクトリストハンドル
		@param[in]	item		: 追加するオブジェクト
	*/
	LN_INSTANCE_API LNResult LNObjectList_Add(LN_HANDLE(LNObjectList) objectList, LN_HANDLE(LNObject) item);

	/**
		@brief		オブジェクトリストから全てのオブジェクトを削除する
		@param[in]	objectList	: オブジェクトリストハンドル
	*/
	LN_INSTANCE_API LNResult LNObjectList_Clear(LN_HANDLE(LNObjectList) objectList);

	/**
		@brief		オブジェクトリストの指定したインデックスの位置にオブジェクトを挿入する
		@param[in]	objectList	: オブジェクトリストハンドル
		@param[in]	index		: item を挿入するインデックス
		@param[in]	item		: 挿入するオブジェクト
	*/
	LN_INSTANCE_API LNResult LNObjectList_Insert(LN_HANDLE(LNObjectList) objectList, int index, LN_HANDLE(LNObject) item);

	/**
		@brief		オブジェクトリスト内で指定したハンドルと一致する最初のオブジェクトを削除する
		@param[in]	objectList	: オブジェクトリストハンドル
		@param[in]	item		: リストから削除するオブジェクト
	*/
	LN_INSTANCE_API LNResult LNObjectList_Remove(LN_HANDLE(LNObjectList) objectList, LN_HANDLE(LNObject) item);

	/**
		@brief		オブジェクトリスト内で指定したハンドルと一致する全てのオブジェクトを削除する
		@param[in]	objectList	: オブジェクトリストハンドル
		@param[in]	item		: リストから削除するオブジェクト
	*/
	LN_INSTANCE_API LNResult LNObjectList_RemoveAll(LN_HANDLE(LNObjectList) objectList, LN_HANDLE(LNObject) item);

	/**
		@brief		オブジェクトリストの指定したインデックスにあるオブジェクトを削除する
		@param[in]	objectList	: オブジェクトリストハンドル
		@param[in]	index		: 削除するオブジェクトのインデックス番号
	*/
	LN_INSTANCE_API LNResult LNObjectList_RemoveAt(LN_HANDLE(LNObjectList) objectList, int index);

LN_CLASS_END

//==============================================================================
/**
	@brief	整数型の多次元配列クラスです。
*/
LN_CLASS(LNIntTable)

	LNOTE_TYPE_INFO_DECL(LNIntTable);

	/**
		@brief		整数型の多次元配列を作成する
		@param[out]	intTable	: 作成されたテーブルハンドルを格納する変数
		@param[in]  xSize		: X 要素数
		@param[in]  ySize		: Y 要素数
		@param[in]  zSize		: Z 要素数
	*/
	LN_INSTANCE_API
	LNResult LNIntTable_Create(LN_HANDLE(LNIntTable)* intTable, int xSize, int ySize LN_DEFAULT_ARG(1), int zSize LN_DEFAULT_ARG(1));

	/**
		@brief		整数型の多次元配列を作成する
		@param[out]	intTable	: 作成されたテーブルハンドルを格納する変数
		@param[in]  xSize		: X 要素数
		@param[in]  ySize		: Y 要素数
		@param[in]  zSize		: Z 要素数
		@param[in]  srcData		: XXXX
	*/
	LN_INSTANCE_API
	LNResult LNIntTable_CreateFromSrcData(LN_HANDLE(LNIntTable)* intTable, int xSize, int ySize, int zSize, const int* srcData);

	/**
		@brief		整数型テーブルの指定したインデックスに値を設定する
		@param[in]	intTable	: 整数型テーブルハンドル
		@param[in]  xSize		: X 要素インデックス
		@param[in]  ySize		: Y 要素インデックス
		@param[in]  zSize		: Z 要素インデックス
		@param[in]  value		: 設定する整数値
		@details	変更前のデータは保持されます。
	*/
	LN_INSTANCE_API
	LNResult LNIntTable_SetValue(LN_HANDLE(LNIntTable) intTable, int xSize, int ySize, int zSize, int value);

	/**
		@brief		整数型テーブルの指定したインデックスに値を設定する
		@param[in]	intTable	: 整数型テーブルハンドル
		@param[in]	xSize		: X 要素インデックス
		@param[in]	ySize		: Y 要素インデックス
		@param[in]	zSize		: Z 要素インデックス
		@param[out]	value		: 値を格納する整数型変数
		@details	変更前のデータは保持されます。
	*/
	LN_INSTANCE_API
	LNResult LNIntTable_GetValue(LN_HANDLE(LNIntTable) intTable, int xSize, int ySize, int zSize, int* value);

	/**
		@brief		整数型テーブルの要素数を変更する
		@param[in]	intTable	: 整数型テーブルハンドル
		@param[in]  xSize		: X 要素数
		@param[in]  ySize		: Y 要素数
		@param[in]  zSize		: Z 要素数
		@details	変更前のデータは保持されます。
	*/
	LN_INSTANCE_API
	LNResult LNIntTable_Resize(LN_HANDLE(LNIntTable) intTable, int xSize, int ySize, int zSize);

	/**
		@brief		整数型テーブルの X 要素数を取得する
		@param[in]	intTable	: 整数型テーブルハンドル
		@param[out]	xSize		: X 要素数を格納する変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNIntTable_GetXSize(LN_HANDLE(LNIntTable) intTable, int* xSize);

	/**
		@brief		整数型テーブルの Y 要素数を取得する
		@param[in]	intTable	: 整数型テーブルハンドル
		@param[out]	ySize		: Y 要素数を格納する変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNIntTable_GetYSize(LN_HANDLE(LNIntTable) intTable, int* ySize);

	/**
		@brief		整数型テーブルの Z 要素数を取得する
		@param[in]	intTable	: 整数型テーブルハンドル
		@param[out]	zSize		: Z 要素数を格納する変数
	*/
	LN_INSTANCE_API
	LN_PROPERTY
	LNResult LNIntTable_GetZSize(LN_HANDLE(LNIntTable) intTable, int* zSize);


LN_CLASS_END

LN_MODULE_END

} // extern "C"

