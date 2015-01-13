//==============================================================================
// FileUtil 
//------------------------------------------------------------------------------
///**
//  @file       FileUtil.h
//  @brief      FileUtil
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <string>
#include "../Base/RefString.h"
#include "../Base/ReferenceBuffer.h"
#include "Common.h"

namespace LNote
{
namespace Core
{
namespace FileIO
{

//==============================================================================
// File
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class FileUtils
{
public:

	/// 指定されたファイルが存在するかどうかを判断する
	static bool exists( const char* path );

	/// 指定されたファイルが存在するかどうかを判断する
	static bool exists( const wchar_t* path );

	/// ファイルサイズを取得する
	static size_t getFileSize( FILE* stream );

	/// 数値をリトルエンディアンとして書き込む
	static void writeU32Little( FILE* stream, lnU32 v );

	/// 現在の位置とデータ(ファイル)サイズ、オフセット、基準を受け取って
	static int setSeekPoint( int curPoint, int maxSize, int offset, int origin );

	/// ディレクトリから直接ファイルを開いて入力ストリームを作成する
	static Stream* createInStream( const lnChar* filename );

	/// メモリ上に展開されたデータから入力ストリームを作成する (元のデータはストリームの作成直後に解放可能)
	static Stream* createInStreamFromMemoryManaged( const void* buffer, int size );

	/// モリ上に展開されたデータから入力ストリームを作成する (元のデータを単純に参照するだけ)
	static Stream* createInStreamFromMemoryUnManaged( const void* buffer, int size );

	/// 数値 (2バイト) の読み込み (ファイルポインタ+2)
	static lnU16 readU16(Stream* stream);

	/// 数値 (4バイト) の読み込み (ファイルポインタ+2)
	static lnU32 readU32(Stream* stream);

	/// 数値 (2バイト) の読み込み (ビッグエンディアンから)
	static lnU16 readU16Big(Stream* stream);

	/// 数値 (4バイト) の読み込み (ビッグエンディアンから)
	static lnU32 readU32Big(Stream* stream);

	/// ファイルの内容をすべて読み込む (バイナリ形式)
	static Base::ReferenceBuffer* readAllBytes( const lnChar* filePath );

	/// ファイルの内容をすべて読み込む (テキスト形式)
	static Base::ReferenceBuffer* readAllText( const lnChar* filePath );

	/// ストリームの内容をすべて読み込む
	static Base::ReferenceBuffer* readAllData(Stream* stream);

	/// 配列の内容をバイナリファイルとして書き出す
	static void writeAllBytes( const lnChar* filePath, const lnByte* buffer, size_t size );

	/// 文字列をテキストファイルとして書き出す
	static void writeAllText( const lnChar* filePath, const lnChar* text );
};
	
//==============================================================================
// Path
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class Path
{
public:

	/// ファイルパスの中からディレクトリパスの部分を取り出す (後ろから最初に見つかった / or \\ の前を返す。/ or \\ は含まない)
	static lnRefStringA getDirectoryPath( const char* path );

	/// ファイルパスの中からディレクトリパスの部分を取り出す (後ろから最初に見つかった / or \\ の前を返す。/ or \\ は含まない)
	static lnRefStringW getDirectoryPath( const wchar_t* path );

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルパスの中から拡張子を含むファイル名の部分を返す
	//*/
	//----------------------------------------------------------------------
	static lnRefString getFileName( const lnChar* path_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルパスからディレクトリパスとファイルパスを返す ( char )
	//
	//  @param[in]  path_     : 操作元のファイルパス
	//  @param[in]  dir_      : ディレクトリパスを格納するバッファへのポインタ または NULL
	//  @param[in]  name_     : ファイル名を格納するバッファへのポインタ または NULL
	//
	//  @return     現在は常に true を返します。
	//
	//  @par
	//              dir_ と name_ は LN_MAX_PATH 分の文字を格納できるサイズが必要です。<br>
	//              <br>
	//              path_ がディレクトリパスだけの場合は name_ にディレクトリ名が
	//              格納されます。<br>
	//              <br>
	//              "C:/Source/Test.cpp"   => dir_ = "C:/Source",  name_ = "Test.cpp"<br>
	//              "C:\\Source\\Test.cpp" => dir_ = "C:\\Source", name_ = "Test.cpp"<br>
	//              "Test.cpp"             => dir_ = "",           name_ = "Test.cpp"<br>
	//*/
	//----------------------------------------------------------------------
	static bool getDirPathAndFilePath( const char* path_, char* dir_, char* name_ );

	/// ファイルパスからディレクトリパスとファイルパスを返す ( wchar_t )
	static bool getDirPathAndFilePath( const wchar_t* path_, wchar_t* dir_, wchar_t* name_ );





	/// ファイルパスの拡張子を取り除いたものを返す
	static Base::RefString getExtName( const char* filePath );

	/// ファイルパスの拡張子を取り除いたものを返す
	static Base::RefWString getExtName( const wchar_t* filePath );

	/// path がフルパスであるかを判定する ("C:/AAA"、"/AAA" 両方判定)
	static bool isFullPath( const char* path );

	/// path がフルパスであるかを判定する ("C:/AAA"、"/AAA" 両方判定)
	static bool isFullPath( const wchar_t* path );

	/// パスのセパレータを統一し、パスの正規化を行う (フルパスで、ドライブレター無しのパスを渡すこと)
	static bool canonicalizePath( const char* inPath, char* outPath );

	/// パスのセパレータを統一し、パスの正規化を行う (フルパスで、ドライブレター無しのパスを渡すこと)
	static bool canonicalizePath( const wchar_t* inPath, wchar_t* outPath );

	/// パスのセパレータを統一する
	static void normalizePath( const char* inPath, char* outPath );

	/// パスのセパレータを統一する
	static void normalizePath( const wchar_t* inPath, wchar_t* outPath );

	/// パスのセパレータを統一する
	static void normalizePath( std::string* path );

	/// パスのセパレータを統一する
	static void normalizePath( std::wstring* path );

	/// パスのセパレータを統一する
	static void normalizePath( LRefString* path );

	/// パスのセパレータを統一する
	static void normalizePath( LRefWString* path );
};

} // namespace FileIO
} // namespace Core
} // namespace LNote
