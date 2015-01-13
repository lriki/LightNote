//==============================================================================
// AnimationTimeline 
//------------------------------------------------------------------------------
///**
//  @file       AnimationTimeline.h
//  @brief      AnimationTimeline
//*/
//==============================================================================

#pragma once

namespace LNote
{
namespace Core
{
namespace GUI
{

//==============================================================================
// AnimationTimeline
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//------------------------------------------------------------------------------
/*
	WPF では Timeline クラスを継承している。
	Timeline クラスは再生中の時間情報などを持ち、
	AnimationTimeline は現在値を持つ。
*/
//==============================================================================
class AnimationTimeline
	: public Base::ReferenceObject
{
public:
	AnimationTimeline();
	~AnimationTimeline();

public:
	const Variant& getCurrentValue();
};

} // namespace GUI
} // namespace Core
} // namespace LNote
