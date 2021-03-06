﻿//==============================================================================
// XA2AudioPlayer 
//------------------------------------------------------------------------------
///**
//  @file       XA2AudioPlayer.h
//  @brief      XA2AudioPlayer
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../../Threading/Mutex.h"
#include "XA2AudioDevice.h"
#include "../AudioPlayerBase.h"
#include "../../Interface.h"

namespace LNote
{
namespace Core
{
namespace Audio
{
namespace XAudio2
{

//==============================================================================
// ■ XAudio2AudioPlayerBase
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
// ■ OnMemory
//------------------------------------------------------------------------------
///**
//  @brief      オンメモリデータの再生を扱うクラス
//*/
//==============================================================================
class OnMemory
    : public XAudio2AudioPlayerBase
{
public:
	OnMemory( AudioDevice* manager );
	virtual ~OnMemory();

public:
	virtual void		initialize( AudioSourceBase* audio_source, bool enable_3d );
	virtual void		setVolume( int volume );
	virtual void		setPitch( int pitch );
    virtual lnU64       getPlayedSamples() const;
	virtual void		play();
	virtual void		stop();
	virtual void		pause( bool is_pause_ );
	virtual bool        polling();

public:
    virtual IXAudio2SourceVoice* getXAudio2SourceVoice() const { return mSourceVoice; }
    virtual EmitterState* getEmitterState() const { return mEmitterState; }

private:
	IXAudio2SourceVoice*	    mSourceVoice;
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
	Threading::Mutex		mLock;

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

} // namespace XAudio2
} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================