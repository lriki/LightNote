﻿
#pragma once
#include "LFTypedef2.h"

#if 0

//==============================================================================
// LFTypedef 
//------------------------------------------------------------------------------
///**
//  @file       LFTypedef.h
//  @brief      LFTypedef
//*/
//==============================================================================

#pragma once

#include "LFCommon.h"

extern "C" {

//==============================================================================
// Common
//==============================================================================

/// 真偽値
typedef enum _LNBool
{
	LN_FALSE	= 0,	///< 偽
	LN_TRUE		= 1,	///< 真

} LNBool;

/// 結果・エラーコード
typedef enum _LNErrorCode 
{
    LN_OK                       = 0,	///< 成功

	LN_ERR_UNKNOWN				= -1,	///< 不明なエラー
	LN_ERR_INVALID_OPERATION	= -2,	///< オブジェクトの現在の状態に対して無効な呼び出しが行われた
	LN_ERR_ARGUMENT				= -3,	///< 渡された引数のいずれかが無効
	LN_ERR_OUT_OF_MEMORY		= -2,	///< メモリ不足
	LN_ERR_FILE_NOT_FOUND       = -3,	///< ファイルが存在しない、またはアクセスできない
	LN_ERR_SYSTEM_CALL			= -6,	///< システム API 呼び出しエラー
	LN_ERR_NOT_IMPLEMENTED		= -7,	///< 未実装の機能を呼び出そうとした
	LN_ERR_INVALID_FORMAT		= -8,	///< ファイル形式などの入力データのフォーマットが無効、またはサポートされていない
	LN_ERR_COMPILATION			= -9,	///< シェーダプログラム等のコンパイルエラーが発生した
	LN_ERR_THREAD				= -10,	///< スレッド実行中に例外が発生した
	LN_ERR_FATAL				= -11,	///< 継続不能なエラー
	LN_ERR_COM_CALL				= -12,	///< COM 呼び出しエラー
	LN_ERR_OPENGL				= -13,	///< OpenGL 関係のエラー
	LN_ERR_DIRECTX				= -14,
	LN_ERR_WIN32API				= -15,

} LNErrorCode, LNResult;

/// 例外発生時のコールバック関数
typedef void (*LNExceptionCallback)( LNErrorCode errCode );

//==============================================================================
// Base
//==============================================================================

/// サイズ (高さ・幅)
typedef struct _LNSize
{
	int		Width;		///< 幅
	int		Height;		///< 高さ

} LNSize;

/// 矩形
typedef struct _LNRect
{
	int		X;			///< 左上 X 座標
	int		Y;			///< 左上 Y 座標
	int		Width;		///< 幅
	int		Height;		///< 高さ

} LNRect;

//==============================================================================
// LFileIO
//==============================================================================

/// ファイルへのアクセス制限
typedef enum _LNFileAccessPriority
{
	LN_FILEACCESSPRIORITY_ARCHIVE_FIRST = 0,	///< アーカイブ優先
	LN_FILEACCESSPRIORITY_DIRECTORY_FIRST,		///< ディレクトリ優先
	LN_FILEACCESSPRIORITY_ARCHIVE_ONLY,			///< アーカイブのみ  

	LN_FILEACCESSPRIORITY_MAX,

} LNFileAccessPriority;

/// ファイルオープンモード
typedef enum _LNFileType
{
	LN_FILETYPE_BINARY = 0,		///< バイナリモードで開く
	LN_FILETYPE_TEXT,			///< テキストモードで開く

	LN_FILETYPE_MAX,

} LNFileType;

/// ファイルを開く方法または作成する方法
typedef enum _LNFileMode
{
	LN_FILEMODE_OPEN = 0,		///< 既存のファイルを開く
	LN_FILEMODE_CREATE,			///< 新しいファイルを作成する。既に存在する場合は上書きされる
	LN_FILEMODE_APPEND,			///< ファイルを開き、ファイルの末尾をシークする。存在しない場合は新しいファイルを作成する

	LN_FILEMODE_MAX,

} LNFileMode;

/// ファイルにアクセスする方法
typedef enum _LNFileAccess
{
	LN_FILEACCESS_READ = 0,		///< 読み取りアクセス
	LN_FILEACCESS_READWRITE,	///< 読み取り及び書き込みアクセス
	LN_FILEACCESS_WRITE,		///< 書き込みアクセス

	LN_FILEACCESS_MAX,

} LNFileAccess;

/// シーク位置移動の起点
typedef enum _LNSeekOrigin
{
	LN_SEEK_BEGIN = 0,				///< ファイルの先頭
	LN_SEEK_CURRENT,				///< 現在のシーク位置
	LN_SEEK_END,					///< ファイルの末尾

} LNSeekOrigin;

/// 非同期処理の状態
typedef enum _LNAsyncIOState
{
	LN_ASYNCIOSTATE_IDLE = 0,		///< 待機状態 (初期状態)
	LN_ASYNCIOSTATE_READY,			///< 実行可能状態 (実際の処理開始を待っている)
	LN_ASYNCIOSTATE_PROCESSING,		///< 実行状態 (処理中)
	LN_ASYNCIOSTATE_COMPLETED,		///< 終了状態 (成功)
	LN_ASYNCIOSTATE_FAILED,			///< 終了状態 (処理終了後・処理に失敗した)
	
	LN_ASYNCIOSTATE_MAX,

} LNAsyncIOState;

//==============================================================================
// Math
//==============================================================================

/// 2Dベクトル
typedef struct _LNVector2
{
	float	X;
	float	Y;

} LNVector2;

/// 3Dベクトル
typedef struct _LNVector3
{
	float	X;
	float	Y;
	float	Z;

} LNVector3;

/// 4Dベクトル
typedef struct _LNVector4
{
	float	X;
	float	Y;
	float	Z;
	float	W;

} LNVector4;

/// 4x4行列
typedef struct _LNMatrix
{
	float M11, M12, M13, M14;
    float M21, M22, M23, M24;
    float M31, M32, M33, M34;
    float M41, M42, M43, M44;

} LNMatrix;

/// クォータニオン
typedef struct _LNQuaternion
{
	float	X;
	float	Y;
	float	Z;
	float	W;

} LNQuaternion;

/// 回転順序
typedef enum _LNRotationOrder
{
	LN_ROTATIONORDER_XYZ = 0,		///< X → Y → Z
	LN_ROTATIONORDER_XZY,			///< X → Z → Y
	LN_ROTATIONORDER_YXZ,			///< Y → X → Z
	LN_ROTATIONORDER_YZX,			///< Y → Z → X
	LN_ROTATIONORDER_ZXY,			///< Z → X → Y
	LN_ROTATIONORDER_ZYX,			///< Z → Y → X

	LN_ROTATIONORDER_MAX,

} LNRotationOrder;

//==============================================================================
// System
//==============================================================================

/// キーボードのキーコード
typedef enum _LNKeyCode
{
	LN_KEY_UNKNOWN = 0,

    LN_KEY_A,
    LN_KEY_B,
    LN_KEY_C,
    LN_KEY_D,
    LN_KEY_E,
    LN_KEY_F,
    LN_KEY_G,
    LN_KEY_H,
    LN_KEY_I,
    LN_KEY_J,
    LN_KEY_K,
    LN_KEY_L,
    LN_KEY_M,
    LN_KEY_N,
    LN_KEY_O,
    LN_KEY_P,
    LN_KEY_Q,
    LN_KEY_R,
    LN_KEY_S,
    LN_KEY_T,
    LN_KEY_U,
    LN_KEY_V,
    LN_KEY_W,
    LN_KEY_X,
    LN_KEY_Y,
    LN_KEY_Z,

    LN_KEY_D0,		///< '0' (NumPad ではない)
    LN_KEY_D1,		///< '1' (NumPad ではない)
    LN_KEY_D2,		///< '2' (NumPad ではない)
    LN_KEY_D3,		///< '3' (NumPad ではない)
    LN_KEY_D4,		///< '4' (NumPad ではない)
    LN_KEY_D5,		///< '5' (NumPad ではない)
    LN_KEY_D6,		///< '6' (NumPad ではない)
    LN_KEY_D7,		///< '7' (NumPad ではない)
    LN_KEY_D8,		///< '8' (NumPad ではない)
    LN_KEY_D9,		///< '9' (NumPad ではない)

	LN_KEY_F1,
	LN_KEY_F2,
	LN_KEY_F3,
	LN_KEY_F4,
	LN_KEY_F5,
	LN_KEY_F6,
	LN_KEY_F7,
	LN_KEY_F8,
	LN_KEY_F9,
	LN_KEY_F10,
	LN_KEY_F11,
	LN_KEY_F12,

	LN_KEY_SPACE,
	LN_KEY_ESCAPE,
	LN_KEY_UP,
	LN_KEY_DOWN,
	LN_KEY_LEFT,
	LN_KEY_RIGHT,
	LN_KEY_LSHIFT,
	LN_KEY_RSHIFT,
	LN_KEY_LCTRL,
	LN_KEY_RCTRL,
	LN_KEY_LALT,
	LN_KEY_RALT,
	LN_KEY_TAB,
	LN_KEY_ENTER,
	LN_KEY_BACKSPACE,
	LN_KEY_INSERT,
	LN_KEY_DELETE,
	LN_KEY_PAGEUP,
	LN_KEY_PAGEDOWN,
	LN_KEY_HOME,
	LN_KEY_END,

    LN_KEY_COLON,			///< :	[非推奨 GLFWでは;]
    LN_KEY_SEMICOLON,		///< ;	[非推奨 GLFWでは=]
    LN_KEY_COMMA,			///< ,
    LN_KEY_PERIOD,			///< .
	LN_KEY_SLASH,			///< /
	LN_KEY_MINUS,			///< -
	LN_KEY_BACKSLASH,		///< \(バックスラッシュ) [非推奨 GLFWではNOWORD2]
	LN_KEY_YEN,				///< \(BaskSpaceキーの左の\キー) [非推奨 GLFWではI]
	LN_KEY_CARET,			///< ^ [非推奨 GLFWではGLFW_KEY_APOSTROPHE(')]
	LN_KEY_LBRACKET,		///< [
	LN_KEY_RBRACKET,		///< ]

    LN_KEY_ANDROID_PLUS,	///< Android '+'

    LN_KEY_MAX = 256,

} LNKeyCode;

/// アクセラレータキー
typedef enum _LNAcceleratorKey
{
	LN_ACCELERATORKEY_TOGGLE_FULLSCREEN = 0,	///< フルスクリーン切り替えを行うアクセラレータキー
	LN_ACCELERATORKEY_SHOW_DEBUG_STATISTICS,	///< デバッグ用の情報を表示切替を行うアクセラレータキー

	LN_ACCELERATORKEY_MAX,

} LNAcceleratorKey;

//==============================================================================
// LInput
//==============================================================================

/// 仮想ボタン
typedef enum _LNButton
{
	LN_BUTTON_0 = 0,				///< ボタン0 (キーボード='Z' ジョイパッド=1番ボタン)
	LN_BUTTON_1,					///< ボタン1 (キーボード='X' ジョイパッド=2)
	LN_BUTTON_2,					///< ボタン2 (キーボード='C' ジョイパッド=3)
	LN_BUTTON_3,					///< ボタン3 (キーボード='A' ジョイパッド=4)
	LN_BUTTON_4,					///< ボタン4 (キーボード='S' ジョイパッド=5)
	LN_BUTTON_5,					///< ボタン5 (キーボード='D' ジョイパッド=6)
	LN_BUTTON_6,					///< ボタン6 (キーボード='Q' ジョイパッド=7)
	LN_BUTTON_7,					///< ボタン7 (キーボード='W' ジョイパッド=8)
	LN_BUTTON_8,					///< ボタン8 (キーボード=None ジョイパッド=9)
	LN_BUTTON_9,					///< ボタン9 (キーボード=None ジョイパッド=10)
	LN_BUTTON_10,					///< ボタン10 (キーボード=None ジョイパッド=11)
	LN_BUTTON_11,					///< ボタン11 (キーボード=None ジョイパッド=12)
	LN_BUTTON_12,					///< ボタン12 (キーボード=None ジョイパッド=13)
	LN_BUTTON_13,					///< ボタン13 (キーボード=None ジョイパッド=14)
	LN_BUTTON_14,					///< ボタン14 (キーボード=None ジョイパッド=15)
	LN_BUTTON_15,					///< ボタン15 (キーボード=None ジョイパッド=16)
	LN_BUTTON_LEFT,					///< 左ボタン (キーボード=LN_KEY_LEFT  ジョイパッド=POV左&第1X軸-)
	LN_BUTTON_RIGHT,				///< 右ボタン (キーボード=LN_KEY_RIGHT ジョイパッド=POV右&第1X軸+)
	LN_BUTTON_UP,					///< 上ボタン (キーボード=LN_KEY_UP    ジョイパッド=POV上&第1Y軸-)
	LN_BUTTON_DOWN,					///< 下ボタン (キーボード=LN_KEY_DOWN  ジョイパッド=POV下&第1Y軸+)

	LN_BUTTON_AXIS_1X_MINUIS,		///< 第1レバー X- 方向
    LN_BUTTON_AXIS_1X_PLUS,			///< 第1レバー X+ 方向
    LN_BUTTON_AXIS_1Y_MINUIS,		///< 第1レバー Y- 方向
    LN_BUTTON_AXIS_1Y_PLUS,			///< 第1レバー Y+ 方向
    LN_BUTTON_AXIS_2X_MINUIS,		///< 第2レバー X- 方向
    LN_BUTTON_AXIS_2X_PLUS,			///< 第2レバー X+ 方向
    LN_BUTTON_AXIS_2Y_MINUIS,		///< 第2レバー Y- 方向
    LN_BUTTON_AXIS_2Y_PLUS,			///< 第2レバー Y+ 方向
	
	LN_BUTTON_MAX,

} LNButton;

/// デバイスボタン番号の分類
typedef enum _LNDeviceButtonType
{
	LN_DEVICEBUTTONTYPE_UNKNOWN = 0,			///< 不明なデバイス種類
	LN_DEVICEBUTTONTYPE_KEYBOARD,				///< キーボード
	LN_DEVICEBUTTONTYPE_MOUSE,					///< マウスボタン
	LN_DEVICEBUTTONTYPE_JOYPAD_BUTTON,			///< ジョイパッドのボタン
	LN_DEVICEBUTTONTYPE_JOYPAD_POV,				///< ジョイパッドのPOV
	LN_DEVICEBUTTONTYPE_JOYPAD_AXIS,			///< ジョイパッドの軸

	LN_DEVICEBUTTONTYPE_MAX,

} LNDeviceButtonType;

/// 全てのデバイス用の仮想ボタン番号 (デバイスボタン番号) (1 ～ 255 は LN_KEY_～と兼用)
typedef enum _LNDeviceButton
{
	LN_DEVICEBUTTON_UNKNOWN = 0,

	LN_DEVICEBUTTON_MOUSE_1 = 256,
	LN_DEVICEBUTTON_MOUSE_2,
	LN_DEVICEBUTTON_MOUSE_3,
	LN_DEVICEBUTTON_MOUSE_4,
	LN_DEVICEBUTTON_MOUSE_5,
	LN_DEVICEBUTTON_MOUSE_6,
	LN_DEVICEBUTTON_MOUSE_7,
	LN_DEVICEBUTTON_MOUSE_8,

	LN_DEVICEBUTTON_JOYPAD_BUTTON_1,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_2,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_3,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_4,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_5,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_6,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_7,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_8,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_9,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_10,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_11,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_12,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_13,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_14,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_15,
	LN_DEVICEBUTTON_JOYPAD_BUTTON_16,

	LN_DEVICEBUTTON_JOYPAD_POV_UP,
	LN_DEVICEBUTTON_JOYPAD_POV_RIGHT,
	LN_DEVICEBUTTON_JOYPAD_POV_DOWN,
	LN_DEVICEBUTTON_JOYPAD_POV_LEFT,

	LN_DEVICEBUTTON_JOYPAD_AXIS_1_MINUIS,      ///< X1 -1.0
	LN_DEVICEBUTTON_JOYPAD_AXIS_1_PLUS,        ///< X1  1.0
	LN_DEVICEBUTTON_JOYPAD_AXIS_2_MINUIS,      ///< Y1 -1.0
	LN_DEVICEBUTTON_JOYPAD_AXIS_2_PLUS,        ///< Y1  1.0
	LN_DEVICEBUTTON_JOYPAD_AXIS_3_MINUIS,      ///< X2 -1.0
	LN_DEVICEBUTTON_JOYPAD_AXIS_3_PLUS,        ///< X2  1.0
	LN_DEVICEBUTTON_JOYPAD_AXIS_4_MINUIS,      ///< Y2 -1.0
	LN_DEVICEBUTTON_JOYPAD_AXIS_4_PLUS,        ///< Y2  1.0
	LN_DEVICEBUTTON_JOYPAD_AXIS_5_MINUIS,      ///< X3 -1.0
	LN_DEVICEBUTTON_JOYPAD_AXIS_5_PLUS,        ///< X3  1.0     XInput LT
	LN_DEVICEBUTTON_JOYPAD_AXIS_6_MINUIS,      ///< Y3 -1.0
	LN_DEVICEBUTTON_JOYPAD_AXIS_6_PLUS,        ///< Y3  1.0     XInput RT
	LN_DEVICEBUTTON_JOYPAD_AXIS_7_MINUIS,      ///< X4 -1.0
	LN_DEVICEBUTTON_JOYPAD_AXIS_7_PLUS,        ///< X4  1.0
	LN_DEVICEBUTTON_JOYPAD_AXIS_8_MINUIS,      ///< Y4 -1.0
	LN_DEVICEBUTTON_JOYPAD_AXIS_8_PLUS,        ///< Y4  1.0

	LN_DEVICEBUTTON_MAX,
};

//==============================================================================
// LAudio
//==============================================================================

/// DirectMusic の初期化方法
typedef enum _LNDirectMusicInitMode
{
    LN_DMINITMODE_NOT_USE = 0,		///< DirectMusic を使用しない
	LN_DMINITMODE_NORMAL,		    ///< 通常
	LN_DMINITMODE_THREAD_WAIT,		///< 別スレッドで初期化して、再生時に未完了の場合は待つ
	LN_DMINITMODE_THREAD_REQUEST,	///< 別スレッドで初期化して、再生時に未完了の場合は再生を予約する

	LN_DMINITMODE_MAX,

} LNDirectMusicInitMode;

/// 再生方法 ( オーディオプレイヤーの種類 )
typedef enum _LNSoundPlayingType
{
	LN_SOUNDPLAYINGTYPE_UNKNOWN = 0,	///< 不明な再生方法
	LN_SOUNDPLAYINGTYPE_AUTO,			///< 自動選択 ( デフォルトではデコード後のサイズが 10000 バイト以上になる場合はストリーミング、そうでない場合はオンメモリ再生になります )
	LN_SOUNDPLAYINGTYPE_ONMEMORY,		///< オンメモリ
	LN_SOUNDPLAYINGTYPE_STREAMING,		///< ストリーミング
	LN_SOUNDPLAYINGTYPE_MIDI,			///< SMF

	LN_SOUNDPLAYINGTYPE_MAX,

} LNSoundPlayingType;

/// 音声の再生状態
typedef enum _LNSoundPlayingState
{
    LN_SOUNDPLAYINGSTATE_STOPPED = 0,	///< 停止中
    LN_SOUNDPLAYINGSTATE_PLAYING,		///< 再生中
    LN_SOUNDPLAYINGSTATE_PAUSING,		///< 一時停止中

    LN_SOUNDPLAYINGSTATE_MAX,

} LNSoundPlayingState;

/// 音量フェード完了時の動作の記述
typedef enum _LNSoundFadeState
{
    LN_SOUNDFADE_CONTINUE = 0,	    ///< 継続(なにもしない)
	LN_SOUNDFADE_STOP,		        ///< 停止する (stop())
	LN_SOUNDFADE_STOP_RESET,	    ///< 停止して、音量を元の値に戻す
	LN_SOUNDFADE_PAUSE,			    ///< 一時停止する (pause( true ))
	LN_SOUNDFADE_PAUSE_RESET,	    ///< 一時停止して、音量を元の値に戻す

	LN_SOUNDFADE_MAX,

} LNSoundFadeState;

/// GameAudio 内部 Sound
typedef enum _LNInternalGameSound 
{
	LN_INTERNALGAMESOUND_BGM = 0,	///< BGM
	LN_INTERNALGAMESOUND_BGS,		///< BGS
	LN_INTERNALGAMESOUND_ME,		///< ME

	LN_INTERNALGAMESOUND_MAX,

} LNInternalGameSound;

//==============================================================================
// LGraphics
//==============================================================================

/// 色
typedef struct _LNColor
{
	uint8_t	R;		///< 赤成分 (0～255)
	uint8_t	G;		///< 緑成分 (0～255)
	uint8_t	B;		///< 青成分 (0～255)
	uint8_t	A;		///< アルファ成分 (0～255)

} LNColor;

/// 色 (実数表現)
typedef struct _LNColorF
{
	float	R;		///< 赤成分 (0.0～1.0)
	float	G;		///< 緑成分 (0.0～1.0)
	float	B;		///< 青成分 (0.0～1.0)
	float	A;		///< アルファ成分 (0.0～1.0)

} LNColorF;

/// 色調
typedef struct _LNTone
{
	float	R;		///< 赤成分 (-1.0～1.0)
	float	G;		///< 緑成分 (-1.0～1.0)
	float	B;		///< 青成分 (-1.0～1.0)
	float	Mono;	///< モノクロ成分 (0.0～1.0)

} LNTone;

/// ウィンドウとバックバッファのリサイズモード
typedef enum _LNBackbufferResizeMode
{
	LN_BACKBUFFERRESIZEMODE_SCALING = 0,				///< バックバッファは初期化時のサイズのまま、ウィンドウサイズに合わせて拡縮を行う
	LN_BACKBUFFERRESIZEMODE_SCALING_WITH_LETTER_BOX,	///< バックバッファは初期化時のサイズのまま、レターボックスを表示してアスペクト比を保ち拡縮を行う
	LN_BACKBUFFERRESIZEMODE_RESIZE,						///< バックバッファをウィンドウサイズに合わせてリサイズする

	LN_BACKBUFFERRESIZEMODE_MAX,

} LNBackbufferResizeMode;

/// テクスチャフォーマット
typedef enum _LNTextureFormat
{
    LN_FMT_UNKNOWN = 0,			///< 不明なフォーマット
    LN_FMT_A8R8G8B8,            ///< 32 ビットのアルファ付きフォーマット
    LN_FMT_X8R8G8B8,            ///< 32 ビットのアルファ無しフォーマット
    LN_FMT_A16B16G16R16F,       ///< 64 ビットの浮動小数点フォーマット
    LN_FMT_A32B32G32R32F,       ///< 128 ビットの浮動小数点フォーマット
    LN_FMT_D24S8,               ///< 32 ビットの Z バッファフォーマット
	
	LN_FMT_MAX,

} LNTextureFormat;

/// 合成方法
typedef enum _LNBlendMode
{
    LN_BLEND_NORMAL = 0,        ///< 通常 (アルファ無視)
    LN_BLEND_ALPHA,             ///< アルファブレンド (デフォルト)
    LN_BLEND_ADD,               ///< 加算
    LN_BLEND_ADD_ALPHA_DISABLE,	///< 加算（アルファチャンネル無視）
    LN_BLEND_SUB,				///< 減算
    LN_BLEND_SUB_ALPHA_DISABLE,	///< 減算（アルファチャンネル無視）
    LN_BLEND_MUL,				///< 乗算（アルファチャンネル無視）
    LN_BLEND_SCREEN,			///< スクリーン（アルファチャンネル無視）
    LN_BLEND_REVERSE,           ///< リバース

	LN_BLEND_MAX,

} LNBlendMode;

/// カリング方法
typedef enum _LNCullingMode
{
    LN_CULL_FRONT = 0,			///< 前面表示
	LN_CULL_BACK,				///< 裏面表示
	LN_CULL_DOUBLE,				///< 両面表示

	LN_CULL_MAX,

} LNCullingMode;

/// テキストアライン
typedef enum _LNTextAlign
{
    LN_TEXTALIGN_LEFT = 0,		///< 左揃え
    LN_TEXTALIGN_CENTER,		///< 中央揃え
    LN_TEXTALIGN_RIGHT,			///< 右揃え

	LN_TEXTALIGN_MAX,

} LNTextAlign;

/// ノードの配置方法
typedef enum _LNPlacementType
{
	LN_PLACEMENTTYPE_NORMAL = 0,	///< 通常の配置方法
	LN_PLACEMENTTYPE_BILLBOARD,		///< 常にカメラの方向を向くビルボード (回転は Z 軸のみ有効)
	LN_PLACEMENTTYPE_Y_FIXED,		///< Y 軸固定

	LN_PLACEMENTTYPE_MAX,

} LNPlacementType;

/// 3D 空間での基準方向を表す値
typedef enum _LNFrontAxis
{
    LN_FRONTAXIS_X = 0,   ///< X+ 方向 (右向き)
	LN_FRONTAXIS_Y,       ///< Y+ 方向 (上向き)
	LN_FRONTAXIS_Z,       ///< Z+ 方向 (奥向き)
    LN_FRONTAXIS_RX,      ///< X- 方向 (左向き)
	LN_FRONTAXIS_RY,      ///< Y- 方向 (下向き)
	LN_FRONTAXIS_RZ,      ///< Z- 方向 (手前向き)

	LN_FRONTAXIS_MAX,

} LNFrontAxis;
 
/// メッシュ生成オプション
typedef enum _LNMeshCreateFlags
{
	LN_MESHCREATEFLAGS_SOFTWARE_SKINNING		= 0x01,		///< ソフトウェアスキニング用のモデル (動的な頂点、インデックスバッファを作成)
	LN_MESHCREATEFLAGS_COLLISION_MESH			= 0x02,		///< 物理演算のメッシュボディとして扱うメッシュを生成する
	LN_MESHCREATEFLAGS_IGNORE_TEXTURE_NOT_FOUND	= 0x04,		///< マテリアルに含まれるテクスチャファイルが見つからなくても無視する

	LN_MESHCREATEFLAGS_MAX,

} LNMeshCreateFlags;

/// カメラ姿勢の設定方法
typedef enum _LNCameraType
{
	LN_CAMERATYPE_POSITION_AND_LOOKAT = 0,	///< 位置と注視点から算出する (3Dカメラのデフォルト。2Dカメラでは無効)
	LN_CAMERATYPE_POSITION_AND_ANGLE,		///< 位置と各軸の回転角度から算出する (2D カメラのデフォルト)
	LN_CAMERATYPE_TRANSFORM_MATRIX,			///< 行列を使用し、正面(0,0,1) と 上方向(0,1,0) を変換する

	LN_CAMERATYPE_MAX,

} LNCameraType;

//==============================================================================
// LScene
//==============================================================================

/// 座標系
typedef enum _LNCoord
{
	LN_COORD_SPACE_2D = 0,			///< 2D 空間
	LN_COORD_SPACE_3D,				///< 3D 空間

	LN_COORD_MAX,

} LNCoord;

//==============================================================================
// 
//==============================================================================

/// ハンドルオブジェクトの種類
typedef enum _LNObjectType
{
	LN_OBJECTTYPE_SOUND,

	LN_OBJECTTYPE_MAX,

} LNObjectType;

} // extern "C" {

#endif
