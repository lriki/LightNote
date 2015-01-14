//==============================================================================
// ALAudioDevice 
//==============================================================================

#include "stdafx.h"
#include "../../../Resource/LNResource.h"
#include "../../../Math/LMath.h"
#include "ALAudioPlayer.h"
#include "ALAudioDevice.h"

namespace LNote
{
namespace Core
{
namespace Audio
{

//==============================================================================
// ■ ALAudioDevice
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ALAudioDevice::ALAudioDevice()
		: mALDevice		( NULL )
		, mALContext	( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ALAudioDevice::~ALAudioDevice()
    {
        LN_LOG_FIN_BEGIN;

		ALLib::alcMakeContextCurrent( NULL );
		ALLib::alcDestroyContext( mALContext );
		ALLib::alcCloseDevice( mALDevice );

        LN_LOG_FIN_END;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool ALAudioDevice::initialize()
    {
        LN_LOG_INIT_BEGIN;

		if ( !ALLib::getInstance().initialize() ) {
			return false;
		}

		mALDevice = ALLib::alcOpenDevice( NULL );
		if ( !mALDevice ) {
			return false;
		}

		mALContext = ALLib::alcCreateContext( mALDevice, NULL );
		ALLib::alcMakeContextCurrent( mALContext );

        LN_LOG_INIT_END;
        return true;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    AudioPlayerBase* ALAudioDevice::createAudioPlayer( AudioSourceBase* source, bool enable_3d, SoundPlayType type )
	{
		Base::RefPtr<AudioPlayerBase> audioPlayer;

        // 種類に応じてプレイヤーを作成する
		switch ( type )
		{
			// オンメモリ再生
			case SOUNDPLAYTYPE_ONMEMORY:
            {
                ALOnMemoryPlayer* player = LN_NEW ALOnMemoryPlayer( this );
				audioPlayer.attach( player );
                player->initialize( source, enable_3d );
				break;
            }
			// ストリーミング再生
			case SOUNDPLAYTYPE_STREAMING:
            {
	//            Streaming* player = LN_NEW Streaming( this );
	//            audioPlayer.attach( player );
				//player->initialize( source, enable_3d );
				break;
            }
			// SMF
			case SOUNDPLAYTYPE_MIDI:
            {
				break;
            }
		}

		audioPlayer.addRef();
		return audioPlayer;
	}

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================