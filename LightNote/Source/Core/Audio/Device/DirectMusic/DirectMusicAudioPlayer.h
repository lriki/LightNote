//==============================================================================
// DirectMusicAudioPlayer 
//------------------------------------------------------------------------------
///**
//  @file       DirectMusicAudioPlayer.h
//  @brief      DirectMusicAudioPlayer
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "DirectMusic.h"
#include "../AudioPlayerBase.h"
#include "../../Interface.h"

namespace LNote
{
namespace Core
{
namespace Audio
{
namespace DirectMusic
{
class AudioDevice;

//==============================================================================
// ■ AudioPlayerBase
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class AudioPlayerBase
    : public Audio::AudioPlayerBase
{
public:
    AudioPlayerBase( AudioDevice* manager );
    virtual ~AudioPlayerBase();

public:

    virtual void			initialize( AudioSourceBase* audio_source, bool enable_3d ) { }
    virtual bool            is3DSound() { return false; }
    virtual void			setPosition( const LVector3& pos ) { }
    virtual const LVector3& getPosition() { return LVector3::Zero; }
    virtual void            setVelocity( const LVector3& v ) { }
    virtual void            setEmitterDistance( lnFloat distance ) { }
    virtual lnFloat         getEmitterDistance() const { return 0; }

protected:

    AudioDevice*    mManager;       ///< 管理クラス
};

//==============================================================================
// ■ MidiPlayer
//------------------------------------------------------------------------------
///**
//  @brief      Midi データの再生を扱うクラス
//
//  @par
//              DirectMusic を別スレッドで初期化する場合、フェードイン・アウト時間の
//              つじつまが合わなくなるということがあります。
//              これは「DirectMusic 初期化中は音を鳴らすことができないため、
//              再生中ではない」という判定が入り、停止中はフェード処理がされないために起こります。
//              <br>
//              これを修正し、Midi の演奏が開始される時間に合わせて正しい音量を
//              設定するという事は可能でしたが、実際にこの不具合に遭遇するような事態というのは
//              ユーザーの方が「ゲーム開始時からフェードイン再生したい」というものがほとんどです。
//              これを修正した場合、開始時に中途半端な音量から演奏を開始することになり、
//              演出としてあまり良くないと考えたため、現段階ではこれは仕様とします。
//*/
//==============================================================================
class MidiPlayer
    : public AudioPlayerBase
    , public DirectMusicManager::PlayerObject
{
public:
	MidiPlayer( AudioDevice* manager );
	virtual ~MidiPlayer();

public:
	virtual void		initialize( AudioSourceBase* audio_source, bool enable_3d );
	virtual void		setVolume( int volume );
	virtual void		setPitch( int pitch );
    virtual lnU64		getPlayedSamples() const;
	virtual void		setLoopState( lnU32 loop_begin, lnU32 loop_length );
	virtual void		play();
	virtual void		stop();
	virtual void		pause( bool is_pause_ );
	virtual bool		polling();

    lnU32 getTotalTime() const;

public:
    virtual void onFinishDMInit( IDirectMusicPerformance8* dm_performance );

private:

	/// 実際に再生する
    void _play();

private:

    Midi*		            mMidiAudioSource;   ///< Midi* にキャストした AudioPlayerBase::mAudioSource
    DirectMusicSegment*     mSegment;           ///< DirectMusic の再生管理
    lnU32                   mPauseTime;
};

} // namespace XAudio2
} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================