//==============================================================================
// Audio::Sound 
//==============================================================================

#include "stdafx.h"
#if defined(LNOTE_DIRECTX)
#include "Device/DirectMusic/DirectMusicAudioPlayer.h"    // for get midi state
#endif
#include "Resource/AudioSource.h"
#include "Device/AudioPlayerBase.h"
#include "Manager.h"
#include "Sound.h"

namespace LNote
{
namespace Core
{
namespace Audio
{

//==============================================================================
// Sound
//==============================================================================

	LN_TYPE_INFO_ACCESS_IMPL(Sound);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Sound::Sound( Manager* manager, AudioPlayerBase* player )
        : mManager          ( manager )
        , mAudioPlayer      ( player )
        , mOrginalVolume    ( 100 )
        , mFadeState        ( SoundFadeState_NotFading )
        , mFlags            ( 0 )
        , mGroupFlags       ( 0 )
        , mIsFading         ( false )
    {
        LN_SAFE_ADDREF( mManager );
        LN_SAFE_ADDREF( mAudioPlayer );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Sound::~Sound()
    {
        if ( mManager )
        {
            mManager->removeSound( this );
        }
        
        LN_SAFE_RELEASE( mAudioPlayer );
        LN_SAFE_RELEASE( mManager );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    int Sound::getVolume()
    {
		Threading::ScopedLock lock( mLock );
        return mAudioPlayer->getVolume();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sound::setVolume( int volume )
    {
		Threading::ScopedLock lock( mLock );
        mAudioPlayer->setVolume( volume );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    int Sound::getPitch()
    {
		Threading::ScopedLock lock( mLock );
        return mAudioPlayer->getPitch();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sound::setPitch( int pitch )
    {
		Threading::ScopedLock lock( mLock );
        mAudioPlayer->setPitch( pitch );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sound::getState( AudioPlayState* state )
    {
		Threading::ScopedLock lock( mLock );

        AudioSourceBase* source = mAudioPlayer->getAudioSource();
        if ( source->getSourceFormat() == AUDIOSOURCE_MIDI )
        {
#if defined(LNOTE_DIRECTX)
            state->TotalUnits    = static_cast< DirectMusic::MidiPlayer* >( mAudioPlayer )->getTotalTime();
            state->SamplesPerSec = LN_MUSICTIMEBASE;
#endif
        }
        else
        {
            state->TotalUnits    = mAudioPlayer->getAudioSource()->getTotalUnits();
            state->SamplesPerSec = mAudioPlayer->getAudioSource()->getWaveFormat()->SamplesPerSec;
        }
        state->CurrentUnits = static_cast< lnU32 >( mAudioPlayer->getPlayedSamples() );
		state->PlayingState = mAudioPlayer->getPlayState();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sound::loop( bool enableFlag )
    {
        mAudioPlayer->loop( enableFlag );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool Sound::isLoop()
    {
        return mAudioPlayer->isLoop();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sound::setLoopState( lnU32 begin_, lnU32 length_ )
    {
		Threading::ScopedLock lock( mLock );
        mAudioPlayer->setLoopState( begin_, length_ );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sound::play()
    {
		Threading::ScopedLock lock( mLock );
        mAudioPlayer->play();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sound::stop()
    {
		Threading::ScopedLock lock( mLock );
        mAudioPlayer->stop();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sound::pause( bool isPause )
    {
		Threading::ScopedLock lock( mLock );
        mAudioPlayer->pause( isPause );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sound::fadeVolume( int targetVolume, int time, SoundFadeState state )
    {
		// 即時更新
		if ( time == 0 ) {
			setVolume( targetVolume );
		}

		Threading::ScopedLock lock( mLock );

		// 現在の音量から volume_ へのフェード
        mFadeValue.start( static_cast< int >( mAudioPlayer->getVolume() ), targetVolume, static_cast< double >( time ) * 0.001 );	
		mFadeState = state;
		mIsFading = true;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool Sound::is3DSound()
    {
        return mAudioPlayer->is3DSound();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sound::setPosition( const LVector3& pos )
    {
		Threading::ScopedLock lock( mLock );
        mAudioPlayer->setPosition( pos );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    const LVector3& Sound::getPosition()
    {
        return mAudioPlayer->getPosition();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sound::setVelocity( const LVector3& v )
    {
		Threading::ScopedLock lock( mLock );
        mAudioPlayer->setVelocity( v );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sound::setMaxDistance( lnFloat distance )
    {
        mAudioPlayer->setEmitterDistance( distance );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sound::setGroupFlag( lnU32 flag )
    {
		Threading::ScopedLock lock( mLock );
        mGroupFlags = flag;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    lnU32 Sound::getGroupFlag()
    {
		Threading::ScopedLock lock( mLock );
        return mGroupFlags;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool Sound::update( float elapsedTime )
    {
		Threading::ScopedLock lock( mLock );
        bool r = mAudioPlayer->polling();

        // フェード中の場合
		if ( mIsFading )
		{
			mFadeValue.advanceTime( elapsedTime );
			
			mAudioPlayer->setVolume( mFadeValue.getValue() );

			// フェード完了
			if ( mFadeValue.isFinished() )
			{
				mIsFading = false; 

				// フェード終了時の動作によって分岐
				switch ( mFadeState )
				{
					// 継続する場合
					case SOUNDFADE_CONTINUE:
						break;
					// 停止する場合
					case SOUNDFADE_STOP:
						mAudioPlayer->stop();
						break;
					// 一時停止する場合
					case SOUNDFADE_PAUSE:
						mAudioPlayer->pause( true );
						break;
					// 停止して音量を戻す場合
					case SOUNDFADE_STOP_RESET:
						mAudioPlayer->stop();
						mAudioPlayer->setVolume( mFadeValue.getStartValue() );
						break;
					// 一時停止して音量を戻す場合
					case SOUNDFADE_PAUSE_RESET:
						mAudioPlayer->pause( true );
						mAudioPlayer->setVolume( mFadeValue.getStartValue() );
						break;
				}
				mFadeState = SoundFadeState_NotFading;
			}
		}

        return r;
    }

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================