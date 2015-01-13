//==============================================================================
// GameAudio 
//------------------------------------------------------------------------------
///**
//  @file       GameAudio.h
//  @brief      GameAudio
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <list>
#include "../Math/Vector3.h"
#include "../Threading/Mutex.h"
#include "Interface.h"
#include "Sound.h"

namespace LNote
{
namespace Core
{
namespace Audio
{

//==============================================================================
// ■ GameAudio
//------------------------------------------------------------------------------
///**
//	@brief      ゲーム用の音声の演奏を扱うクラス
//*/
//==============================================================================
class GameAudio
    : public Base::ReferenceObject
{
public:
	enum InternalGameSound 
	{
		InternalGameSound_BGM = 0,
		InternalGameSound_BGS,
		InternalGameSound_ME,

		InternalGameSound_Max,
	};

public:
	GameAudio( Manager* mamager );
	~GameAudio();

public:

	/// 管理クラスの取得
    Manager* getManager() const { return mManager; }

	/// BGM を演奏する
	void playBGM( const lnChar* filename, int volume = 100, int pitch = 100, int fade_time = 0 );

	/// 指定された Sound を BGM として演奏する
	void playBGMFromSound( Sound* sound, int volume = 100, int pitch = 100  , int fade_time = 0 );

	/// BGM の演奏を停止する
	void stopBGM( int fade_time = 0 );

	/// BGS ( 環境音 ) を演奏する
	void playBGS( const lnChar* filename, int volume = 100, int pitch = 100, int fade_time = 0 );

	/// 指定された Sound を BGS として演奏する
	void playBGSFromSound( Sound* sound, int volume = 100, int pitch = 100  , int fade_time = 0 );

	/// BGS の演奏を停止する
	void stopBGS( int fade_time = 0 );

	// ME ( 効果音楽 ) を演奏する
	void playME( const lnChar* filename, int volume = 100, int pitch = 100 );

	/// 指定された Sound を ME として演奏する
	void playMEFromSound( Sound* sound, int volume = 100, int pitch = 100 );

	/// ME の演奏を停止する
	void stopME();

	/// SE を演奏する
	void playSE( const lnChar* filename, int volume = 100, int pitch = 100 );

	/// SE を 3D 空間上で演奏する
    void playSE( const lnChar* filename, const LVector3& position, float distance, int volume = 100, int pitch = 100 );

	/// 指定された Sound を SE として演奏する
	void playSEFromSound( Sound* sound, int volume = 100, int pitch = 100 );

	/// 全ての SE の演奏を停止する
	void stopSE();

	/// ME 演奏時の BGM のフェード時間を設定する
    void setMEFadeState( int begin, int end );

	/// 再生中のBGMの音量を設定する (フェードアウト中は無効)
	void setBGMVolume( int volume, int fadeTime = 0 );

	/// 再生中のBGSの音量を設定する (フェードアウト中は無効)
	void setBGSVolume( int volume, int fadeTime = 0 );

	/// 同名 BGM の演奏再開フラグの設定
    void setEnableBGMRestart( bool flag ) { mBGMRestart = flag; }

	/// 同名 BGS の演奏再開フラグの設定
	void setEnableBGSRestart( bool flag ) { mBGSRestart = flag; }

	/// ゲーム音声として再生中の Sound の取得
	Sound* getInternalGameSound( InternalGameSound type );

public:

	/// 更新
	void polling();
    
private:

	/// 再生終了時解放リストに追加する
    void _pushReleaseAtPlayEndList( Sound* sound );

private:

    typedef std::list<Sound*>	ReleaseAtPlayEndList;

	Manager*	                mManager;
    Threading::Mutex*			mLock;
    ReleaseAtPlayEndList        mReleaseAtPlayEndList;  ///< 再生終了時に解放する音声リスト
	Sound*		                mBGM;
    Sound*		                mBGS;
    Sound*						mME;
    ln_std_tstring              mBGMName;
    ln_std_tstring              mBGSName;
    int                         mBGMVolume;
    int                         mBGMPitch;
    int                         mBGSVolume;
    int                         mBGSPitch;
    int                         mBGMFadeOutTime;
    int                         mBGMFadeInTime;
    bool                        mMEPlaying;
    bool                        mBGMRestart;
    bool                        mBGSRestart;

	LNOTE_TEST_UNIT;
};


} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================