//==============================================================================
// Setter 
//==============================================================================

#include "stdafx.h"
#include "../Common.h"
#include "../Manager.h"
#include "../Data/ClassDefinition.h"
#include "Setter.h"

namespace LNote
{
namespace Core
{
namespace GUI
{

//==============================================================================
// Setter
//==============================================================================

Property*	Setter::PropertyProperty = NULL;
Property*	Setter::ValueProperty = NULL;

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void Setter::initialize(Manager* manager)
{
	// "Setter" �N���X
	ClassDefinition* type = LN_NEW ClassDefinition();
	type->create(_T("Setter"), NULL, createInstance, NULL);

	// "Property" �v���p�e�B
	PropertyProperty = LN_NEW Property();
	PropertyProperty->create(ClassDefinition::PropertyType, _T("Property"));
	type->registerProperty(PropertyProperty);

	// "Value" �v���p�e�B
	ValueProperty = LN_NEW Property();
	ValueProperty->create(ClassDefinition::ObjectType, _T("Value"));
	type->registerProperty(ValueProperty);

	manager->registerType(type);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
Setter::Setter()
{
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
Setter::~Setter()
{
}

} // namespace GUI
} // namespace Core
} // namespace LNote
