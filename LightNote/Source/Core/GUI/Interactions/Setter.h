//==============================================================================
// Setter 
//------------------------------------------------------------------------------
///**
//  @file       Setter.h
//  @brief      Setter
//*/
//==============================================================================

#pragma once

#include "../Data/Variant.h"
#include "../Data/DependencyObject.h"

namespace LNote
{
namespace Core
{
namespace GUI
{

//==============================================================================
// Setter
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class Setter
	: public DependencyObject
{
public:
	static Property*	PropertyProperty;
	static Property*	ValueProperty;

	static void initialize(Manager* manager);
	static DependencyObject* createInstance() { return LN_NEW Setter(); }

protected:
	Setter();
	~Setter();

public:

	/// 値のセット対象プロパティ
	Property* getProperty() { variant_cast<Property*>(DependencyObject::getValue(PropertyProperty)); }

	/// セットする値
	const Variant& getValue() { DependencyObject::getValue(ValueProperty); }

private:
};

} // namespace GUI
} // namespace Core
} // namespace LNote
