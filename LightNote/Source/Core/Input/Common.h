//==============================================================================
// Interface 
//------------------------------------------------------------------------------
//**
//  @file       Interface.h
//  @brief      Interface
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Math/Vector2.h"
#include "../System/Interface.h"

namespace LNote
{
namespace Core
{
namespace Input
{
class Manager;
class InputDeviceBase;
class Joypad;

static const int LN_MAX_JOYPADS = 4;            ///< �W���C�p�b�h�̍ő吔
static const int LN_MAX_JOYPAD_BUTTONS = 16;    ///< �W���C�p�b�h�̃{�^���̍ő吔
static const int LN_MAX_JOYPAD_AXIS = 8;        ///< �W���C�p�b�h�̎��̍ő吔
static const int LN_MAX_GAME_CONTROLLERS = 4;   ///< ���z�R���g���[���̍ő吔
#define          LN_JOYPAD_PLAY_RANGE   (0.2f)  ///< �W���C�p�b�h�̂����є͈�
#define          LN_JOYPAD_SWITCH_RANGE (0.8f)

/// �}�E�X�̃{�^���ԍ�
enum LNMouseButton
{
	LN_MOUSE_BUTTON_1 = 0,
	LN_MOUSE_BUTTON_2,
	LN_MOUSE_BUTTON_3,
	LN_MOUSE_BUTTON_4,
	LN_MOUSE_BUTTON_5,
	LN_MOUSE_BUTTON_6,
	LN_MOUSE_BUTTON_7,
	LN_MOUSE_BUTTON_8,

	LN_MAX_MOUSE_BUTTONS
};

/// �Q�[���{�^����\���l (()���̓f�t�H���g�Ŋ��蓖�Ă�Ă���L�[�A�{�^���A��)
enum LNGameControllerButton
{
	LN_BUTTON_A = 0,        ///< A�{�^�� (�L�[�{�[�h='Z' �W���C�p�b�h=1�ԃ{�^��)
	LN_BUTTON_B,            ///< B�{�^�� (�L�[�{�[�h='X' �W���C�p�b�h=2)
	LN_BUTTON_C,            ///< C�{�^�� (�L�[�{�[�h='C' �W���C�p�b�h=3)
	LN_BUTTON_X,            ///< X�{�^�� (�L�[�{�[�h='A' �W���C�p�b�h=4)
	LN_BUTTON_Y,            ///< Y�{�^�� (�L�[�{�[�h='S' �W���C�p�b�h=5)
	LN_BUTTON_Z,            ///< Z�{�^�� (�L�[�{�[�h='D' �W���C�p�b�h=6)
	LN_BUTTON_L,            ///< L�{�^�� (�L�[�{�[�h='Q' �W���C�p�b�h=7)
	LN_BUTTON_R,            ///< R�{�^�� (�L�[�{�[�h='W' �W���C�p�b�h=8)
	LN_BUTTON_L2,           ///< L2�{�^�� (�L�[�{�[�h=None �W���C�p�b�h=9)
	LN_BUTTON_R2,           ///< R2�{�^�� (�L�[�{�[�h=None �W���C�p�b�h=10)
	LN_BUTTON_L3,           ///< L3�{�^�� (�L�[�{�[�h=None �W���C�p�b�h=11)
	LN_BUTTON_R3,           ///< R3�{�^�� (�L�[�{�[�h=None �W���C�p�b�h=12)
	LN_BUTTON_START,        ///< Start�{�^�� (�L�[�{�[�h=None �W���C�p�b�h=13)
	LN_BUTTON_SELECT,       ///< Select�{�^�� (�L�[�{�[�h=None �W���C�p�b�h=14)
	LN_BUTTON_SUB1,         ///< Sub1�{�^�� (�L�[�{�[�h=None �W���C�p�b�h=15)
	LN_BUTTON_SUB2,         ///< Sub2�{�^�� (�L�[�{�[�h=None �W���C�p�b�h=16)
	LN_BUTTON_LEFT,         ///< ���{�^�� (�L�[�{�[�h=LN_KEY_LEFT  �W���C�p�b�h=POV��&��1X��-)
	LN_BUTTON_RIGHT,        ///< �E�{�^�� (�L�[�{�[�h=LN_KEY_RIGHT �W���C�p�b�h=POV�E&��1X��+)
	LN_BUTTON_UP,           ///< ��{�^�� (�L�[�{�[�h=LN_KEY_UP    �W���C�p�b�h=POV��&��1Y��-)
	LN_BUTTON_DOWN,         ///< ���{�^�� (�L�[�{�[�h=LN_KEY_DOWN  �W���C�p�b�h=POV��&��1Y��+)

