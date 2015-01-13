//==============================================================================
// UIElementTemplate 
//------------------------------------------------------------------------------
///**
//  @file       UIElementTemplate.h
//  @brief      UIElementTemplate
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
// UIElementTemplate
//------------------------------------------------------------------------------
///**
//  @brief      
//	@note
//				WPF �Ō����Ƃ���� FrameworkTemplate �� ControlTemplate�B
//*/
//------------------------------------------------------------------------------
/*
	��UIElementTemplate(ControlTemplate) �ƁAUIElementFactory(FrameworkElementFactory) �̈Ⴂ
		�����Ƃ� SetValue �� Style�ATrigger ��ݒ肷�邱�Ƃ��ł�
		�Ȃ�ƂȂ����Ă��邪�A�g�p�ړI�͂��񂺂�Ⴄ�B
		�EUIElementFactory
			�P����1�� UIElement �����B�������Ƃ������B
			�q�v�f��ǉ����邱�ƂŁA�c���[���̂��̂�\����B
			�C���X�^���X���������̂͂��̂܂� Visual�c���[�ɂ�Logical�c���[�ɂ��g����B
		�EUIElementTemplate
			1�̘_���I�ȃR���g���[���̍쐬�ɕK�v�ȏ��̏W���B�Ⴆ�� Button �� Chrome �� TextBlock ���琬�藧���Ă��邪�A
			�g���Ƃ��͂���Ȃ̋C�ɂ��Ȃ��ŁA�ЂƂ� Button �Ƃ��� Visual �܂��� Logical �c���[�Ɍq�����Ƃ��ł���B
			UIElementTemplate ���K�w�\���ɂ��邱�Ƃ͂ł��Ȃ��B
			�y�Ή����� Presenter ���K���K�v�ɂȂ�B�z
	
	��UIElementFactory����̃C���X�^���X��
		UIElementFactory::createUIElement() �Ŏ��g���쐬�B
		�q�v�f Factor ������΁A���ꂼ��� createUIElement() ���ĂсA
		�쐬���ꂽ UIElement ���K�w�\���Ɍ������ĕԂ��B
		UIElement ���쐬����Ƃ��́A���O�ɂ����Manager ���� UIElementTemplate ���擾���A
		UIElementTemplate.createUIElement() ���Ăяo���B
		1�� create ������ UIElementTemplate �� UIElementFactory ���݂��Ⴂ�ɌĂяo����Ă��Ȃ蕡�G�ɂȂ�̂Œ��ӁB
		�yManager �ɃR���g���[���Ƃ��Ē��ړo�^�����̂� UIElementTemplate �ł���z
*/
//==============================================================================
class UIElementTemplate
	: public Base::ReferenceObject
{
public:
	UIElementTemplate();
	~UIElementTemplate();

public:

};

} // namespace GUI
} // namespace Core
} // namespace LNote
