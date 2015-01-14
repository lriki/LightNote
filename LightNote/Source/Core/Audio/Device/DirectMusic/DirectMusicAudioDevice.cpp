//==============================================================================
// DirectMusicAudioDevice
//==============================================================================

#include "stdafx.h"
#include "../../../System/Environment.h"
#include "../../../Math/LMath.h"
#include "DirectMusic.h"
#include "DirectMusicAudioPlayer.h"
#include "DirectMusicAudioDevice.h"

namespace LNote
{
namespace Core
{
namespace Audio
{
namespace DirectMusic
{

//==============================================================================
// AudioDevice
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    AudioDevice::AudioDevice()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    AudioDevice::~AudioDevice()
    {
        LN_LOG_FIN_BEGIN;

        if ( DirectMusicManager::getInstance() )
        {
            DirectMusicManager::getInstance()->finalize();
        }

        LN_LOG_FIN_END;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void AudioDevice::initialize( const ConfigData& configData )
    {
        LN_LOG_INIT_BEGIN;

        // DirectMusic を初期化する場合
        if ( configData.DMInitMode != DMINITMODE_NOTUSE && configData.Window )
        {
            DirectMusicManager::ConfigData dm_data;
            dm_data.DMInitMode      = configData.DMInitMode;
			dm_data.WindowHandle    = System::Environment::getWindowHandle( configData.Window );
            DirectMusicManager::initialize( dm_data );
        }

        LN_LOG_INIT_END;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    AudioPlayerBase* AudioDevice::createAudioPlayer( AudioSourceBase* source, bool enable_3d, SoundPlayType type )
    {
		Base::RefPtr<MidiPlayer> audio_player;

        if ( type == SOUNDPLAYTYPE_MIDI )
        {
			LN_ERR2_ASSERT( DirectMusicManager::getInstance() );

            audio_player.attach( LN_NEW MidiPlayer( this ), false );
			audio_player->initialize( source, enable_3d );
        }

		audio_player.addRef();
        return audio_player;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void AudioDevice::update()
    {
        if ( DirectMusicManager::getInstance() )
        {
            DirectMusicManager::getInstance()->polling();
        }
    }

} // namespace DirectMusic
} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================