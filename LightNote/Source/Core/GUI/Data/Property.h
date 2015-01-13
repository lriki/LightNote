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
//				WPF �Ō����Ƃ���� DependencyProperty�B
//				�v���p�e�B�̖ړI�́A
//				�E���C�u�������[�U�[�����R�Ƀf�[�^��ǉ��ł���悤�ɂ���B
//				�E�f�[�^���X�^�C�����A�e���v���[�g���ł���悤�ɂ���B
//				�E�f�[�^�� XML �Ɍ��J����B
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
