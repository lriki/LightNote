//==============================================================================
// AudioPlayerBase 
//------------------------------------------------------------------------------
///**
//  @file       AudioPlayerBase.h
//  @brief      AudioPlayerBase
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Base/Container/NodeList.h"
#include "../Interface.h"
#include "../Types.h"

namespace LNote
{
namespace Core
{
namespace Audio
{
class AudioDeviceBase;

//==============================================================================
// ■ AudioPlayerBase
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class AudioPlayerBase
    : public Base::ReferenceObject
    , public Base::NodeList< AudioPlayerBase >::Node
{
public:
    AudioPlayerBase( AudioDeviceBase* device );
    virtual ~AudioPlayerBase();

public:
    virtual void initialize( AudioSourceBase* audio_source, bool enable_3d );

public:
    virtual AudioSourceBase*	getAudioSource() const { return mAudioSource; }
    virtual void				setVolume( int volume );
    virtual int					getVolume() const { return static_cast< int >( mVolume ); }
    virtual void				setPitch( int pitch );
	virtual int					getPitch() const { return static_cast< int >( mPitch ); }
	virtual void				setLoopState( lnU32 loop_begin, lnU32 loop_length );
    virtual bool				isPlaying() const { return mIsPlaying; }
    virtual bool				isPausing() const { return mIsPausing; }
    virtual SoundPlayState		getPlayState() const { return (mIsPausing) ? SOUNDPLAYSTATE_PAUSING : ( (mIsPlaying) ? SOUNDPLAYSTATE_PLAYING : SOUNDPLAYSTATE_STOPPED ); }
    virtual void				loop( bool enableLoop ) { mIsLoop = enableLoop; }
    virtual bool				isLoop() const { return mIsLoop; }

public:

	/// 再生したサンプル数の取得 ( Midi の場合はミュージックタイム )
    virtual lnU64 getPlayedSamples() const = 0;

	/// 再生
	virtual void play() = 0;

	/// 停止
	virtual void stop() = 0;

	/// 一時停止
	virtual void pause( bool isPause ) = 0;

	/// ポーリング更新
	virtual bool polling() = 0;

	/// 3D 音源かを判定する
	virtual bool is3DSound() = 0;

	/// 3D 音源としての位置を設定する
    virtual void setPosition( const LVector3& pos ) = 0;

	/// 3D 音源としての位置を取得する
    virtual const LVector3& getPosition() = 0;

	/// 速度の設定
    virtual void setVelocity( const LVector3& v ) = 0;

	/// 3D 音源の影響範囲 (音の届く距離) の設定
    virtual void setEmitterDistance( lnFloat distance ) = 0;

	/// 3D 音源の影響範囲 (音の届く距離) の取得
    virtual lnFloat getEmitterDistance() const = 0;
	
protected:
    AudioDeviceBase*    mDevice;
    AudioSourceBase*	mAudioSource;
    float			    mVolume;		///< 音量 ( 0 ～ 100 )
	float			    mPitch;			///< ピッチ ( 50 ～ 200 )
	lnU32			    mLoopBegin;		///< ループされる領域の最初のサンプル (Midi なら ミュージックタイム単位)
	lnU32			    mLoopLength;	///< ループ領域の長さ (サンプル数単位)  (Midi なら ミュージックタイム単位)
	bool			    mIsPlaying;
	bool			    mIsPausing;
	bool			    mIsLoop;
};

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================