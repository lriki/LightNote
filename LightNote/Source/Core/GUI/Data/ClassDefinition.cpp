//==============================================================================
// ClassDefinition 
//==============================================================================

#include "stdafx.h"
#include "../Common.h"
#include "ClassDefinition.h"

namespace LNote
{
namespace Core
{
namespace GUI
{

//==============================================================================
// ClassDefinition
//==============================================================================

ClassDefinition* ClassDefinition::ObjectType = NULL;
ClassDefinition* ClassDefinition::IntType = NULL;
ClassDefinition* ClassDefinition::FloatType = NULL;
ClassDefinition* ClassDefinition::BoolType = NULL;
ClassDefinition* ClassDefinition::StringType = NULL;
ClassDefinition* ClassDefinition::PropertyType = NULL;

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
ClassDefinition::ClassDefinition()
{
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
ClassDefinition::~ClassDefinition()
{
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void ClassDefinition::create(const lnChar* name, ClassDefinition* baseClass, CreateInstanceFunc createInstanceFunc, TryParseFunc tryParseFunc)
{

}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void ClassDefinition::registerProperty(Property* prop)
{
	mPropertyArray.add(prop->getName(), prop);
	prop->addRef();
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
DependencyObject* newInstance()
{
	// CreateInstanceFunc でインスタンス作って、
	// instance->mMemberPropertyArray にプロパティ追加していく

	LN_THROW_NotImpl(0);
	return NULL;
}

} // namespace GUI
} // namespace Core
} // namespace LNote