	LN_BUTTON_AXIS_1X_MINUIS,   ///< ��1���o�[ X- ����
    LN_BUTTON_AXIS_1X_PLUS,     ///< ��1���o�[ X+ ����
    LN_BUTTON_AXIS_1Y_MINUIS,   ///< ��1���o�[ Y- ����
    LN_BUTTON_AXIS_1Y_PLUS,     ///< ��1���o�[ Y+ ����
    LN_BUTTON_AXIS_2X_MINUIS,   ///< ��2���o�[ X- ����
    LN_BUTTON_AXIS_2X_PLUS,     ///< ��2���o�[ X+ ����
    LN_BUTTON_AXIS_2Y_MINUIS,   ///< ��2���o�[ Y- ����
    LN_BUTTON_AXIS_2Y_PLUS,     ///< ��2���o�[ Y+ ����
	
	LN_MAX_BUTTONS,

    LN_BUTTON_BEGIN = LN_BUTTON_A,
    LN_BUTTON_END   = LN_BUTTON_DOWN,
    LN_BUTTON_AXIS_BEGIN   = LN_BUTTON_AXIS_1X_MINUIS,
    LN_BUTTON_AXIS_END     = LN_BUTTON_AXIS_2Y_PLUS,
};

/// �R���g���[���̃A�^�b�` (�L�[�R���t�B�O) �^�[�Q�b�g
// TODO: LNGameControllerButton �Ɠ����ɂȂ����B����Ȃ�
enum LNGameControllerAttachTarget
{
	LN_AT_BUTTON_A = 0,     ///< A�{�^��
	LN_AT_BUTTON_B,         ///< B�{�^��
	LN_AT_BUTTON_C,         ///< C�{�^��
	LN_AT_BUTTON_X,         ///< X�{�^��
	LN_AT_BUTTON_Y,         ///< Y�{�^��
	LN_AT_BUTTON_Z,         ///< Z�{�^��
	LN_AT_BUTTON_L,         ///< L�{�^��
	LN_AT_BUTTON_R,         ///< R�{�^��
	LN_AT_BUTTON_L2,        ///< L2�{�^��
	LN_AT_BUTTON_R2,        ///< R2�{�^��
	LN_AT_BUTTON_L3,        ///< L3�{�^��
	LN_AT_BUTTON_R3,        ///< R3�{�^��
	LN_AT_BUTTON_START,     ///< R3�{�^��
	LN_AT_BUTTON_SELECT,    ///< R3�{�^��
	LN_AT_BUTTON_SUB1,      ///< R3�{�^��
	LN_AT_BUTTON_SUB2,      ///< R3�{�^��
	LN_AT_BUTTON_LEFT,      ///< ���{�^��
	LN_AT_BUTTON_RIGHT,     ///< �E�{�^��
	LN_AT_BUTTON_UP,        ///< ��{�^��
	LN_AT_BUTTON_DOWN,      ///< ���{�^��

    LN_AT_AXIS_1X_MINUIS,   ///< ��1���o�[ X- ����
    LN_AT_AXIS_1X_PLUS,     ///< ��1���o�[ X+ ����
    LN_AT_AXIS_1Y_MINUIS,   ///< ��1���o�[ Y- ����
    LN_AT_AXIS_1Y_PLUS,     ///< ��1���o�[ Y+ ����
    LN_AT_AXIS_2X_MINUIS,   ///< ��2���o�[ X- ����
    LN_AT_AXIS_2X_PLUS,     ///< ��2���o�[ X+ ����
    LN_AT_AXIS_2Y_MINUIS,   ///< ��2���o�[ Y- ����
    LN_AT_AXIS_2Y_PLUS,     ///< ��2���o�[ Y+ ����


