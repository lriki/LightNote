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
	// 定義済み型
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

	/// 文字列をオブジェクトに変換する 。
	/// 主に XML 上で値として書かれた文字列を使う。
	/// 例①) このクラスが enum Visibility で、"Collapsed" が入力されたら int 型にして返す。
	/// 例②) このクラスが struct Thickness で、"0, 1, 2, 3" が入力されたらインスタンスを作って返す。
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
