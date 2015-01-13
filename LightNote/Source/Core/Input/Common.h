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

static const int LN_MAX_JOYPADS = 4;            ///< ジョイパッドの最大数
static const int LN_MAX_JOYPAD_BUTTONS = 16;    ///< ジョイパッドのボタンの最大数
static const int LN_MAX_JOYPAD_AXIS = 8;        ///< ジョイパッドの軸の最大数
static const int LN_MAX_GAME_CONTROLLERS = 4;   ///< 仮想コントローラの最大数
#define          LN_JOYPAD_PLAY_RANGE   (0.2f)  ///< ジョイパッドのあそび範囲
#define          LN_JOYPAD_SWITCH_RANGE (0.8f)

/// マウスのボタン番号
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

/// ゲームボタンを表す値 (()内はデフォルトで割り当てらているキー、ボタン、軸)
enum LNGameControllerButton
{
	LN_BUTTON_A = 0,        ///< Aボタン (キーボード='Z' ジョイパッド=1番ボタン)
	LN_BUTTON_B,            ///< Bボタン (キーボード='X' ジョイパッド=2)
	LN_BUTTON_C,            ///< Cボタン (キーボード='C' ジョイパッド=3)
	LN_BUTTON_X,            ///< Xボタン (キーボード='A' ジョイパッド=4)
	LN_BUTTON_Y,            ///< Yボタン (キーボード='S' ジョイパッド=5)
	LN_BUTTON_Z,            ///< Zボタン (キーボード='D' ジョイパッド=6)
	LN_BUTTON_L,            ///< Lボタン (キーボード='Q' ジョイパッド=7)
	LN_BUTTON_R,            ///< Rボタン (キーボード='W' ジョイパッド=8)
	LN_BUTTON_L2,           ///< L2ボタン (キーボード=None ジョイパッド=9)
	LN_BUTTON_R2,           ///< R2ボタン (キーボード=None ジョイパッド=10)
	LN_BUTTON_L3,           ///< L3ボタン (キーボード=None ジョイパッド=11)
	LN_BUTTON_R3,           ///< R3ボタン (キーボード=None ジョイパッド=12)
	LN_BUTTON_START,        ///< Startボタン (キーボード=None ジョイパッド=13)
	LN_BUTTON_SELECT,       ///< Selectボタン (キーボード=None ジョイパッド=14)
	LN_BUTTON_SUB1,         ///< Sub1ボタン (キーボード=None ジョイパッド=15)
	LN_BUTTON_SUB2,         ///< Sub2ボタン (キーボード=None ジョイパッド=16)
	LN_BUTTON_LEFT,         ///< 左ボタン (キーボード=LN_KEY_LEFT  ジョイパッド=POV左&第1X軸-)
	LN_BUTTON_RIGHT,        ///< 右ボタン (キーボード=LN_KEY_RIGHT ジョイパッド=POV右&第1X軸+)
	LN_BUTTON_UP,           ///< 上ボタン (キーボード=LN_KEY_UP    ジョイパッド=POV上&第1Y軸-)
	LN_BUTTON_DOWN,         ///< 下ボタン (キーボード=LN_KEY_DOWN  ジョイパッド=POV下&第1Y軸+)

	LN_BUTTON_AXIS_1X_MINUIS,   ///< 第1レバー X- 方向
    LN_BUTTON_AXIS_1X_PLUS,     ///< 第1レバー X+ 方向
    LN_BUTTON_AXIS_1Y_MINUIS,   ///< 第1レバー Y- 方向
    LN_BUTTON_AXIS_1Y_PLUS,     ///< 第1レバー Y+ 方向
    LN_BUTTON_AXIS_2X_MINUIS,   ///< 第2レバー X- 方向
    LN_BUTTON_AXIS_2X_PLUS,     ///< 第2レバー X+ 方向
    LN_BUTTON_AXIS_2Y_MINUIS,   ///< 第2レバー Y- 方向
    LN_BUTTON_AXIS_2Y_PLUS,     ///< 第2レバー Y+ 方向
	
	LN_MAX_BUTTONS,

    LN_BUTTON_BEGIN = LN_BUTTON_A,
    LN_BUTTON_END   = LN_BUTTON_DOWN,
    LN_BUTTON_AXIS_BEGIN   = LN_BUTTON_AXIS_1X_MINUIS,
    LN_BUTTON_AXIS_END     = LN_BUTTON_AXIS_2Y_PLUS,
};

