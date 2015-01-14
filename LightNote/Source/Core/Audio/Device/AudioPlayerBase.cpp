//==============================================================================
// AudioPlayerBase
//==============================================================================

#include "stdafx.h"
#include "../Resource/AudioSource.h"
#include "AudioDeviceBase.h"
#include "AudioPlayerBase.h"

namespace LNote
{
namespace Core
{
namespace Audio
{

//==============================================================================
// AudioPlayerBase 
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    AudioPlayerBase::AudioPlayerBase( AudioDeviceBase* device )
        : mDevice           ( device )
        , mAudioSource      ( NULL )
		, mVolume           ( 100.f )
		, mPitch            ( 100.f )
		, mLoopBegin        ( 0 )
		, mLoopLength       ( 0 )
		, mIsPlaying        ( false )
		, mIsPausing        ( false )
		, mIsLoop           ( false )
    {
        LN_SAFE_ADDREF( mDevice );
        if ( mDevice ) mDevice->addAudioPlayer( this );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    AudioPlayerBase::~AudioPlayerBase()
    {
		LN_SAFE_RELEASE( mAudioSource );

        if ( mDevice ) mDevice->removeAudioPlayer( this );
        LN_SAFE_RELEASE( mDevice );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void AudioPlayerBase::initialize( AudioSourceBase* audio_source, bool enable_3d )
    { 
		LN_ERR2_ASSERT( audio_source );

        mAudioSource = audio_source;
		mAudioSource->addRef();

        // ソースからループ位置を取得
        lnU32 loop_begin, loop_length;
        mAudioSource->getLoopState( &loop_begin, &loop_length );
        mLoopBegin  = loop_begin;
		mLoopLength = loop_length;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AudioPlayerBase::setVolume( int volume )
	{
        mVolume = static_cast< float >( volume );
        mVolume = LMath::Clamp( mVolume, 0.0f, 100.0f );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AudioPlayerBase::setPitch( int pitch )
	{
		mPitch = static_cast< float >( pitch );
		mPitch = LMath::Clamp(mPitch, 50.0f, 200.0f);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void AudioPlayerBase::setLoopState( lnU32 loop_begin, lnU32 loop_length )
	{
        if ( loop_begin == 0 && loop_length == 0 )
        {
            // ソースからループ位置を取得して設定する
            lnU32 begin, length;
            mAudioSource->getLoopState( &begin, &length );
            mLoopBegin  = begin;
		    mLoopLength = length;
        }
        else
        {
            mLoopBegin = loop_begin;
		    mLoopLength = loop_length;
        }
	}

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================