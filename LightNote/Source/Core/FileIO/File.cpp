//==============================================================================
// File 
//==============================================================================

#include "stdafx.h"
#include "Manager.h"
#include "File.h"
#include "FileUtil.h"

namespace LNote
{
namespace Core
{
namespace FileIO
{

//==============================================================================
// File
//==============================================================================

LN_TYPE_INFO_ACCESS_IMPL(File);

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
File::File()
	: mManager(NULL)
	, mInFile(NULL)
	, mStream(NULL)
	, mFileAccess(FileAccess_Read)
	, mLoadedData(NULL)
	, mSavingData(NULL)
	, mAsyncLoadState(AsyncIOState_Idle)
	, mAsyncSaveState(AsyncIOState_Idle)
	, mAsyncLoadTargetBuffer(NULL)
	, mAsyncLoadTargetBufferSize(0)
	, mAsyncSaveTargetBuffer(NULL)
	, mAsyncSaveTargetBufferSize(0)
{
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
File::~File()
{
	close();
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void File::open(
	Manager* manager,
	const char* filePath, 
	FileMode mode,
	FileAccess access)
{
	Base::UnicodeString path;
	path.setDecodeBytes(filePath);
	open(manager, path.getString(), mode, access);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void File::open(
	Manager* manager,
	const wchar_t* filePath, 
	FileMode mode,
	FileAccess access)
{
	close();
	LN_REFOBJ_SET(mManager, manager);

	mFileAccess = access;

	// 読み取り専用の場合は InFile を使う
	if (mode == FileMode_Open && access == FileAccess_Read)
	{
		mInFile = mManager->createInFile(filePath, FileType_Binary);
	}
	// それ以外は普通のファイルストリーム
	else
	{
		const wchar_t* modeTable[FileMode_Max][FileAccess_Max] = {
				// FileAccess_Read	FileAccess_ReadWrite	FileAccess_Write
				{ NULL,				L"w+b",					L"wb" },		// FileMode_Create
				{ L"rb",			L"w+b",					L"wb" },		// FileMode_Open
				{ L"ab",			L"a+b",					L"a+b" },		// FileMode_Append
		};

		const wchar_t* m = modeTable[mode][access];
		LN_THROW_FileNotFound(m != NULL, filePath);

		errno_t err = _wfopen_s(&mStream, filePath, m);
		LN_THROW_FileNotFound(err == 0, filePath);
	}
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void File::create(Manager* manager, Stream* stream)
{
	close();
	LN_REFOBJ_SET(mManager, manager);

	mFileAccess = FileAccess_Read;
	LN_REFOBJ_SET(mInFile, stream);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void File::close()
{
	SAFE_DELETE_ARRAY(mLoadedData);
	SAFE_DELETE_ARRAY(mSavingData);

	LN_SAFE_RELEASE(mInFile);
	if (mStream != NULL)
		fclose(mStream);

	LN_SAFE_RELEASE(mManager);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void File::readAsync(void* buffer, int byteCount)
{
	LN_THROW_InvalidOperation(mFileAccess != FileAccess_Write);

	mAsyncLoadTargetBuffer = buffer;
	mAsyncLoadTargetBufferSize = byteCount;

	mManager->requestAsyncSave(this);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void File::write(const void* buffer, int byteCount)
{
	LN_THROW_InvalidOperation(mFileAccess != FileAccess_Read);

	if (mStream != NULL) {
		fwrite(buffer, 1, byteCount, mStream);
	}
	else {
		LN_THROW_InvalidOperation(0);
	}
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void File::writeAsync(const void* buffer, int byteCount)
{
	LN_THROW_InvalidOperation(mFileAccess != FileAccess_Read);

	if (mStream != NULL) {
		mAsyncSaveTargetBuffer = buffer;
		mAsyncSaveTargetBufferSize = byteCount;
		mManager->requestAsyncSave(this);
	}
	else {
		LN_THROW_InvalidOperation(0);
	}
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void File::load()
{
	LN_THROW_InvalidOperation(mFileAccess != FileAccess_Write);

	SAFE_DELETE_ARRAY(mLoadedData);
	size_t size = getSize();
	mLoadedData = LN_NEW lnByte[size];
	read(mLoadedData, size, size);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void File::loadAsync()
{
	LN_THROW_InvalidOperation(mFileAccess != FileAccess_Write);

	SAFE_DELETE_ARRAY(mLoadedData);
	size_t size = getSize();
	mLoadedData = LN_NEW lnByte[size];

	mAsyncLoadTargetBuffer = mLoadedData;
	mAsyncLoadTargetBufferSize = size;

	mManager->requestAsyncLoad(this);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void File::saveAsync(const void* buffer, int byteCount)
{
	LN_THROW_InvalidOperation(mFileAccess != FileAccess_Read);

	SAFE_DELETE_ARRAY(mSavingData);
	mSavingData = LN_NEW lnByte[byteCount];

	mAsyncSaveTargetBuffer = mSavingData;
	mAsyncSaveTargetBufferSize = byteCount;

	mManager->requestAsyncSave(this);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
int File::getSize()
{
	if (mInFile != NULL)
		return mInFile->getSize();
	else
		return FileUtils::getFileSize(mStream);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
int File::getPosition()
{
	if (mInFile != NULL)
		return mInFile->getPosition();
	else
		return ftell(mStream);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
int File::read(void* buffer, int bufferSize, int readSize)
{
	LN_THROW_InvalidOperation(mFileAccess != FileAccess_Write);

	if (mInFile != NULL)
		return mInFile->read(buffer, bufferSize, readSize);
	else
		return fread(buffer, 1, readSize, mStream);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void File::seek(int offset, int origin)
{
	if (mInFile != NULL)
		mInFile->seek(offset, origin);
	else
		fseek(mStream, offset, origin);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void File::onLoadProc()
{
	read(mAsyncLoadTargetBuffer, mAsyncLoadTargetBufferSize, mAsyncLoadTargetBufferSize);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void File::onLoadProcFinished()
{
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void File::onSaveProc()
{
	write(mAsyncSaveTargetBuffer, mAsyncSaveTargetBufferSize);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void File::onSaveProcFinished()
{
}

} // namespace FileIO
} // namespace Core
} // namespace LNote