/// コントローラのアタッチ (キーコンフィグ) ターゲット
// TODO: LNGameControllerButton と同じになった。いらない
enum LNGameControllerAttachTarget
{
	LN_AT_BUTTON_A = 0,     ///< Aボタン
	LN_AT_BUTTON_B,         ///< Bボタン
	LN_AT_BUTTON_C,         ///< Cボタン
	LN_AT_BUTTON_X,         ///< Xボタン
	LN_AT_BUTTON_Y,         ///< Yボタン
	LN_AT_BUTTON_Z,         ///< Zボタン
	LN_AT_BUTTON_L,         ///< Lボタン
	LN_AT_BUTTON_R,         ///< Rボタン
	LN_AT_BUTTON_L2,        ///< L2ボタン
	LN_AT_BUTTON_R2,        ///< R2ボタン
	LN_AT_BUTTON_L3,        ///< L3ボタン
	LN_AT_BUTTON_R3,        ///< R3ボタン
	LN_AT_BUTTON_START,     ///< R3ボタン
	LN_AT_BUTTON_SELECT,    ///< R3ボタン
	LN_AT_BUTTON_SUB1,      ///< R3ボタン
	LN_AT_BUTTON_SUB2,      ///< R3ボタン
	LN_AT_BUTTON_LEFT,      ///< 左ボタン
	LN_AT_BUTTON_RIGHT,     ///< 右ボタン
	LN_AT_BUTTON_UP,        ///< 上ボタン
	LN_AT_BUTTON_DOWN,      ///< 下ボタン

    LN_AT_AXIS_1X_MINUIS,   ///< 第1レバー X- 方向
    LN_AT_AXIS_1X_PLUS,     ///< 第1レバー X+ 方向
    LN_AT_AXIS_1Y_MINUIS,   ///< 第1レバー Y- 方向
    LN_AT_AXIS_1Y_PLUS,     ///< 第1レバー Y+ 方向
    LN_AT_AXIS_2X_MINUIS,   ///< 第2レバー X- 方向
    LN_AT_AXIS_2X_PLUS,     ///< 第2レバー X+ 方向
    LN_AT_AXIS_2Y_MINUIS,   ///< 第2レバー Y- 方向
    LN_AT_AXIS_2Y_PLUS,     ///< 第2レバー Y+ 方向


	LN_MAX_AT_TARGETS,

    LN_AT_BUTTON_BEGIN = LN_AT_BUTTON_A,
    LN_AT_BUTTON_END   = LN_AT_BUTTON_DOWN,
    LN_AT_AXIS_BEGIN   = LN_AT_AXIS_1X_MINUIS,
    LN_AT_AXIS_END     = LN_AT_AXIS_2Y_PLUS,

};

/// 全てのデバイス用の仮想ボタン番号 (キーコンフィグで使う  例)attachButton( LN_BUTTON_L2, LN_VBUTTON_JOYPAD_AXIS_3_PLUS ))
enum LNVirtualButton
{
    LN_VBUTTON_NULL = 0,

    LN_VBUTTON_MOUSE_1 = 256,		// GLFW と合わせて 1 スタート
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

    LN_VBUTTON_KEYBOARD_BEGIN      = 1,        // 1～255 はとりあえずキーボード用
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

/// 入力デバイスの有効フラグ
enum LNInputAttachFlags
{
	LN_INPUTATTACH_NONE		= 0x00,
	LN_INPUTATTACH_MOUSE	= 0x01,		///< マウスから入力を受け取る
    LN_INPUTATTACH_KEYBOARD	= 0x02,		///< キーボードから入力を受け取る
	LN_INPUTATTACH_JOYPAD	= 0x04,		///< ジョイパッドから入力を受け取る

    LN_INPUTATTACH_ALL      = 0xff,     ///< 上記すべてのデバイスから入力を受け取る
};

/// マウスの状態を表す構造体
struct LNMouseDeviceState
{
	long	X;
	long	Y;
	long	RelX;						        ///< 1 フレーム前の位置からの相対 X 座標
	long	RelY;
	long	Wheel;						        ///< ホイール
	bool	Buttons[ LN_MAX_MOUSE_BUTTONS ];	///< ボタンを押下している場合は true
	bool	Succeeded;					        ///< データが正常であることを示すフラグ ( false の場合、直前に取得できた値が格納されている )
};

/// ジョイパッドの POV の向きを表すフラグ
enum LNPOVDirFlag
{
    POV_NONE    = 0x00,
    POV_UP      = 0x01,
    POV_RIGHT   = 0x02,
    POV_DOWN    = 0x04,
    POV_LEFT    = 0x08,
};

/// ジョイパッドの状態を表す構造体
struct LNJoypadDeviceState
{
	float		Axes[LN_MAX_JOYPAD_AXIS];
	lnU32		POV;						        ///< POV 状態 (POVDir の組み合わせ  GLFWでは取れない)
	lnU8		Buttons[LN_MAX_JOYPAD_BUTTONS];
};

/// タッチの状態を表す構造体
struct LNTouchDeviceState
{
	long	X;
	long	Y;
	long	RelX;   ///< 1 フレーム前の位置からの相対 X 座標
	long	RelY;
	bool	Touch;  ///< 押している場合は true
};

} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================