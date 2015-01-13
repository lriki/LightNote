//==============================================================================
// Manager 
//------------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//*/
//==============================================================================

#pragma once

#include <vector>
#include <list>
#include <string>
#include <stack>
#include <map>
#include "../Base/Unicode.h"
#include "../Threading/Mutex.h"
#include "../Threading/EventFlag.h"
#include "../Threading/Thread.h"
#include "Interface.h"
#include "StreamObject.h"
#include "InFile.h"
#include "PathName.h"

namespace LNote
{
namespace Core
{
namespace FileIO
{

//==============================================================================
// Manager
//------------------------------------------------------------------------------
///**
//  @brief		ファイル入出力の管理クラス
//*/
//==============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

	/// 初期化データ
	struct ConfigData
	{
        ArchiveOpenDataList	ArchiveList;    ///< アーカイブの設定データのリスト
		FileAccessPriority	AccessMode;     ///< ファイルアクセスモード
	};

public:
    Manager();
	virtual ~Manager();

public:

	/// 初期化
    bool initialize( const ConfigData& configData );

	/// 終了処理
    void finalize();

	/// 指定されたファイルが存在するかどうかを判断する(アーカイブ考慮)
	bool existsFile(const char* filePath);

	/// 指定されたファイルが存在するかどうかを判断する(アーカイブ考慮)
	bool existsFile(const wchar_t* filePath);

	/// 指定されたファイルが存在するかどうかを判断する(アーカイブ考慮)
	bool existsFile(const PathNameW& filePath) { return existsFile(filePath.getPath()); }

	/// InFile の作成
	InFile* createInFile(const char* filePath, FileType type = FileType_Binary);

	/// InFile の作成
	InFile* createInFile(const wchar_t* filePath, FileType type = FileType_Binary);

	/// InFile の作成
	InFile* createInFile(const PathNameW& filePath, FileType type = FileType_Binary) { return createInFile(filePath.getPath(), type); }

	/// OutFile の作成
	OutFile* createOutFile(const lnChar* filePath, FileType type = FileType_Binary);

	/// カレントディレクトリを示す PathName の取得
	const PathNameW& getCurrentDirectory();

#if 0

	/// 現在のスレッドのカレントディレクトリの移動 (現在のパスをスタックに積む)
	void moveCurrentDirectory( const char* dirPath ){}

	/// 現在のスレッドのカレントディレクトリの移動 (現在のパスをスタックに積む)
	void moveCurrentDirectory(const wchar_t* dirPath){}

	/// 現在のスレッドのカレントディレクトリの復帰 (スタックに積んだものを戻す)
	void returnCurrentDirectory(){}

	/// 現在のスレッドのカレントディレクトリの取得
	const Base::UnicodeString& getCurrentDirectory() const { return _getCurrentDirectory(); }

	/// 相対パスからフルパスを返す (outPath は LN_MAX_PATH 分の領域があること)
	void getFullPath( wchar_t* outPath, const char* path );

	/// 相対パスからフルパスを返す (outPath は LN_MAX_PATH 分の領域があること)
	void getFullPath(wchar_t* outPath, const wchar_t* path);
#endif

public:
	Stream* createArchiveStream(const char* filePath, FileType type);
	Stream* createArchiveStream(const wchar_t* filePath, FileType type);
	void requestAsyncLoad(IAsyncLoadableObject* obj);
	//void removeAsyncLoadObject(IAsyncLoadableObject* obj);
	void requestAsyncSave(IAsyncSavableObject* obj);
	//void removeAsyncSaveObject(AsyncSaveObject* obj);
	//bool isFinishedAsyncLoad(IAsyncLoadableObject* obj);
	//bool isFinishedAsyncSave(IAsyncSavableObject* obj);

	// TODO: Obsolete
	void requestAsyncLoadObject( AsyncLoadObject* obj );
	void removeAsyncLoadObject( AsyncLoadObject* obj );
	void requestAsyncSaveObject( AsyncSaveObject* obj );
	void removeAsyncSaveObject( AsyncSaveObject* obj );

	/// キューにある非同期処理オブジェクトをひとつ取り出して処理を行う
	/// (設定により Manager 内部で呼ばれている場合、別の個所から呼ぶ必要はない。
	///  false を返した場合、finalize() が呼び出されているので呼び出しスレッドを終了する必要がある)
	bool pollingAsyncIOProc();

	void broadcastAsyncIOProcFinished();

public:

	/// 終了要求があるかを判定する (ローディングスレッドが終了チェックするために呼ぶ) これもいらない
	bool isEndRequested() const { return mEndFlag->isTrue(); }


	/// ローディング処理を呼び出すスレッド
    class LoadThread
        : public Threading::Thread
	{
	public:

		/// コンストラクタ
		LoadThread( Manager* manager_ )
			: mManager( manager_ )
		{}

		/// デストラクタ
		~LoadThread()
		{
			wait();
			mManager = NULL;
		}

	private:

		virtual void run()
		{
			while ( mManager->pollingAsyncIOProc() ) {
				Threading::Thread::sleep(10);
			}
		}
		Manager* mManager;
	};

