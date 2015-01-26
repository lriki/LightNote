
#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

extern "C" {

/**
	@brief	ファイルやメモリストリームの読み書きや、ファイルシステムに関するモジュールです。
*/
LN_MODULE(IO)

//==============================================================================
// LNFile
//------------------------------------------------------------------------------
///**
//  @defgroup   group_file LNFile
//  @brief      ディレクトリ上のファイルまたは暗号化アーカイブ内のファイルにアクセスするためのモジュールです
//  @{
//*/
//==============================================================================
LN_CLASS(LNFile)
	
	LNOTE_TYPE_INFO_DECL(LNFile);


	/**
	//	@brief      ファイルを開く
		@param[out]	file		: 作成されたファイルハンドルを格納する変数
		@param[in]  filePath	: ファイル名
		@param[in]  mode		: ファイルを開く方法または作成する方法
		@param[in]  access		: ファイルにアクセスする方法
		@return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	ファイルの読み込みが終了したら LNFileStream_Close または LNObject_Release でファイルを閉じる必要があります。
	*/

	LNOTEAPI LNResult LNFile_Create(LNHandle* file, const LNChar* filePath, LNFileMode mode LNOTE_DEFAULT_ARG(LN_FILEMODE_OPEN), LNFileAccess access LNOTE_DEFAULT_ARG(LN_FILEACCESS_READ));


	/**
		@brief      ファイルを閉じる
		@param[in]  file	: ファイルハンドル
		@return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	*/

	LNOTEAPI LNResult LNFile_Close(LNHandle file);


	/**
		@brief      ファイルサイズを取得する
		@param[in]  file	: ファイルハンドル
		@param[out]	size	: ファイルサイズ (バイト単位) を格納する変数
		@return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	*/

	LNOTEAPI LNResult LNFile_GetSize(LNHandle file, int* size);


	/**
		@brief      ファイルポインタの位置を取得する
		@param[in]	file		: ファイルハンドル
		@param[out]	position	: ファイルポインタの位置 (バイト単位) を格納する変数
		@return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	*/

	LNOTEAPI LNResult LNFile_GetPosition(LNHandle file, int* position);


	/**
		@brief      ファイルポインタの位置を設定する
		@param[in]  file		: ファイルハンドル
		@param[in]  offset		: 読み込んだデータを格納するバッファ
		@param[in]  origin		: 起点
		@return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	*/

	LNOTEAPI LNResult LNFile_Seek(LNHandle file, int offset, LNSeekOrigin origin);


	/**
		@brief			ファイルからデータを読み込む
		@param[in]		file		: ファイルハンドル
		@param[out]		buffer		: 読み込んだデータを格納するバッファ
		@param[in]		readSize	: 読み込むサイズ (バイト数)
		@param[out]		validSize	: 読み込んだサイズ (バイト数) を格納する変数
		@return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	*/

	LNOTEAPI LNResult LNFile_Read(LNHandle file, void* buffer, int readSize, int* validSize LNOTE_DEFAULT_ARG(NULL));
	

	/**
		@brief			ファイルからデータを読み込む
		@param[in]		file		: ファイルハンドル
		@param[in]		buffer		: 書き込むデータのアドレス
		@param[in]		size		: 読み込むバイト数
		@return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	*/
	LNOTEAPI LNResult LNFile_Write(LNHandle file, const void* buffer, int size);


	/**
		@brief      ファイルの存在を調べる
		@param[in]	filePath    : ファイルパス
	//	@param[out]	result		: 結果を格納する変数 (LN_TRUE = 存在する / LN_FALSE = 存在しない)
		@return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	ディレクトリと全ての暗号化アーカイブを対象として確認します。
	*/
    LNOTEAPI LNResult LNFile_Exists( const LNChar* filePath, LNBool* result );

LN_CLASS_END

LN_MODULE_END

} // extern "C"
