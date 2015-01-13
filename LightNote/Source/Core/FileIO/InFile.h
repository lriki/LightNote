//==============================================================================
// InFile 
//------------------------------------------------------------------------------
///**
//  @file       InFile.h
//  @brief      InFile
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "Interface.h"
#include "AsyncProcessObject.h"

namespace LNote
{
namespace Core
{
namespace FileIO
{

//==============================================================================
// ■ InFile
//------------------------------------------------------------------------------
///**
//  @brief      読み込み用のファイルを扱うクラス
//*/
//==============================================================================
class InFile
    : public AsyncLoadObject
    , public Stream
{
public:
	InFile( Manager* manager, Stream* stream );
	virtual ~InFile();
    LN_REFOBJ_METHODS;

public:
    virtual int		getSize() { return mInStream->getSize(); }
	virtual int		getPosition() { return mInStream->getPosition(); }
    virtual int		read( void* buffer, int buffer_size, int read_size = -1 );
    virtual void	seek( int offset, int origin ) { mInStream->seek( offset, origin ); }
	virtual void	load( bool async = false );
    virtual void*	getData() { return mData; }
	virtual bool	isLoadFinished() { return AsyncLoadObject::getAsyncIOState() == AsyncIOState_Completed || AsyncLoadObject::getAsyncIOState() == AsyncIOState_Failed || mLoadFinished; } 
    virtual bool	isError() { return AsyncLoadObject::getAsyncIOState() == AsyncIOState_Failed; }

protected:
	virtual bool	onLoadProc();
	virtual void	onLoadProcFinished() {}

private:
    Stream*		mInStream;      ///< 読むストリーム(FILE* とか)が入っている入力ストリームクラス
    lnByte*     mData;          ///< load() で読み込んだデータ
    bool        mLoadFinished;  ///< 読み込みの終了フラグ
};

} // namespace FileIO
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================