//==============================================================================
// TriggerAction 
//------------------------------------------------------------------------------
///**
//  @file       TriggerAction.h
//  @brief      TriggerAction
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
// TriggerAction
//------------------------------------------------------------------------------
///**
//  @brief      
//	@note
//				
//*/
//==============================================================================
class TriggerAction
	: public Base::ReferenceObject
{
public:
	TriggerAction();
	~TriggerAction();

public:

	/// �A�N�V�������s (targetElement �����̃A�N�V�������Ăяo�����I�u�W�F�N�g)
	virtual void invoke(UIElement* targetElement) = 0;
};

} // namespace GUI
} // namespace Core
} // namespace LNote
