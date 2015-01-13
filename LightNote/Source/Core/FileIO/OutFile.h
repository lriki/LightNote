//==============================================================================
// OutFile 
//------------------------------------------------------------------------------
///**
//  @file       OutFile.h
//  @brief      OutFile
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
// ■ OutFile
//------------------------------------------------------------------------------
///**
//  @brief      書き込み用のファイルを扱うクラス
//*/
//==============================================================================
class OutFile
    : public AsyncSaveObject
{
public:
	OutFile(Manager* manager_, const lnChar* filePath, FileType type);
	virtual ~OutFile();

public:
	virtual void save( const void* buffer, int size, bool async );
	virtual bool isSaveFinished() { return AsyncSaveObject::getAsyncIOState() == AsyncIOState_Completed || AsyncSaveObject::getAsyncIOState() == AsyncIOState_Failed || mSaveFinished; } 
	virtual bool isError() { return AsyncSaveObject::getAsyncIOState() == AsyncIOState_Failed; }

protected:
	virtual bool onSaveProc();
	virtual void onSaveProcFinished() {}

private:	
    lnString		mFilePath;
	FileType		mType;
    const void*     mData;
    int		        mSize;
    bool			mSaveFinished;
};

} // namespace FileIO
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================