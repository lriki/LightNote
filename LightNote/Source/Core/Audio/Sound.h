//==============================================================================
// Sound 
//------------------------------------------------------------------------------
///**
//  @file       Sound.h
//  @brief      Sound
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Base/Container/NodeList.h"
#include "../Threading/Mutex.h"
#include "../Math/LMath.h"
#include "../Animation/FadeValue.h"
#include "Interface.h"

namespace LNote
{
namespace Core
{
namespace Audio
{

//==============================================================================
// ■ Sound
//------------------------------------------------------------------------------
///**
//  @brief      音声とその再生を扱うクラス
//*/
//==============================================================================
class Sound
    : public Base::ReferenceObject
    , public Base::NodeList< Sound >::Node
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
    Sound( Manager* manager, AudioPlayerBase* player );
	virtual ~Sound();
    LN_REFOBJ_METHODS;

public:

	/// 音量の取得
	int getVolume();

	/// 音量の設定
	void setVolume( int volume );

	/// ピッチの取得
	int getPitch();

	/// ピッチの設定
	void setPitch( int pitch );

	/// サウンドの状態を取得する
    void getState( AudioPlayState* state );

	/// ループ再生の有効設定
    void loop( bool enableFlag );

	/// ループ再生が有効かを判定する
    bool isLoop();

	/// ループ情報の設定
	void setLoopState( lnU32 begin, lnU32 length );

	/// 再生
	void play();

	/// 停止
	void stop();

	/// 一時停止
	void pause( bool isPause );

	/// 音量のフェード
	void fadeVolume( int targetVolume, int time, SoundFadeState state );

	/// 音量フェード状態の取得
	SoundFadeState getFadeState() const { return mFadeState; }

	/// 3D 音源かを判定する
    bool is3DSound();

	/// 3D 音源としての位置を設定する
    void setPosition( const LVector3& pos );

	/// 3D 音源としての位置を取得する
    const LVector3& getPosition();

	/// 速度の設定
    void setVelocity( const LVector3& v );

	/// 減衰距離の設定
    void setMaxDistance( lnFloat distance );

	/// グループの設定 (ビットフラグ)
	void setGroupFlag( lnU32 flag );

	/// グループの取得
    lnU32 getGroupFlag();

	/// 内部に持っている音声再生クラスの取得
    AudioPlayerBase* getAudioPlayer() { return mAudioPlayer; }

	/// フェード状態の更新
    bool update( float elapsedTime );

private:
    Manager*					mManager;
    AudioPlayerBase*			mAudioPlayer;
    Threading::Mutex			mLock;

    int 						mOrginalVolume;	        ///< setVolume() で設定される、元の音量
    SoundFadeState				mFadeState;
    Animation::FadeValue<int>	mFadeValue;
    
    lnU32						mFlags;                 ///< LNAudioPlayerFlag の値
    lnU32						mGroupFlags;
    bool						mIsFading;		        ///< フェードイン、またはフェードアウト中フラグ (TODO: 削除予定)
    bool						mOnlyRefFromManager;    ///< Manager::deleteAudioPlayer() から操作される
};

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================