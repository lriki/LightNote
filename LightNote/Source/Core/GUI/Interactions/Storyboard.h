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
//				Storyboard �͕����� AnimationTimeline ���Ǘ����郆�[�e�B���e�B�I�ȃI�u�W�F�N�g�B
//				(���ꂪ�����Ă��A�j���[�V�����͉\)
//				Storyboard ���g��Ȃ��ꍇ�AUIElement.beginAnimation() �ɒ���
//				AnimationTimeline ��n�����ƂŃA�j���[�V�������J�n�ł���B
//				Storyboard ���g���ꍇ�Abegin(���ۂɂ�Trigger�����Ȃ̂� invoke()) ���ꂽ��
//				Storyboard �������Ă��邷�ׂĂ� AnimationTimeline �� UIElement.beginAnimation() ��
//				���荞��ŃA�j���[�V�������J�n����B
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
