//==============================================================================
// Property 
//------------------------------------------------------------------------------
///**
//  @file       Property.h
//  @brief      Property
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
// Property
//------------------------------------------------------------------------------
///**
//  @brief      
//	@note
//				WPF で言うところの DependencyProperty。
//				プロパティの目的は、
//				・ライブラリユーザーが自由にデータを追加できるようにする。
//				・データをスタイル化、テンプレート化できるようにする。
//				・データを XML に公開する。
//*/
//==============================================================================
class Property
	: public Base::ReferenceObject
{
public:
	Property();
	~Property();

public:
	void create(ClassDefinition* type, const lnChar* name);
	const lnRefString& getName() const { return mName; }

public:
	static void trySetProperty(Property** prop, ClassDefinition* type, const lnChar* name);

private:
	ClassDefinition*	mType;
	lnRefString			mName;
};

} // namespace GUI
} // namespace Core
} // namespace LNote
