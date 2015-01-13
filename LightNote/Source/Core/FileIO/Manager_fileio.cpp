//==============================================================================
// Manager 
//------------------------------------------------------------------------------
/*
	■ createInFile() でのパスの扱い
		 一度すべてフルパスに変換し、正規化する。
		 これは、カレントディレクトリ変更 + 相対パスのままでは
		 アーカイブ内のファイルへのアクセスが難しいため。

	■ カレントディレクトリの変更
		OS上ではカレントディレクトリはプロセスに対して1つだけだが、
		FileIO::Manager では複数スレッドからの読み書きできるようにするため、
		(特に、3Dモデルファイルやシェーダファイルからの相対パスでリソースを作るとき)
		スレッド単位で持つようにする。
*/
//==============================================================================

#include "stdafx.h"
#include "../Resource/LNResource.h"
#include "../Base/STLUtils.h"
#include "../System/Environment.h"
#include "FileUtil.h"
#include "Archive.h"
#include "AsyncProcessObject.h"
#include "InFile.h"
#include "OutFile.h"
#include "Manager.h"

namespace LNote
{
namespace Core
{
namespace FileIO
{

//==============================================================================
// ■ Manager
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Manager::Manager()
        : mLoadThreadLock		( NULL )
        , mEndFlag				( NULL )
        , mLoadThread			( NULL )
        , mLock					( NULL )
		, mLoadThreadException( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Manager::~Manager()
    {
		LN_SAFE_RELEASE( mEndFlag );
		LN_SAFE_RELEASE( mLoadThreadLock );

        lnU32 n = mArchiveList.size();
        for ( lnU32 i = 0; i < n; ++i )
        {
            LN_SAFE_RELEASE( mArchiveList[i] );
        }
        mArchiveList.clear();

        LN_SAFE_RELEASE( mLock );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool Manager::initialize( const ConfigData& configData )
    {
        LN_LOG_INIT_BEGIN;

		// カレントディレクトリ記憶
		lnChar dir[LN_MAX_PATH] = { 0 };
		System::Environment::getCurrentDirectory( dir );
		mBaseCurrentDirectory.setDecodeBytes( dir );

        //-----------------------------------------------------
        // アーカイブ数チェック

        int archive_num = configData.ArchiveList.size();
		FileAccessPriority mode = configData.AccessMode;

		// アーカイブ or ディレクトリ優先
		if ( mode == FileAccessPriority_ArchiveFirst || mode == FileAccessPriority_DirectoryFirst )
		{
			archive_num += 1;  // ダミー分の +1
		}

        LN_ERR2_ASSERT_S( archive_num > 0 );

        //-----------------------------------------------------
        // アーカイブ作成
		
		// ディレクトリ優先の場合
		if ( mode == FileAccessPriority_DirectoryFirst )
		{
			// [0] はダミーで作る
            mArchiveList.push_back( LN_NEW DummyArchive() );

			// 引数で受け取った分を作る
            ArchiveOpenDataList::const_iterator it = configData.ArchiveList.begin();
			for ( ; it != configData.ArchiveList.end(); ++it )
			{
                Archive* archive = LN_NEW Archive();
				archive->initialize(
                    it->Directory.c_str(), 
					it->ArchiveName.c_str(), 
					it->ArchiveKey.c_str() );
                mArchiveList.push_back( archive );
			}
		}
        // アーカイブのみ or アーカイブ優先の場合
		else
		{
			// 引数で受け取った分を作る
			ArchiveOpenDataList::const_iterator it = configData.ArchiveList.begin();
			for ( ; it != configData.ArchiveList.end(); ++it )
			{
                Archive* archive = LN_NEW Archive();
				archive->initialize(
                    it->Directory.c_str(), 
					it->ArchiveName.c_str(), 
					it->ArchiveKey.c_str() );
                mArchiveList.push_back( archive );
			}
			// アーカイブ優先の場合、最後にダミーを作る
			if ( mode == FileAccessPriority_ArchiveFirst )
			{
                mArchiveList.push_back( LN_NEW DummyArchive() );
			}
		}

		// 埋め込みリソース
        //mArchiveList.push_back( LN_NEW Resource::HardCodeResourceArchive() );
        
        //-----------------------------------------------------
        // ローディングスレッド関係

        mLock = LN_NEW Threading::Mutex();

        mLoadThreadLock = LN_NEW Threading::Mutex();
		mEndFlag	    = LN_NEW Threading::EventFlag( false );
		mLoadThread     = LN_NEW LoadThread( this );
		mLoadThread->start();

        LN_LOG_INIT_END;
        return true;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::finalize()
    {
        LN_LOG_FIN_BEGIN;

        if ( mEndFlag ) {
            mEndFlag->setTrue();		    // 終了フラグを立てた後、
        }
        SAFE_DELETE( mLoadThread );         // スレッドの終了を待つ

		// リクエストが残っていればすべて解放 
		// (終了通知待ちも通知を行わずに解放する。こうしないと、Graphics::finalize() 後にテクスチャ生成、等が起こる)


		//ln_foreach(AsyncLoadEntry* e, mAsyncLoadEntryArray) {
		//	e->Object->release();
		//}
		//mAsyncLoadEntryArray.clear();
		//ln_foreach(AsyncSaveEntry* e, mAsyncSaveEntryArray) {
		//	e->Object->release();
		//}
		//mAsyncSaveEntryArray.clear();

		ln_foreach(IAsyncLoadableObject* obj, mAsyncLoadableObjectList) {
			obj->release();
		}
		ln_foreach(IAsyncSavableObject* obj, mAsyncSavableObjectList) {
			obj->release();
		}
		ln_foreach(IAsyncLoadableObject* obj, mFinishedAsyncLoadableObjectList) {
			obj->release();
		}
		ln_foreach(IAsyncSavableObject* obj, mFinishedAsyncSavableObjectList) {
			obj->release();
		}
		mAsyncLoadableObjectList.clear();
		mAsyncSavableObjectList.clear();
		mFinishedAsyncLoadableObjectList.clear();
		mFinishedAsyncSavableObjectList.clear();

#if 1	// TOOD: Obsolete
		ln_foreach( AsyncLoadObject* obj, mAsyncLoadObjectList ) {
			obj->release();
		}
		ln_foreach( AsyncSaveObject* obj, mAsyncSaveObjectList ) {
			obj->release();
		}
		ln_foreach( AsyncLoadObject* obj, mFinishedAsyncLoadObjectList ) {
			obj->release();
		}
		ln_foreach( AsyncSaveObject* obj, mFinishedAsyncSaveObjectList ) {
			obj->release();
		}
		mAsyncLoadObjectList.clear();
		mAsyncSaveObjectList.clear();
		mFinishedAsyncLoadObjectList.clear();
		mFinishedAsyncSaveObjectList.clear();
#endif

		SAFE_DELETE(mLoadThreadException);

        LN_LOG_FIN_END;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Manager::existsFile( const char* filePath )
	{
		Base::UnicodeString path;
		path.setDecodeBytes( filePath );
		return existsFile( path.getString() );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Manager::existsFile( const wchar_t* filePath )
	{
		// フルパスを作る
		PathNameW fullPath;
		checkMakeFullPath(filePath, &fullPath);
		//wchar_t fullPath[LN_MAX_PATH] = { 0 };
		//if ( !_makeFullPath( fullPath, filePath ) )
		//{
		//	LN_THROW_InvalidCall( 0 );
		//}

		// アーカイブ内検索
        ln_foreach( Archive* archive, mArchiveList )
        {
			if (archive->existsFile(fullPath.getPath())) {
				return true;
			}
			/*
			try {
				IInStream* stream = archive->createStream( fullPath, FileOpenMode_Binary );
				if ( stream ) {
					stream->release();
					return true;
				}
			}
			catch ( Base::FileNotFoundException& ) {
				// 次のアーカイブも探すため、ここでは例外としない。
				// ループが終わった後、stream が NULL であれば例外とする。
			}
			*/
        }

		return false;
		//return File::exists( fullPath );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
#if 0
	LNRESULT Manager::createInFile( InFile** file_, const lnChar* filename_, LNFileOpenMode mode_ )
    {
        LNRESULT lr = LN_OK;
        *file_ = NULL;

        //mLock->lock();

        ln_std_tstring path;

		if ( FileIO::Path::isFullPath( filename_ ) )
		{
			path = filename_;
		}
        else if ( filename_[0] == _T( '$' ) )
        {
            path = filename_ + 1;
        }
        else
        {
            LRefTString tmp = (mCurrentPath + filename_).c_str();
            FileIO::Path::normalizePathName( &tmp );
            FileIO::Path::makeFileName( &path, tmp.c_str() );
        }

        
  //      for ( int i = 0; i < mArchiveNum; ++i )
		//{
  //          mArchives[ i ].createStream( &stream, path.c_str(), mode_ );
		//	if ( stream )
		//	{
		//		break;
		//	}
		//}


        IInStream* stream = NULL;
            ln_foreach( IArchive* ac, mArchiveList )
            {
                if ( !stream )
                    stream = ac->createStream( path.c_str(), mode_ );
            }
        

        
        LN_THROW_FileNotFound( stream, path.c_str() );


        *file_ = LN_NEW InFile( this, stream );


        return lr;
    }
#endif
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	InFile* Manager::createInFile(const char* filePath, FileType type)
    {
		Base::UnicodeString path;
		path.setDecodeBytes( filePath );

		return createInFile( path.getString(), type );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	InFile* Manager::createInFile(const wchar_t* filePath, FileType type)
	{
		return LN_NEW InFile(this, createArchiveStream(filePath, type));
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	OutFile* Manager::createOutFile(const lnChar* filePath, FileType type)
    {
		return LN_NEW OutFile(this, filePath, type);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	const PathNameW& Manager::getCurrentDirectory()
	{
		static PathNameW curDir;
		
		wchar_t path[LN_MAX_PATH];
		System::Environment::getCurrentDirectory(path);

		if (curDir != path) {
			curDir = PathNameW(path);
		}
		return curDir;
	}

#if 0
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::moveCurrentDirectory( const char* dirPath )
	{
		Base::UnicodeString path;
		path.setDecodeBytes( dirPath );

		moveCurrentDirectory( path.getString() );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::moveCurrentDirectory( const wchar_t* dirPath )
	{
		wchar_t fullPath[LN_MAX_PATH] = { 0 };
		if ( _makeFullPath( fullPath, dirPath ) == false )
		{
			LN_THROW_InvalidCall( 0 );
		}

		// 現在のスレッドのスタックを取得
		DirectoryStack& dirStack = _getCurrentDirectoryStack();

		// カレントディレクトリをスタックに積む
		dirStack.push( _getCurrentDirectory() );

		// 新しくカレントディレクトリを設定
		Base::UnicodeString str;
		str.setDecodeBytes( fullPath );
		_setCurrentDirectory( str );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::returnCurrentDirectory()
	{
		// 現在のスレッドのスタックを取得
		DirectoryStack& dirStack = _getCurrentDirectoryStack();
		
		// 新しくカレントディレクトリを設定
		_setCurrentDirectory( dirStack.top() );
		dirStack.pop();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::getFullPath( wchar_t* out, const char* path )
	{
		Base::UnicodeString pt;
		pt.setDecodeBytes( path );
		getFullPath( out, pt.getString() );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::getFullPath( wchar_t* out, const wchar_t* path )
	{
		if ( !_makeFullPath( out, path ) )
		{
			LN_THROW_InvalidCall( 0 );
		}
	}
#endif

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Stream* Manager::createArchiveStream(const char* filePath, FileType type)
	{
		Base::UnicodeString path;
		path.setDecodeBytes(filePath);
		return createArchiveStream(path.getString(), type);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Stream* Manager::createArchiveStream(const wchar_t* filePath, FileType type)
	{
		Threading::ScopedLock lock(mLockForCreateStream);
		Stream* stream = NULL;

		// 埋め込みリソースから検索
		//if ( Resource::HardCodeResourceArchive::checkResourceRootPath( filePath ) )
		//{
		//	wchar_t normalPath[LN_MAX_PATH] = { 0 };
		//	Path::normalizePath( filePath, normalPath );
		//	stream = Resource::HardCodeResourceArchive::createStream( normalPath, mode );
		//}

		if (!stream)
		{
			// フルパスを作る
			PathNameW fullPath;
			checkMakeFullPath(filePath, &fullPath);
			//wchar_t fullPath[LN_MAX_PATH] = { 0 };
			//if (!_makeFullPath(fullPath, filePath))
			//{
			//	LN_THROW_InvalidCall(0);
			//}

			// アーカイブ内検索
			ln_foreach(Archive* archive, mArchiveList)
			{
				if (!stream) {
					try {
						stream = archive->createStream(fullPath.getPath(), type);
					}
					catch (Base::FileNotFoundException&) {
						// 次のアーカイブも探すため、ここでは例外としない。
						// ループが終わった後、stream が NULL であれば例外とする。
					}
				}
			}
#if 0
			// ディレクトリから開く (内部カレントディレクトリから)
			if (!stream && File::exists(fullPath))
			{
				LRefPtr<InFileStream> ins(LN_NEW InFileStream());
				ins->open(fullPath, mode);
				stream = ins;
				stream->addRef();	// InFile に渡すため
			}

			// ディレクトリから開く (プロセスカレントディレクトリから)
			if (!stream && File::exists(filePath))
			{
				LRefPtr<InFileStream> ins(LN_NEW InFileStream());
				ins->open(filePath, mode);
				stream = ins;
				stream->addRef();	// InFile に渡すため
			}
#endif
			LN_THROW_FileNotFound(stream, fullPath.getPath());
		}

		return stream;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::requestAsyncLoad(IAsyncLoadableObject* obj)
	{
		LN_THROW_Argument(obj->getAsyncLoadState() == AsyncIOState_Idle);

		// 終了要求が来ている場合は追加しない
		if (isEndRequested()) {
			return;
		}

		// 管理リストへ
		//AsyncLoadEntry* e = LN_NEW AsyncLoadEntry();
		//e->Object = obj;
		//e->State = AsyncIOState_Ready;
		//mAsyncLoadEntryArray.push_back(e);
		//obj->addRef();

		// 実行待ちリストへ
		Threading::ScopedLock lock(*mLoadThreadLock);
		mAsyncLoadableObjectList.push_back(obj);
		obj->addRef();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::requestAsyncSave(IAsyncSavableObject* obj)
	{
		LN_THROW_Argument(obj->getAsyncSaveState() == AsyncIOState_Idle);

		// 終了要求が来ている場合は追加しない
		if (isEndRequested()) {
			return;
		}

		// 管理リストへ
		//AsyncSaveEntry* e = LN_NEW AsyncSaveEntry();
		//e->Object = obj;
		//e->State = AsyncIOState_Ready;
		//mAsyncSaveEntryArray.push_back(e);
		//obj->addRef();

		// 実行待ちリストへ
		Threading::ScopedLock lock(*mLoadThreadLock);
		mAsyncSavableObjectList.push_back(obj);
		obj->addRef();
	}

#if 0
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Manager::isFinishedAsyncLoad(IAsyncLoadableObject* obj)
	{
		struct Compare
		{
			IAsyncLoadableObject* Object;
			bool operator()(const AsyncLoadEntry* e) {
				return e->Object == Object;
			}
		} c;
		c.Object = obj;

		// 検索
		AsyncLoadEntryArray::iterator itr = std::find_if(
			mAsyncLoadEntryArray.begin(), mAsyncLoadEntryArray.end(), c);
		if (itr != mAsyncLoadEntryArray.end())
			return (*itr)->State;

		// 見つからなかった
		LN_THROW_InvalidCall(0);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Manager::isFinishedAsyncSave(IAsyncSavableObject* obj)
	{

		struct Compare
		{
			IAsyncSavableObject* Object;
			bool operator()(const AsyncSaveEntry* e) {
				return e->Object == Object;
			}
		} c;
		c.Object = obj;

		// 検索
		AsyncSaveEntryArray::iterator itr = std::find_if(
			mAsyncSaveEntryArray.begin(), mAsyncSaveEntryArray.end(), c);
		if (itr != mAsyncSaveEntryArray.end())
			return (*itr)->State;

		// 見つからなかった
		LN_THROW_InvalidCall(0);
	}
#endif







	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::requestAsyncLoadObject( AsyncLoadObject* obj )
	{
		// 終了要求が来ている場合は追加しない
        if ( isEndRequested() ) {
            return;
        }

        // 実行待ち
		obj->mAsyncIOState = AsyncIOState_Ready;

		Threading::ScopedLock lock( *mLoadThreadLock );
		mAsyncLoadObjectList.push_back( obj );
		obj->addRef();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::removeAsyncLoadObject( AsyncLoadObject* obj )
	{
		size_t prevSize = 0;
		{
			Threading::ScopedLock lock( *mLoadThreadLock );
			prevSize = mAsyncLoadObjectList.size();
			mAsyncLoadObjectList.remove( obj );
		}
		// リストから取り除けていれば release()
		if ( mAsyncLoadObjectList.size() < prevSize ) {
			obj->release();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::requestAsyncSaveObject( AsyncSaveObject* obj )
	{
		// 終了要求が来ている場合は追加しない
        if ( isEndRequested() ) {
            return;
        }

        // 実行待ち
		obj->mAsyncIOState = AsyncIOState_Ready;

		Threading::ScopedLock lock( *mLoadThreadLock );
		mAsyncSaveObjectList.push_back( obj );
		obj->addRef();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::removeAsyncSaveObject( AsyncSaveObject* obj )
	{
		size_t prevSize = 0;
		{
			Threading::ScopedLock lock( *mLoadThreadLock );
			prevSize = mAsyncSaveObjectList.size();
			mAsyncSaveObjectList.remove( obj );
		}
		// リストから取り除けていれば release()
		if ( mAsyncSaveObjectList.size() < prevSize ) {
			obj->release();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Manager::pollingAsyncIOProc()
	{
		{
			//-------------------------------------------------
			// Load 処理
			if (mLoadThreadException == NULL)
			{
				// 読み込みリクエストを取り出す
				IAsyncLoadableObject* loadObj = NULL;
				mLoadThreadLock->lock();
				if (!mAsyncLoadObjectList.empty())
				{
					loadObj = mAsyncLoadableObjectList.front();
					loadObj->setAsyncLoadState(AsyncIOState_Processing);
					mAsyncLoadableObjectList.pop_front();
				}
				mLoadThreadLock->unlock();

				// 読み込みリクエスト実行
				if (loadObj)
				{
					try
					{
						loadObj->onLoadProc();

						// 完了リストへ
						Threading::ScopedLock lock(*mLoadThreadLock);
						mFinishedAsyncLoadableObjectList.push_back(loadObj);
					}
					catch (Base::Exception& e)
					{
						if (mLoadThreadException == NULL) {
							mLoadThreadException = e.copy();
						}
					}
				}
			}

			//-------------------------------------------------
			// Save 処理
			if (mLoadThreadException == NULL)
			{
				// 書き込みリクエストを取り出す
				IAsyncSavableObject* saveObj = NULL;
				mLoadThreadLock->lock();
				if (!mAsyncSavableObjectList.empty())
				{
					saveObj = mAsyncSavableObjectList.front();
					saveObj->setAsyncSaveState(AsyncIOState_Processing);
					mAsyncSavableObjectList.pop_front();
				}
				mLoadThreadLock->unlock();

				// 書き込みリクエスト実行
				if (saveObj)
				{
					try
					{
						saveObj->onSaveProc();

						// 完了リストへ
						Threading::ScopedLock lock(*mLoadThreadLock);
						mFinishedAsyncSavableObjectList.push_back(saveObj);
					}
					catch (Base::Exception& e)
					{
						if (mLoadThreadException == NULL) {
							mLoadThreadException = e.copy();
						}
					}
				}
			}
		}

#if 1	// TODO: Obsolete
		{

			// 読み込みリクエストを取り出す
			AsyncLoadObject* loadObj = NULL;
			mLoadThreadLock->lock();
			if (!mAsyncLoadObjectList.empty())
			{
				loadObj = mAsyncLoadObjectList.front();
				mAsyncLoadObjectList.pop_front();
				loadObj->mNotAsyncIOProccessing.setFalse();
				loadObj->mAsyncIOState = AsyncIOState_Processing;
			}
			mLoadThreadLock->unlock();

			// 読み込みリクエスト実行
			if (loadObj)
			{
				loadObj->_doLoadProc();
				loadObj->mNotAsyncIOProccessing.setTrue();

				mLoadThreadLock->lock();
				mFinishedAsyncLoadObjectList.push_back(loadObj);
				mLoadThreadLock->unlock();
			}

			// 書き込みリクエストを取り出す
			AsyncSaveObject* saveObj = NULL;
			mLoadThreadLock->lock();
			if (!mAsyncSaveObjectList.empty())
			{
				saveObj = mAsyncSaveObjectList.front();
				mAsyncSaveObjectList.pop_front();
				saveObj->mNotAsyncIOProccessing.setFalse();
				saveObj->mAsyncIOState = AsyncIOState_Processing;
			}
			mLoadThreadLock->unlock();

			// 書き込みリクエスト実行
			if (saveObj)
			{
				saveObj->_doSaveProc();
				saveObj->mNotAsyncIOProccessing.setTrue();

				mLoadThreadLock->lock();
				mFinishedAsyncSaveObjectList.push_back(saveObj);
				mLoadThreadLock->unlock();
			}
		}
#endif

		// リクエストが残っていても終了する
		return !mEndFlag->isTrue();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::broadcastAsyncIOProcFinished()
	{
		{
			// 読み込み完了通知
			if (!mFinishedAsyncLoadableObjectList.empty())
			{
				Threading::ScopedLock lock(*mLoadThreadLock);
				ln_foreach(IAsyncLoadableObject* e, mFinishedAsyncLoadableObjectList)
				{
					e->onLoadProcFinished();
					e->release();
				}
				mFinishedAsyncLoadableObjectList.clear();
			}

			// 書き込み完了通知
			if (!mFinishedAsyncSavableObjectList.empty())
			{
				Threading::ScopedLock lock(*mLoadThreadLock);
				ln_foreach(IAsyncSavableObject* e, mFinishedAsyncSavableObjectList)
				{
					e->onSaveProcFinished();
					e->release();
				}
				mFinishedAsyncSavableObjectList.clear();
			}

			// 非同期処理で例外が発生していればここで投げなおす
			if (mLoadThreadException) {
				throw *mLoadThreadException;
			}
		}
#if 1	// TODO: Obsolete
		{
			// 読み込み完了通知
			if (!mFinishedAsyncLoadObjectList.empty())
			{
				Threading::ScopedLock lock(*mLoadThreadLock);
				ln_foreach(AsyncLoadObject* obj, mFinishedAsyncLoadObjectList)
				{
					obj->onLoadProcFinished();
					obj->release();
				}
				mFinishedAsyncLoadObjectList.clear();
			}

			// 書き込み完了通知
			if (!mFinishedAsyncSaveObjectList.empty())
			{
				Threading::ScopedLock lock(*mLoadThreadLock);
				ln_foreach(AsyncSaveObject* obj, mFinishedAsyncSaveObjectList)
				{
					obj->onSaveProcFinished();
					obj->release();
				}
				mFinishedAsyncSaveObjectList.clear();
			}
		}
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::checkMakeFullPath(const wchar_t* path, PathNameW* outPath)
	{
		// フルパスの場合はそのままコピー
		if (Path::isFullPath(path))
		{
			PathNameW tmpPath(path);
			*outPath = tmpPath.canonicalizePath(this);
		}
		// 相対パスの場合はカレントディレクトリの後につなげてコピー
		else
		{
			PathNameW tmpPath(getCurrentDirectory(), path);
			*outPath = tmpPath.canonicalizePath(this);
		}
	}

	// スレッド系カレントディレクトリ
#if 0
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::_setCurrentDirectory( const Base::UnicodeString& path )
	{
		DirectoryMap::iterator itr = mCurrentDirectoryMap.find(Threading::Thread::getCurrentThreadID());
		if ( itr == mCurrentDirectoryMap.end() )
		{
			// 新しくマップに追加
			mCurrentDirectoryMap.insert(
				DirectoryPair(Threading::Thread::getCurrentThreadID(), path));
		}
		else
		{
			itr->second = path;
		}
	}

#if 1
#else
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	const Base::UnicodeString& Manager::_getCurrentDirectory() const
	{
		DirectoryMap::const_iterator itr = mCurrentDirectoryMap.find(Threading::Thread::getCurrentThreadID());
		if ( itr != mCurrentDirectoryMap.end() )
		{
			return itr->second;
		}
		return mBaseCurrentDirectory;
	}
#endif

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Manager::DirectoryStack& Manager::_getCurrentDirectoryStack()
	{
		// 現在のスレッドのスタックを取得
		DirectoryStack* dirStack = NULL;
		DirectoryStackMap::iterator itr = mCurrentDirectoryStackMap.find(Threading::Thread::getCurrentThreadID());
		if ( itr == mCurrentDirectoryStackMap.end() )
		{
			// 新しくスタックを作ってマップに追加
			DirectoryStack s;
			std::pair<DirectoryStackMap::iterator, bool> result = mCurrentDirectoryStackMap.insert(
				DirectoryStackPair(Threading::Thread::getCurrentThreadID(), s));
			return (*result.first).second;
		}
		else
		{
			return itr->second;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
//	bool Manager::_makePath( wchar_t* out, const wchar_t* path )
//	{
//#if 1
//		const PathName curDir = getCurrentDirectory();
//		wchar_t tmp[LN_MAX_PATH] = { 0 };
//
//		// curDir tmp を tmp にｎコピーし、
//		bool r = ln_tcscat(
//			tmp, LN_MAX_PATH,
//			curDir.getPath(), wcslen(curDir.getPath()),
//			L"\\", 1 );
//		if ( r == false ) return false;
//
//		r = ln_tcscat(
//			out, LN_MAX_PATH,
//			tmp, curDir.getCharCount() + 1,
//			path, wcslen( path ) );
//		return r;
//#else
//		const Base::UnicodeString& curDir = _getCurrentDirectory();
//		wchar_t tmp[LN_MAX_PATH] = { 0 };
//
//		bool r = ln_tcscat(
//			tmp, LN_MAX_PATH,
//			curDir.getString(), curDir.getCharCount(),
//			L"\\", 1 );
//		if ( r == false ) return false;
//
//		r = ln_tcscat(
//			out, LN_MAX_PATH,
//			tmp, curDir.getCharCount() + 1,
//			path, wcslen( path ) );
//		return r;
//#endif
//	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Manager::_makeFullPath( wchar_t* out, const wchar_t* path )
	{
		wchar_t tmpFullPath[LN_MAX_PATH] = { 0 };
		if ( Path::isFullPath( path ) )
		{
			// フルパスであればそのままコピー
			ln_tcscpy( tmpFullPath, LN_MAX_PATH, path, wcslen( path ) );
		}
		else
		{
			if ( _makePath( tmpFullPath, path ) == false )
				return false;
		}

		return Path::canonicalizePath( tmpFullPath, out );
	}
#endif

} // namespace FileIO
} // namespace Core
} // namespace LNote
