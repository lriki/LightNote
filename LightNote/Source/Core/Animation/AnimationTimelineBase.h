//==============================================================================
// AnimationTimelineBase 
//------------------------------------------------------------------------------
///**
//  @file       AnimationTimelineBase.h
//  @brief      AnimationTimelineBase
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

//==============================================================================
// ■ AnimationTimelineBase
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class AnimationTimelineBase
	: public Core::Base::ReferenceObject
{
public:
	AnimationTimelineBase();
	virtual ~AnimationTimelineBase() {}

public:

	/// 時間のスケール係数の設定 (通常はフレームレートの値をそのまま渡しておく。デフォルト値は 60。setTime() で時間の値に乗算される)
    void setTimeTickPerSec( lnFloat tick ) { mTimeTick = tick; }

	/// ループ領域の設定
	void setLoopState( LNAnimationLoopMode loop_mode, lnFloat begin_frame_pos, lnFloat end_frame_pos );

	/// 再生が終了しているかを判定する (時間が最後のキーのフレーム位置以降の場合 true。ループ再生時は常に false)
    bool isFinished();

public:

	/// 値の種類
	virtual ValueType getValueType() const = 0;

	/// 時間の設定 (mTime をそのまま、mCurrentFramePos をループ考慮で設定する)
	virtual void setTime( double time );

	/// 時間を進める (進めた時間で setTime() が呼ばれる)
    virtual void advanceTime( lnFloat deltaTime );

	/// 時間を進める (現在時間と増分値を使用する。最終的な現在時間は time + delta_timeとなる)
    virtual void advanceTime( double time, lnFloat deltaTime );

	/// 終端フレーム位置の取得
	virtual double getLastFramePos() = 0;

	/// XMLElement へ保存
	virtual XML::Element* serializeXMLElement( XML::Document* doc ) { LN_THROW_NotImpl(0); }

	/// XMLElement から復元
    virtual void deserializeXMLElement( XML::Element* element ) { LN_THROW_NotImpl(0); }

protected:

	lnFloat				mTimeTick;
	double				mTime;				///< setTime() によって設定された値 (Tick未考慮)
	double				mCurrentFramePos;   ///< setTime() で設定された時間のフレーム位置 (ループ範囲内)
	LNAnimationLoopMode	mLoopMode;
	double				mLoopBeginPos;
    double				mLoopEndPos;
	
};

} // namespace Animation
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================