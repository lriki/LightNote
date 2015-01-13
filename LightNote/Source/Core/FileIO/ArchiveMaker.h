//==============================================================================
// ArchiveMaker 
//------------------------------------------------------------------------------
///**
//  @file       ArchiveMaker.h
//  @brief      ArchiveMaker
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Base/RefString.h"

namespace LNote
{
namespace Core
{
namespace FileIO
{

//==============================================================================
// ■ ArchiveMaker
//------------------------------------------------------------------------------
///**
//  @brief      アーカイブファイルを作るためのクラス
//	@note
//				Windows (wchar_t が UTF-16 の環境)限定。
//*/
//==============================================================================
class ArchiveMaker
    : public Base::NonCopyable
{
public:
	ArchiveMaker();
	virtual ~ArchiveMaker();

public:

	//----------------------------------------------------------------------
	///**
	//  @brief		新しくアーカイブファイルを開く
	//
	//  @param[in]  filename_ : 新しく作成するアーカイブファイルの名前
	//  @param[in]  key_      : 暗号化に使う文字列 ( \0 終端 )
	//
	//  @retval     true  : 成功
	//  @retval     false : ファイルを開くことができなかった
	//
	//  @par
	//              ファイルを書き込み専用で開きます。
	//              ( fopen() に "wb" を指定した状態。無い場合は新規作成 )<br><br>
	//*/
	//----------------------------------------------------------------------
	bool open(const char* filename_, const char* key_ = NULL);
	bool open(const wchar_t* filePath, const char* key = NULL);

	//----------------------------------------------------------------------
	///**
	//  @brief		アーカイブファイルを閉じる
	//
	//  @par
	//              追加されたファイルの数を書き込んで、ファイルストリームを閉じます。
	//              開かれていない場合はなにもしません。<br>
	//              デストラクタでも呼び出されます。(std::fstream と同じ仕様)
	//*/
	//----------------------------------------------------------------------
    void close();

	//----------------------------------------------------------------------
	///**
	//  @brief		ファイルを追加する
	//
	//  @param[in]  filePath	: 追加するファイルの名前
	//  @param[in]  accessName	: プログラムからアクセスする時の名前
	//
	//  @retval     true  : 成功
	//  @retval     false : ファイルを開くことができなかった
	//
	//  @par
	//              filename_ のファイルをアーカイブに追加します。<br>
	//              access_name_ を指定した場合、ファイルに別名をつけた上で追加します。
	//              Archive クラスの open() によってアーカイブ内のファイルにアクセスする場合、
	//              この名前でアクセスするようにします。<br>
	//              access_name_ を省略した場合は filename_ がそのまま使われます。
	//*/
	//----------------------------------------------------------------------
    bool addFile( const wchar_t* filePath, const wchar_t* accessName = NULL );

private:

	/// パディングを考慮して、開いているアーカイブファイルにデータを書き込む
	void _writePadding16( const lnByte* data, lnU32 size );

	/// パディングを考慮して、整数を書き込む (ファイル名長さ、ファイルサイズ用)
	void _writeU32Padding16( lnU32 v0, lnU32 v1 );

private:

	// camellia key table type.
	static const int L_CAMELLIA_TABLE_BYTE_LEN = 272;
	static const int L_CAMELLIA_TABLE_WORD_LEN = (L_CAMELLIA_TABLE_BYTE_LEN / 4);
	typedef unsigned int KEY_TABLE_TYPE[L_CAMELLIA_TABLE_WORD_LEN];

    static const int KEY_SIZE  = 128;

	FILE*			        mStream;			///< アーカイブファイルのファイルストリーム
    Base::RefString	        mEncryptionKey;		///< 暗号化に使うキー
	lnU32				        mFileNum;			///< ファイル数
    //lnByte                  mKeyTable[ 256 ];
	KEY_TABLE_TYPE			mKeyTable;
    int                     mBlockLength;
};

} // namespace FileIO
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================