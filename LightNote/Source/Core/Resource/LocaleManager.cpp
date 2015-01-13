//==============================================================================
// LocaleManager 
//==============================================================================

#include "stdafx.h"
#include "LocaleManager.h"

namespace LNote
{
namespace Core
{
namespace Resource
{

	//static const lnChar* ERR_InvalidCallException = L"メソッドの呼び出しが無効です。引数に不正な値が渡された可能性があります。");
	//static const lnChar* ERR_OutOfMemoryException = L"操作を完了するのに十分なメモリ領域がありません。");
	//static const lnChar* ERR_FileNotFoundException = L"ファイルが存在しないか、アクセスできないファイルです。");
	//static const lnChar* ERR_SystemCallException = L"システム機能の呼び出しでエラーが発生しました。");
	//static const lnChar* ERR_FileFormatException = L"対応していないファイルフォーマットです。");
	//static const lnChar* ERR_NotImplementedException = L"未実装、または未対応の機能が呼び出されました。");
	//static const lnChar* ERR_InitializedException = L"オブジェクトは既に初期化されています。");
	//static const lnChar* ERR_FormatException = L"無効なフォーマットです。");
	//static const lnChar* ERR_ThreadException = L"スレッド内でハンドルされない例外が発生しました。");
	//static const lnChar* ERR_FatalException = L"致命的なエラーが発生しました。");
	//static const lnChar* ERR_ComException = L"HRESULT にエラーが返されました。");
	//static const lnChar* ERR_OpenGLException = L"OpenGL でエラーが返されました。");

	//static const wchar_t* ERR_ThreadExceptionAbortW = L"実行中のスレッドで例外が発生しました。";

//==============================================================================
// LocaleManager
//==============================================================================

std::vector< std::wstring >	LocaleManager::mStringTable;

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void LocaleManager::initialize()
{
	mStringTable.resize(String_Max);
	mStringTable[String_ERR_InvalidOperationException] = L"オブジェクトの状態に対する操作が無効です。";
	mStringTable[String_ERR_ArgumentException] = L"不正な引数です。";
	mStringTable[String_ERR_OutOfMemoryException] = L"操作を完了するのに十分なメモリ領域がありません。";
	mStringTable[String_ERR_FileNotFoundException] = L"ファイルが存在しないか、アクセスできないファイルです。";
	mStringTable[String_ERR_SystemCallException] = L"システム機能の呼び出しでエラーが発生しました。";
	mStringTable[String_ERR_NotImplementedException] = L"未実装、または未対応の機能が呼び出されました。";
	mStringTable[String_ERR_InitializedException] = L"オブジェクトは既に初期化されています。";
	mStringTable[String_ERR_InvalidFormatException] = L"無効なフォーマットです。";
	mStringTable[String_ERR_ThreadException] = L"スレッド内でハンドルされない例外が発生しました。";
	mStringTable[String_ERR_FatalException] = L"致命的なエラーが発生しました。";
	mStringTable[String_ERR_ComException] = L"HRESULT にエラーが返されました。";
	mStringTable[String_ERR_OpenGLException] = L"OpenGL でエラーが返されました。";
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
const wchar_t* LocaleManager::getString(int resourceID)
{
	return mStringTable[resourceID].c_str();
}

} // namespace Resource
} // namespace Core
} // namespace LNote
