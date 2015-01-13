//==============================================================================
// StreamObject 
//------------------------------------------------------------------------------
///**
//  @file       StreamObject.h
//  @brief      StreamObject
//*/
//==============================================================================

#pragma once

#include "Interface.h"

namespace LNote
{
namespace Core
{
namespace FileIO
{

//==============================================================================
// InFileStream
//------------------------------------------------------------------------------
///**
//  @brief      単純な入力ファイルのクラス
//*/
//==============================================================================
class InFileStream
    : public Stream
    //, public Base::ReferenceObject
{
public:
	InFileStream();
	virtual ~InFileStream();
    LN_REFOBJ_METHODS;

public:

	/// ファイルを開く
	void open(const char* filePath, FileType type = FileType_Binary);

	/// ファイルを開く
	void open(const wchar_t* filePath, FileType type = FileType_Binary);

	/// ファイル (データ) サイズの取得
    virtual int getSize() { return mSize; }

	/// ファイルポインタの位置の取得
	virtual int getPosition() { return ftell( mStream ); }

	/// データの読み込み
	virtual int read( void* buffer, int bufferSize, int readSize );

	/// ファイルポインタの設定 (範囲外は基本的に assert とする)
	virtual void seek( int offset, int origin );

private:

	/// ファイルを閉じる
    void _close();

private:

    FILE*   mStream;
    int     mSize;
};

//==============================================================================
// InManagedMemoryStream
//------------------------------------------------------------------------------
///**
//  @brief      メモリ上に展開されたデータを入力ストリームとして扱うクラス
//
//  @note
//              initialize() で受け取ったデータは、それと同じ大きさの
//              メモリを確保してコピーします。<br>
//              元のメモリは initialize() の直後に解放することができます。
//*/
//==============================================================================
class InManagedMemoryStream
    : public Stream
    //, public Base::ReferenceObject
{
public:
	InManagedMemoryStream();
	virtual ~InManagedMemoryStream();
    LN_REFOBJ_METHODS;

public:

	/// 作成
    void create( const void* buffer, int size );

	/// ファイル ( データ ) サイズの取得
    virtual int getSize() { return mSize; }

	/// ファイルポインタの位置の取得
    virtual int getPosition() { return mSeekPoint; }

	/// データの読み込み
	virtual int read( void* buffer, int buffer_size, int read_size );

	/// ファイルポインタの設定
	virtual void seek( int offset, int origin );

private:
    void _dispose();

private:
    int     mSize;          ///< データサイズ
    lnByte*	mBuffer;        ///< データ本体
    int     mSeekPoint;     ///< シーク位置
};

//==============================================================================
// InUnManagedMemoryStream
//------------------------------------------------------------------------------
///**
//  @brief      メモリ上に展開されたデータを入力ストリームとして扱うクラス
//*/
//==============================================================================
class InUnManagedMemoryStream
    : public Stream
    //, public Base::ReferenceObject
{
public:
	InUnManagedMemoryStream();
	virtual ~InUnManagedMemoryStream();
    LN_REFOBJ_METHODS;

public:

	/// 作成
    void create( const void* buffer, int size );

	const lnByte* getCurrentBuffer() const { return (mBuffer + mSeekPoint); }

	/// ファイル ( データ ) サイズの取得
    virtual int getSize() { return mSize; }

	/// ファイルポインタの位置の取得
    virtual int getPosition() { return mSeekPoint; }

	/// データの読み込み
	virtual int read( void* buffer, int buffer_size, int read_size );

	/// ファイルポインタの設定
	virtual void seek( int offset, int origin );

private:
    int             mSize;          ///< データサイズ
    const lnByte*   mBuffer;        ///< データ本体
    int             mSeekPoint;     ///< シーク位置
};

} // namespace FileIO
} // namespace Core
} // namespace LNote