	LN_MAX_AT_TARGETS,

    LN_AT_BUTTON_BEGIN = LN_AT_BUTTON_A,
    LN_AT_BUTTON_END   = LN_AT_BUTTON_DOWN,
    LN_AT_AXIS_BEGIN   = LN_AT_AXIS_1X_MINUIS,
    LN_AT_AXIS_END     = LN_AT_AXIS_2Y_PLUS,

};

/// �S�Ẵf�o�C�X�p�̉��z�{�^���ԍ� (�L�[�R���t�B�O�Ŏg��  ��)attachButton( LN_BUTTON_L2, LN_VBUTTON_JOYPAD_AXIS_3_PLUS ))
enum LNVirtualButton
{
    LN_VBUTTON_NULL = 0,

    LN_VBUTTON_MOUSE_1 = 256,		// GLFW �ƍ��킹�� 1 �X�^�[�g
    LN_VBUTTON_MOUSE_2,
    LN_VBUTTON_MOUSE_3,
    LN_VBUTTON_MOUSE_4,
    LN_VBUTTON_MOUSE_5,
    LN_VBUTTON_MOUSE_6,
    LN_VBUTTON_MOUSE_7,
    LN_VBUTTON_MOUSE_8,

    LN_VBUTTON_JOYPAD_BUTTON_1,
    LN_VBUTTON_JOYPAD_BUTTON_2,
    LN_VBUTTON_JOYPAD_BUTTON_3,
    LN_VBUTTON_JOYPAD_BUTTON_4,
    LN_VBUTTON_JOYPAD_BUTTON_5,
    LN_VBUTTON_JOYPAD_BUTTON_6,
    LN_VBUTTON_JOYPAD_BUTTON_7,
    LN_VBUTTON_JOYPAD_BUTTON_8,
    LN_VBUTTON_JOYPAD_BUTTON_9,
    LN_VBUTTON_JOYPAD_BUTTON_10,
    LN_VBUTTON_JOYPAD_BUTTON_11,
    LN_VBUTTON_JOYPAD_BUTTON_12,
    LN_VBUTTON_JOYPAD_BUTTON_13,
    LN_VBUTTON_JOYPAD_BUTTON_14,
    LN_VBUTTON_JOYPAD_BUTTON_15,
    LN_VBUTTON_JOYPAD_BUTTON_16,

    LN_VBUTTON_JOYPAD_POV_UP,
    LN_VBUTTON_JOYPAD_POV_RIGHT,
    LN_VBUTTON_JOYPAD_POV_DOWN,
    LN_VBUTTON_JOYPAD_POV_LEFT,

    LN_VBUTTON_JOYPAD_AXIS_1_MINUIS,      ///< X1 -1.0
    LN_VBUTTON_JOYPAD_AXIS_1_PLUS,        ///< X1  1.0
    LN_VBUTTON_JOYPAD_AXIS_2_MINUIS,      ///< Y1 -1.0
    LN_VBUTTON_JOYPAD_AXIS_2_PLUS,        ///< Y1  1.0
    LN_VBUTTON_JOYPAD_AXIS_3_MINUIS,      ///< X2 -1.0
    LN_VBUTTON_JOYPAD_AXIS_3_PLUS,        ///< X2  1.0
    LN_VBUTTON_JOYPAD_AXIS_4_MINUIS,      ///< Y2 -1.0
    LN_VBUTTON_JOYPAD_AXIS_4_PLUS,        ///< Y2  1.0
    LN_VBUTTON_JOYPAD_AXIS_5_MINUIS,      ///< X3 -1.0
    LN_VBUTTON_JOYPAD_AXIS_5_PLUS,        ///< X3  1.0     XInput LT
    LN_VBUTTON_JOYPAD_AXIS_6_MINUIS,      ///< Y3 -1.0
    LN_VBUTTON_JOYPAD_AXIS_6_PLUS,        ///< Y3  1.0     XInput RT
    LN_VBUTTON_JOYPAD_AXIS_7_MINUIS,      ///< X4 -1.0
    LN_VBUTTON_JOYPAD_AXIS_7_PLUS,        ///< X4  1.0
    LN_VBUTTON_JOYPAD_AXIS_8_MINUIS,      ///< Y4 -1.0
    LN_VBUTTON_JOYPAD_AXIS_8_PLUS,        ///< Y4  1.0

