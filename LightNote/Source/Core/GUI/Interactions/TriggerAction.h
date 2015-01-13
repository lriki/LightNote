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

	/// アクション実行 (targetElement がこのアクションを呼び出したオブジェクト)
	virtual void invoke(UIElement* targetElement) = 0;
};

} // namespace GUI
} // namespace Core
} // namespace LNote
