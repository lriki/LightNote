//==============================================================================
// LFAudio 
//==============================================================================

#include "stdafx.h"
#include "../Core/Audio/Manager.h"
#include "../Core/Audio/GameAudio.h"
#include "../Core/Audio/Device/AudioDeviceBase.h"
#include "../Core/Audio/Device/AudioPlayerBase.h"
#include "LFInternal.h"
#include "LFBase.h"
#include "LFAudio.h"

//==============================================================================
// LNAudio
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    LNResult LNAudio_PlayBGM( const LNChar* filePath, int volume, int pitch, int fadeTime )
    {
		LN_FUNC_TRY_BEGIN;
		FuncLibManager::AudioManager->getGameAudio()->playBGM( filePath, volume, pitch, fadeTime );
		LN_FUNC_TRY_END_RETURN;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNAudio_PlayBGMMem(const void* data, int data_size, int volume, int pitch, int fade_time)
    {
        LN_FUNC_TRY_BEGIN;
		LNHandle h;
		LNSound_CreateMem(&h, data, data_size, LN_FALSE);
		Core::Audio::Sound* sound = TO_SOUND(h);
        FuncLibManager::AudioManager->getGameAudio()->playBGMFromSound( sound, volume, pitch, fade_time );
        LNObject_Release( LN_TO_INT( sound ) );
        return ::LN_OK;
		LN_FUNC_TRY_END;
		return LNException_GetLastErrorCode();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNAudio_StopBGM(int fade_time)
    {
		LN_FUNC_TRY_BEGIN;
		FuncLibManager::AudioManager->getGameAudio()->stopBGM( fade_time );
        LN_FUNC_TRY_END_RETURN;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    LNResult LNAudio_PlayBGS( const LNChar* filename, int volume, int pitch, int fade_time )
    {
		LN_FUNC_TRY_BEGIN;
		FuncLibManager::AudioManager->getGameAudio()->playBGS( filename, volume, pitch, fade_time );
        return ::LN_OK;
		LN_FUNC_TRY_END;
		return LNException_GetLastErrorCode();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNAudio_PlayBGSMem(const void* data, int data_size, int volume, int pitch, int fade_time)
    {
		LN_FUNC_TRY_BEGIN;
		LNHandle h;
		LNSound_CreateMem(&h, data, data_size, LN_FALSE);
		Core::Audio::Sound* sound = TO_SOUND(h);
        FuncLibManager::AudioManager->getGameAudio()->playBGSFromSound( sound, volume, pitch, fade_time );
        LNObject_Release( LN_TO_INT( sound ) );
		return ::LN_OK;
		LN_FUNC_TRY_END;
		return LNException_GetLastErrorCode();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNAudio_StopBGS(int fade_time)
    {
		LN_FUNC_TRY_BEGIN;
		FuncLibManager::AudioManager->getGameAudio()->stopBGS( fade_time );
		LN_FUNC_TRY_END_RETURN;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    LNResult LNAudio_PlayME( const LNChar* filename, int volume, int pitch )
    {
		LN_FUNC_TRY_BEGIN;
		FuncLibManager::AudioManager->getGameAudio()->playME( filename, volume, pitch );
		return ::LN_OK;
		LN_FUNC_TRY_END;
		return LNException_GetLastErrorCode();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNAudio_PlayMEMem(const void* data, int data_size, int volume, int pitch)
    {
		LN_FUNC_TRY_BEGIN;
		LNHandle h;
		LNSound_CreateMem(&h, data, data_size, LN_FALSE);
		Core::Audio::Sound* sound = TO_SOUND(h);
        FuncLibManager::AudioManager->getGameAudio()->playMEFromSound( sound, volume, pitch );
        LNObject_Release( LN_TO_INT( sound ) );
		return ::LN_OK;
		LN_FUNC_TRY_END;
		return LNException_GetLastErrorCode();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNAudio_StopME()
    {
		LN_FUNC_TRY_BEGIN;
		FuncLibManager::AudioManager->getGameAudio()->stopME();
        LN_FUNC_TRY_END;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    LNResult LNAudio_PlaySE( const LNChar* filename, int volume, int pitch )
    {
		LN_FUNC_TRY_BEGIN;
		FuncLibManager::AudioManager->getGameAudio()->playSE( filename, volume, pitch );
		return ::LN_OK;
		LN_FUNC_TRY_END;
		return LNException_GetLastErrorCode();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNAudio_PlaySE3D(const LNChar* filename, const LNVector3* position, float distance, int volume, int pitch)
	{
		LN_FUNC_TRY_BEGIN;
		FuncLibManager::AudioManager->getGameAudio()->playSE(filename, TO_CORE_VEC3_REF(position), distance, volume, pitch);
		return ::LN_OK;
		LN_FUNC_TRY_END;
		return LNException_GetLastErrorCode();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    LNResult LNAudio_PlaySE3DXYZ( const LNChar* filename, float x, float y, float z, float distance, int volume, int pitch )
	{
		LNVector3 pos = { x, y, z };
		return LNAudio_PlaySE3D(filename, &pos, distance, volume, pitch);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNAudio_PlaySEMem(const void* data, int data_size, int volume, int pitch)
    {
		LN_FUNC_TRY_BEGIN;
		LNHandle h;
		LNSound_CreateMem(&h, data, data_size, LN_FALSE);
		Core::Audio::Sound* sound = TO_SOUND(h);
        FuncLibManager::AudioManager->getGameAudio()->playSEFromSound( sound, volume, pitch );
        LNObject_Release( LN_TO_INT( sound ) );
		return ::LN_OK;
		LN_FUNC_TRY_END;
		return LNException_GetLastErrorCode();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNAudio_PlaySE3DMem(const void* data, int dataSize, const LNVector3* position, float distance, int volume, int pitch)
	{
		LN_FUNC_TRY_BEGIN;
		LNHandle h;
		LNSound_CreateMem(&h, data, dataSize, LN_FALSE);
		Core::Audio::Sound* sound = TO_SOUND(h);
		sound->setPosition(TO_CORE_VEC3_REF(position));
		sound->setMaxDistance(distance);
		FuncLibManager::AudioManager->getGameAudio()->playMEFromSound(sound, volume, pitch);
		LNObject_Release(LN_TO_INT(sound));
		return ::LN_OK;
		LN_FUNC_TRY_END;
		return LNException_GetLastErrorCode();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNAudio_PlaySE3DMemXYZ(const void* data, int dataSize, float x, float y, float z, float distance, int volume, int pitch)
    {
		LNVector3 pos = { x, y, z };
		return LNAudio_PlaySE3DMem(data, dataSize, &pos, distance, volume, pitch);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNAudio_StopSE()
    {
        LN_FUNC_TRY_BEGIN;
		FuncLibManager::AudioManager->getGameAudio()->stopSE();
        LN_FUNC_TRY_END;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNAudio_SetMetreUnitDistance(float distance)
	{
		LN_FUNC_TRY_BEGIN;
		FuncLibManager::AudioManager->getAudioDevice()->setMetreUnitDistance( distance );
		LN_FUNC_TRY_END_RETURN;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNAudio_SetBGMVolume(int volume, int fadeTime)
	{
        LN_FUNC_TRY_BEGIN;
		FuncLibManager::AudioManager->getGameAudio()->setBGMVolume( volume, fadeTime );
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNAudio_SetBGSVolume(int volume, int fadeTime)
	{
        LN_FUNC_TRY_BEGIN;
		FuncLibManager::AudioManager->getGameAudio()->setBGSVolume( volume, fadeTime );
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNHandle LNAudio_GetInternalGameSound( LNInternalGameSound type )
	{
		return LN_TO_INT( FuncLibManager::AudioManager->getGameAudio()->getInternalGameSound( 
			(LNote::Core::Audio::GameAudio::InternalGameSound)type ) );
	}

//==============================================================================
// LNSoundListener
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSoundListener_SetPosition(const LNVector3* position)
	{
		FuncLibManager::AudioManager->getAudioDevice()->getSoundListener()->Position.Set(position->X, position->Y, position->Z);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void LNSoundListener_SetPositionXYZ( float x, float y, float z )
    {
        FuncLibManager::AudioManager->getAudioDevice()->getSoundListener()->Position.Set( x, y, z );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSoundListener_SetDirection(const LNVector3* direction)
	{
		FuncLibManager::AudioManager->getAudioDevice()->getSoundListener()->Direction.Set(direction->X, direction->Y, direction->Z);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void LNSoundListener_SetDirectionXYZ( float x, float y, float z )
    {
        FuncLibManager::AudioManager->getAudioDevice()->getSoundListener()->Direction.Set( x, y, z );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSoundListener_SetUpDirection(const LNVector3* direction)
	{
		FuncLibManager::AudioManager->getAudioDevice()->getSoundListener()->UpDirection.Set(direction->X, direction->Y, direction->Z);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void LNSoundListener_SetUpDirectionXYZ( float x, float y, float z )
    {
        FuncLibManager::AudioManager->getAudioDevice()->getSoundListener()->UpDirection.Set( x, y, z );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSoundListener_Velocity(const LNVector3* direction)
	{
		FuncLibManager::AudioManager->getAudioDevice()->getSoundListener()->Velocity.Set(direction->X, direction->Y, direction->Z);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void LNSoundListener_VelocityXYZ( float x, float y, float z )
    {
        FuncLibManager::AudioManager->getAudioDevice()->getSoundListener()->Velocity.Set( x, y, z );
    }

//==============================================================================
// LNSound
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(Audio::Sound, LNSound);
#if 1
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_Create(LNHandle* sound, const LNChar* filename, LNBool enable3d, LNSoundPlayingType playType)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::Audio::Sound> obj(
			FuncLibManager::AudioManager->createSound(filename, (Core::Audio::SoundPlayType)playType, LNOTE_BOOL_TO_LNBOOL(enable3d)));
		obj.addRef();
		*sound = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}
#else
	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルから音声を作成する
	//  @param[in]  filePath	: ファイル名
	//  @param[in]  enable3d	: true の場合、3D 音源として作成する
	//  @param[in]  playType	: 音声の再生方法
	//  @return     サウンドハンドル
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNHandle LNSound_Create(const LNChar* filePath, LNBool enable3d LNOTE_DEFAULT_ARG(LN_FALSE), LNSoundPlayingType playType LNOTE_DEFAULT_ARG(LN_SOUNDPLAYINGTYPE_AUTO));

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    LNOTEAPI LNHandle LNSound_Create( const LNChar* filename, LNBool enable3d, LNSoundPlayingType playType )
    {
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::Audio::Sound> obj(
			FuncLibManager::AudioManager->createSound(filename, (Core::Audio::SoundPlayType)playType, LNOTE_BOOL_TO_LNBOOL(enable3d)));
		obj.addRef();
		return TO_HANDLE( FuncLibManager::pushObject( obj ) );
		LN_FUNC_TRY_END;
		return NULL;
    }
#endif

#if 1
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_CreateMem(LNHandle* sound, const void* data, int data_size, LNBool enable_3d)
	{
		LN_FUNC_TRY_BEGIN;

		LRefPtr<Core::FileIO::Stream> stream(
			FileIO::FileUtils::createInStreamFromMemoryManaged(data, data_size));

		LRefPtr<Core::Audio::Sound> obj(
			FuncLibManager::AudioManager->createSound(
			stream, Core::Audio::SOUNDPLAYTYPE_ONMEMORY, LNOTE_BOOL_TO_LNBOOL(enable_3d), Core::Base::SharingKey::NULL_KEY ) );
		obj.addRef();

		*sound = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}
#else
	//----------------------------------------------------------------------
	///**
	//  @brief      メモリ上のデータから音声を作成する
	//  @param[in]  data		: メモリ上の音声データへのポインタ
	//  @param[in]  dataSize	: データサイズ (バイト単位)
	//  @param[in]  enable3d	: true の場合、3D 音源として作成する
	//  @return     サウンドハンドル
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNHandle LNSound_CreateMem(const LNByte* data, int dataSize, LNBool enable3d LNOTE_DEFAULT_ARG(LN_FALSE));

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    LNHandle LNSound_CreateMem( const LNByte* data, int data_size, LNBool enable_3d )
    {
        LN_FUNC_TRY_BEGIN;

		LRefPtr<Core::FileIO::IInStream> stream(
			FileIO::File::createInStreamFromMemoryManaged( data, data_size ) );
            
		LRefPtr<Core::Audio::Sound> obj(
			FuncLibManager::AudioManager->createSound(
				stream, Core::Audio::SOUNDPLAYTYPE_ONMEMORY, LNOTE_BOOL_TO_LNBOOL(enable_3d), Core::Base::SharingKey::NULL_KEY ) );
		obj.addRef();
		return TO_HANDLE( FuncLibManager::pushObject( obj ) );
		LN_FUNC_TRY_END;
		return NULL;
	}
#endif

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルから音声を作成する
	//  @param[out] outHandle	: サウンドハンドルを格納する変数のポインタ
	//  @param[in]  filePath	: ファイル名
	//  @param[in]  enable3d	: true の場合、3D 音源として作成する
	//  @param[in]  playType	: 音声の再生方法
	//                      @arg LN_SOUNDPLAYTYPE_AUTO          : 自動選択
	//                      @arg LN_SOUNDPLAYTYPE_ONMEMORY      : オンメモリ
	//                      @arg LN_SOUNDPLAYTYPE_STREAMING     : ストリーミング
	//*/
	//----------------------------------------------------------------------[{hsp name=LNSound_Create}]
	LNOTEAPI void LNSound_CreateToPtr(LNHandle* outHandle, const LNChar* filePath, LNBool enable3d LNOTE_DEFAULT_ARG(LN_FALSE), LNSoundPlayingType playType LNOTE_DEFAULT_ARG(LN_SOUNDPLAYINGTYPE_AUTO));


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    //void LNSound_CreateToPtr( LNHandle* out_handle, const LNChar* filename, LNBool enable_3d, LNSoundPlayingType play_type )
    //{
    //    //*out_handle = LNSound_Create( filename, LNOTE_BOOL_TO_LNBOOL(enable_3d), play_type );
    //}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_SetVolume(LNHandle sound, int volume)
    {
		LN_FUNC_TRY_BEGIN;
        TO_SOUND( sound )->setVolume( volume );
		LN_FUNC_TRY_END_RETURN;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_GetVolume(LNHandle sound, int* volume)
    {
		*volume = TO_SOUND(sound)->getVolume();
		return ::LN_OK;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_SetPitch(LNHandle sound, int pitch)
    {
		LN_FUNC_TRY_BEGIN
        TO_SOUND( sound )->setPitch( pitch );
		LN_FUNC_TRY_END_RETURN;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_GetPitch(LNHandle sound, int* pitch)
    {
		*pitch = TO_SOUND(sound)->getPitch();
		return ::LN_OK;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_SetLoopState(LNHandle sound, LNBool loop, int begin, int length)
    {
		LN_FUNC_TRY_BEGIN;
        TO_SOUND( sound )->loop( loop != LN_FALSE );
        TO_SOUND( sound )->setLoopState( begin, length );
		LN_FUNC_TRY_END_RETURN;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_IsLoop(int sound, LNBool* enabled)
    {
		*enabled = LNOTE_BOOL_TO_LNBOOL(TO_SOUND(sound)->isLoop());
		return ::LN_OK;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_GetPlayState(LNHandle sound, LNSoundPlayingState* outState)
    {
		Core::Audio::AudioPlayState state;
		state.PlayingState = Core::Audio::SOUNDPLAYSTATE_STOPPED;
		LN_FUNC_TRY_BEGIN;
		TO_SOUND(sound)->getState(&state);
		*outState = (LNSoundPlayingState)state.PlayingState;
		LN_FUNC_TRY_END_RETURN;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_Play(LNHandle sound)
    {
		LN_FUNC_TRY_BEGIN;
        TO_SOUND( sound )->play();
		LN_FUNC_TRY_END_RETURN;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_Stop(LNHandle sound)
    {
		LN_FUNC_TRY_BEGIN;
        TO_SOUND( sound )->stop();
		LN_FUNC_TRY_END_RETURN;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_Pause(LNHandle sound, LNBool pause)
    {
		LN_FUNC_TRY_BEGIN;
        TO_SOUND( sound )->pause( pause != LN_FALSE );
		LN_FUNC_TRY_END_RETURN;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_FadeVolume(LNHandle sound, int target_volume, int time, LNSoundFadeState fade_state)
    {
		LN_FUNC_TRY_BEGIN;
        TO_SOUND( sound )->fadeVolume( target_volume, time, (Core::Audio::SoundFadeState)fade_state );
		LN_FUNC_TRY_END_RETURN;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_GetUnitsPlayed(LNHandle sound, int* samples)
    {
        Core::Audio::AudioPlayState state;
		LN_FUNC_TRY_BEGIN;
		TO_SOUND(sound)->getState(&state);
		*samples = state.CurrentUnits;
		LN_FUNC_TRY_END_RETURN;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_GetTotalUnits(LNHandle sound, int* samples)
    {
        Core::Audio::AudioPlayState state;
		LN_FUNC_TRY_BEGIN;
		TO_SOUND(sound)->getState(&state);
		*samples = state.TotalUnits;
		LN_FUNC_TRY_END_RETURN;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_GetUnitsParSecond(LNHandle sound, int* frequency)
    {
        Core::Audio::AudioPlayState state;
		LN_FUNC_TRY_BEGIN;
        TO_SOUND( sound )->getState( &state );
		*frequency = state.SamplesPerSec;
		LN_FUNC_TRY_END_RETURN;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_Is3DSound(LNHandle sound, LNBool* enabled)
    {
		*enabled = LNOTE_BOOL_TO_LNBOOL(TO_SOUND(sound)->is3DSound());
		return ::LN_OK;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_SetEmitterPosition(LNHandle sound, const LNVector3* position)
	{
		TO_SOUND(sound)->setPosition(TO_CORE_VEC3_REF(position));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_SetEmitterPositionXYZ(LNHandle sound, float x, float y, float z)
    {
		TO_SOUND(sound)->setPosition(LVector3(x, y, z));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_SetEmitterVelocity(int sound, const LNVector3* velocity)
	{
		TO_SOUND(sound)->setVelocity(TO_CORE_VEC3_REF(velocity));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_SetEmitterVelocityXYZ(int sound, float x, float y, float z)
    {
		TO_SOUND(sound)->setVelocity(LVector3(x, y, z));
		return ::LN_OK;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSound_SetEmitterDistance(int sound, float distance)
    {
		TO_SOUND( sound )->setMaxDistance( distance );
		return ::LN_OK;
    }