    LN_VBUTTON_LAST,

    LN_VBUTTON_KEYBOARD_BEGIN      = 1,        // 1�`255 �͂Ƃ肠�����L�[�{�[�h�p
    LN_VBUTTON_KEYBOARD_END        = 255,
    LN_VBUTTON_MOUSE_BEGIN         = LN_VBUTTON_MOUSE_1,
    LN_VBUTTON_MOUSE_END           = LN_VBUTTON_MOUSE_8,
    LN_VBUTTON_JOYPAD_BUTTON_BEGIN = LN_VBUTTON_JOYPAD_BUTTON_1,
    LN_VBUTTON_JOYPAD_BUTTON_END   = LN_VBUTTON_JOYPAD_BUTTON_16,
    LN_VBUTTON_JOYPAD_POV_BEGIN    = LN_VBUTTON_JOYPAD_POV_UP,
    LN_VBUTTON_JOYPAD_POV_END      = LN_VBUTTON_JOYPAD_POV_LEFT,
    LN_VBUTTON_JOYPAD_AXIS_BEGIN   = LN_VBUTTON_JOYPAD_AXIS_1_MINUIS,
    LN_VBUTTON_JOYPAD_AXIS_END     = LN_VBUTTON_JOYPAD_AXIS_8_PLUS,
};

/// ���̓f�o�C�X�̗L���t���O
enum LNInputAttachFlags
{
	LN_INPUTATTACH_NONE		= 0x00,
	LN_INPUTATTACH_MOUSE	= 0x01,		///< �}�E�X������͂��󂯎��
    LN_INPUTATTACH_KEYBOARD	= 0x02,		///< �L�[�{�[�h������͂��󂯎��
	LN_INPUTATTACH_JOYPAD	= 0x04,		///< �W���C�p�b�h������͂��󂯎��

    LN_INPUTATTACH_ALL      = 0xff,     ///< ��L���ׂẴf�o�C�X������͂��󂯎��
};

/// �}�E�X�̏�Ԃ�\���\����
struct LNMouseDeviceState
{
	long	X;
	long	Y;
	long	RelX;						        ///< 1 �t���[���O�̈ʒu����̑��� X ���W
	long	RelY;
	long	Wheel;						        ///< �z�C�[��
	bool	Buttons[ LN_MAX_MOUSE_BUTTONS ];	///< �{�^�����������Ă���ꍇ�� true
	bool	Succeeded;					        ///< �f�[�^������ł��邱�Ƃ������t���O ( false �̏ꍇ�A���O�Ɏ擾�ł����l���i�[����Ă��� )
};

/// �W���C�p�b�h�� POV �̌�����\���t���O
enum LNPOVDirFlag
{
    POV_NONE    = 0x00,
    POV_UP      = 0x01,
    POV_RIGHT   = 0x02,
    POV_DOWN    = 0x04,
    POV_LEFT    = 0x08,
};

/// �W���C�p�b�h�̏�Ԃ�\���\����
struct LNJoypadDeviceState
{
	float		Axes[LN_MAX_JOYPAD_AXIS];
	lnU32		POV;						        ///< POV ��� (POVDir �̑g�ݍ��킹  GLFW�ł͎��Ȃ�)
	lnU8		Buttons[LN_MAX_JOYPAD_BUTTONS];
};

/// �^�b�`�̏�Ԃ�\���\����
struct LNTouchDeviceState
{
	long	X;
	long	Y;
	long	RelX;   ///< 1 �t���[���O�̈ʒu����̑��� X ���W
	long	RelY;
	bool	Touch;  ///< �����Ă���ꍇ�� true
};

} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================