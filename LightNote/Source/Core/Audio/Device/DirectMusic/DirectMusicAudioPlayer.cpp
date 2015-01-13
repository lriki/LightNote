//==============================================================================
// DirectMusicAudioPlayer 
//==============================================================================

#include "stdafx.h"
#include "DirectMusicAudioDevice.h"
#include "DirectMusicAudioPlayer.h"

namespace LNote
{
namespace Core
{
namespace Audio
{
namespace DirectMusic
{

//==============================================================================
// AudioPlayerBase 
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    AudioPlayerBase::AudioPlayerBase( AudioDevice* manager )
        : Audio::AudioPlayerBase    ( manager )
        , mManager                  ( manager )
    {
        LN_SAFE_ADDREF( mManager );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    AudioPlayerBase::~AudioPlayerBase()
    {
        if ( mManager )
        {
            mManager->removeAudioPlayer( this );
            LN_SAFE_RELEASE( mManager );
        }
    }

//==============================================================================
// MidiPlayer
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	MidiPlayer::MidiPlayer( AudioDevice* manager )
		: AudioPlayerBase           ( manager )
        , mMidiAudioSource          ( NULL )
        , mSegment                  ( NULL )
        , mPauseTime                ( 0 )
	{
        LN_SAFE_ADDREF( mManager );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	MidiPlayer::~MidiPlayer()
	{
        SAFE_DELETE( mSegment );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void MidiPlayer::initialize( AudioSourceBase* audio_source, bool enable_3d )
    {
        mMidiAudioSource = dynamic_cast< Midi* >( audio_source );

		// AudioSource の管理・削除は基底に任せる
		mAudioSource = mMidiAudioSource;
		mAudioSource->addRef();

		// オンメモリ再生用に内部に持ってるバッファを埋める
		mMidiAudioSource->fillBufferSafe();

        if ( DirectMusicManager::getInstance()->isInitialized() && !mSegment )
        {
            mSegment = LN_NEW DirectMusicSegment(
                DirectMusicManager::getInstance()->createDMPerformance(),
                mMidiAudioSource );
        }
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void MidiPlayer::setVolume( int volume )
	{
        AudioPlayerBase::setVolume( volume );
        if ( mSegment ) mSegment->setVolume( this->mVolume );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void MidiPlayer::setPitch( int pitch )
	{
        AudioPlayerBase::setPitch( pitch );
        if ( mSegment ) mSegment->setPitch( this->mPitch );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    lnU64 MidiPlayer::getPlayedSamples() const
    {
        if ( mSegment )
        {
		    return mSegment->getPlayPosition();
        }
        return 0;
    }

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
	void MidiPlayer::setLoopState( lnU32 loop_begin, lnU32 loop_length )
	{
		mLoopBegin = loop_begin;
		mLoopLength = loop_length;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void MidiPlayer::play()
	{
        if ( !DirectMusicManager::getInstance()->isInitialized() )
        {
            // 同じものは追加されないのでこのままで OK
            DirectMusicManager::getInstance()->addPlayRequest( this );
            return;
        }

        // 初期化完了前にユーザーによってループ位置が設定されていなければ
        // CC111 を目印とした位置に設定する
        if ( mLoopBegin == 0 && mLoopLength == 0 )
        {
            lnU32 cc111time;
            lnU32 base_time;
            mMidiAudioSource->getLoopState( &cc111time, &base_time );

            mLoopBegin  = cc111time * LN_MUSICTIMEBASE / base_time;
            mLoopLength = 0;
        }

		// 再生開始
		_play();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void MidiPlayer::stop()
	{
		if ( mSegment )
		{
            mSegment->stop();
		}
		mIsPlaying = false;
		mIsPausing = false;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void MidiPlayer::pause( bool isPause )
	{
		// 再生中の場合
		if ( mIsPlaying )
		{
			// 一時停止する場合で、現在一時停止していない場合
			if ( isPause && !mIsPausing )
			{
				// 現在の演奏位置を記憶して停止
				mPauseTime = mSegment->getPlayPosition();
				mSegment->stop();
				
				mIsPausing = true;
			}
			else if ( !isPause && mIsPausing )
			{
				// 一時停止した時の再生位置に移動
				mSegment->setPlayPosition( mPauseTime );

				mSegment->play();

				mIsPausing = false;
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool MidiPlayer::polling()
	{
		// 再生中ではない場合は中断
		if ( !mIsPlaying || mIsPausing )
		{
			return false;
		}

		// ループ再生ではない場合
		if ( !mIsLoop )
		{
            if ( mSegment )
            {
                // 再生開始直後は IDirectMusicPerformance8::IsPlaying() は false になってしまい、
                // TotalTime() も実際に音が鳴る長さよりも若干短くなってしまっている。
                // (DirectMusic の仕様？というか、バグな気もする…)
                // そのため、音が鳴っている事と、再生位置による二重判定を行う。
                if ( !mSegment->isPlaying() && mSegment->getPlayPosition() >= mSegment->getTotalTime() )
			    {
                    stop();
				    return false;
			    }
		    }
        }

        return true;
	}

	// 臨時用。全体の再生時間の取得
    lnU32 MidiPlayer::getTotalTime() const
    {
        if ( mSegment ) // 未初期化対策
        {
            return mSegment->getTotalTime();
        }
        return 0;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void MidiPlayer::onFinishDMInit( IDirectMusicPerformance8* dmPerformance )
    {
        mSegment = LN_NEW DirectMusicSegment( dmPerformance, mMidiAudioSource );

        // 初期化中に設定されたパラメータを再設定する
        setVolume( static_cast< int >( mVolume ) );
        setPitch( static_cast< int >( mPitch ) );

        // 初期化完了前にユーザーによってループ位置が設定されていなければ
        // CC111 を目印とした位置に設定する
        if ( mLoopBegin == 0 && mLoopLength == 0 )
        {
            lnU32 cc111time;
            lnU32 base_time;
            mMidiAudioSource->getLoopState( &cc111time, &base_time );

            mLoopBegin  = cc111time * LN_MUSICTIMEBASE / base_time;
            mLoopLength = 0;

        }

        _play();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void MidiPlayer::_play()
	{
        // とりあえず停止
        stop();

        if ( !mSegment )
        {
            mSegment = LN_NEW DirectMusicSegment(
                DirectMusicManager::getInstance()->createDMPerformance(),
                mMidiAudioSource );

            // ちなみに setAudioSource() で作成するのはダメ。
            // DirectMusic の初期化中に setAudioSource() が呼ばれた場合はもちろん mSegment = NULL だけど、
            // そのあと play() が呼ばれたときにも当然 NULL のまま。
            // 初期化中に一度でも play() を呼んで要求リストに追加していれば onFinishDMInit() が呼ばれるけど、
            // そうでない場合はずっと NULL のままになってしまう。
        }

		// ループ再生する場合
		if ( mIsLoop )
		{
			mSegment->setLoopState( true, mLoopBegin, mLoopLength );
		}
		// ループ再生しない場合
		else
		{
			mSegment->setLoopState( false, 0, 0 );
		}

        mSegment->play();

        mIsPlaying = true;
		mIsPausing = false;
    }

} // namespace DirectMusic
} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================