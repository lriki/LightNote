//==============================================================================
// ALAudioPlayer 
//------------------------------------------------------------------------------
///**
//  @file       ALAudioPlayer.h
//  @brief      ALAudioPlayer
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "ALLib.h"
#include "../AudioPlayerBase.h"
#include "../../Interface.h"

namespace LNote
{
namespace Core
{
namespace Audio
{

//==============================================================================
// ■ ALOnMemoryPlayer
//------------------------------------------------------------------------------
///**
//  @brief 
//*/
//==============================================================================
class ALOnMemoryPlayer
    : public AudioPlayerBase
{
public:
	ALOnMemoryPlayer( ALAudioDevice* device );
	virtual ~ALOnMemoryPlayer();

public:
	virtual void		initialize( AudioSourceBase* audioSource, bool enable3d );
	virtual void		setVolume( int volume );
	virtual void		setPitch( int pitch );
    virtual lnU64       getPlayedSamples() const;
	virtual void		play();
	virtual void		stop();
	virtual void		pause( bool isPause );
	virtual bool        polling();

public:

    virtual bool            is3DSound() { return false; }
	virtual void			setPosition( const LVector3& pos ) {}
	virtual const LVector3& getPosition() { return LVector3::ZERO; }
	virtual void            setVelocity( const LVector3& v ) {}
    virtual void            setEmitterDistance( lnFloat distance ) {  }
    virtual lnFloat         getEmitterDistance() const { return 0; }


private:
	ALuint	mALSource;
	ALuint	mALBuffer;
};


#if 0
//==============================================================================
// ■ ALAudioPlayerBase
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class XAudio2AudioPlayerBase
    : public Audio::AudioPlayerBase
{
public:
    XAudio2AudioPlayerBase( AudioDevice* manager );
    virtual ~XAudio2AudioPlayerBase();

public:
    virtual void			initialize( AudioSourceBase* audio_source, bool enable_3d );
    virtual bool            is3DSound() { return ( mEmitterState != NULL ); }
    virtual void			setPosition( const LVector3& pos );
    virtual const LVector3& getPosition() { return mEmitterState->Position; }
    virtual void            setVelocity( const LVector3& v );
    virtual void            setEmitterDistance( lnFloat distance ) { if ( mEmitterState ) mEmitterState->Distance = distance; }
    virtual lnFloat         getEmitterDistance() const { return ( mEmitterState ) ? mEmitterState->Distance : 0; }

public:

	/// IXAudio2SourceVoice の取得
    virtual IXAudio2SourceVoice* getXAudio2SourceVoice() const = 0;

	/// 3D 音源の情報の取得
    virtual EmitterState* getEmitterState() const { return mEmitterState; }

protected:

    AudioDevice*    mManager;       ///< 管理クラス
    EmitterState*   mEmitterState;  ///< 3D 音源の情報 (3D音源として初期化された場合に作成される)
};


//==============================================================================
// ■ Streaming
//------------------------------------------------------------------------------
/**
//  @brief      ストリーミングでの再生を扱うクラス
//  @note       
//              開始位置指定を実装する場合、ループ位置計算の都合で
//              ストリーミングは制限とする方向で。
//              (デコード時のブロックサイズがまちまちなので、一度流さないとわからない)
//*/
//==============================================================================
class Streaming
    : public XAudio2AudioPlayerBase
{
public:
	Streaming( AudioDevice* manager );
	virtual ~Streaming();

public:
	virtual void		initialize( AudioSourceBase* audio_source, bool enable_3d );
	virtual void		setVolume( int volume );
	virtual void		setPitch( int pitch );
    virtual lnU64		getPlayedSamples() const;
	virtual void		play();
	virtual void		stop();
	virtual void		pause( bool is_pause_ );
	virtual bool		polling();

public:
    virtual IXAudio2SourceVoice* getXAudio2SourceVoice() const { return mSourceVoice; }
    virtual EmitterState* getEmitterState() const { return mEmitterState; }

private:

	/// セカンダリに新しいデータを書き込んで、再生キューに追加する
	void _addNextBuffer();

private:
	IXAudio2SourceVoice*    mSourceVoice;

	lnU8*				    mPrimaryAudioData;		///< ダブルバッファリング用プライマリ
	lnU8*				    mSecondaryAudioData;	///< ダブルバッファリング用セカンダリ
	lnU32				    mAudioDataBufferSize;	///< プライマリバッファとセランダリバッファのサイズ ( 1 秒分 )
	lnU32				    mReadCursor;

    lnU32                   mLoopBeginBlockOffset;
    lnU32                   mCursorBeginPos;
    lnU32                   mCursorEndPos;

    lnU32                   mWriteSampleNum;
	bool					mEOF;			///< ファイルの終端まで再生したフラグ
    bool                    mPassedLoopBeginSample;
};
#endif

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================