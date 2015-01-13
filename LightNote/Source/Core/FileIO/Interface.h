//==============================================================================
// File 
//------------------------------------------------------------------------------
///**
//  @file       Interface.h
//  @brief      Interface
//*/
//==============================================================================

#pragma once

#include "Common.h"

namespace LNote
{
namespace Core
{
namespace FileIO
{

//==============================================================================
// Stream
//------------------------------------------------------------------------------
///**
//  @brief      入力ストリーム (読み込み専用) のインターフェイス
//*/
//==============================================================================
class Stream
    : public Base::ReferenceObject//public Base::Interface
{
public:

	/// ファイルサイズの取得
	virtual int getSize() = 0;

	/// ファイルポインタの位置の取得
	virtual int getPosition() = 0;

	/// データの読み込み (readSize = -1 ですべてのデータを読み込む)
	virtual int read( void* buffer, int bufferSize, int readSize = -1 ) = 0;

	/// ファイルポインタの設定
	virtual void seek( int offset_, int origin = SEEK_SET ) = 0;

protected:
	virtual ~Stream() {}
};

//==============================================================================
// Archive
//------------------------------------------------------------------------------
///**
//  @brief     
//*/
//==============================================================================
//class Archive
//    : public Base::ReferenceObject//public Base::Interface
//{
//public:
//
//	/// ファイルの存在を調べる
//	virtual bool existsFile( const wchar_t* fileFullPath ) = 0;
//
//	/// 入力ストリームの作成 (パスはフルパスであること)
//	virtual Stream* createStream(const wchar_t* fileFullPath, FileType type) = 0;
//};

} // namespace FileIO
} // namespace Core
} // namespace LNote
