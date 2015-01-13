//==============================================================================
// AudioResourceManager 
//==============================================================================

#include "stdafx.h"
#include "AudioSource.h"
#include "AudioResourceManager.h"
#include "../AudioUtil.h"

namespace LNote
{
namespace Core
{
namespace Audio
{

//==============================================================================
// ■ ResourceManager
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ResourceManager::ResourceManager()
    {
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ResourceManager::~ResourceManager()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ResourceManager::initialize( const ConfigData& configData )
    {
        LN_LOG_INIT_BEGIN;

		Base::CacheManager::initialize( configData.AudioSourceCacheSize, 0 );

        LN_LOG_INIT_END;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ResourceManager::finalize()
    {
		LN_LOG_FIN_BEGIN;

		Base::CacheManager::finalize();

		LN_LOG_FIN_END;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    AudioSourceBase* ResourceManager::findAudioSource( Base::SharingKey key )
    {
		Threading::ScopedLock lock( mLock );
		return static_cast<AudioSourceBase*>( Base::CacheManager::findCacheAddRef( key ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    AudioSourceBase* ResourceManager::createAudioSource( FileIO::Stream* stream, lnSharingKey key )
    {
		LN_ERR2_ASSERT( stream );

		AudioSourceFormat format = AudioUtil::checkFormat( stream );

		Threading::ScopedLock lock( mLock );

		LNRESULT lr = ResultCode_OK;
		Base::RefPtr<AudioSourceBase> audio_source;

		switch( format )
		{
			// Wave
			case AUDIOSOURCE_WAVE:
                audio_source.attach( LN_NEW Wave( this ), false );
				break;
#if defined(LNOTE_MSVC)
			// MP3
			case AUDIOSOURCE_MP3:
                audio_source.attach( LN_NEW MP3( this ), false );
				break;
#endif
			// OGG
			case AUDIOSOURCE_OGG:
                audio_source.attach( LN_NEW Ogg( this ), false );
				break;
#if defined(LNOTE_MSVC)
			// MIDI
			case AUDIOSOURCE_MIDI:
                audio_source.attach( LN_NEW Midi( this ), false );
				break;
#endif
			default:
				LN_THROW_InvalidFormat(0, "Unknown Audio File");
                return NULL;
        }

		// 新しく作成できた場合
		if ( audio_source.getPtr() != NULL )
		{
			// 入力ファイルを設定する
			audio_source->setStream( stream );

            // MP3 と MIDI 以外は登録
			if ( key.isEmpty() == false && (format != AUDIOSOURCE_MP3 && format != AUDIOSOURCE_MIDI) )
            {
				Base::CacheManager::registerCachedObject( key, audio_source );
            }
		}

        stream->seek( 0 );

		audio_source->addRef();
		return audio_source.getPtr();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ResourceManager::clearCache() 
	{ 
		Base::CacheManager::clearCacheList(); 
	}

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================