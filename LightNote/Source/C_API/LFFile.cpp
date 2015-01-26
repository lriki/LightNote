//==============================================================================
// LFFile 
//------------------------------------------------------------------------------
/*	■ FileStream という名前にするべきか？
		.NET に慣れてる人ならいいけど、それ以外の人は混乱しそう。
		LNFile のインスタンスメソッドは System.IO.FileStream、
		LNFile の static メソッドは System.IO.File な感じでいいと思う。
*/
//==============================================================================

#include "stdafx.h"
#include "LFInternal.h"
#include "LFBase.h"
#include "LFFile.h"
#include "../Core/FileIO/File.h"
#include "../Core/FileIO/InFile.h"
#include "../Core/FileIO/OutFile.h"
#include "../Core/FileIO/ArchiveMaker.h"

#define FuncLibManager ::LNote::Core::Function::Manager

using namespace LNote;
using namespace LNote::Core;

//==============================================================================
// LNFile
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(FileIO::File, LNFile);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFile_Create(LNHandle* file, const LNChar* filePath, LNFileMode mode, LNFileAccess access)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::FileIO::File> obj(LN_NEW Core::FileIO::File());
		obj->open(
			FuncLibManager::FileIOManager, 
			filePath,
			(Core::FileIO::FileMode)mode,
			(Core::FileIO::FileAccess)access);
		obj.addRef();
		*file = TO_HANDLE(FuncLibManager::registerReferenceObject((Base::ReferenceObject*)obj));
		LN_FUNC_TRY_END_RETURN;
	}


	//----------------------------------------------------------------------
	///**
	//  @brief      メモリ上のデータから読み込み用ファイルを開く
	//  @param[out]	file		: 作成されたファイルハンドルを格納する変数
	//  @param[in]  data        : メモリ上のデータ配列
	//  @param[in]  dataSize	: データサイズ (バイト単位)
	//  @param[in]  copyData	: LN_TRUE の場合、data の内容のコピーを作成する
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@par
	//				メモリ上に確保されたバイト配列をファイルとみなして読み取りを行うための関数です。
	//				ファイルの読み込みが終了したら LNFile_Close または LNObject_Release でファイルを閉じる必要があります。<br>
	//				copyData が true の場合 data の内容のコピーを作成し、
	//				そこから読み取りを行います。この場合 data 内容はすぐに解放することができます。<br>
	//				逆に false の場合は data の内容を直接読み取ります。
	//				ファイルを閉じる前に data の内容を変更・解放すると、不正アクセスが発生する可能性があります。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFile_CreateFromMemory(LNHandle* file, const void* data, int dataSize, LNBool copyData);


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFile_CreateFromMemory(LNHandle* file, const void* data, int dataSize, LNBool copyData)
	{
		LN_FUNC_TRY_BEGIN;
		Base::RefPtr<Core::FileIO::Stream> stream;

		if (copyData != LN_FALSE) {
			Base::RefPtr<Core::FileIO::InManagedMemoryStream> memoryStream(
				LN_NEW Core::FileIO::InManagedMemoryStream());
			memoryStream->create(data, dataSize);
			stream = memoryStream;
		}
		else {
			Base::RefPtr<Core::FileIO::InUnManagedMemoryStream> memoryStream(
				LN_NEW Core::FileIO::InUnManagedMemoryStream());
			memoryStream->create(data, dataSize);
			stream = memoryStream;
		}

		LRefPtr<Core::FileIO::File> obj(LN_NEW Core::FileIO::File());
		obj->create(FuncLibManager::FileIOManager, stream);
		obj.addRef();
		*file = TO_HANDLE(FuncLibManager::registerReferenceObject((Base::ReferenceObject*)obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFile_Close(LNHandle file)
	{
		return LNObject_Release(file);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFile_GetSize(LNHandle file, int* size)
	{
		LN_CHECK_ARG_HANDLE(file);

		*size = TO_REFOBJ(FileIO::File, file)->getSize();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFile_GetPosition(LNHandle file, int* position)
	{
		LN_CHECK_ARG_HANDLE(file);

		*position = TO_REFOBJ(FileIO::File, file)->getPosition();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFile_Seek(LNHandle file, int offset, LNSeekOrigin origin)
	{
		LN_CHECK_ARG_HANDLE(file);

		TO_REFOBJ(FileIO::File, file)->seek(offset, origin);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFile_Read(LNHandle file, void* buffer, int readSize, int* validSize)
	{
		LN_FUNC_TRY_BEGIN;
		int s = TO_REFOBJ(FileIO::File, file)->read(buffer, readSize);
		if (validSize)
			*validSize = s;
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFile_Write(LNHandle file, const void* buffer, int size)
	{
		LN_FUNC_TRY_BEGIN;
		TO_REFOBJ(FileIO::File, file)->write(buffer, size);
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	///**
	//  @brief			ファイルからデータを非同期的に読み込む
	//  @param[in]		file		: ファイルハンドル
	//  @param[in,out]	buffer		: 読み込んだデータを格納するバッファ
	//  @param[in]		readSize	: 読み込むサイズ (バイト数)
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@par
	//					buffer に指定されたバッファは、処理が完了するまで解放してはいけません。<br>
	//					処理の完了やエラー状態は LNFile_GetAsyncIOState で確認できます。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFile_ReadAsync(LNHandle file, void* buffer, int readSize);
	/*DeclOpt
	CS_Enabled = false;
	DeclOpt*/
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFile_ReadAsync(LNHandle file, void* buffer, int readSize)
	{
		LN_FUNC_TRY_BEGIN;
		TO_REFOBJ(FileIO::File, file)->readAsync(buffer, readSize);
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルから内部メモリにデータを読み込む
	//  @param[in]  file		: ファイルハンドル
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@par
	//				内部でファイルサイズ分のメモリを確保し、ファイルの内容をすべて読み込みます。
	//				読み込んだ内容には LNFile_GetLoadedData でアクセスします。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFile_Load(LNHandle file);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFile_Load(LNHandle file)
	{
		LN_FUNC_TRY_BEGIN;
		TO_REFOBJ(FileIO::File, file)->load();
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルから内部メモリにデータを非同期的に読み込む
	//  @param[in]  file		: ファイルハンドル
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@par
	//				内部でファイルサイズ分のメモリを確保し、ファイルの内容をすべて読み込みます。
	//				読み込んだ内容には LNFile_GetLoadedData でアクセスします。<br>
	//				処理の完了やエラー状態は LNFile_GetAsyncLoadState で確認できます。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFile_LoadAsync(LNHandle file);
	/*DeclOpt
	CS_Enabled = false;
	DeclOpt*/
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFile_LoadAsync(LNHandle file)
	{
		LN_FUNC_TRY_BEGIN;
		TO_REFOBJ(FileIO::File, file)->loadAsync();
		LN_FUNC_TRY_END_RETURN;
	}


	//----------------------------------------------------------------------
	///**
	//  @brief      内部メモリに読み込んだデータの取得
	//  @param[in]  file		: ファイルハンドル
	//  @param[in]  data		: 読み込んだデータへのアドレスを格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@par
	//				LNFile_Load で読み込んだデータを取得します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFile_GetLoadedData(LNHandle file, const void** data);
	/*DeclOpt
	CS_Enabled = false;
	DeclOpt*/
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFile_GetLoadedData(LNHandle file, const void** data)
	{
		LN_FUNC_TRY_BEGIN;
		*data = TO_REFOBJ(FileIO::File, file)->getLoadedData();
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	///**
	//  @brief      非同期読み込みの処理状態の取得
	//  @param[in]  file		: ファイルハンドル
	//  @param[out] state		: 現在の状態を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFile_GetAsyncLoadState(LNHandle file, LNAsyncIOState* state);
	/*DeclOpt
	CS_Enabled = false;
	DeclOpt*/
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFile_GetAsyncLoadState(LNHandle file, LNAsyncIOState* state)
	{
		LN_FUNC_TRY_BEGIN;
		*state = (LNAsyncIOState)TO_REFOBJ(FileIO::File, file)->getAsyncLoadState();
		LN_FUNC_TRY_END_RETURN;
	}


	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルに指定バッファの内容を非同期的に書き込む
	//  @param[in]  file		: ファイルハンドル
	//	@param[in]  data		: 書き込むデータ
	//	@param[in]  dataSize	: 書き込むデータのサイズ (バイト数)
	//  @param[in]  async		: LN_TRUE の場合、非同期で書き込む
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@par
	//				非同期で書き込む場合、data の内容は一度内部のメモリにコピーされます。
	//				data 内容はすぐに解放することができます。<br>
	//				開始した場合は LNFile_GetAsyncSaveState で処理の完了やエラー状態を確認できます。			
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFile_SaveAsync(LNHandle file, const void* data, int dataSize);
	/*DeclOpt
	CS_Enabled = false;
	DeclOpt*/
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFile_SaveAsync(LNHandle file, const void* data, int dataSize)
	{
		LN_FUNC_TRY_BEGIN;
		TO_REFOBJ(FileIO::File, file)->saveAsync(data, dataSize);
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	///**
	//  @brief      非同期書き込みの処理状態の取得
	//  @param[in]  file		: ファイルハンドル
	//  @param[out]	state		: 現在の状態を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFile_GetAsyncSaveState(LNHandle file, LNAsyncIOState* state);
	/*DeclOpt
	CS_Enabled = false;
	DeclOpt*/
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFile_GetAsyncSaveState(LNHandle file, LNAsyncIOState* state)
	{
		LN_FUNC_TRY_BEGIN;
		*state = (LNAsyncIOState)TO_REFOBJ(FileIO::File, file)->getAsyncSaveState();
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFile_Exists(const LNChar* filePath, LNBool* result)
	{
		LN_FUNC_TRY_BEGIN;
		*result = LNOTE_BOOL_TO_LNBOOL(FuncLibManager::FileIOManager->existsFile(filePath));
		LN_FUNC_TRY_END_RETURN;
	}

//==============================================================================
// LDirectory
//==============================================================================


	//==============================================================================
	// LDirectory
	//------------------------------------------------------------------------------
	///**
	//  @defgroup   group_directory LDirectory
	//  @brief      ディレクトリユーティリティ
	//  @{
	//*/
	//==============================================================================

	//----------------------------------------------------------------------
	///**
	//  @brief      カレントディレクトリを移動する
	//  @param[in]  path		: ディレクトリパス
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@par
	//				現在のカレントディレクトリをスタックに記憶し、
	//				新しいカレントディレクトリを設定します。
	//				記憶したカレントディレクトリの復帰には LNFileIO_ReturnCurrentDirectory を使用します。<br>
	//				カレントディレクトリはスレッドごとに管理されます。<br>
	//				<br>
	//				注意)このカレントディレクトリはプロセスのカレントディレクトリに影響しません。
	//				(Win32API の SetCurrentDirectory 等は呼び出しません)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LDirectory_MoveCurrentDirectory(const LNChar* path);

	//----------------------------------------------------------------------
	///**
	//  @brief      カレントディレクトリを復帰する
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@par
	//				LNFileIO_MoveCurrentDirectory でスタックに記憶したディレクトリパスをひとつ取り出し、
	//				カレントディレクトリに設定します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LDirectory_ReturnCurrentDirectory();

	/** @} */	// defgroup


	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
	//LNResult LDirectory_MoveCurrentDirectory(const LNChar* path)
	//{
	//	LN_FUNC_TRY_BEGIN;
	//	FuncLibManager::FileIOManager->moveCurrentDirectory( path );
	//	LN_FUNC_TRY_END_RETURN;
	//}

	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
	//LNResult LDirectory_ReturnCurrentDirectory()
	//{
	//	LN_FUNC_TRY_BEGIN;
	//	FuncLibManager::FileIOManager->returnCurrentDirectory();
	//	LN_FUNC_TRY_END_RETURN;
	//}
#if 0
//==============================================================================
// LInFile
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LInFile_Create(LNHandle* inFile, const LNChar* filePath, _LNFileType mode)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::FileIO::InFile> obj(
			FuncLibManager::FileIOManager->createInFile( 
			filePath,
			(Core::FileIO::FileType)mode));
		obj.addRef();
		*inFile = TO_HANDLE(FuncLibManager::pushObject((Base::ReferenceObject*)obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LInFile_CreateFromMemory(LNHandle* inFile, const void* data, int dataSize, LNBool copyData)
	{
		//Base::RefPtr<Core::FileIO::InFile> file;
		
		LN_FUNC_TRY_BEGIN;
		Base::RefPtr<Core::FileIO::IInStream> stream;
		
		if ( copyData != LN_FALSE ) {
			Base::RefPtr<Core::FileIO::InManagedMemoryStream> memoryStream(
				LN_NEW Core::FileIO::InManagedMemoryStream() );
			memoryStream->create( data, dataSize );
			stream = memoryStream;
		}
		else {
			Base::RefPtr<Core::FileIO::InUnManagedMemoryStream> memoryStream(
				LN_NEW Core::FileIO::InUnManagedMemoryStream() );
			memoryStream->create( data, dataSize );
			stream = memoryStream;
		}

		LRefPtr<Core::FileIO::InFile> obj(
			LN_NEW Core::FileIO::InFile( FuncLibManager::FileIOManager, stream ) );
		obj.addRef();
		*inFile = TO_HANDLE(FuncLibManager::pushObject((Base::ReferenceObject*)obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LInFile_Close(LNHandle inFile)
	{
		return LObject_Release(inFile);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LInFile_GetSize(LNHandle inFile, int* size)
	{
		*size = TO_IN_FILE(inFile)->getSize();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LInFile_GetPosition(LNHandle inFile, int* position)
	{
		*position = TO_IN_FILE(inFile)->getPosition();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	int LInFile_Read(LNHandle inFile, LNByte* buffer, int readSize)
	{
		LN_FUNC_TRY_BEGIN;
		return TO_IN_FILE(inFile)->read(buffer, readSize);
        LN_FUNC_TRY_END;
		return 0;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LInFile_Seek(LNHandle inFile, int offset, LNSeekOrigin origin)
	{
		LN_FUNC_TRY_BEGIN;
		TO_IN_FILE(inFile)->seek(offset, origin);
        LN_FUNC_TRY_END;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LInFile_Load(LNHandle inFile, LNBool async)
	{
		LN_FUNC_TRY_BEGIN;
		TO_IN_FILE(inFile)->load((async != LN_FALSE));
        LN_FUNC_TRY_END;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void* LInFile_GetLoadedData(LNHandle inFile)
	{
		return TO_IN_FILE(inFile)->getData();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNAsyncIOState LInFile_GetAsyncIOState(LNHandle inFile)
	{
		return (LNAsyncIOState)TO_IN_FILE(inFile)->getAsyncIOState();
	}

//==============================================================================
// LOutFile
//==============================================================================
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNHandle LOutFile_Create(const LNChar* filePath, _LNFileType mode)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::FileIO::OutFile> obj(
			FuncLibManager::FileIOManager->createOutFile( 
				filePath,
				(Core::FileIO::FileType)mode));
		obj.addRef();
		return TO_HANDLE( FuncLibManager::pushObject( obj ) );
		LN_FUNC_TRY_END;
		return NULL;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LOutFile_Close(LNHandle outFile)
	{
		LObject_Release(outFile);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LOutFile_Save(LNHandle outFile, const void* data, int dataSize, LNBool async)
	{
		LN_FUNC_TRY_BEGIN;
		TO_OUT_FILE(outFile)->save(data, dataSize, (async != LN_FALSE));
        LN_FUNC_TRY_END;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNAsyncIOState LOutFile_GetAsyncIOState(LNHandle outFile)
	{
		return (LNAsyncIOState)TO_OUT_FILE(outFile)->getAsyncIOState();
	}
#endif

//==============================================================================
// ArchiveMaker
//==============================================================================

	Core::FileIO::ArchiveMaker gArchiveMaker;

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	LNOTEAPI LNBool LArchiveMaker_Open(const LNChar* filePath, const char* password)
	{
		return LNOTE_BOOL_TO_LNBOOL(gArchiveMaker.open(filePath, password));
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	LNOTEAPI LNBool LArchiveMaker_AddFile(const wchar_t* filePath, const wchar_t* accessName)
	{
		return LNOTE_BOOL_TO_LNBOOL(gArchiveMaker.addFile(filePath, accessName));
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	LNOTEAPI void LArchiveMaker_Close()
	{
		gArchiveMaker.close();
	}

#if 0

	//==============================================================================
	// LInFile
	//------------------------------------------------------------------------------
	///**
	//  @defgroup   group_in_file LInFile
	//  @brief		読み込み用ファイルオブジェクト
	//  @{
	//*/
	//==============================================================================

	//----------------------------------------------------------------------
	///**
	//	@brief      ファイルを読み込み用として開く
	//  @param[out]	inFile		: 作成された入力ファイルハンドルを格納する変数
	//  @param[in]  filePath	: ファイル名
	//  @param[in]  type		: オープンモード
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@par
	//				ファイルの読み込みが終了したら LInFile_Close または LObject_Release でファイルを閉じる必要があります。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LInFile_Create(LNHandle* inFile, const LNChar* filePath, LNFileType type LNOTE_DEFAULT_ARG(LN_FILETYPE_BINARY));

	//----------------------------------------------------------------------
	///**
	//  @brief      メモリ上のデータから読み込み用ファイルを開く
	//  @param[out]	inFile		: 作成された入力ファイルハンドルを格納する変数
	//  @param[in]  data        : メモリ上のデータへのポインタ
	//  @param[in]  dataSize	: データサイズ (バイト単位)
	//  @param[in]  copyData	: LN_TRUE の場合、data の内容のコピーを作成する
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@par
	//				メモリ上に確保されたバイト配列を入力ファイルとして扱い、
	//				読み取りを行うための関数です。
	//				ファイルの読み込みが終了したら LInFile_Close または LObject_Release でファイルを閉じる必要があります。<br>
	//				copyData が true の場合、data の内容のコピーを作成し、
	//				そこから読み取りを行います。data 内容はすぐに解放することができます。<br>
	//				逆に false の場合は data の内容を直接読み取ります。
	//				ファイルを閉じる前に data の内容を解放すると、不正アクセスが発生する可能性があります。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LInFile_CreateFromMemory(LNHandle* inFile, const LNByte* data, int dataSize, LNBool copyData);

	//----------------------------------------------------------------------
	///**
	//  @brief      入力ファイルを閉じる
	//  @param[in]  inFile	: 入力ファイルハンドル
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LInFile_Close( LNHandle inFile );

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルサイズを取得する
	//  @param[in]  inFile	: 入力ファイルハンドル
	//  @param[out]	inFile	: ファイルサイズ (バイト単位) を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LInFile_GetSize(LNHandle inFile, int* size);

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルポインタの位置を取得する
	//  @param[in]  inFile	: 入力ファイルハンドル
	//  @param[out]  inFile	: ファイルポインタの位置 (バイト単位) を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LInFile_GetPosition(LNHandle inFile, int* position);

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルからデータを読み込む
	//  @param[in]  inFile		: 入力ファイルハンドル
	//  @param[in]  buffer		: 読み込んだデータを格納するバッファ
	//  @param[in]  readSize	: 読み込むサイズ (バイト数)
	//	@return		読み込んだサイズ (バイト数)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI int LInFile_Read(LNHandle inFile, LNByte* buffer, int readSize);

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルポインタの位置を設定する
	//  @param[in]  inFile		: 入力ファイルハンドル
	//  @param[in]  offset		: 読み込んだデータを格納するバッファ
	//  @param[in]  origin		: 起点
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LInFile_Seek(LNHandle inFile, int offset, LNSeekOrigin origin);

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルから内部メモリにデータを読み込む
	//  @param[in]  inFile		: 入力ファイルハンドル
	//  @param[in]  async		: true の場合、非同期で読み込む
	//	@par
	//				内部でファイルサイズ分のメモリを確保し、
	//				ファイルの内容をすべて読み込みます。
	//				読み込んだ内容には LInFile_GetLoadedData でアクセスします。<br>
	//				読み込みを非同期で開始した場合は LInFile_GetAsyncIOState で
	//				処理の完了やエラー状態を確認できます。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LInFile_Load(LNHandle inFile, LNBool async LNOTE_DEFAULT_ARG(LN_FALSE));

	//----------------------------------------------------------------------
	///**
	//  @brief      内部メモリに読み込んだデータの取得
	//  @param[in]  inFile		: 入力ファイルハンドル
	//	@return		読み込んだデータ
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void* LInFile_GetLoadedData(LNHandle inFile);

	//----------------------------------------------------------------------
	///**
	//  @brief      非同期読み込みの処理状態の取得
	//  @param[in]  inFile		: 入力ファイルハンドル
	//	@return		現在の状態
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNAsyncIOState LInFile_GetAsyncIOState(LNHandle inFile);

	/** @} */	// defgroup LInFile

	//==============================================================================
	// LOutFile
	//------------------------------------------------------------------------------
	///**
	//  @defgroup   group_out_file LOutFile
	//  @brief      書き込み用ファイルオブジェクト
	//  @{
	//*/
	//==============================================================================

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルを書き込み用として開く
	//  @param[in]  filePath	: ファイル名
	//  @param[in]  type		: オープンモード
	//	@return     出力ファイルハンドル
	//	@par
	//				ファイルの書き込みが終了したら LOutFile_Close で
	//				ファイルを閉じる必要があります。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNHandle LOutFile_Create(const LNChar* filePath, LNFileType type LNOTE_DEFAULT_ARG(LN_FILETYPE_BINARY));

	//----------------------------------------------------------------------
	///**
	//  @brief      出力ファイルを閉じる
	//  @param[in]  outFile		: 出力ファイルハンドル
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LOutFile_Close(LNHandle outFile);

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルにメモリの内容をすべて書き込む
	//  @param[in]  outFile		: 出力ファイルハンドル
	//	@param[in]  data		: 書き込むデータ
	//	@param[in]  dataSize	: 書き込むデータのサイズ (バイト数)
	//  @param[in]  async		: LN_TRUE の場合、非同期で書き込む
	//	@par
	//				非同期で書き込む場合、data の内容は一度内部のメモリにコピーされます。
	//				data 内容はすぐに解放することができます。<br>
	//				開始した場合は LOutFile_GetAsyncIOState で処理の完了やエラー状態を確認できます。			
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LOutFile_Save(LNHandle outFile, const void* data, int dataSize, LNBool async LNOTE_DEFAULT_ARG(LN_FALSE));

	//----------------------------------------------------------------------
	///**
	//  @brief      非同期書き込みの処理状態の取得
	//  @param[in]  outFile		: 出力ファイルハンドル
	//	@return		現在の状態
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNAsyncIOState LOutFile_GetAsyncIOState(LNHandle outFile);

	/** @} */	// defgroup LOutFile
#endif

#if 0

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
LNOTE_FUNC_NAMESPACE_BEGIN

//==============================================================================
// ■ LInFile
//==============================================================================

//---------------------------------------------------------------------
// ファイルを開く
//---------------------------------------------------------------------
int LInFile_Open( const LNChar* filename_ )
{
    FileIO::InFile* file;
    gFramework->getFileManager()->createInFile( &file, filename_ );

    return LN_TO_INT( file );
}

//---------------------------------------------------------------------
// ファイルを閉じる
//---------------------------------------------------------------------
void LInFile_Close( int file_ )
{
    FileIO::InFile* file = LN_INT_TO( FileIO::IInFile*, file_ );
    LN_SAFE_RELEASE( file );
}

//---------------------------------------------------------------------
// ファイルサイズの取得
//---------------------------------------------------------------------
int LInFile_GetSize( int file_ )
{
    return LN_INT_TO( FileIO::InFile*, file_ )->getSize();
}

//---------------------------------------------------------------------
// ファイルの内容を全て読み込む
//---------------------------------------------------------------------
bool LInFile_Load( int file_, bool async_ )
{
    LN_INT_TO( FileIO::InFile*, file_ )->load( async_ );
    return true;
}

//---------------------------------------------------------------------
// load() で読み込まれたデータの取得
//---------------------------------------------------------------------
void* LInFile_GetData( int file_ )
{
    return LN_INT_TO( FileIO::InFile*, file_ )->getData();
}

//---------------------------------------------------------------------
// 非同期読み込みの完了を判定する
//---------------------------------------------------------------------
bool LInFile_IsLoadFinished( int file_ )
{
    return LN_INT_TO( FileIO::InFile*, file_ )->isLoadFinished();
}

//---------------------------------------------------------------------
// 読み込みでエラーが発生したかを判定する
//---------------------------------------------------------------------
bool LInFile_IsError( int file_ )
{
    return LN_INT_TO( FileIO::InFile*, file_ )->isError();
}

//==============================================================================
// ■ ArchiveMaker
//==============================================================================

Core::FileIO::ArchiveMaker gArchiveMaker;

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
int LArchiveMaker_Open( const LNChar* filename_, const LNChar* key_ )
{
    return gArchiveMaker.open( filename_, key_ );
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
int LArchiveMaker_AddFile( const LNChar* filename_, const LNChar* access_name_ )
{
    return gArchiveMaker.addFile( filename_, access_name_ );
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void LArchiveMaker_Close()
{
    gArchiveMaker.close();
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

LNOTE_FUNC_NAMESPACE_END

#endif



#if 0

//==============================================================================
// LFFile 
//------------------------------------------------------------------------------
///**
//  @file       LFFile.h
//  @brief      LFFile
//	@defgroup   group_lib_file FileIO
//	@{
//*/
//==============================================================================

#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

extern "C" {

//==============================================================================
// LNFile
//------------------------------------------------------------------------------
///**
//  @defgroup   group_file LNFile
//  @brief      ディレクトリ上のファイルまたは暗号化アーカイブ内のファイルにアクセスするためのモジュールです
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNFile);

	//----------------------------------------------------------------------
	///**
	//	@brief      ファイルを開く
	//  @param[out]	file		: 作成されたファイルハンドルを格納する変数
	//  @param[in]  filePath	: ファイル名
	//  @param[in]  mode		: ファイルを開く方法または作成する方法
	//  @param[in]  access		: ファイルにアクセスする方法
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	ファイルの読み込みが終了したら LNFileStream_Close または LNObject_Release でファイルを閉じる必要があります。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFile_Create(LNHandle* file, const LNChar* filePath, LNFileMode mode LNOTE_DEFAULT_ARG(LN_FILEMODE_OPEN), LNFileAccess access LNOTE_DEFAULT_ARG(LN_FILEACCESS_READ));

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルを閉じる
	//  @param[in]  file	: ファイルハンドル
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFile_Close(LNHandle file);

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルサイズを取得する
	//  @param[in]  file	: ファイルハンドル
	//  @param[out]	size	: ファイルサイズ (バイト単位) を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFile_GetSize(LNHandle file, int* size);

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルポインタの位置を取得する
	//  @param[in]	file		: ファイルハンドル
	//  @param[out]	position	: ファイルポインタの位置 (バイト単位) を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFile_GetPosition(LNHandle file, int* position);

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルポインタの位置を設定する
	//  @param[in]  file		: ファイルハンドル
	//  @param[in]  offset		: 読み込んだデータを格納するバッファ
	//  @param[in]  origin		: 起点
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFile_Seek(LNHandle file, int offset, LNSeekOrigin origin);

	//----------------------------------------------------------------------
	///**
	//  @brief			ファイルからデータを読み込む
	//  @param[in]		file		: ファイルハンドル
	//  @param[out]		buffer		: 読み込んだデータを格納するバッファ
	//  @param[in]		readSize	: 読み込むサイズ (バイト数)
	//  @param[out]		validSize	: 読み込んだサイズ (バイト数) を格納する変数
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFile_Read(LNHandle file, void* buffer, int readSize, int* validSize LNOTE_DEFAULT_ARG(NULL));
	
	//----------------------------------------------------------------------
	///**
	//  @brief			ファイルからデータを読み込む
	//  @param[in]		file		: ファイルハンドル
	//  @param[in]		buffer		: 書き込むデータのアドレス
	//  @param[in]		size		: 読み込むバイト数
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFile_Write(LNHandle file, const void* buffer, int size);

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルの存在を調べる
	//  @param[in]	filePath    : ファイルパス
	//	@param[out]	result		: 結果を格納する変数 (LN_TRUE = 存在する / LN_FALSE = 存在しない)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	ディレクトリと全ての暗号化アーカイブを対象として確認します。
	//*/
	//----------------------------------------------------------------------
    LNOTEAPI LNResult LNFile_Exists( const LNChar* filePath, LNBool* result );

/** @} */	// defgroup

} // extern "C"
	
/** @} */	// defgroup


#endif

