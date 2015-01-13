//==============================================================================
// InputDeviceBase 
//------------------------------------------------------------------------------
///**
//  @file       InputDeviceBase.h
//  @brief      InputDeviceBase
//  @version    1.0
//*/
//==============================================================================

#pragma once

#include <deque>
#include "Common.h"

namespace LNote
{
namespace Core
{
namespace Input
{

//==============================================================================
// �� InputDeviceBase
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class InputDeviceBase
    : public Base::ReferenceObject
{
public:
	InputDeviceBase();
	virtual ~InputDeviceBase();

public:

	/// �ڑ��ς݂̃W���C�p�b�h���̎擾
	virtual int getJoypadNum() = 0;

	/// �}�E�X�̏�Ԃ̎擾
	virtual const LNMouseDeviceState& getMouseState() = 0;

	/// �L�[�{�[�h�̏�Ԃ̎擾 (�L�[��������Ă��邩)
	virtual bool getKeyState( LNVirtualKeyCode vkey );

	/// �L�[�{�[�h�̃o�b�t�@�����O�f�[�^�̎擾 (LN_KEY_UNKNOWN ��Ԃ��܂� while �ŉ񂵂Ď擾)
    virtual LNVirtualKeyCode getKeyBufferState();

	/// �W���C�p�b�h�̏�Ԃ̎擾
	virtual void getJoypadState( int joypadNumber, LNJoypadDeviceState* state ) = 0;

	/// �W���C�p�b�h�̐U�����J�n���� (power �͈̔͂� 0�`1000�Btime �̓~���b)
	virtual void startVibration( int joypadNumber, int power, int time ) = 0;

	/// �W���C�p�b�h�̐U�����~����
	virtual void stopVibration( int joypadNumber ) = 0;
    
	/// �^�b�`���̎擾
    virtual void getTouchState( int touchID, LNTouchDeviceState* state ) = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief      ���z�{�^���̏�Ԃ̎擾
	//  @param[in]  button			: ���ׂ鉼�z�{�^�� (���z�L�[�R�[�h�܂��� LNVirtualDeviceButton)
	//  @param[in]  keyboard		: true �̏ꍇ�A�L�[�{�[�h�̏�Ԃ��l������
	//  @param[in]  mouse			: true �̏ꍇ�A�}�E�X�̏�Ԃ��l������
	//  @param[in]  joypadNumber	: 0 �ȏ� �̏ꍇ�A���̔ԍ��̃W���C�p�b�h�̏�Ԃ��l������
	//  @return
	//              0.0�`1.0 �̒l
	//*/
	//----------------------------------------------------------------------
	virtual float getVirtualButtonState( lnU32 button, bool keyboard, bool mouse, int joypadNumber ) = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief      �}�E�X���W�̐ݒ�
	//  @par
	//              ���W�̓N���C�A���g�̈�̍�������_ (0, 0) �Ƃ����l�Őݒ肵�܂��B
	//              �E�B���h�E���A�N�e�B�u�ł͂Ȃ��ꍇ�͂Ȃɂ����܂���B
	//*/
	//----------------------------------------------------------------------
	virtual void setMousePoint( const LVector2& point ) = 0;

	/// �}�E�X�J�[�\���̈ʒu���N���C�A���g�̈�̒����ɐݒ肷��
    virtual void setMouseCursorCenter() = 0;

	/// ���͏��̍X�V
	virtual void update() = 0;

	/// �f�o�C�X�̍ăZ�b�g�A�b�v (USB �@��̐ڑ����m���ȂǂɌĂяo��)
	virtual void refresh() = 0;

	/// �֘A�t�����Ă��� IWindow �̎擾
    virtual System::Window* getWindow() = 0;
    
public:

	/// �C�x���g���� (Manager::onEvent() ����Ă΂��)
    virtual bool onEvent( const System::EventArgs& e );
	
private:
    typedef std::deque< LNVirtualKeyCode >     KeyBuffer;

private:
    bool                        mKeyStatus[LN_MAX_KEYS];
    KeyBuffer                   mKeyBuffer;
	bool						mEnableKeyBuffering;
};

} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================