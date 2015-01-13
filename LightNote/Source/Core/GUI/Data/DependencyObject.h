//==============================================================================
// DependencyObject 
//------------------------------------------------------------------------------
///**
//  @file       DependencyObject.h
//  @brief      DependencyObject
//*/
//==============================================================================

#pragma once

#include "../../Base/SortedArray.h"
#include "../Data/Variant.h"
#include "../Data/Property.h"

namespace LNote
{
namespace Core
{
namespace GUI
{

//==============================================================================
// DependencyObject
//------------------------------------------------------------------------------
///**
//  @brief      依存関係プロパティを持てるクラスのベースクラス
//*/
//------------------------------------------------------------------------------
/*
	WPF とは違い、XML に記述できる参照型以外 (Variant) のクラスは
	すべて DependencyObject を継承する。
	(WPF では XAML には自由にオブジェクトを書くことができ、子要素を追加できるものは
	すべて IAddChild を実装しているが LuminoUI ではすべて addChild() 実装で統一する。
	あまり複雑にしたくない。
	実際に追加したとき、追加先が子要素をサポートしない場合は例外)
*/
//==============================================================================
class DependencyObject
	: public Base::ReferenceObject
{
public:
	DependencyObject();
	~DependencyObject();

public:

	//TODO 型情報が必要。addChild したとき、それは追加できるか？の判定。あと、継承

	const Variant&	getValue(Property* prop);						// GetValue()
	void			setValue(Property* prop, const Variant& value);	// SetValue()
	
public:
	// IAddChild
	virtual void addChild(Variant* value) { LN_THROW_InvalidOperation(0); }

private:	
	// internal
	friend class ClassDefinition;
	Base::SortedArray<Property*, Variant>	mMemberPropertyArray;
};

} // namespace GUI
} // namespace Core
} // namespace LNote
