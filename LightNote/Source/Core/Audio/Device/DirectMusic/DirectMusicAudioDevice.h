//==============================================================================
// DirectMusicAudioDevice
//------------------------------------------------------------------------------
///**
//  @file       DirectMusicAudioDevice.h
//  @brief      DirectMusicAudioDevice
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../../System/Interface.h"
#include "../../Interface.h"
#include "../AudioDeviceBase.h"

namespace LNote
{
namespace Core
{
namespace Audio
{
namespace DirectMusic
{
class MidiPlayer;

//==============================================================================
// AudioDevice
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class AudioDevice
    : public Audio::AudioDeviceBase
{
public:

    struct ConfigData
    {
        DirectMusicInitMode		DMInitMode;     ///< DirectMusic を初期化する時の方法
        System::Window*			Window;         ///< DirectMusic の初期化に使うウィンドウハンドルを持つウィンドウクラス
    };

public:
    AudioDevice();
    virtual ~AudioDevice();
    LN_REFOBJ_METHODS();

public:
    void initialize( const ConfigData& configData );
    virtual AudioPlayerBase* createAudioPlayer( AudioSourceBase* source, bool enable_3d, SoundPlayType type );
    virtual SoundListener* getSoundListener() { return NULL; }
    virtual void update();
    virtual void setMetreUnitDistance( lnFloat d ) {}
};

} // namespace DirectMusic
} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================