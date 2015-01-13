//==============================================================================
// Manager 
//==============================================================================

#include "stdafx.h"
#include "../Resource/LNResource.h"
#include "../System/Environment.h"
#include "../FileIO/Manager.h"
#include "../FileIO/InFile.h"
#include "Device/OpenAL/ALAudioDevice.h"
#if defined(LNOTE_DIRECTX)
    #include "Device/XAudio2/XA2AudioDevice.h"
    #include "Device/DirectMusic/DirectMusic.h"
#elif defined(LNOTE_ANDROID)
    #include "Device/OpenSLES/SLESAudioDevice.h"
#endif
#include "Device/NullDevice/NullAudioDevice.h"
#include "Resource/AudioSource.h"
#include "AudioUtil.h"
#include "Interface.h"
#include "Sound.h"
#include "Manager.h"

namespace LNote
{
namespace Core
{
namespace Audio
{

//==============================================================================
// ■ Manager
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Manager::Manager()
        : mLogFile              ( NULL )
        , mAudioDevice          ( NULL )
        , mResourceManager      ( NULL )
        , mGameAudio            ( NULL )
        , mFileIOManager        ( NULL )
        , mThread               ( NULL )
        , mEndRequest           ( NULL )
        , mLock                 ( NULL )
        , mOnMemoryLimitSize    ( 100000 )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Manager::~Manager()
	{
        finalize();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    LNRESULT Manager::initialize( const ConfigData& configData )
    {
        LN_LOG_INIT_BEGIN;

        mFileIOManager = configData.FileIOManager;
        LN_SAFE_ADDREF( mFileIOManager );

        // AudioDevice
#if defined(LNOTE_WIN32)

#if 0
		// OpenAL
		if ( mAudioDevice == NULL )
		{
			ALAudioDevice* alDevice = LN_NEW ALAudioDevice();
			if ( alDevice->initialize() )
			{
				LN_REFOBJ_SET( mAudioDevice, alDevice );
			}
			LN_SAFE_RELEASE( alDevice );
		}
#endif

		if ( mAudioDevice == NULL )
		{
			XAudio2::AudioDevice::ConfigData data;
			data.EnableDebugFlag    = false;
			data.DMInitMode         = configData.DMInitMode;
			data.Window             = configData.Window;
			XAudio2::AudioDevice* device = LN_NEW XAudio2::AudioDevice();
			if ( device->initialize( data ) )
			{
				mAudioDevice = device;
			}
			else
			{
				device->release();
				device = NULL; 

				// 音声無しで続行します
				// TODO: メッセージ
				//System::Environment::showMessageBox(
				//	NULL, Resource::String::WARNING_Audio_DirectXIsNotInstalled, NULL);
				//LN_WARNING_S( Resource::String::WARNING_Audio_DirectXIsNotInstalled );

				mAudioDevice = LN_NEW NullAudioDevice();
			}
		}
#elif defined(LNOTE_GNUC)
		// OpenAL
		if ( mAudioDevice == NULL )
		{
			ALAudioDevice* alDevice = LN_NEW ALAudioDevice();
			if ( alDevice->initialize() )
			{
				LN_REFOBJ_SET( mAudioDevice, alDevice );
				printf("al ok\n");
			}
			LN_SAFE_RELEASE( alDevice );
		}
#elif defined(LNOTE_ANDROID)
        SLES::AudioDevice::Initdata data;
        data.LogFile            = mLogFile;
        SLES::AudioDevice* device = LN_NEW SLES::AudioDevice();
        LN_CALL_R( device->initialize( data ) );
        mAudioDevice = device;
#endif

        // ResourceManager
		ResourceManager::ConfigData rm_data;
        rm_data.AudioSourceCacheSize    = configData.AudioSourceCacheSize;
        mResourceManager = LN_NEW ResourceManager();
		mResourceManager->initialize( rm_data );

        // GameAudio
        mGameAudio = LN_NEW GameAudio( this );
        
        // スレッド関係
        mEndRequest = LN_NEW Threading::EventFlag( false );
        mLock = LN_NEW Threading::Mutex();
		mThread = LN_NEW PollingThread( this );

		// スレッド開始
		mThread->start();

        LN_LOG_INIT_END;
		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::finalize()
    {
        LN_LOG_FIN_BEGIN;

        if ( mEndRequest )
        {
		    // 終了要求フラグを ON
		    mEndRequest->setTrue();

		    // スレッド終了
		    SAFE_DELETE( mThread );
        }

        if ( mGameAudio )
        {
            // GameAudio のデストラクタでは Sound の stop() を呼んでいる。
            // そのため、単純にミューテックスで囲んで delete するとデッドロックする。

            GameAudio* a = mGameAudio;

            mLock->lock();
            mGameAudio = NULL;
            mLock->unlock();
            
            SAFE_DELETE( a );
        }

        if ( mResourceManager )
        {
            mResourceManager->finalize();
        }

		LN_SAFE_RELEASE( mEndRequest );
        LN_SAFE_RELEASE( mLock );

        // mSoundList に何か残っていれば全て解放
        mSoundList.clear();

        LN_SAFE_RELEASE( mResourceManager );
        LN_SAFE_RELEASE( mAudioDevice );

        LN_SAFE_RELEASE( mFileIOManager );

        LN_LOG_FIN_END;
    }
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Sound* Manager::createSound( FileIO::Stream* stream, SoundPlayType type, bool enable_3d, lnSharingKey key )
    {
        Threading::ScopedLock lock( *mLock );
		
        SoundPlayType player_type = SOUNDPLAYTYPE_UNKNOWN;

        //-----------------------------------------------------
        // AudioSourceBase
		
		// キャッシュ検索
		Base::RefPtr<AudioSourceBase> source(
			mResourceManager->findAudioSource( key ), false );

		// キャッシュに無かった
        if ( source.getPtr() == NULL )
        {
			// 検索のみ行ったが、見つからなかった
			if ( stream == NULL ) return NULL;
			
			source.attach(
				mResourceManager->createAudioSource( stream, key ), false );
			
            // 正しい種類をチェック
            player_type = AudioUtil::checkAudioPlayType( type, source, mOnMemoryLimitSize );
        }
		// キャッシュに見つかった
		else
		{
			// MIDI はそのまま
			if (source->getSourceFormat() == AUDIOSOURCE_MIDI) {
				player_type = SOUNDPLAYTYPE_MIDI;
			}
			// すでにオンメモリロードが完了しているものはすべてオンメモリ
			else if (source->getOnmemoryPCMBuffer()) {
				player_type = SOUNDPLAYTYPE_ONMEMORY;
			}
			// それ以外はストリーミング
			else {
				player_type = SOUNDPLAYTYPE_STREAMING;
			}
		}

        //-----------------------------------------------------
        // AudioPlayerBase
		Base::RefPtr<AudioPlayerBase> player(
			mAudioDevice->createAudioPlayer( source, enable_3d, player_type ) );

		// Sound 作成
        Sound* sound = LN_NEW Sound( this, player );
        mSoundList.push_back( sound );
        return sound;;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Sound* Manager::createSound( const lnChar* filename, SoundPlayType type, bool enable_3d )
    {
		LN_ERR2_ASSERT( filename );

        // 既に作成されているものを検索
        Sound* sound = createSound( NULL, type, enable_3d, lnSharingKey( filename ) );
		if ( sound ) return sound;

        // 無かった場合は新しく作る
		Base::RefPtr<FileIO::InFile> stream(
			mFileIOManager->createInFile( filename ) );

        return createSound( stream, type, enable_3d, lnSharingKey( filename ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Sound* Manager::createSound( AudioSourceBase* source, SoundPlayType type, bool enable_3d )
    {
		LN_ERR2_ASSERT( source );
        Threading::ScopedLock lock( *mLock );

        // AudioPlayerBase
		Base::RefPtr<AudioPlayerBase> player(
			mAudioDevice->createAudioPlayer( source, enable_3d, type ) );

        // Sound 作成
        Sound* sound = LN_NEW Sound( this, player );
        mSoundList.push_back( sound );
		return sound;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::removeSound( Sound* sound )
    {
		LN_ERR2_ASSERT( sound );
        Threading::ScopedLock lock( *mLock );
        mSoundList.remove( sound );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::stopGroup( lnU32 group )
	{
        Threading::ScopedLock lock( *mLock );

        SoundList::iterator it  = mSoundList.begin();
        SoundList::iterator end = mSoundList.end();
        for ( ; it != end; ++it )
        {
            if ( (*it)->getGroupFlag() & group )
            {
                (*it)->stop();
            }
        }
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::update( float elapsed_time )
    {
		{
			Threading::ScopedLock lock( *mLock );

			mAudioDevice->update();

			Sound* sound = NULL;
			bool incr = false;
			SoundList::iterator it  = mSoundList.begin();
			SoundList::iterator end = mSoundList.end();
			for ( ; it != end; )
			{
				sound = ( *it );
				incr = true;
  
				// 更新して、再生が終了した場合 ( ループ再生の場合は常に true )
				if ( !sound->update( elapsed_time ) )
				{
					/*
					// リストにだけ残っていて、再生終了後に解放する場合
					if ( ( sound->getRefCount() == 0 ) &&
						 ( sound->getPlayerFlags() & LN_AUDIOPLAYER_AFTERRELEASE ) )
					{
						//printf( "停止・解放\n" );
						it = mSoundList.erase( it );
						//LN_SAFE_RELEASE( sound );
						SAFE_DELETE( sound );

						printf( "旧 再生終了検出\n" );

						incr = false;
					}
					*/
				}

				if ( incr )
				{
					++it;
				}
			}
		}

        // finalize() が呼ばれた後もスレッド側から呼ばれる可能性があるので NULL チェック
        if ( mGameAudio )
        {
            mGameAudio->polling();
        }
    }

//==============================================================================
// ■ Manager::PollingThread
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::PollingThread::run()
	{
#if defined(LNOTE_DIRECTX)
        // COM 初期化
        HRESULT hr = ::CoInitializeEx( NULL, COINIT_MULTITHREADED );
#endif
		// 終了要求が来るまで回し続ける
		while( !mManager->isEndRequest() )
		{
            mManager->update( 0.020f );
			Threading::Thread::sleep(20);
		}
#if defined(LNOTE_DIRECTX)
        ::CoUninitialize();
#endif
	}

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================