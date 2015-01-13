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
//  @brief      �ˑ��֌W�v���p�e�B�����Ă�N���X�̃x�[�X�N���X
//*/
//------------------------------------------------------------------------------
/*
	WPF �Ƃ͈Ⴂ�AXML �ɋL�q�ł���Q�ƌ^�ȊO (Variant) �̃N���X��
	���ׂ� DependencyObject ���p������B
	(WPF �ł� XAML �ɂ͎��R�ɃI�u�W�F�N�g���������Ƃ��ł��A�q�v�f��ǉ��ł�����̂�
	���ׂ� IAddChild ���������Ă��邪 LuminoUI �ł͂��ׂ� addChild() �����œ��ꂷ��B
	���܂蕡�G�ɂ������Ȃ��B
	���ۂɒǉ������Ƃ��A�ǉ��悪�q�v�f���T�|�[�g���Ȃ��ꍇ�͗�O)
*/
//==============================================================================
class DependencyObject
	: public Base::ReferenceObject
{
public:
	DependencyObject();
	~DependencyObject();

public:

	//TODO �^��񂪕K�v�BaddChild �����Ƃ��A����͒ǉ��ł��邩�H�̔���B���ƁA�p��

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
