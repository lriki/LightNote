//==============================================================================
// ALLib 
//==============================================================================

#include "stdafx.h"
#include "ALLib.h"

namespace LNote
{
namespace Core
{
namespace Audio
{

//==============================================================================
// ■ ALLib
//==============================================================================

#define STATIC_DEFINE( func ) ALLib::MD_##func ALLib::func = NULL;

	STATIC_DEFINE( alcOpenDevice );
	STATIC_DEFINE( alcCloseDevice );

	STATIC_DEFINE( alcCreateContext );
	STATIC_DEFINE( alcMakeContextCurrent );
	STATIC_DEFINE( alcDestroyContext );
	STATIC_DEFINE( alcGetCurrentContext );

	STATIC_DEFINE( alGenBuffers );
	STATIC_DEFINE( alDeleteBuffers );
	STATIC_DEFINE( alBufferData );

	STATIC_DEFINE( alGenSources );
	STATIC_DEFINE( alDeleteSources );

	STATIC_DEFINE( alSourcei );
	STATIC_DEFINE( alGetSourcei );
	
	STATIC_DEFINE( alSourcePlay );
	STATIC_DEFINE( alSourceStop );
	STATIC_DEFINE( alSourceRewind );
	STATIC_DEFINE( alSourcePause );

#undef STATIC_DEFINE

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ALLib::ALLib()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ALLib::~ALLib()
	{
#ifdef LNOTE_MSVC
        if ( mModule )
        {
            ::FreeLibrary( mModule );
        }
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool ALLib::initialize()
	{
#ifdef LNOTE_MSVC
		if ( mModule ) {
			return true;
		}

		mModule = ::LoadLibrary( _T( "D:/Programing/OpenAL/openal-soft-1.15.1-bin/Win32/soft_oal.dll" ) );
		if ( mModule == NULL ) {
			return false;
		}
#define LOAD_FUNC( func ) func = (MD_##func)::GetProcAddress( mModule, #func );
#endif
#ifdef LNOTE_GNUC
		#define LOAD_FUNC( func ) func = ::func;
#endif

		LOAD_FUNC( alcOpenDevice );
		LOAD_FUNC( alcCloseDevice );

		LOAD_FUNC( alcCreateContext );
		LOAD_FUNC( alcMakeContextCurrent );
		LOAD_FUNC( alcDestroyContext );
		LOAD_FUNC( alcGetCurrentContext );
		
		LOAD_FUNC( alGenBuffers );
		LOAD_FUNC( alDeleteBuffers );
		LOAD_FUNC( alBufferData );

		LOAD_FUNC( alGenSources );
		LOAD_FUNC( alDeleteSources );

		LOAD_FUNC( alSourcei );
		LOAD_FUNC( alGetSourcei );
	
		LOAD_FUNC( alSourcePlay );
		LOAD_FUNC( alSourceStop );
		LOAD_FUNC( alSourceRewind );
		LOAD_FUNC( alSourcePause );

#undef LOAD_FUNC
		return true;
	}

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================