//==============================================================================
// Manager 
//------------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Base/Container/NodeList.h"
#include "../Threading/Thread.h"
#include "../Threading/EventFlag.h"
#include "../Threading/Mutex.h"
#include "../FileIO/Interface.h"
#include "../System/Interface.h"
#include "Resource/AudioResourceManager.h"
#include "Sound.h"
#include "GameAudio.h"

namespace LNote
{
namespace Core
{
namespace Audio
{

//==============================================================================
// ■ Manager
//------------------------------------------------------------------------------
///**
//  @brief      オーディオ機能の管理クラス
//*/
//==============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

	/// initialize() に渡す初期化データ
	struct ConfigData
	{
        FileIO::Manager*		FileIOManager;          ///< ファイルからの読み込みに使うファイル管理クラス
        lnU32					AudioSourceCacheSize;   ///< キャッシュサイズ (ファイル数)
        DirectMusicInitMode		DMInitMode;             ///< DirectMusic の初期化方法
        System::Window*			Window;                 ///< DirectMusic の初期化に使うウィンドウハンドルを持つウィンドウクラス
    };

public:
	Manager();
	virtual ~Manager();

public:

	/// 初期化
    LNRESULT initialize( const ConfigData& configData );

	/// 終了処理 ( GameAudio の解放 )
    void finalize();

	/// オンメモリorストリーミング自動選択の音声データバイト数閾値
	void setAutoPlayTypeSelectThreshold(lnU32 threshold) { mOnMemoryLimitSize = threshold; }

	/// GameAudio クラスの取得
    GameAudio* getGameAudio() { return mGameAudio; }

	/// デバイスクラスの取得
    AudioDeviceBase* getAudioDevice() { return mAudioDevice; }

	/// リソース管理クラスの取得
    ResourceManager* getResourceManager() { return mResourceManager; }

	/// Sound の作成 ( stream_ = NULL でキーを使った検索だけ行う )
    Sound* createSound( FileIO::Stream* stream, SoundPlayType type, bool enable_3d, lnSharingKey key );

	/// Sound の作成 ( ファイル名指定 )
    Sound* createSound( const lnChar* filename, SoundPlayType type, bool enable_3d );

	/// Sound の作成 ( IAudioSource 指定 )
    Sound* createSound( AudioSourceBase* source, SoundPlayType type, bool enable_3d );

	/// グループの停止
    void stopGroup( lnU32 group );

public:

	/// Sound のデストラクタから呼ばれる
    void removeSound( Sound* sound );

	/// フレーム更新 ( スレッドを使ってポーリングしている場合はそっちから呼ばれる )
    void update( float elapsed_time );

	/// 終了要求が来ているかを判定する
    bool isEndRequest() const { return mEndRequest->isTrue(); }

private:

	/// ポーリングスレッドクラス
    class PollingThread
        : public Threading::Thread
	{
	public:

		/// コンストラクタ
		PollingThread( Manager* manager )
            : mManager  ( manager )
        {}

		/// デストラクタ
		~PollingThread()
		{
			// 再生終了を待つ
			// ( このデストラクタは、Manager のデストラクタから呼ばれる。
			//   その際、Manager 側では delete の直前に mEndRequest を true にする。)
			wait();
			mManager = NULL;
		}

		/// スレッド関数
		virtual void run();

	private:

		Manager* mManager;		///< Audio::Manager のインスタンスへのポインタ。コンストラクタで受け取る
	};

private:
    typedef Base::NodeList< Sound > SoundList;

    Base::LogFile*          mLogFile;
    AudioDeviceBase*		mAudioDevice;
    ResourceManager*        mResourceManager;
    SoundList               mSoundList;

    GameAudio*              mGameAudio;
    FileIO::Manager*			mFileIOManager;

    PollingThread*	        mThread;		        ///< ポーリングスレッドクラス
    Threading::EventFlag*	mEndRequest;	        ///< ポーリングスレッドに終了を伝えるためのフラグ
    Threading::Mutex*		mLock;

    lnU32					mOnMemoryLimitSize;
};

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================