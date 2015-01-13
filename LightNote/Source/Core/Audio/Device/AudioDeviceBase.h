//==============================================================================
// AudioDeviceBase 
//------------------------------------------------------------------------------
///**
//  @file       AudioDeviceBase.h
//  @brief      AudioDeviceBase
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Interface.h"
#include "../Types.h"

namespace LNote
{
namespace Core
{
namespace Audio
{
class AudioPlayerBase;

//==============================================================================
// ■ AudioDeviceBase
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class AudioDeviceBase
    : public Base::ReferenceObject
{
public:
    AudioDeviceBase();
    virtual ~AudioDeviceBase();

public:

	/// 3Dサウンドリスナーの取得
    virtual SoundListener* getSoundListener() { return &mSoundListener; }

	/// AudioPlayerBase をに加える ( 各 AudioPlayer のコンストラクタから呼ばれる )
    void addAudioPlayer( AudioPlayerBase* player );

	/// AudioPlayerBase をリストから外す ( 各 AudioPlayer のデストラクタから呼ばれる )
    void removeAudioPlayer( AudioPlayerBase* player );

public:
	
	/// AudioPlayer を作成する (type に LN_SOUNDPLAYTYPE_AUTO は指定できないので注意)
    virtual AudioPlayerBase* createAudioPlayer( AudioSourceBase* source, bool enable_3d, SoundPlayType type ) = 0;

	/// 更新 (更新スレッドから呼ばれる)
    virtual void update() = 0;

	/// 3D 空間の1メートル相当の距離の設定
    virtual void setMetreUnitDistance( lnFloat d ) = 0;

protected:

    typedef std::vector<AudioPlayerBase*>   AudioPlayerList;

    AudioPlayerList     mAudioPlayerList;
    SoundListener		mSoundListener;
};

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================