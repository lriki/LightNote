//==============================================================================
// Common 
//------------------------------------------------------------------------------
///**
//  @file       Common.h
//  @brief      Common
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <vector>

namespace LNote
{
namespace Core
{
namespace FileIO
{
//class IInStream;
class Stream;
class InFile;
class OutFile;
class Archive;
class Manager;

/// ファイルへのアクセス制限
enum FileAccessPriority
{
	FileAccessPriority_ArchiveFirst = 0,	///< アーカイブ優先
	FileAccessPriority_DirectoryFirst,		///< ディレクトリ優先
	FileAccessPriority_ArchiveOnly,			///< アーカイブのみ   
};

/// ファイル形式
enum FileType
{
	FileType_Binary = 0,			///< バイナリモードで開く
	FileType_Text,					///< テキストモードで開く

	FileType_Max,
};

/// ファイルを開く方法または作成する方法
enum FileMode
{
	FileMode_Open = 0,			///< 既存のファイルを開く
	FileMode_Create,			///< 新しいファイルを作成する。既に存在する場合は上書きされる
	FileMode_Append,			///< ファイルを開き、ファイルの末尾をシークする。存在しない場合は新しいファイルを作成。

	FileMode_Max,
};

/// ファイルにアクセスする方法
enum FileAccess
{
	FileAccess_Read = 0,		///< 読み取りアクセス
	FileAccess_ReadWrite,		///< 読み取り及び書き込みアクセス
	FileAccess_Write,			///< 書き込みアクセス

	FileAccess_Max,
};

/// 非同期処理の状態
enum AsyncIOState
{
	AsyncIOState_Idle = 0,				///< 待機状態 (初期状態)
	AsyncIOState_Ready,					///< 実行可能状態 (start() 後。キューに入っている状態)
	AsyncIOState_Processing,			///< 実行状態 (処理中)
	AsyncIOState_Completed,				///< 終了状態 (処理終了後)
	AsyncIOState_Failed,				///< 終了状態 (処理終了後・処理に失敗した)

	AsyncIOState_Max,
};

/// アーカイブファイルのファイル名とキーの組み合わせ
struct ArchiveOpenData
{
    lnString	Directory;				///< アーカイブファイルが保存されている実際のディレクトリ
    lnString	ArchiveName;			///< アーカイブファイルの名前
    lnString	ArchiveKey;				///< 暗号化アーカイブファイルを開くためのキー
};

#pragma pack(1)
struct ArchiveHeader
{
    char	ID[4];			///< 'l' 'n' 'a' ' '
	lnU8	Version;
    lnByte	Padding[11];	///< サイズが 16 の倍数になるようにするためのパディング
};
#pragma pack()

typedef std::vector<ArchiveOpenData> ArchiveOpenDataList;

} // namespace FileIO
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================