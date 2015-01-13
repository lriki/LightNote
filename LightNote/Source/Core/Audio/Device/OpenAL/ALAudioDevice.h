//==============================================================================
// ALAudioDevice 
//------------------------------------------------------------------------------
///**
//  @file       ALAudioDevice.h
//  @brief      ALAudioDevice
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "ALLib.h"
#include "../../../Base/Container/NodeList.h"
#include "../../../System/Interface.h"
#include "../../Interface.h"
#include "../AudioDeviceBase.h"

namespace LNote
{
namespace Core
{
namespace Audio
{

//==============================================================================
// ■ ALAudioDevice
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class ALAudioDevice
    : public Audio::AudioDeviceBase
{
public:
    ALAudioDevice();
    virtual ~ALAudioDevice();

public:

	/// 初期化
    bool initialize();

public:
    virtual AudioPlayerBase*	createAudioPlayer( AudioSourceBase* source, bool enable_3d, SoundPlayType type );
	virtual void				update() {}
	virtual LNRESULT			setListenerState(const LVector3& position, const LVector3& front) { return ResultCode_OK; }
    virtual void				setMetreUnitDistance( lnFloat d ) { }

private:
	ALCdevice*	mALDevice;
    ALCcontext*	mALContext;

};

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================