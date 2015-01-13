//==============================================================================
// Panel 
//------------------------------------------------------------------------------
///**
//  @file       Panel.h
//  @brief      Panel
//*/
//==============================================================================

#pragma once

#include "UIElement.h"

namespace LNote
{
namespace Core
{
namespace GUI
{

//==============================================================================
// Panel
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class Panel
	: public UIElement
{
public:
	Panel();
	~Panel();

public:
	void addChild(UIElement* element);
	void removeChild(UIElement* element);

};

} // namespace GUI
} // namespace Core
} // namespace LNote
