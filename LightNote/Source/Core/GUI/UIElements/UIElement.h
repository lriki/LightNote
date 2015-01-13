//==============================================================================
// UIElement 
//------------------------------------------------------------------------------
///**
//  @file       UIElement.h
//  @brief      UIElement
//*/
//==============================================================================

#pragma once

#include "../../Geometry/DrawingShapes.h"
#include "../Common.h"
#include "../Data/DependencyObject.h"
#include "../Data/Event.h"
#include "../Data/EventArgs.h"

namespace LNote
{
namespace Core
{
namespace GUI
{

//==============================================================================
// UIElement
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class UIElement
	: public DependencyObject
{
public:
	static Property*	WidthProperty;
	static Property*	HeightProperty;

public:
	UIElement();
	~UIElement();

public:

	/*
	/// �}�E�X�̃q�b�g�e�X�g���s����
	IsHitTestVisible

		ActualHeight	���̗v�f�̕`�掞�̍������擾���܂��B
		�p�u���b�N �v���p�e�B	ActualWidth
		void                setVisible(bool flag_) { mVisible = flag_; }
	bool                isVisible() const { return mVisible; }

	Opacity
		Visibility

		// ���̗v�f��Ƀ}�E�X �|�C���^�[���u���ꂽ�Ƃ��ɕ\�������J�[�\��
		Cursor

		ClipToBounds	// �q�v�f�N���b�v
		AddHandler(RoutedEvent, Delegate)
		AddVisualChild

		// ���̗v�f���L�[�{�[�h �t�H�[�J�X���󂯎�����Ƃ��ɓK�p�����O��
		FocusVisualStyle

		Height	�v�f�̍����̐����l���擾�܂��͐ݒ肵�܂��B
		�p�u���b�N �v���p�e�B	HorizontalAlignment	���̗v�f���APanel �܂��̓A�C�e�� �R���g���[���̂悤�Ȑe�v�f���ɍ\�������Ƃ��ɓK�p����鐅�������̔z�u�������擾�܂��͐ݒ肵�܂��B

		Margin	�v�f�̊O���̗]�����擾�܂��͐ݒ肵�܂��B
		�p�u���b�N �v���p�e�B	MaxHeight	�v�f�̍����̍ő�l���擾�܂��͐ݒ肵�܂��B
		�p�u���b�N �v���p�e�B	MaxWidth	�v�f�̕��̍ő�l���擾�܂��͐ݒ肵�܂��B
		�p�u���b�N �v���p�e�B	MinHeight	�v�f�̍����̍ŏ��l���擾�܂��͐ݒ肵�܂��B
		�p�u���b�N �v���p�e�B	MinWidth	�v�f�̕��̍ŏ��l���擾�܂��͐ݒ肵�܂��B
		�p�u���b�N �v���p�e�B	Name	�v�f�̎��ʖ����擾�܂��͐ݒ肵�܂��B ���̖��O�́AXAML �v���Z�b�T�ɂ�鏈�����ɍ\�z���ꂽ�}�[�N�A�b�v�v�f���A���̌�ɕ����R�[�h(�C�x���g �n���h���[�̃R�[�h�Ȃ�) ����Q�Ƃ��邽�߂Ɏg�p�ł��܂��B

		Parent	���̗v�f�̘_����̐e�v�f���擾���܂��B

		Resources	���[�J���Œ�`���ꂽ���\�[�X �f�B�N�V���i�����擾�܂��͐ݒ肵�܂��B


		Style	�\�����ɁA���̗v�f���g�p����X�^�C�����擾�܂��͐ݒ肵�܂��B
		Tag	���̗v�f�Ɋւ��郆�[�U�[��`���̊i�[�Ɏg�p�ł���C�ӂ̃I�u�W�F�N�g�l���擾�܂��͐ݒ肵�܂��B
		Triggers	���̗v�f�Œ���(�܂��͎q�v�f��) �ݒ肳��Ă���g���K�[�̃R���N�V�������擾���܂��B
		Width	�v�f�̕����擾�܂��͐ݒ肵�܂��B

		void captureMouse();
	void releaseMouseCapture();

	BeginAnimation(DependencyProperty, AnimationTimeline, HandoffBehavior)
		HitTest
		RaiseEvent
	*/
	/// ���̗v�f���L���ł��邩
	void setEnable(bool enabled) { mEnabled = enabled; }

	/// ���̗v�f���L���ł��邩
	bool isEnable() const { return mEnabled; }

	/// �t�H�[�J�X�𓾂邱�Ƃ��ł��邩
	virtual bool isFocusable() const { return false; }

	/// �L�[�{�[�h�t�H�[�J�X�y�ј_���t�H�[�J�X��ݒ肷��
	void focus();

	/// ���̗v�f�ɘ_���t�H�[�J�X�����邩�ǂ����𔻒肷��
	bool isFocused() const;

public:
	void beginAnimation(Property* targetProp, AnimationTimeline* timeline);

public:
	virtual void measure(LSize availableSize);
	virtual void arrange(LRect finalRect);
	virtual void updateFrame(float elapsedTime);

public:
	const LSize& getDesiredSize() const { return mDesiredSize; }

	UIElement* getVisualChild(int index) { LN_THROW_NotImpl(0); return NULL; }
	int getVisualChildrenCount() const { LN_THROW_NotImpl(0); return 0; }
		
	// RoutedEvent
	void			registerEvent(Event* ev) { LN_THROW_NotImpl(0); }							// EventManager.RegisterRoutedEvent()
	void			addEventHandler(const Event* ev, int messageId) { LN_THROW_NotImpl(0); }	// AddHandler()
	void			raiseEvent(const Event* ev, EventArgs* args) { LN_THROW_NotImpl(0); }						// RaiseEvent()

protected:
	bool	mEnabled;
	LSize	mDesiredSize;
};

} // namespace GUI
} // namespace Core
} // namespace LNote
