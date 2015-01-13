//==============================================================================
// ALLib 
//------------------------------------------------------------------------------
///**
//  @file       ALLib.h
//  @brief      ALLib
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include "../../../Base/Misc.h"

namespace LNote
{
namespace Core
{
namespace Audio
{

//==============================================================================
// ■ ALLib
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class ALLib
	: public Base::Singleton<ALLib>
{
public:
    ALLib();
	~ALLib();

public:

	/// 初期化
    bool initialize();

public:

	typedef ALCdevice* ( ALC_APIENTRY * MD_alcOpenDevice )( const ALCchar *devicename );
	typedef ALCboolean ( ALC_APIENTRY * MD_alcCloseDevice )( ALCdevice *device );

	typedef ALCcontext* ( ALC_APIENTRY * MD_alcCreateContext )(ALCdevice *device, const ALCint* attrlist);
	typedef ALCboolean  ( ALC_APIENTRY * MD_alcMakeContextCurrent )(ALCcontext *context);
	typedef void        ( ALC_APIENTRY * MD_alcDestroyContext )(ALCcontext *context);
	typedef ALCcontext* ( ALC_APIENTRY * MD_alcGetCurrentContext )(void);

	typedef void ( AL_APIENTRY * MD_alGenBuffers )(ALsizei n, ALuint *buffers);
	typedef void ( AL_APIENTRY * MD_alDeleteBuffers )(ALsizei n, const ALuint *buffers);
	typedef void ( AL_APIENTRY * MD_alBufferData )(ALuint buffer, ALenum format, const ALvoid *data, ALsizei size, ALsizei freq);

	typedef void ( AL_APIENTRY * MD_alGenSources )(ALsizei n, ALuint *sources);
	typedef void ( AL_APIENTRY * MD_alDeleteSources )(ALsizei n, const ALuint *sources);


	typedef void ( AL_APIENTRY * MD_alSourcei )(ALuint source, ALenum param, ALint value);
	typedef void ( AL_APIENTRY * MD_alGetSourcei )(ALuint source,  ALenum param, ALint *value);
	
	typedef void ( AL_APIENTRY * MD_alSourcePlay )(ALuint source);
	typedef void ( AL_APIENTRY * MD_alSourceStop )(ALuint source);
	typedef void ( AL_APIENTRY * MD_alSourceRewind )(ALuint source);
	typedef void ( AL_APIENTRY * MD_alSourcePause )(ALuint source);

public:

	static MD_alcOpenDevice alcOpenDevice;
	static MD_alcCloseDevice alcCloseDevice;

	static MD_alcCreateContext alcCreateContext;
	static MD_alcMakeContextCurrent alcMakeContextCurrent;
	static MD_alcDestroyContext alcDestroyContext;
	static MD_alcGetCurrentContext alcGetCurrentContext;

	static MD_alGenBuffers alGenBuffers;
	static MD_alDeleteBuffers alDeleteBuffers;
	static MD_alBufferData alBufferData;

	static MD_alGenSources alGenSources;
	static MD_alDeleteSources alDeleteSources;

	static MD_alSourcei alSourcei;
	static MD_alGetSourcei alGetSourcei;
	
	static MD_alSourcePlay alSourcePlay;
	static MD_alSourceStop alSourceStop;
	static MD_alSourceRewind alSourceRewind;
	static MD_alSourcePause alSourcePause;

private:
#ifdef LNOTE_WIN32
	HMODULE	mModule;
#else
#endif
};

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================