	//class AsyncLoadEntry
	//{
	//public:
	//	IAsyncLoadableObject*	Object;
	//	AsyncIOState			State;
	//};

	//class AsyncSaveEntry
	//{
	//public:
	//	IAsyncSavableObject*	Object;
	//	AsyncIOState			State;
	//};

    typedef std::vector<Archive*>						ArchiveList;
	typedef std::map<lnThreadID, Base::UnicodeString>	DirectoryMap;
	typedef std::pair<lnThreadID, Base::UnicodeString>	DirectoryPair;
	typedef std::stack<Base::UnicodeString>				DirectoryStack;
	typedef std::map<lnThreadID, DirectoryStack>		DirectoryStackMap;
	typedef std::pair<lnThreadID, DirectoryStack>		DirectoryStackPair;

	//typedef std::vector<IAsyncLoadableObject*>				AsyncLoadEntryArray;
	//typedef std::vector<IAsyncSavableObject*>				AsyncSaveEntryArray;
	typedef std::list<IAsyncLoadableObject*>			AsyncLoadableObjectList;
	typedef std::list<IAsyncSavableObject*>				AsyncSavableObjectist;

	// TODO: Obsolete 
	typedef std::list<AsyncLoadObject*>					AsyncLoadObjectList;
	typedef std::list<AsyncSaveObject*>					AsyncSaveObjectList;
	
private:

	/// path を outPath に格納する (必要があればフルパス化・単純化・正規化を行う)
	void checkMakeFullPath(const wchar_t* path, PathNameW* outPath);

	// スレッド系カレントディレクトリ
#if 0
	/// 現在のスレッドのカレントディレクトリの設定
	void _setCurrentDirectory( const Base::UnicodeString& path );

	/// 現在のスレッドのカレントディレクトリの取得
	const Base::UnicodeString& _getCurrentDirectory() const;

	/// 現在のスレッドのカレントディレクトリスタックの取得
	DirectoryStack& _getCurrentDirectoryStack();

	/// カレントディレクトリの後ろに path を追加したフルパスを返す (正規化は行わない)
	bool _makePath( wchar_t* out, const wchar_t* path );

	/// カレントディレクトリと path から、正規化したフルパスを返す
	bool _makeFullPath( wchar_t* out, const wchar_t* path );
#endif

private:
    ArchiveList                     mArchiveList;       ///< 時間があるときに mArchives はこっちに含める
	
    Threading::Mutex*				mLoadThreadLock;    ///< mAsyncProcessRequests へのアクセスブロック用
    Threading::EventFlag*			mEndFlag;
	LoadThread*			            mLoadThread;
    Threading::Mutex*				mLock;
    Threading::Mutex				mLockForCreateStream;

	Base::UnicodeString				mBaseCurrentDirectory;
	DirectoryMap					mCurrentDirectoryMap;
	DirectoryStackMap				mCurrentDirectoryStackMap;

	//AsyncLoadEntryArray				mAsyncLoadEntryArray;
	//AsyncSaveEntryArray				mAsyncSaveEntryArray;
	AsyncLoadableObjectList			mAsyncLoadableObjectList;
	AsyncSavableObjectist			mAsyncSavableObjectList;
	AsyncLoadableObjectList			mFinishedAsyncLoadableObjectList;
	AsyncSavableObjectist			mFinishedAsyncSavableObjectList;
	//AsyncLoadableObjectList			mAsyncLoadableObjectRemovedList;
	//AsyncSavableObjectList			mAsyncSavableObjectRemovedList;

	// TODO: Obsolete 
	AsyncLoadObjectList				mAsyncLoadObjectList;
	AsyncSaveObjectList				mAsyncSaveObjectList;
	AsyncLoadObjectList				mFinishedAsyncLoadObjectList;
	AsyncSaveObjectList				mFinishedAsyncSaveObjectList;
	AsyncLoadObjectList				mAsyncLoadObjectRemovedList;
	AsyncSaveObjectList				mAsyncSaveObjectRemovedList;


	Base::Exception*				mLoadThreadException;

	LNOTE_TEST_UNIT;
};

#if 0
//==============================================================================
// ScopedMoveCurrentDirectory
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ScopedMoveCurrentDirectory
{
public:
	ScopedMoveCurrentDirectory( Manager* manager, const wchar_t* dirPath )
	{
		mManager = manager;
		mManager->moveCurrentDirectory( dirPath );
	}

	~ScopedMoveCurrentDirectory()
	{
		mManager->returnCurrentDirectory();
	}

private:
	Manager*	mManager;
};
#endif

} // namespace FileIO
} // namespace Core
} // namespace LNote
