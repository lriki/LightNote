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
	WPF �ł� Timeline �N���X���p�����Ă���B
	Timeline �N���X�͍Đ����̎��ԏ��Ȃǂ������A
	AnimationTimeline �͌��ݒl�����B
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
