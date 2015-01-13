//==============================================================================
// Storyboard 
//------------------------------------------------------------------------------
///**
//  @file       Storyboard.h
//  @brief      Storyboard
//*/
//==============================================================================

#pragma once

#include "TriggerAction.h"

namespace LNote
{
namespace Core
{
namespace GUI
{

//==============================================================================
// Storyboard
//------------------------------------------------------------------------------
///**
//  @brief      
//	@note
//				Storyboard は複数の AnimationTimeline を管理するユーティリティ的なオブジェクト。
//				(これが無くてもアニメーションは可能)
//				Storyboard を使わない場合、UIElement.beginAnimation() に直接
//				AnimationTimeline を渡すことでアニメーションを開始できる。
//				Storyboard を使う場合、begin(実際にはTrigger扱いなので invoke()) されたら
//				Storyboard が持っているすべての AnimationTimeline を UIElement.beginAnimation() に
//				放り込んでアニメーションを開始する。
//*/
//------------------------------------------------------------------------------
/*
	Storyboard.cs
	http://referencesource.microsoft.com/#PresentationFramework/src/Framework/System/Windows/Media/Animation/Storyboard.cs

	BeginStoryboard.cs
	http://referencesource.microsoft.com/#PresentationFramework/src/Framework/System/Windows/Media/Animation/BeginStoryboard.cs
*/
//==============================================================================
class Storyboard
	: public TriggerAction
{
public:
	Storyboard();
	~Storyboard();

public:

public:
	// override TriggerAction
	virtual void invoke(UIElement* targetElement);
};

} // namespace GUI
} // namespace Core
} // namespace LNote
