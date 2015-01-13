//==============================================================================
// Archive 
//------------------------------------------------------------------------------
///**
//  @file       Archive.h
//  @brief      Archive
//*/
//==============================================================================

#pragma once

#include <map>
#include <string>
#include "../Base/RefString.h"
#include "../Base/Unicode.h"
#include "../Threading/Mutex.h"
#include "Interface.h"

namespace LNote
{
namespace Core
{
namespace FileIO
{
class ArchiveInStream;

//==============================================================================
// Archive
//------------------------------------------------------------------------------
///**
//  @brief      アーカイブファイルを扱うクラス
//	@note
//				アーカイブファイル内のファイル名文字コードは UTF-16。
//				ファイルマップに展開するときは環境依存の wchar_t に変換される。
//*/
//==============================================================================
class Archive
	: public Base::ReferenceObject/*public Base::ReferenceObject
    , *///public Archive
{
public:	
	static const int Version_100 = 100;
	static const lnByte InternalKey[16];

public:
	Archive();
	virtual ~Archive();
    LN_REFOBJ_METHODS;

public:

	//----------------------------------------------------------------------
    /**
	//  @brief      アーカイブファイルを開いてアクセスの準備をする
	//
	//  @param[in]  directory_	: アーカイブファイルのあるディレクトリのフルパス
	//  @param[in]  filename_	: アーカイブファイルのファイル名
	//  @param[in]  key_		: 複合に使うキー ( \0 終端文字列 または NULL )
	//
	//  @par
	//              filename_ に NULL を渡すと、ダミーアーカイブとして初期化します。
	//              ダミーアーカイブは、直接ディレクトリ上のファイルを扱います。
	//
	//              filename_ にディレクトリパスが含まれている場合、
	//              アーカイブに含まれているファイルはすべてそのディレクトリパス
	//              以下のファイルとして扱います。
@code
// アーカイブファイル内に「Chara.png」「Map/Ground.png」というファイルがある場合…

// 例 1 )
// 実行ファイルと同じフォルダにあるアーカイブファイル "Image.lna" を使う
archive.initialize( "Image.lna" );

file.open( "Image/Chara.png" );
file.open( "Image/Map/Ground.png" );


// 例 2 )
// 実行ファイルと同じフォルダにある「Data」フォルダ内のアーカイブファイル "Image.lna" を使う
archive.initialize( "Data/Image.lna" );

file.open( "Data/Image/Chara.png" );
file.open( "Data/Image/Map/Ground.png" );
@endcode
	//*/
	//----------------------------------------------------------------------
    void initialize( const lnChar* directory, const lnChar* filename, const char* key );

private:
	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルストリームとデータ位置までのオフセット取得  ※安全性のため、使用非推奨。createStream() を使ってください 
	//
	//  @param[in]  filename_    : ファイル名 (/ は \\ に変換済みであること)
	//  @param[out] out_stream_  : ファイルストリームへのポインタを格納する変数へのポインタ
	//  @param[out] data_offset_ : データがある場所までのオフセット ( バイト単位 ) を格納する変数へのポインタ
	//  @param[out] data_size_   : ファイルサイズを格納する変数へのポインタ
	//
	//  @par
	//              ファイルが見つからなかった場合は out_stream_ に NULL が代入されます。   <br>
	//                                                                                      <br>
	//              ダミーアーカイブの場合、out_stream_ は常に NULL です。          
	//
	//  @attention 
	//              この関数で取得したファイルストリームは必ずこのクラスの close() 関数で
	//              閉じてください。fclose() で閉じるとアクセス違反が発生します。
	//*/
	//----------------------------------------------------------------------
	void open(const wchar_t* filename_, FILE** out_stream_, int* data_offset_, int* data_size_, FileType type);

public:
	virtual bool		existsFile( const wchar_t* fileFullPath );
	virtual Stream*	createStream(const wchar_t* fileFullPath, FileType type);

	//----------------------------------------------------------------------
	///**
	//  @brief      データを読み込む
	//
	//  @param[out] buffer_ : データを書き込むバッファへのポインタ
	//  @param[in]  count_	: 書き込むデータサイズ ( バイト数 )
	//  @param[in]  stream_ : ファイルストリーム
	//
	//  @return     読み込んだデータサイズ
	//
	//  @par
	//              データの暗号を解除しながら読み込みを行います。
	//              fread() の代わりに使います。
	//              fseek() で正しい位置を指しているストリームを渡してください。<br>
	//              ダミーアーカイブの場合の動作は fread() と同じです
	//*/
	//----------------------------------------------------------------------
	lnU32 read(void* buffer, lnU32 count, FILE* stream, FileType type);

private:

	// 数値を 16 にそろえるために加算する数値「( 16 - ( v_ % 16 ) ) % 16」の最適化 ( 5 は 11、27 は 5 等 )
    int _padding16( int v ) const { return ( v != 0 ) ? ( 16 - ( v & 0x0000000f ) ) & 0x0000000f : 16; }
    lnU32 _padding16( lnU32 v ) const { return ( v != 0 ) ? ( 16 - ( v & 0x0000000f ) ) & 0x0000000f : 16; }

	/// 一時バッファのサイズチェック
    void _checkTempBuffer( lnU32 request_size );

	/// パディングを考慮して整数を読み込む
	lnU32 _readU32Padding16();

	/// パディングを考慮して整数を読み込む (ファイル名長さ、ファイルサイズ用)
	void _readU32Padding16( lnU32* v0, lnU32* v1 );

	/// パディングを考慮してデータを読み込む
	int _readPadding16( void* buffer, int count );

private:

	/// ファイルひとつ分の情報
	struct Entry
	{
		lnU32		mOffset;		///< ストリーム先頭からファイルの位置までのオフセット
		lnU32		mSize;			///< ファイルサイズ
	};

	typedef std::map<std::wstring, Entry>	EntriesMap;
	typedef std::pair<std::wstring, Entry>	EntriesPair;

private:

	// camellia key table type.
	static const int L_CAMELLIA_TABLE_BYTE_LEN = 272;
	static const int L_CAMELLIA_TABLE_WORD_LEN = (L_CAMELLIA_TABLE_BYTE_LEN / 4);
	typedef unsigned int KEY_TABLE_TYPE[L_CAMELLIA_TABLE_WORD_LEN];

    static const int KEY_SIZE  = 128;

    std::wstring		mArchiveDirectory;  ///< アーカイブファイルをディレクトリに見立てた時の、そこまのパス ( [.lnaの親フルパス]/[拡張子を除いたアーカイブファイル名]/ )
	EntriesMap	        mEntriesMap;	    ///< ファイル名に対応するファイル情報を格納する map
	FILE*		        mStream;		    ///< アーカイブファイルのストリーム
	int			        mFileNum;		    ///< アーカイブファイル内のファイル数
    Base::RefTString    mKey;			    ///< 復号キー (char)
	KEY_TABLE_TYPE      mKeyTable/*[256]*/;
    lnByte*             mTempBuffer;
    lnU32				mTempBufferSize;
    Threading::Mutex	mLock;
};

//==============================================================================
// ArchiveInStream
//------------------------------------------------------------------------------
///**
//  @brief      アーカイブからファイルを読むためのストリームクラス
//  @note
//              コンストラクタは private。
//              Archive クラスから作成すること。
//*/
//==============================================================================
class ArchiveInStream
    : /*public Base::ReferenceObject
	, */public Stream
{
private:
	ArchiveInStream(Archive* archive, FILE* stream, lnU32 data_offset, lnU32 data_size, FileType type);
	virtual ~ArchiveInStream();
    LN_REFOBJ_METHODS;

public:

	/// ファイル ( データ ) サイズの取得
    virtual int getSize() { return mDataSize; }

	/// ファイルポインタの位置の取得
	virtual int getPosition() { return mSeekPoint; }

	/// データの読み込み
	virtual int read( void* buffer, int buffer_size, int read_size = -1 );

	/// ファイルポインタの設定
	virtual void seek( int offset, int origin = SEEK_SET );

private:

    Archive*        mArchive;           ///< このクラスを作成したアーカイブクラス
    FILE*           mStream;            ///< ファイルストリーム
    lnU32			mDataOffset;        ///< ファイルの先頭からデータの先頭位置までのオフセット
    lnU32			mDataSize;          ///< データサイズ
    lnU32			mSeekPoint;         ///< シーク位置
	FileType		mOpenType;

    friend class Archive;
};

//==============================================================================
// DummyArchive
//------------------------------------------------------------------------------
///**
//  @brief      ダミーアーカイブ(ディレクトリ上から開く)
//*/
//==============================================================================
class DummyArchive
    : /*public Base::ReferenceObject
    , */public Archive
{
public:
	DummyArchive() {}
	virtual ~DummyArchive() {}
    //LN_REFOBJ_METHODS;

public:
	virtual bool		existsFile( const wchar_t* fileFullPath );
	virtual Stream*	createStream(const wchar_t* fileFullPath, FileType mode);
};

} // namespace FileIO
} // namespace Core
} // namespace LNote
