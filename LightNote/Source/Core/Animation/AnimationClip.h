//==============================================================================
// AnimationClip 
//------------------------------------------------------------------------------
///**
//  @file       AnimationClip.h
//  @brief      AnimationClip
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "Types.h"
#include "AnimationTimelineBase.h"

namespace LNote
{
namespace Core
{
namespace Animation
{

/// アニメーション再生の処理方法
enum WrapMode
{
	WrapMode_Once = 0,		///< 最後まで再生された後、停止する
	WrapMode_Loop,			///< 最後まで再生された後、先頭に戻ってループする
	//WrapMode_PingPong,		///< 最後まで再生された後、逆方向に戻ってループする
	//WrapMode_Default,
	//WrapMode_ClampForever,
};

//==============================================================================
// AnimationClip
//------------------------------------------------------------------------------
///**
//  @brief	タイムラインの集合 (3Dモデル用だと、歩行、ジャンプなどの1モーションを表す)
//*/
//==============================================================================
//class AnimationClip2
//	: public Core::Base::ReferenceObject
//{
//public:
//    AnimationClip2();
//    virtual ~AnimationClip2();
//
//public:
//
//	/// アニメーションクリップ名の設定
//	void setName( const lnRefStringW& name ) { mName = name; }
//
//	/// アニメーションクリップ名取得
//	const lnRefStringW& getName() const { return mName; }
//
//	/// 終端フレーム位置
//	double getLastFramePos() { return mLastFramePosition; }
//
//	/// 再生方法の設定
//	void setWrapMode( WrapMode mode ) { mWrapMode = mode; }
//
//	/// 再生方法の取得
//	WrapMode getWrapMode() { return mWrapMode; }
//
//	/// アニメーションの追加 (3Dモデルモーションであれば、ボーン名, SQTTransformAnimation。Eventアニメであれば NULL)
//	void addAnimationCurve( const wchar_t* relativePath, AnimationTimelineBase* curve );
//
//	/// 時間を進める (全てのアニメーションの advanceTime() が呼ばれる)
//	void advanceTime( double currentTime, lnFloat deltaTime );
//
//public:
//
//	struct AnimationCurveState
//	{
//		lnRefStringW			RelativePath;
//		AnimationTimelineBase*	AnimationCurve;
//	};
//
//	typedef std::vector<AnimationCurveState>	AnimationCurveStateArray;
//
//	AnimationCurveStateArray& getAnimationCurveStateArray() { return mAnimationCurveStateArray; }
//
//private:
//	lnRefStringW				mName;
//	double						mLastFramePosition;
//	WrapMode					mWrapMode;
//	AnimationCurveStateArray	mAnimationCurveStateArray;
//};




//==============================================================================
// ■ AnimationClip
//------------------------------------------------------------------------------
///**
//  @brief	AnimationTimelineBase の集合 (3Dモデル用だと、歩行、ジャンプなどの1モーション単位)
//*/
//==============================================================================
class AnimationClip
	: public Core::Base::ReferenceObject
{
public:

public:
    AnimationClip();
    virtual ~AnimationClip();

public:

	/// アニメーションクリップ名の設定
	void setName( const lnRefString& name ) { mName = name; }

	/// アニメーションクリップ名取得
	const lnRefKeyString& getName() const { return mName; }

	/// 終端フレーム位置
	double getLastFramePos() { return mLastFramePosition; }

	/// 再生方法の設定
	void setWrapMode( WrapMode mode ) { mWrapMode = mode; }

	/// 再生方法の取得
	WrapMode getWrapMode() { return mWrapMode; }

	/// アニメーションの追加 (3Dモデルモーションであれば、ボーン名, SQTTransformAnimation。Eventアニメであれば NULL)
	void addAnimationCurve( const lnKeyChar* relativePath, AnimationTimelineBase* curve );

	/// 時間を進める (全てのアニメーションの advanceTime() が呼ばれる)
	void advanceTime( double currentTime, lnFloat deltaTime );

public:

	struct AnimationCurveState
	{
		lnRefKeyString				RelativePath;
		AnimationTimelineBase*	AnimationCurve;
	};

	typedef std::vector<AnimationCurveState>	AnimationCurveStateArray;

	AnimationCurveStateArray& getAnimationCurveStateArray() { return mAnimationCurveStateArray; }

private:

	
	
	lnRefKeyString				mName;
	double						mLastFramePosition;
	WrapMode					mWrapMode;
	AnimationCurveStateArray	mAnimationCurveStateArray;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================