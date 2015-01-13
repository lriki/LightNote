//==============================================================================
// File 
//------------------------------------------------------------------------------
///**
//  @file       File.h
//  @brief      File
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
// File
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class File
    : public Stream
	, public IAsyncLoadableObject
	, public IAsyncSavableObject
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
	File();
	virtual ~File();
    LN_REFOBJ_METHODS;

public:

	/// ファイルを開く
	void open(
		Manager* manager,
		const char* filePath, 
		FileMode mode = FileMode_Open,
		FileAccess access = FileAccess_Read);

	/// ファイルを開く
	void open(
		Manager* manager,
		const wchar_t* filePath, 
		FileMode mode = FileMode_Open,
		FileAccess access = FileAccess_Read);

	/// 別のストリームをラップする (参照カウントをインクリメントするので注意)
	void create(Manager* manager, Stream* stream);

	/// ファイルを閉じる (デストラクタでも呼ばれる)
	void close();

	/// データ非同期でファイルから読み取る
	void readAsync(void* buffer, int byteCount);

	/// データをファイルに書き込む
	void write(const void* buffer, int byteCount);

	/// データ非同期でファイルに書き込む
	void writeAsync(const void* buffer, int byteCount);

	/// データを全て読み取り、内部バッファに格納する
	void load();

	/// データ非同期でを全て読み取り、内部バッファに格納する
	void loadAsync();

	/// load() または loadAsync() で格納したバッファを取得する
	void* getLoadedData() const { return mLoadedData; }
	
	/// データを非同期的にファイルに書き込む (buffer の内容は一時コピーされる)
	void saveAsync(const void* buffer, int byteCount);

public:
	// override IInStream
	virtual int		getSize();
	virtual int		getPosition();
    virtual int		read( void* buffer, int bufferSize, int readSize = -1 );
	virtual void	seek(int offset, int origin);

public:
	// override IAsyncLoadableObject
	virtual void			onLoadProc();
	virtual void			onLoadProcFinished();
	virtual AsyncIOState	getAsyncLoadState() { return mAsyncLoadState; }
	virtual void			setAsyncLoadState(AsyncIOState state) { mAsyncLoadState = state; }

	// override IAsyncSavableObject
	virtual void			onSaveProc();
	virtual void			onSaveProcFinished();
	virtual AsyncIOState	getAsyncSaveState() { return mAsyncSaveState; }
	virtual void			setAsyncSaveState(AsyncIOState state) { mAsyncSaveState = state; }

private:
	// TODO: あとでちゃんとひとつの Stream クラスにしたいが…
	Manager*		mManager;
    Stream*			mInFile;		///< 読み取り専用の場合はコレを使う (アーカイブからの読み取りもできる)
	FILE*			mStream;		///< 読み取り専用以外の場合はコレを使う
	FileAccess		mFileAccess;
	lnByte*			mLoadedData;
	lnByte*			mSavingData;
	AsyncIOState	mAsyncLoadState;
	AsyncIOState	mAsyncSaveState;

	void*			mAsyncLoadTargetBuffer;
	size_t			mAsyncLoadTargetBufferSize;

	const void*		mAsyncSaveTargetBuffer;
	size_t			mAsyncSaveTargetBufferSize;

};

} // namespace FileIO
} // namespace Core
} // namespace LNote
