//==============================================================================
// ClassDefinition 
//------------------------------------------------------------------------------
///**
//  @file       ClassDefinition.h
//  @brief      ClassDefinition
//*/
//==============================================================================

#pragma once

#include "../../Base/SortedArray.h"
#include "Property.h"
#include "DependencyObject.h"
#include "Variant.h"

namespace LNote
{
namespace Core
{
namespace GUI
{
typedef DependencyObject* (*CreateInstanceFunc)();
typedef bool (*TryParseFunc)(const lnRefString& str, Variant* value);

//==============================================================================
// ClassDefinition
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class ClassDefinition
	: public Base::ReferenceObject
{
public:
	// ��`�ς݌^
	static ClassDefinition* ObjectType;
	static ClassDefinition* IntType;
	static ClassDefinition* FloatType;
	static ClassDefinition* BoolType;
	static ClassDefinition* StringType;
	static ClassDefinition* PropertyType;

public:
	ClassDefinition();
	~ClassDefinition();

public:
	void create(const lnChar* name, ClassDefinition* baseClass, CreateInstanceFunc createInstanceFunc, TryParseFunc tryParseFunc);

	void registerProperty(Property* prop);						// DependencyProperty.Register()


	DependencyObject* newInstance();

	/// ��������I�u�W�F�N�g�ɕϊ����� �B
	/// ��� XML ��Œl�Ƃ��ď����ꂽ��������g���B
	/// ��@) ���̃N���X�� enum Visibility �ŁA"Collapsed" �����͂��ꂽ�� int �^�ɂ��ĕԂ��B
	/// ��A) ���̃N���X�� struct Thickness �ŁA"0, 1, 2, 3" �����͂��ꂽ��C���X�^���X������ĕԂ��B
	bool tryParse(const lnRefString str, Variant* value);

private:
	lnRefString			mName;
	ClassDefinition*	mBaseClass;
	TryParseFunc		mTryParseFunc;
	Base::SortedArray<lnRefString, Property*>	mPropertyArray;

};

} // namespace GUI
} // namespace Core
} // namespace LNote
