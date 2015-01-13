//==============================================================================
// AnimationController 
//------------------------------------------------------------------------------
///**
//  @file       AnimationController.h
//  @brief      AnimationController
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "Types.h"

namespace LNote
{
namespace Core
{
namespace Animation
{
class AnimationClip;

enum PlayMode
{
	StopSameLayer = 0,	///< 同レイヤーのアニメーションをすべて停止する
	StopAll,			///< 全てのアニメーションを停止する
};

enum QueueMode
{
	CompleteOthers = 0,	///< 他のすべてのアニメーションが停止した後に再生を開始する
	PlayNow				///< すぐに再生する
};

/// アニメーション値更新通知を受け取るオブジェクト
class IAnimationApplyTarget2
{
public:
	virtual ~IAnimationApplyTarget2() {}

public:

	/// AnimationClip にセットしたアニメーションと関連付けるためのキー (通常、ボーン名)
	virtual const lnRefStringW& getAnimationTargetName() const = 0;

	/// 値が更新された時に呼ばれる
	virtual void onUpdateAnimationValue( ValueType type, void* value ) = 0;
};




















class IAnimationApplyTarget
{
public:
	virtual ~IAnimationApplyTarget() {}

public:

	/// AnimationClip にセットしたアニメーションと関連付けるためのキー (通常、ボーン名)
	virtual const lnRefKeyString& getAnimationTargetName() const = 0;

	/// 値が更新された時に呼ばれる
	virtual void onUpdateAnimationValue( ValueType type, void* value ) = 0;
};


/// アニメーション適用値を一時的に格納する場所と、それを設定するターゲットのセット
struct AnimationApplyTargetState
{
	enum ValueType			ValueType;
	lnFloat					Value[16];	// advanceTime() ではここに値を作って、
	IAnimationApplyTarget*	Target;		// 最終的にここに設定する。
};

struct AnimationState
{
	AnimationState()
	{
		mTime = 0;
	}

	class AnimationClip*	AnimationClip;


	/// (複数のClip間ではインデックスとボーン名が一致しているとは限らないので State に持たせておく)
	struct AnimationTargetSet
	{
		//IAnimationApplyTarget*	Target;
		AnimationTimelineBase*		Animation;		// このアニメーションは
		AnimationApplyTargetState*	TargetState;	// TargetState.Value に値を作る
	};
	typedef std::vector<AnimationTargetSet>	AnimationTargetSetArray;
	AnimationTargetSetArray		mAnimationTargetSetArray;
	double						mTime;
};

//==============================================================================
// ■ AnimationController
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class AnimationController
	: public Core::Base::ReferenceObject
{
public:

public:
    AnimationController();
    ~AnimationController();

public:

	/// 再生中であるかを調べる
	bool isPlaying() const;
    
	/// 再生 (他のアニメーションはすべて停止する)
	void play( const lnKeyChar* animName, PlayMode mode = StopSameLayer );

	/// ブレンド (アニメーションの再生には影響しない。停止中のアニメーションがこの関数によって再生開始されることはない)
	void blend( const lnKeyChar* animName, lnFloat targetWeight, lnFloat fadeLength );

	/// クロスフェード
	void crossFade( const lnKeyChar* animName, lnFloat fadeLength, PlayMode mode = StopSameLayer );

	/// 前のアニメーションが終了した後、再生を開始する
	void playQueued( const lnKeyChar* animName, QueueMode queueMode = CompleteOthers, PlayMode playMode = StopSameLayer );

	/// 前のアニメーションが終了するとき、クロスフェードで再生を開始する
	void crossFadeQueued( const lnKeyChar* animName, lnFloat fadeLength, QueueMode queueMode = CompleteOthers, PlayMode playMode = StopSameLayer );

	/// 同レイヤー内のアニメーション再生速度の同期
	void syncLayer( int layer );

	/// アニメーションを進める
	virtual void advanceTime( lnFloat deltaTime );

public:

	/// アニメーションクリップの追加   TODO: 名前 atach
	void addAnimationClip( AnimationClip* animationClip );

	/// アニメーションクリップの削除
	void removeAnimationClip( const lnKeyChar* clipName );

	/// AnimationState の検索
	AnimationState* findAnimationState( const lnKeyChar* clipName );

	/// AnimationApplyTarget の追加 (できるだけ AnimationController 作成直後に設定すること。重複禁止)
	void addAnimationApplyTarget( IAnimationApplyTarget* target );

private:


	/// 追加済み AnimationApplyTarget の検索
	AnimationApplyTargetState* findAnimationApplyTargetState( const lnKeyChar* naem );

	typedef std::vector<AnimationState*>			AnimationStateArray;
	typedef std::vector<AnimationApplyTargetState>	AnimationApplyTargetStateArray;

	AnimationStateArray				mAnimationStateArray;
	AnimationApplyTargetStateArray	mAnimationApplyTargetStateArray;
};










} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================