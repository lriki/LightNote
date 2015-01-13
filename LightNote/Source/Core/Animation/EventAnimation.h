//==============================================================================
// EventAnimation 
//------------------------------------------------------------------------------
///**
//  @file       EventAnimation.h
//  @brief      EventAnimation
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

//==============================================================================
// ■ EventAnimationKeyBase
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class EventAnimationKeyBase
	: public Core::Base::ReferenceObject
{
public:

	double	FramePos;

public:
	EventAnimationKeyBase();
	virtual ~EventAnimationKeyBase() {}

public:

	/// 実行関数
	virtual void execute() = 0;

};

//==============================================================================
// ■ EventAnimation
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class EventAnimation
	: public AnimationTimelineBase
{
public:
	EventAnimation();
	virtual ~EventAnimation();

public:

	/// キーフレームの追加 (終端より前に追加した場合はソートを行う)
    void addKeyFrame( EventAnimationKeyBase* key );

	/// 時間を設定する (前回からの時間内にイベントがある場合はコールバックする)
	void setTimeWithEvent( double time );

	/// キー数の取得
	int getKeyCount() const { return mEventAnimationKeyArray.size(); }

	/// キーの取得
	EventAnimationKeyBase* getKeyAt( int index ) { return mEventAnimationKeyArray[index]; }

	/// キーのソート
	void sortKeys();

public:
	virtual ValueType getValueType() const { return ValueType_Event; }
	virtual void setTime( double time );			// イベントはコールバックされない
    virtual void advanceTime( lnFloat delta_time );	// 前回からの時間内にイベントがある場合はコールバックする
	virtual double getLastFramePos();
    virtual XML::Element* serializeXMLElement( XML::Document* doc );
    virtual void deserializeXMLElement( XML::Element* element );

private:

	// begin_frame <= key < end_frame で実行
	void executeRange( double begin_frame, double end_frame );

private:
	EventAnimationKeyArray	mEventAnimationKeyArray;
	int						mNextExecuteIndex;
};

} // namespace Animation
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================