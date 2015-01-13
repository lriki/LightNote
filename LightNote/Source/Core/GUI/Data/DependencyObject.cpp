//==============================================================================
// DependencyObject 
//==============================================================================

#include "stdafx.h"
#include "../Common.h"
#include "../Data/UIException.h"
#include "DependencyObject.h"

namespace LNote
{
namespace Core
{
namespace GUI
{

//==============================================================================
// DependencyObject
//==============================================================================

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
DependencyObject::DependencyObject()
{
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
DependencyObject::~DependencyObject()
{
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
const Variant& DependencyObject::getValue(Property* prop)
{ 
	Variant* v = mMemberPropertyArray.find(prop);
	LN_THROW_UI(v != NULL, ArgumentException);
	return *v;
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void DependencyObject::setValue(Property* prop, const Variant& value)
{
	Variant* v = mMemberPropertyArray.find(prop);
	LN_THROW_UI(v != NULL, ArgumentException);
	*v = value;
}

} // namespace GUI
} // namespace Core
} // namespace LNote
