//==============================================================================
// NullAudioDevice 
//------------------------------------------------------------------------------
///**
//  @file       NullAudioDevice.h
//  @brief      NullAudioDevice
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../AudioDeviceBase.h"
#include "NullAudioPlayer.h"

namespace LNote
{
namespace Core
{
namespace Audio
{
class AudioPlayerBase;

//==============================================================================
// ■ NullAudioDevice
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class NullAudioDevice
    : public AudioDeviceBase
{
public:
	NullAudioDevice() {}
	virtual ~NullAudioDevice() {}

public:
	virtual AudioPlayerBase* createAudioPlayer( AudioSourceBase* source, bool enable3D, SoundPlayType type )
	{
		Base::RefPtr<NullAudioPlayer> player(
			LN_NEW NullAudioPlayer( this ) );
		player->initialize( source, enable3D );
		player.addRef();
		return player;
	}

	virtual void update() {}
    virtual void setMetreUnitDistance( lnFloat d ) { }
};

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================