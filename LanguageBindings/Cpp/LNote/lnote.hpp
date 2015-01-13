#pragma once
#include <vector>
#include "lnote.h"

#ifndef LNOTE_NEW
    #define LNOTE_NEW new
#endif

namespace LN
{
class ReferenceObject;
struct Size;
struct Rect;
class Exception;
class IntTable;
struct Vector2;
struct Vector3;
struct Vector4;
struct Matrix;
struct Quaternion;
class Random;
class FileStream;
class Input;
class VirtualPad;
class Audio;
class SoundListener;
class Sound;
struct Color;
struct ColorF;
struct Tone;
class Texture;
class Font;
class SceneGraph;
class SceneNode;
class VisualNode;
class Sprite;
class SpritePath;
class Model;
class Effect;
class Tilemap;
class Shader;
class ViewPane;
class Layer;
class ViewFilter;
class ToneFilter;
class TransitionFilter;
class BlurFilter;
class Camera;
class Config;
class Core;


enum ErrorCode
{
    /// 成功
    ErrorCode_OK,

    /// 不明なエラー
    ErrorCode_Unknown,

    /// オブジェクトの現在の状態に対して無効な呼び出しが行われた
    ErrorCode_InvalidOperation,

    /// 渡された引数のいずれかが無効
    ErrorCode_Argument,

    /// メモリ不足
    ErrorCode_OutOfMemory,

    /// ファイルが存在しない、またはアクセスできない
    ErrorCode_FileNotFound,

    /// システム API 呼び出しエラー
    ErrorCode_SystemCall,

    /// 未実装の機能を呼び出そうとした
    ErrorCode_NotImplemented,

    /// ファイル形式などの入力データのフォーマットが無効、またはサポートされていない
    ErrorCode_InvalidFormat,

    /// スレッド実行中に例外が発生した
    ErrorCode_Thread,

    /// 継続不能なエラー
    ErrorCode_Fatal,

    /// COM 呼び出しエラー
    ErrorCode_ComCall,

    /// OpenGL 関係のエラー
    ErrorCode_Opengl,

    /// 
    ErrorCode_Directx,

    /// 
    ErrorCode_Win32api,

};
enum FileAccessPriority
{
    /// アーカイブ優先
    FileAccessPriority_ArchiveFirst,

    /// ディレクトリ優先
    FileAccessPriority_DirectoryFirst,

    /// アーカイブのみ
    FileAccessPriority_ArchiveOnly,

    /// 
    FileAccessPriority_Max,

};
enum FileType
{
    /// バイナリモードで開く
    FileType_Binary,

    /// テキストモードで開く
    FileType_Text,

    /// 
    FileType_Max,

};
enum FileMode
{
    /// 既存のファイルを開く
    FileMode_Open,

    /// 新しいファイルを作成する。既に存在する場合は上書きされる
    FileMode_Create,

    /// ファイルを開き、ファイルの末尾をシークする。存在しない場合は新しいファイルを作成する
    FileMode_Append,

    /// 
    FileMode_Max,

};
enum FileAccess
{
    /// 読み取りアクセス
    FileAccess_Read,

    /// 読み取り及び書き込みアクセス
    FileAccess_Readwrite,

    /// 書き込みアクセス
    FileAccess_Write,

    /// 
    FileAccess_Max,

};
enum SeekOrigin
{
    /// ファイルの先頭
    SeekOrigin_Begin,

    /// 現在のシーク位置
    SeekOrigin_Current,

    /// ファイルの末尾
    SeekOrigin_End,

};
enum AsyncIOState
{
    /// 待機状態 (初期状態)
    AsyncIOState_Idle,

    /// 実行可能状態 (実際の処理開始を待っている)
    AsyncIOState_Ready,

    /// 実行状態 (処理中)
    AsyncIOState_Processing,

    /// 終了状態 (成功)
    AsyncIOState_Completed,

    /// 終了状態 (処理終了後・処理に失敗した)
    AsyncIOState_Failed,

    /// 
    AsyncIOState_Max,

};
enum RotationOrder
{
    /// X → Y → Z
    RotationOrder_XYZ,

    /// X → Z → Y
    RotationOrder_XZY,

    /// Y → X → Z
    RotationOrder_YXZ,

    /// Y → Z → X
    RotationOrder_YZX,

    /// Z → X → Y
    RotationOrder_ZXY,

    /// Z → Y → X
    RotationOrder_ZYX,

    /// 
    RotationOrder_Max,

};
enum KeyCode
{
    /// 
    KeyCode_Unknown,

    /// 
    KeyCode_A,

    /// 
    KeyCode_B,

    /// 
    KeyCode_C,

    /// 
    KeyCode_D,

    /// 
    KeyCode_E,

    /// 
    KeyCode_F,

    /// 
    KeyCode_G,

    /// 
    KeyCode_H,

    /// 
    KeyCode_I,

    /// 
    KeyCode_J,

    /// 
    KeyCode_K,

    /// 
    KeyCode_L,

    /// 
    KeyCode_M,

    /// 
    KeyCode_N,

    /// 
    KeyCode_O,

    /// 
    KeyCode_P,

    /// 
    KeyCode_Q,

    /// 
    KeyCode_R,

    /// 
    KeyCode_S,

    /// 
    KeyCode_T,

    /// 
    KeyCode_U,

    /// 
    KeyCode_V,

    /// 
    KeyCode_W,

    /// 
    KeyCode_X,

    /// 
    KeyCode_Y,

    /// 
    KeyCode_Z,

    /// '0' (NumPad ではない)
    KeyCode_D0,

    /// '1' (NumPad ではない)
    KeyCode_D1,

    /// '2' (NumPad ではない)
    KeyCode_D2,

    /// '3' (NumPad ではない)
    KeyCode_D3,

    /// '4' (NumPad ではない)
    KeyCode_D4,

    /// '5' (NumPad ではない)
    KeyCode_D5,

    /// '6' (NumPad ではない)
    KeyCode_D6,

    /// '7' (NumPad ではない)
    KeyCode_D7,

    /// '8' (NumPad ではない)
    KeyCode_D8,

    /// '9' (NumPad ではない)
    KeyCode_D9,

    /// 
    KeyCode_F1,

    /// 
    KeyCode_F2,

    /// 
    KeyCode_F3,

    /// 
    KeyCode_F4,

    /// 
    KeyCode_F5,

    /// 
    KeyCode_F6,

    /// 
    KeyCode_F7,

    /// 
    KeyCode_F8,

    /// 
    KeyCode_F9,

    /// 
    KeyCode_F10,

    /// 
    KeyCode_F11,

    /// 
    KeyCode_F12,

    /// 
    KeyCode_Space,

    /// 
    KeyCode_Escape,

    /// 
    KeyCode_Up,

    /// 
    KeyCode_Down,

    /// 
    KeyCode_Left,

    /// 
    KeyCode_Right,

    /// 
    KeyCode_Lshift,

    /// 
    KeyCode_Rshift,

    /// 
    KeyCode_Lctrl,

    /// 
    KeyCode_Rctrl,

    /// 
    KeyCode_Lalt,

    /// 
    KeyCode_Ralt,

    /// 
    KeyCode_Tab,

    /// 
    KeyCode_Enter,

    /// 
    KeyCode_Backspace,

    /// 
    KeyCode_Insert,

    /// 
    KeyCode_Delete,

    /// 
    KeyCode_Pageup,

    /// 
    KeyCode_Pagedown,

    /// 
    KeyCode_Home,

    /// 
    KeyCode_End,

    /// :	[非推奨 GLFWでは;]
    KeyCode_Colon,

    /// ;	[非推奨 GLFWでは=]
    KeyCode_Semicolon,

    /// ,
    KeyCode_Comma,

    /// .
    KeyCode_Period,

    /// /
    KeyCode_Slash,

    /// -
    KeyCode_Minus,

    /// \(バックスラッシュ) [非推奨 GLFWではNOWORD2]
    KeyCode_Backslash,

    /// \(BaskSpaceキーの左の\キー) [非推奨 GLFWではI]
    KeyCode_Yen,

    /// ^ [非推奨 GLFWではGLFW_KEY_APOSTROPHE(')]
    KeyCode_Caret,

    /// [
    KeyCode_Lbracket,

    /// ]
    KeyCode_Rbracket,

    /// Android '+'
    KeyCode_AndroidPlus,

    /// 
    KeyCode_Max,

};
enum AcceleratorKey
{
    /// フルスクリーン切り替えを行うアクセラレータキー
    AcceleratorKey_ToggleFullscreen,

    /// デバッグ用の情報を表示切替を行うアクセラレータキー
    AcceleratorKey_ShowDebugStatistics,

    /// 
    AcceleratorKey_Max,

};
enum Button
{
    /// Aボタン (キーボード='Z' ジョイパッド=1番ボタン)
    Button_A,

    /// Bボタン (キーボード='X' ジョイパッド=2)
    Button_B,

    /// Cボタン (キーボード='C' ジョイパッド=3)
    Button_C,

    /// Xボタン (キーボード='A' ジョイパッド=4)
    Button_X,

    /// Yボタン (キーボード='S' ジョイパッド=5)
    Button_Y,

    /// Zボタン (キーボード='D' ジョイパッド=6)
    Button_Z,

    /// Lボタン (キーボード='Q' ジョイパッド=7)
    Button_L,

    /// Rボタン (キーボード='W' ジョイパッド=8)
    Button_R,

    /// L2ボタン (キーボード=None ジョイパッド=9)
    Button_L2,

    /// R2ボタン (キーボード=None ジョイパッド=10)
    Button_R2,

    /// L3ボタン (キーボード=None ジョイパッド=11)
    Button_L3,

    /// R3ボタン (キーボード=None ジョイパッド=12)
    Button_R3,

    /// Startボタン (キーボード=None ジョイパッド=13)
    Button_Start,

    /// Selectボタン (キーボード=None ジョイパッド=14)
    Button_Select,

    /// Sub1ボタン (キーボード=None ジョイパッド=15)
    Button_Sub1,

    /// Sub2ボタン (キーボード=None ジョイパッド=16)
    Button_Sub2,

    /// 左ボタン (キーボード=LN_KEY_LEFT  ジョイパッド=POV左&第1X軸-)
    Button_Left,

    /// 右ボタン (キーボード=LN_KEY_RIGHT ジョイパッド=POV右&第1X軸+)
    Button_Right,

    /// 上ボタン (キーボード=LN_KEY_UP    ジョイパッド=POV上&第1Y軸-)
    Button_Up,

    /// 下ボタン (キーボード=LN_KEY_DOWN  ジョイパッド=POV下&第1Y軸+)
    Button_Down,

    /// 第1レバー X- 方向
    Button_Axis1XMinuis,

    /// 第1レバー X+ 方向
    Button_Axis1XPlus,

    /// 第1レバー Y- 方向
    Button_Axis1YMinuis,

    /// 第1レバー Y+ 方向
    Button_Axis1YPlus,

    /// 第2レバー X- 方向
    Button_Axis2XMinuis,

    /// 第2レバー X+ 方向
    Button_Axis2XPlus,

    /// 第2レバー Y- 方向
    Button_Axis2YMinuis,

    /// 第2レバー Y+ 方向
    Button_Axis2YPlus,

    /// 
    Button_Max,

};
enum DeviceButtonType
{
    /// 不明なデバイス種類
    DeviceButtonType_Unknown,

    /// キーボード
    DeviceButtonType_Keyboard,

    /// マウスボタン
    DeviceButtonType_Mouse,

    /// ジョイパッドのボタン
    DeviceButtonType_JoypadButton,

    /// ジョイパッドのPOV
    DeviceButtonType_JoypadPov,

    /// ジョイパッドの軸
    DeviceButtonType_JoypadAxis,

    /// 
    DeviceButtonType_Max,

};
enum DeviceButton
{
    /// 
    DeviceButton_Unknown,

    /// 
    DeviceButton_Mouse1,

    /// 
    DeviceButton_Mouse2,

    /// 
    DeviceButton_Mouse3,

    /// 
    DeviceButton_Mouse4,

    /// 
    DeviceButton_Mouse5,

    /// 
    DeviceButton_Mouse6,

    /// 
    DeviceButton_Mouse7,

    /// 
    DeviceButton_Mouse8,

    /// 
    DeviceButton_JoypadButton1,

    /// 
    DeviceButton_JoypadButton2,

    /// 
    DeviceButton_JoypadButton3,

    /// 
    DeviceButton_JoypadButton4,

    /// 
    DeviceButton_JoypadButton5,

    /// 
    DeviceButton_JoypadButton6,

    /// 
    DeviceButton_JoypadButton7,

    /// 
    DeviceButton_JoypadButton8,

    /// 
    DeviceButton_JoypadButton9,

    /// 
    DeviceButton_JoypadButton10,

    /// 
    DeviceButton_JoypadButton11,

    /// 
    DeviceButton_JoypadButton12,

    /// 
    DeviceButton_JoypadButton13,

    /// 
    DeviceButton_JoypadButton14,

    /// 
    DeviceButton_JoypadButton15,

    /// 
    DeviceButton_JoypadButton16,

    /// 
    DeviceButton_JoypadPovUp,

    /// 
    DeviceButton_JoypadPovRight,

    /// 
    DeviceButton_JoypadPovDown,

    /// 
    DeviceButton_JoypadPovLeft,

    /// X1 -1.0
    DeviceButton_JoypadAxis1Minuis,

    /// X1  1.0
    DeviceButton_JoypadAxis1Plus,

    /// Y1 -1.0
    DeviceButton_JoypadAxis2Minuis,

    /// Y1  1.0
    DeviceButton_JoypadAxis2Plus,

    /// X2 -1.0
    DeviceButton_JoypadAxis3Minuis,

    /// X2  1.0
    DeviceButton_JoypadAxis3Plus,

    /// Y2 -1.0
    DeviceButton_JoypadAxis4Minuis,

    /// Y2  1.0
    DeviceButton_JoypadAxis4Plus,

    /// X3 -1.0
    DeviceButton_JoypadAxis5Minuis,

    /// X3  1.0     XInput LT
    DeviceButton_JoypadAxis5Plus,

    /// Y3 -1.0
    DeviceButton_JoypadAxis6Minuis,

    /// Y3  1.0     XInput RT
    DeviceButton_JoypadAxis6Plus,

    /// X4 -1.0
    DeviceButton_JoypadAxis7Minuis,

    /// X4  1.0
    DeviceButton_JoypadAxis7Plus,

    /// Y4 -1.0
    DeviceButton_JoypadAxis8Minuis,

    /// Y4  1.0
    DeviceButton_JoypadAxis8Plus,

    /// 
    DeviceButton_Max,

};
enum DirectMusicInitMode
{
    /// DirectMusic を使用しない
    DirectMusicInitMode_NotUse,

    /// 通常
    DirectMusicInitMode_Normal,

    /// 別スレッドで初期化して、再生時に未完了の場合は待つ
    DirectMusicInitMode_ThreadWait,

    /// 別スレッドで初期化して、再生時に未完了の場合は再生を予約する
    DirectMusicInitMode_ThreadRequest,

    /// 
    DirectMusicInitMode_Max,

};
enum SoundPlayingType
{
    /// 不明な再生方法
    SoundPlayingType_Unknown,

    /// 自動選択 ( デフォルトではデコード後のサイズが 10000 バイト以上になる場合はストリーミング、そうでない場合はオンメモリ再生になります )
    SoundPlayingType_Auto,

    /// オンメモリ
    SoundPlayingType_Onmemory,

    /// ストリーミング
    SoundPlayingType_Streaming,

    /// SMF
    SoundPlayingType_Midi,

    /// 
    SoundPlayingType_Max,

};
enum SoundPlayingState
{
    /// 停止中
    SoundPlayingState_Stopped,

    /// 再生中
    SoundPlayingState_Playing,

    /// 一時停止中
    SoundPlayingState_Pausing,

    /// 
    SoundPlayingState_Max,

};
enum SoundFadeState
{
    /// 継続(なにもしない)
    SoundFadeState_Continue,

    /// 停止する (stop())
    SoundFadeState_Stop,

    /// 停止して、音量を元の値に戻す
    SoundFadeState_StopReset,

    /// 一時停止する (pause( true ))
    SoundFadeState_Pause,

    /// 一時停止して、音量を元の値に戻す
    SoundFadeState_PauseReset,

    /// 
    SoundFadeState_Max,

};
enum InternalGameSound
{
    /// BGM
    InternalGameSound_BGM,

    /// BGS
    InternalGameSound_BGS,

    /// ME
    InternalGameSound_ME,

    /// 
    InternalGameSound_Max,

};
enum BackbufferResizeMode
{
    /// バックバッファは初期化時のサイズのまま、ウィンドウサイズに合わせて拡縮を行う
    BackbufferResizeMode_Scaling,

    /// バックバッファは初期化時のサイズのまま、レターボックスを表示してアスペクト比を保ち拡縮を行う
    BackbufferResizeMode_ScalingWithLetterBox,

    /// バックバッファをウィンドウサイズに合わせてリサイズする
    BackbufferResizeMode_Resize,

    /// 
    BackbufferResizeMode_Max,

};
enum TextureFormat
{
    /// 不明なフォーマット
    TextureFormat_Unknown,

    /// 32 ビットのアルファ付きフォーマット
    TextureFormat_A8R8G8B8,

    /// 32 ビットのアルファ無しフォーマット
    TextureFormat_X8R8G8B8,

    /// 64 ビットの浮動小数点フォーマット
    TextureFormat_A16B16G16R16F,

    /// 128 ビットの浮動小数点フォーマット
    TextureFormat_A32B32G32R32F,

    /// 32 ビットの Z バッファフォーマット
    TextureFormat_D24S8,

    /// 
    TextureFormat_Max,

};
enum BlendMode
{
    /// 通常 (アルファ無視)
    BlendMode_Normal,

    /// アルファブレンド (デフォルト)
    BlendMode_Alpha,

    /// 加算
    BlendMode_Add,

    /// 加算（アルファチャンネル無視）
    BlendMode_AddAlphaDisable,

    /// 減算
    BlendMode_Sub,

    /// 減算（アルファチャンネル無視）
    BlendMode_SubAlphaDisable,

    /// 乗算（アルファチャンネル無視）
    BlendMode_Mul,

    /// スクリーン（アルファチャンネル無視）
    BlendMode_Screen,

    /// リバース
    BlendMode_Reverse,

    /// 
    BlendMode_Max,

};
enum CullingMode
{
    /// 前面表示
    CullingMode_Front,

    /// 裏面表示
    CullingMode_Back,

    /// 両面表示
    CullingMode_Double,

    /// 
    CullingMode_Max,

};
enum TextAlign
{
    /// 左揃え
    TextAlign_Left,

    /// 中央揃え
    TextAlign_Center,

    /// 右揃え
    TextAlign_Right,

    /// 
    TextAlign_Max,

};
enum PlacementType
{
    /// 通常の配置方法
    PlacementType_Normal,

    /// 常にカメラの方向を向くビルボード (回転は Z 軸のみ有効)
    PlacementType_Billboard,

    /// Y 軸固定
    PlacementType_YFixed,

    /// 
    PlacementType_Max,

};
enum FrontAxis
{
    /// X+ 方向 (右向き)
    FrontAxis_X,

    /// Y+ 方向 (上向き)
    FrontAxis_Y,

    /// Z+ 方向 (奥向き)
    FrontAxis_Z,

    /// X- 方向 (左向き)
    FrontAxis_Rx,

    /// Y- 方向 (下向き)
    FrontAxis_Ry,

    /// Z- 方向 (手前向き)
    FrontAxis_Rz,

    /// 
    FrontAxis_Max,

};
enum MeshCreateFlags
{
    /// ソフトウェアスキニング用のモデル (動的な頂点、インデックスバッファを作成)
    MeshCreateFlags_SoftwareSkinning,

    /// 物理演算のメッシュボディとして扱うメッシュを生成する
    MeshCreateFlags_CollisionMesh,

    /// マテリアルに含まれるテクスチャファイルが見つからなくても無視する
    MeshCreateFlags_IgnoreTextureNotFound,

    /// 
    MeshCreateFlags_Max,

};
enum CameraType
{
    /// 位置と注視点から算出する (3Dカメラのデフォルト。2Dカメラでは無効)
    CameraType_PositionAndLookat,

    /// 位置と各軸の回転角度から算出する (2D カメラのデフォルト)
    CameraType_PositionAndAngle,

    /// 行列を使用し、正面(0,0,1) と 上方向(0,1,0) を変換する
    CameraType_TransformMatrix,

    /// 
    CameraType_Max,

};
enum Coord
{
    /// 2D 空間
    Coord_Space2D,

    /// 3D 空間
    Coord_Space3D,

    /// 
    Coord_Max,

};
enum ObjectType
{
    /// 
    ObjectType_Sound,

    /// 
    ObjectType_Max,

};


enum _lnInternal
{
    LN_INTERNAL_BLOCK = 0,
};

struct TypeInfo
{
	typedef ReferenceObject*(*ReferenceObjectFactory) (lnHandle handle);

	ReferenceObjectFactory Factory;

    static ReferenceObject* IntTableFactory(lnHandle handle);
    static ReferenceObject* RandomFactory(lnHandle handle);
    static ReferenceObject* FileStreamFactory(lnHandle handle);
    static ReferenceObject* VirtualPadFactory(lnHandle handle);
    static ReferenceObject* SoundFactory(lnHandle handle);
    static ReferenceObject* TextureFactory(lnHandle handle);
    static ReferenceObject* FontFactory(lnHandle handle);
    static ReferenceObject* SceneNodeFactory(lnHandle handle);
    static ReferenceObject* VisualNodeFactory(lnHandle handle);
    static ReferenceObject* SpriteFactory(lnHandle handle);
    static ReferenceObject* SpritePathFactory(lnHandle handle);
    static ReferenceObject* ModelFactory(lnHandle handle);
    static ReferenceObject* EffectFactory(lnHandle handle);
    static ReferenceObject* TilemapFactory(lnHandle handle);
    static ReferenceObject* ShaderFactory(lnHandle handle);
    static ReferenceObject* ViewPaneFactory(lnHandle handle);
    static ReferenceObject* LayerFactory(lnHandle handle);
    static ReferenceObject* ViewFilterFactory(lnHandle handle);
    static ReferenceObject* ToneFilterFactory(lnHandle handle);
    static ReferenceObject* TransitionFilterFactory(lnHandle handle);
    static ReferenceObject* BlurFilterFactory(lnHandle handle);
    static ReferenceObject* CameraFactory(lnHandle handle);


    static void _RegisterTypeInfo()
    {
        static TypeInfo _IntTableTypeInfo;
        _IntTableTypeInfo.Factory = IntTableFactory;
        LIntTable_SetTypeUserData(&_IntTableTypeInfo);
        static TypeInfo _RandomTypeInfo;
        _RandomTypeInfo.Factory = RandomFactory;
        LRandom_SetTypeUserData(&_RandomTypeInfo);
        static TypeInfo _FileStreamTypeInfo;
        _FileStreamTypeInfo.Factory = FileStreamFactory;
        LFileStream_SetTypeUserData(&_FileStreamTypeInfo);
        static TypeInfo _VirtualPadTypeInfo;
        _VirtualPadTypeInfo.Factory = VirtualPadFactory;
        LVirtualPad_SetTypeUserData(&_VirtualPadTypeInfo);
        static TypeInfo _SoundTypeInfo;
        _SoundTypeInfo.Factory = SoundFactory;
        LSound_SetTypeUserData(&_SoundTypeInfo);
        static TypeInfo _TextureTypeInfo;
        _TextureTypeInfo.Factory = TextureFactory;
        LTexture_SetTypeUserData(&_TextureTypeInfo);
        static TypeInfo _FontTypeInfo;
        _FontTypeInfo.Factory = FontFactory;
        LFont_SetTypeUserData(&_FontTypeInfo);
        static TypeInfo _SceneNodeTypeInfo;
        _SceneNodeTypeInfo.Factory = SceneNodeFactory;
        LSceneNode_SetTypeUserData(&_SceneNodeTypeInfo);
        static TypeInfo _VisualNodeTypeInfo;
        _VisualNodeTypeInfo.Factory = VisualNodeFactory;
        LVisualNode_SetTypeUserData(&_VisualNodeTypeInfo);
        static TypeInfo _SpriteTypeInfo;
        _SpriteTypeInfo.Factory = SpriteFactory;
        LSprite_SetTypeUserData(&_SpriteTypeInfo);
        static TypeInfo _SpritePathTypeInfo;
        _SpritePathTypeInfo.Factory = SpritePathFactory;
        LSpritePath_SetTypeUserData(&_SpritePathTypeInfo);
        static TypeInfo _ModelTypeInfo;
        _ModelTypeInfo.Factory = ModelFactory;
        LModel_SetTypeUserData(&_ModelTypeInfo);
        static TypeInfo _EffectTypeInfo;
        _EffectTypeInfo.Factory = EffectFactory;
        LEffect_SetTypeUserData(&_EffectTypeInfo);
        static TypeInfo _TilemapTypeInfo;
        _TilemapTypeInfo.Factory = TilemapFactory;
        LTilemap_SetTypeUserData(&_TilemapTypeInfo);
        static TypeInfo _ShaderTypeInfo;
        _ShaderTypeInfo.Factory = ShaderFactory;
        LShader_SetTypeUserData(&_ShaderTypeInfo);
        static TypeInfo _ViewPaneTypeInfo;
        _ViewPaneTypeInfo.Factory = ViewPaneFactory;
        LViewPane_SetTypeUserData(&_ViewPaneTypeInfo);
        static TypeInfo _LayerTypeInfo;
        _LayerTypeInfo.Factory = LayerFactory;
        LLayer_SetTypeUserData(&_LayerTypeInfo);
        static TypeInfo _ViewFilterTypeInfo;
        _ViewFilterTypeInfo.Factory = ViewFilterFactory;
        LViewFilter_SetTypeUserData(&_ViewFilterTypeInfo);
        static TypeInfo _ToneFilterTypeInfo;
        _ToneFilterTypeInfo.Factory = ToneFilterFactory;
        LToneFilter_SetTypeUserData(&_ToneFilterTypeInfo);
        static TypeInfo _TransitionFilterTypeInfo;
        _TransitionFilterTypeInfo.Factory = TransitionFilterFactory;
        LTransitionFilter_SetTypeUserData(&_TransitionFilterTypeInfo);
        static TypeInfo _BlurFilterTypeInfo;
        _BlurFilterTypeInfo.Factory = BlurFilterFactory;
        LBlurFilter_SetTypeUserData(&_BlurFilterTypeInfo);
        static TypeInfo _CameraTypeInfo;
        _CameraTypeInfo.Factory = CameraFactory;
        LCamera_SetTypeUserData(&_CameraTypeInfo);

    }   
};

/**
 *	@brief		参照カウントを持つクラスのベースクラス
 */
class ReferenceObject
{
private:
	int mRefCount;

protected:
	lnHandle mHandle;

protected:
	ReferenceObject(_lnInternal t)
        : mHandle(NULL)
		, mRefCount(1)
	{
	}

public:
	virtual ~ReferenceObject()
	{
		if (mHandle != NULL)
			LObject_Release(mHandle);
		mHandle = NULL;
	}

	/**
	 *	@brief	    オブジェクトの解放
	 *	@details    指定されたオブジェクトの参照を解放します。(参照カウントをデクリメントします)
	 */
	void Release()
	{
		mRefCount--;
		if (mRefCount <= 0) {
			delete this;
		}
	}

	/**
	 *	@brief	参照カウントのインクリメント
	 */
	void AddRef()
	{
		++mRefCount;
	}

	/**
	 *	@brief	参照カウント取得する
	 *	@return	現在の参照カウント
	 */
	int GetRefCount() const
	{
		return mRefCount;
	}

    /**
	 *	@brief	ハンドルの取得
	 */
	lnHandle GetHandle() const
	{
		return mHandle;
	}

public:
    virtual void _AttachHandle(lnHandle handle, bool addRef)
	{
        mHandle = handle;
        if (addRef)
		    LObject_AddRef(mHandle);  
	}

	static bool _IsObjectList() { return false; }
};


/**
 *	@brief		ReferenceObject 用のスマートポインタ
 */
template <class T>
class RefPtr
{
public:

	/**
	 *	@brief		コンストラクタ
	 */
	RefPtr()
		: mPtr(NULL)
	{}

	/**
	 *	@brief		コンストラクタ
	 *	@param[in]	ptr		: 管理対象としてセットする ReferenceObject インスタンスのポインタ
	 *	@param[in]	addRef	: true の場合、セットされた ReferenceObject の参照カウントをインクリメントする
	 */
	explicit RefPtr(T* ptr, bool addRef = false)
		: mPtr(ptr)
	{
		if (mPtr && addRef) mPtr->AddRef();
	}

	/**
	 *	@brief		コピーコンストラクタ
	 *	@param[in]	obj		:
	 */
	explicit RefPtr(const RefPtr<T>& obj)
		: mPtr(obj.mPtr)
	{
		if (mPtr) mPtr->AddRef();
	}

	/**
	 *	@brief		デストラクタ
	 */
	~RefPtr()
	{
		SafeRelease();
	}

public:

	/**
	 *	@brief		ReferenceObject インスタンスのポインタを管理対象としてセットする
	 *	@param[in]	ptr		: 管理対象としてセットする ReferenceObject インスタンスのポインタ
	 *	@param[in]	addRef	: true の場合、セットされた ReferenceObject の参照カウントをインクリメントする
	 */
	void Attach(T* ptr, bool addRef = false)
	{
		SafeRelease();
		mPtr = ptr;
		if (mPtr && addRef) mPtr->AddRef();
	}

	/**
	 *	@brief		管理対象オブジェクトの参照カウントをインクリメントする
	 */
	void SafeAddRef()
	{ 
		if (mPtr) mPtr->AddRef();
	}

	/**
	 *	@brief		管理対象オブジェクトの参照カウントをデクリメントし、管理対象から外す
	 */
	void SafeRelease()
	{ 
		if (mPtr) mPtr->Release();
		mPtr = NULL;
	}

	/**
	 *	@brief		管理対象オブジェクトへのポインタを取得する
	 */
	T* GetPtr() { return mPtr; }

	/**
	 *	@brief		管理対象オブジェクトへのポインタを取得する
	 */
	const T* GetPtr() const { return mPtr; }

public:

	/// 管理対象オブジェクトへのポインタ型への変換を実装する
	operator T* () { return mPtr; }

	/// 管理対象オブジェクトへのポインタ型への変換を実装する
	operator const T* () const { return mPtr; }

	/// operator=
	T* operator= (T* ptr)
	{
		if (ptr) ptr->AddRef();
		SafeRelease();
		mPtr = ptr;
		return mPtr;
	}

	/// operator*
	T& operator* () { return *mPtr; }

	/// operator*
	const T& operator* () const { return *mPtr; }

	/// operator->
	T* operator-> () { return mPtr; }

	/// operator->
	const T* operator-> () const { return mPtr; }

	/// operator!
	bool operator! () const { return (mPtr == NULL); }

	/// operator== 
	bool operator== (const T* ptr) const { return (mPtr == ptr); }

	/// operator!=
	bool operator!= (const T* ptr) const { return (mPtr != ptr); }

	/// operator< (for STL cmp)
	bool operator< (const T* ptr) const { return mPtr < ptr; }

public:
	void _NewAndAttachHandleAddRef(lnHandle handle)
	{
        T* obj;
        if (T::_IsObjectList()) {
            // ObjectList の場合はここでインスタンスを作る。
            // (TypeUserData に登録できるのはひとつの型のファクトリのみであるため。ObjectList を継承したクラスを作られたら手におえないが…)
            obj = LNOTE_NEW T(LN_INTERNAL_BLOCK);
            obj->_AttachHandle(handle, true);
        }
        else {
            TypeInfo* info = (TypeInfo*)LObject_GetTypeUserData(handle);
    	    obj = static_cast<T*>(info->Factory(handle));
        }
        Attach(obj, false);
	}

private:
	T* mPtr;
};

/**
 *	@brief	ReferenceObject のコレクション (動的配列)
 */
template<class T>
class ObjectList : public ReferenceObject
{
private:
	std::vector<T>	mArray;

public:
	ObjectList()
	{
	}
	virtual ~ObjectList()
	{
	}
	
public:
	
	/**
	 *	@brief	格納されているオブジェクトの数を取得する
	 *	@return	要素の数
	 */
	int GetCount() const 
	{
		int count;
		LObjectList_GetCount(mHandle, &count);
		return count;
	}
	
	/**
	 *	@brief		指定したインデックスのオブジェクトを取得する
	 *	@param[in]	index		: インデックス(要素番号)
	 *	@param[in]	item		: 設定するオブジェクト
	 */
	void SetAt(int index, T item)
	{
		_SyncItems();
		mArray[index] = item;
		LObjectList_SetAt(mHandle, index, item->GetHandle());
	}
	
	/**
	 *	@brief		指定したインデックスのオブジェクトを取得する
	 *	@param[in]	index		: インデックス(要素番号)
	 */
	T GetAt(int index)
	{
		_SyncItems();
		return mArray[index];
	}
	
	/**
	 *	@brief		末尾にオブジェクトを追加する
	 *	@param[in]	item		: 追加するオブジェクト
	 */
	void Add(T item)
	{
		_SyncItems();
		mArray.push_back(item);
	    LObjectList_Add(mHandle, item->GetHandle());
	}
	
	/**
	 *	@brief		全てのオブジェクトを削除する
	 *	@param[in]	item		: リストから削除するオブジェクト
	 */
	void Clear()
	{
		mArray.clear();
    	LObjectList_Clear(mHandle);
	}
	
	/**
	 *	@brief		オブジェクトリストの指定したインデックスの位置にオブジェクトを挿入する
	 *	@param[in]	index		: item を挿入するインデックス
	 *	@param[in]	item		: 挿入するオブジェクト
	 */
	void Insert(int index, T item)
	{
		_SyncItems();
		mArray.insert(mArray.begin() + index, item);
	    LObjectList_Insert(mHandle, index, item->GetHandle());
	}
	
	/**
	 *	@brief		指定したオブジェクトと一致する最初のオブジェクトを削除する
	 *	@param[in]	item		: リストから削除するオブジェクト
	 */
	void RemoveAll(int index, T item)
	{
		_SyncItems();
		std::vector<T>::iterator itr = mArray.begin();
		std::vector<T>::iterator end = mArray.end();
		for (; itr != end; ++itr)
		{
			if (*itr == item) {
				mArray.erase(itr);
				break;
			}
		}
	    LObjectList_Remove(mHandle, item->GetHandle());
	}
	
	/**
	 *	@brief		指定したハンドルと一致する全てのオブジェクトを削除する
	 *	@param[in]	item		: リストから削除するオブジェクト
	 */
	void RemoveAt(int index, T item)
	{
		_SyncItems();
		std::vector<T>::iterator itr = mArray.begin();
		std::vector<T>::iterator end = mArray.end();
		for (; itr != end;)
		{
			if (*itr == item)
				itr = mArray.erase(itr);
			else
				++itr;
		}
	    LObjectList_RemoveAll(mHandle, item->GetHandle());
	}
	
	/**
	 *	@brief		指定したインデックスにあるオブジェクトを削除する
	 *	@param[in]	index		: 削除するオブジェクトのインデックス番号
	 */
	void RemoveAt(int index)
	{
		_SyncItems();
		mArray.erase(mArray.begin() + index);
		LObjectList_RemoveAt(mHandle, index);
	}

private:
	void _SyncItems()
	{
		int count = GetCount();
		if (((int)mArray.size()) < count) 
		{
			mArray.resize(count);

			for (int i = 0; i < count; ++i)
			{
				lnHandle item;
				LObjectList_GetAt(mHandle, i, &item);
				if (mArray[i] == NULL || mArray[i]->GetHandle() != item)
				{
					if (mArray[i]) mArray[i]->Release();
					TypeInfo* info = (TypeInfo*)LObject_GetTypeUserData(item);
					mArray[i] = static_cast<T>(info->Factory(item));
				}
			}
		}
	}

public:
	ObjectList(_lnInternal t) : ReferenceObject(t) {}
	static bool _IsObjectList() { return true; }
	friend struct TypeInfo;
};
struct Size : public lnSize
{
public:

private:

    friend struct TypeInfo;
};
struct Rect : public lnRect
{
public:
    bool Contains(int x, int y);


private:

    friend struct TypeInfo;
};
class Exception
{
public:
    static void SetHandler(lnExceptionCallback handler);

    static void Raise(lnResult errCode, const lnChar* message, const lnChar* file, int line, const lnChar* func);

    static void GetLastErrorCode();

    static void ProcDefault();


private:

    friend struct TypeInfo;
};
class IntTable : public ReferenceObject
{
public:
     IntTable(int xSize, int ySize = 1, int zSize = 1);

     IntTable(int xSize, int ySize, int zSize, const int* srcData);

    void SetValue(int xSize, int ySize, int zSize, int value);

    int GetValue(int xSize, int ySize, int zSize);

    void Resize(int xSize, int ySize, int zSize);

    int GetXSize();

    int GetYSize();

    int GetZSize();

    public:IntTable(_lnInternal t) : ReferenceObject(t) {}


private:

    friend struct TypeInfo;
};
struct Vector2 : public lnVector2
{
public:
    void Set(float x, float y);

    float GetLength();

    float GetSquareLength();

    static Vector2 Normalize(const Vector2& vec);

    Vector2 Normalize();


private:

    friend struct TypeInfo;
};
struct Vector3 : public lnVector3
{
public:
    void Set(float x, float y, float z);

    void Set(const Vector2& vec2, float z);

    float GetLength();

    float GetSquareLength();

    static Vector3 Normalize(const Vector3& vec);

    Vector3 Normalize();

    static float Dot(const Vector3& vec1, const Vector3& vec2);

    static Vector3 Cross(const Vector3& vec1, const Vector3& vec2);

    static Vector3 Reflect(const Vector3& vec, const Vector3& normal);

    static Vector3 Slide(const Vector3& vec, const Vector3& normal);

    static Vector3 Lerp(const Vector3& vec1, const Vector3& vec2, float t);

    static Vector3 CatmullRom(const Vector3& vec1, const Vector3& vec2, const Vector3& vec3, const Vector3& vec4, float t);

    static Vector3 Transform(const Vector3& vec, const Matrix& mat);

    static Vector3 TransformCoord(const Vector3& vec, const Matrix& mat);

    static Vector3 RotateAxis(const Vector3& vec, const Vector3& axis, float r);

    static Vector3 RotateMatrix(const Vector3& vec, const Matrix& mat);


private:

    friend struct TypeInfo;
};
struct Vector4 : public lnVector4
{
public:

private:

    friend struct TypeInfo;
};
struct Matrix : public lnMatrix
{
public:
    static Matrix Identity();

    void Translate(float x, float y, float z);

    void Translate(const Vector3& vec);

    void RotateX(float radian);

    void RotateY(float radian);

    void RotateZ(float radian);

    void Rotate(float xRad, float yRad, float zRad, RotationOrder rotOrder = RotationOrder_XYZ);

    void Rotate(const Vector3& vec, RotationOrder rotOrder = RotationOrder_XYZ);

    void RotateAxis(const Vector3& axis, float radian);

    void RotateQuaternion(const Quaternion& qua);

    void Scale(float xyz);

    void Scale(float x, float y, float z);

    void Scale(const Vector3& scale);

    static Matrix Multiply(const Matrix& mat1, const Matrix& mat2);

    static Matrix Inverse(const Matrix& mat);

    Matrix Inverse();

    static Matrix Transpose(const Matrix& mat);

    Matrix Transpose();

    static Matrix ViewTransformLH(const Vector3& pos, const Vector3& lookAt, const Vector3& upDir);

    static Matrix ViewTransformRH(const Vector3& pos, const Vector3& lookAt, const Vector3& upDir);

    static Matrix PerspectiveFovLH(float fovY, float aspect, float nearZ, float farZ);

    static Matrix PerspectiveFovRH(float fovY, float aspect, float nearZ, float farZ);

    static Matrix OrthoLH(float width, float height, float nearZ, float farZ);

    static Matrix OrthoRH(float width, float height, float nearZ, float farZ);

    Vector3 GetRight();

    Vector3 GetUp();

    Vector3 GetFront();

    Vector3 GetPosition();

    static Vector3 ToEuler(const Matrix& mat);

    Vector3 ToEuler();

    static void Decompose(const Matrix& mat, Vector3* scale, Matrix* rot, Vector3* trans);

    static Matrix TransformBasis(const Matrix& mat);

    Matrix TransformBasis();


private:

    friend struct TypeInfo;
};
struct Quaternion : public lnQuaternion
{
public:
    void Set(float x, float y, float z, float w);

    static Quaternion Identity();

    static Quaternion RotationAxis(const Vector3& axis, float r);

    static Quaternion RotationMatrix(const Matrix& mat);

    static Quaternion RotationYawPitchRoll(float yaw, float pitch, float roll);

    static Quaternion Normalize(const Quaternion& qua);

    Quaternion Normalize();

    static Quaternion Conjugate(const Quaternion& qua);

    Quaternion Conjugate();

    static Quaternion Multiply(const Quaternion& qua1, const Quaternion& qua2);

    static Quaternion Slerp(const Quaternion& qua1, const Quaternion& qua2, float t);


private:

    friend struct TypeInfo;
};
class Random : public ReferenceObject
{
public:
     Random();

     Random(int seed);

    void SetSeed(int seed);

    int GetInt();

    int GetIntRange(int minValue, int maxValue);

    int GetIntDeflect(int center, int width);

    public:Random(_lnInternal t) : ReferenceObject(t) {}


private:

    friend struct TypeInfo;
};
class FileStream : public ReferenceObject
{
public:
     FileStream(const lnChar* filePath, FileMode mode = FileMode_Open, FileAccess access = FileAccess_Read);

    void Close();

    int GetSize();

    int GetPosition();

    void Seek(int offset, SeekOrigin origin);

    int Read(void* buffer, int readSize);

    static bool Exists(const lnChar* filePath);

    public:FileStream(_lnInternal t) : ReferenceObject(t) {}


private:

    friend struct TypeInfo;
};
class Input
{
public:
    static bool IsPress(int button);

    static bool IsTrigger(int button);

    static bool IsOffTrigger(int button);

    static bool IsRepeat(int button);

    static float GetPower(int button);

    static Vector2 GetAxisState(int axis);


private:

    friend struct TypeInfo;
};
class VirtualPad : public ReferenceObject
{
public:
    static VirtualPad* GetVirtualPad(int index);bool IsPress(int button);

    bool IsTrigger(int button);

    bool IsOffTrigger(int button);

    bool IsRepeat(int button);

    float GetPower(int button);

    static Vector2 GetAxisState(const VirtualPad* virtualPad, int axis);

    int GetPressedDeviceButton();

    static DeviceButtonType GetDeviceButtonType(int deviceButton);

    void AttachVirtualButton(int virtualButton, int deviceButton);

    void DetachVirtualButton(int virtualButton, int deviceButton);

    public:VirtualPad(_lnInternal t) : ReferenceObject(t) {}


private:

    friend struct TypeInfo;
};
class Audio
{
public:
    static void PlayBGM(const lnChar* filePath, int volume = 100, int pitch = 100, int fadeTime = 0);

    static void PlayBGMMem(const void* data, int dataSize, int volume = 100, int pitch = 100, int fadeTime = 0);

    static void StopBGM(int fadeTime = 0);

    static void PlayBGS(const lnChar* filePath, int volume = 100, int pitch = 100, int fadeTime = 0);

    static void PlayBGSMem(const void* data, int dataSize, int volume = 100, int pitch = 100, int fadeTime = 0);

    static void StopBGS(int fadeTime = 0);

    static void PlayME(const lnChar* filePath, int volume = 100, int pitch = 100);

    static void PlayMEMem(const void* data, int dataSize, int volume = 100, int pitch = 100);

    static void StopME();

    static void PlaySE(const lnChar* filePath, int volume = 100, int pitch = 100);

    static void PlaySE3D(const lnChar* filePath, const Vector3& position, float distance, int volume = 100, int pitch = 100);

    static void PlaySE3D(const lnChar* filePath, float x, float y, float z, float distance, int volume = 100, int pitch = 100);

    static void PlaySEMem(const void* data, int dataSize, int volume = 100, int pitch = 100);

    static void PlaySE3DMem(const void* data, int dataSize, const Vector3& position, float distance, int volume = 100, int pitch = 100);

    static void PlaySE3DMem(const void* data, int dataSize, float x, float y, float z, float distance, int volume = 100, int pitch = 100);

    static void StopSE();

    static void SetMetreUnitDistance(float distance);

    static void SetBGMVolume(int volume, int fadeTime = 0);

    static void SetBGSVolume(int volume, int fadeTime = 0);

    static void GetInternalGameSound(InternalGameSound type);


private:

    friend struct TypeInfo;
};
class SoundListener
{
public:
    static void SetPosition(const Vector3& position);

    static void SetPosition(float x, float y, float z);

    static void SetDirection(const Vector3& direction);

    static void SetDirection(float x, float y, float z);

    static void SetUpDirection(const Vector3& direction);

    static void SetUpDirection(float x, float y, float z);

    static void Velocity(const Vector3& velocity);

    static void Velocity(float x, float y, float z);


private:

    friend struct TypeInfo;
};
class Sound : public ReferenceObject
{
public:
     Sound(const lnChar* filePath, bool enable3d = false, SoundPlayingType playType = SoundPlayingType_Auto);

     Sound(const void* data, int dataSize, bool enable3d = false);

    void SetVolume(int volume);

    int GetVolume();

    void SetPitch(int pitch);

    int GetPitch();

    void SetLoopState(bool loopEnable, int begin = 0, int length = 0);

    bool IsLoop();

    SoundPlayingState GetPlayState();

    void Play();

    void Stop();

    void Pause(bool pause);

    void FadeVolume(int targetVolume, int time, SoundFadeState fadeState);

    int GetUnitsPlayed();

    int GetTotalUnits();

    int GetUnitsParSecond();

    bool Is3DSound();

    void SetEmitterPosition(const Vector3& position);

    void SetEmitterPosition(float x, float y, float z);

    void SetEmitterVelocity(const Vector3& velocity);

    void SetEmitterVelocity(float x, float y, float z);

    void SetEmitterDistance(float distance);

    public:Sound(_lnInternal t) : ReferenceObject(t) {}


private:

    friend struct TypeInfo;
};
struct Color : public lnColor
{
public:

private:

    friend struct TypeInfo;
};
struct ColorF : public lnColorF
{
public:

private:

    friend struct TypeInfo;
};
struct Tone : public lnTone
{
public:

private:

    friend struct TypeInfo;
};
class Texture : public ReferenceObject
{
public:
     Texture(int width, int height);

     Texture(const lnChar* filePath);

     Texture(int width, int height, int levels, TextureFormat format);

     Texture(const lnChar* filePath, lnU32 colorKey, int levels, TextureFormat format);

    int GetWidth();

    int GetHeight();

    int GetRealWidth();

    int GetRealHeight();

    void Clear(const Color& color);

    void ClearRGBA(int r, int g, int b, int a);

    void DrawText(const lnChar* text, const Rect& rect, const Font* font = NULL, int length = -1, TextAlign align = TextAlign_Left);

    void DrawText(const lnChar* text, int x, int y, const Font* font = NULL, int length = -1, TextAlign align = TextAlign_Left);

    void BitBlt(int x, int y, const Texture* srcTexture, const Rect& srcRect);

    public:Texture(_lnInternal t) : ReferenceObject(t) {}


private:

    friend struct TypeInfo;
};
class Font : public ReferenceObject
{
public:
     Font();

     Font(const lnChar* filePath);

     Font(const Font* srcFont);

    static void SetDefaultFont(const Font* font);

    void SetName(const lnChar* fontName);

    const lnChar* GetName();

    void SetSize(int size);

    int GetSize();

    void SetColor(const Color& color);

    Color GetColor();

    void SetEdgeColor(const Color& color);

    Color GetEdgeColor();

    void SetEdgeSize(int size);

    int GetEdgeSize();

    void SetBold(bool enabled);

    bool IsBold();

    void SetItalic(bool enabled);

    bool IsItalic();

    void SetAntiAlias(bool enabled);

    bool IsAntiAlias();

    static Size GetTextSize(const Font* font, const lnChar* text, int length);

    public:Font(_lnInternal t) : ReferenceObject(t) {}


private:

    friend struct TypeInfo;
};
class SceneGraph
{
public:

private:

    friend struct TypeInfo;
};
class SceneNode : public ReferenceObject
{
public:
    const lnChar* GetName();

    void SetName(const lnChar* name);

    void SetPosition(const Vector3& vec);

    void SetPosition(float x, float y, float z);

    Vector3 GetPosition();

    void GetPosition(float* x, float* y, float* z);

    void SetAngle(const Vector3& vec);

    void SetAngle(float x, float y, float z);

    Vector3 GetAngle();

    void GetAngle(float* x, float* y, float* z);

    void SetScale(const Vector3& vec);

    void SetScale(float x, float y, float z);

    Vector3 GetScale();

    void GetScale(float* x, float* y, float* z);

    void SetCenter(const Vector3& vec);

    void SetCenter(float x, float y, float z);

    Vector3 GetCenter();

    void GetCenter(float* x, float* y, float* z);

    void SetMatrix(const Matrix& mat);

    Matrix GetMatrix();

    void SetRotateOrder(RotationOrder rotOrder);

    RotationOrder GetRotateOrder();

    void SetPlacementType(PlacementType type);

    PlacementType GetPlacementType();

    void AddChild(const SceneNode* childSceneNode);

    void RemoveChild(const SceneNode* childSceneNode);

    void Update(float elapsedTime);

    void SetEnableAutoUpdate(bool enabled);

    bool IsEnableAutoUpdate();

    public:SceneNode(_lnInternal t) : ReferenceObject(t) {}


private:

    friend struct TypeInfo;
};
class VisualNode : public SceneNode
{
public:
    void SetVisible(bool enabled);

    void SetOpacity(float opacity);

    void SetColorScale(int materialIndex, const ColorF& color);

    void SetBlendColor(int materialIndex, const ColorF& color);

    void SetTone(int materialIndex, const Tone& tone);

    void SetUVTransform(int materialIndex, const Matrix& matrix);

    void SetBlendMode(BlendMode mode);

    void SetCullingMode(CullingMode mode);

    void SetEnableDepthTest(bool enabled);

    void SetEnableDepthWrite(bool enabled);

    void SetShader(const Shader* shader);

    public:VisualNode(_lnInternal t) : SceneNode(t) {}


private:

    friend struct TypeInfo;
};
class Sprite : public VisualNode
{
public:
     Sprite(const Texture* texture);

    void SetTexture(const Texture* texture);

    void SetSize(float width, float height);

    void SetSrcRect(const Rect& rect);

    void SetSrcRect(int x, int y, int w, int h);

    Rect GetSrcRect();

    void GetSrcRect(int* x, int* y, int* w, int* h);

    void SetFlipH(bool enabled);

    bool IsFlipH();

    void SetFlipV(bool enabled);

    bool IsFlipV();

    public:Sprite(_lnInternal t) : VisualNode(t) {}


private:

    friend struct TypeInfo;
};
class SpritePath : public VisualNode
{
public:
     SpritePath(int maxSpriteCount, Coord coord = Coord_Space2D);

    void SetPathBlendMode(BlendMode blendMode);

    void SetPathTransform(const Matrix& matrix);

    void DrawSprite(const Vector3& position, const Vector3& center, const Vector2& size, const Texture* texture, const Rect& srcRect, const ColorF& vertexColors, FrontAxis frontAxis = FrontAxis_Rz);

    public:SpritePath(_lnInternal t) : VisualNode(t) {}


private:

    friend struct TypeInfo;
};
class Model : public VisualNode
{
public:
     Model(const lnChar* filePath, MeshCreateFlags meshCreateFlags);

    public:Model(_lnInternal t) : VisualNode(t) {}


private:

    friend struct TypeInfo;
};
class Effect : public VisualNode
{
public:
     Effect(const lnChar* filePath);

    void Play(bool overlap);

    void Stop();

    bool IsPlaying();

    public:Effect(_lnInternal t) : VisualNode(t) {}


private:

    friend struct TypeInfo;
};
class Tilemap : public VisualNode
{
public:
     Tilemap();

    void SetMapData(const IntTable* intTable);

    void SetTilesetTexture(const Texture* texture);

    void SetTilesetHorizontalTileCount(int count);

    void SetTilePixelSize(int size);

    public:Tilemap(_lnInternal t) : VisualNode(t) {}


private:

    friend struct TypeInfo;
};
class Shader : public ReferenceObject
{
public:
     Shader(const lnChar* filePath);

    public:Shader(_lnInternal t) : ReferenceObject(t) {}


private:

    friend struct TypeInfo;
};
class ViewPane : public ReferenceObject
{
public:
    static ViewPane* GetDefaultViewPane();

    void SetBackgroundColor(const ColorF& color);

    ObjectList<Layer*>* GetLayers();

    public:ViewPane(_lnInternal t) : ReferenceObject(t) {}


private:
        RefPtr< ObjectList<Layer*> > mGetLayers;

    friend struct TypeInfo;
};
class Layer : public ReferenceObject
{
public:
    void SetPriority(int priority);

    int GetPriority();

    public:Layer(_lnInternal t) : ReferenceObject(t) {}


private:

    friend struct TypeInfo;
};
class ViewFilter : public Layer
{
public:
    void Update(float elapsedTime);

    void SetEnableAutoUpdate(bool enabled);

    bool IsEnableAutoUpdate();

    public:ViewFilter(_lnInternal t) : Layer(t) {}


private:

    friend struct TypeInfo;
};
class ToneFilter : public ViewFilter
{
public:
     ToneFilter();

    void SetTone(const Tone& tone, float duration);

    public:ToneFilter(_lnInternal t) : ViewFilter(t) {}


private:

    friend struct TypeInfo;
};
class TransitionFilter : public ViewFilter
{
public:
     TransitionFilter();

    void Freeze();

    void Transition(double duration, const Texture* maskTexture, float vague);

    public:TransitionFilter(_lnInternal t) : ViewFilter(t) {}


private:

    friend struct TypeInfo;
};
class BlurFilter : public ViewFilter
{
public:
     BlurFilter();

    void Blur(double duration, float power, float scale, const Vector3& center, const Camera* baseCamera);

    public:BlurFilter(_lnInternal t) : ViewFilter(t) {}


private:

    friend struct TypeInfo;
};
class Camera : public SceneNode
{
public:
    static Camera* GetDefault3DCamera();

    static Camera* GetDefault2DCamera();

    void SetCameraType(CameraType type);

    void SetPosition(const Vector3& position);

    void SetPosition(float x, float y, float z);

    void SetLookAt(const Vector3& at);

    void SetLookAt(float x, float y, float z);

    void SetUpDirection(const Vector3& up);

    void SetUpDirection(float x, float y, float z);

    void SetAngle(const Vector3& angle);

    void SetAngle(float x, float y, float z);

    void SetMatrix(const Matrix& matrix);

    void SetFovY(float fov);

    void SetNearFarClip(float nearClip, float farClip);

    Matrix GetViewMatrix();

    Matrix GetProjectionMatrix();

    Matrix GetViewProjectionMatrix();

    public:Camera(_lnInternal t) : SceneNode(t) {}


private:

    friend struct TypeInfo;
};
class Config
{
public:
    static void SetWindowTitle(const lnChar* title);

    static void SetWindowSize(int width, int height);

    static void SetBackbufferSize(int width, int height);

    static void SetWindowResizeEnabled(bool enabled);

    static void SetAcceleratorKey(AcceleratorKey acceleratorKey, KeyCode keyCode, bool isShift, bool isCtrl, bool isAlt);

    static void SetLogFileEnabled(bool flag);

    static void SetConsoleEnabled(bool flag);

    static void RegisterArchive(const lnChar* filePath, const lnChar* password);

    static void SetFileAccessPriority(FileAccessPriority priority);

    static void SetDirectMusicInitMode(DirectMusicInitMode mode);

    static void SetSoundMaxCacheCount(int count);

    static void SoundPlayTypeThreshold(int bytes);

    static void SetFrameRate(int frameRate);

    static void SetEnableFrameWait(bool enabled);

    static void SetAutoUpdateTimeScale(float timeScale);

    static void RegisterFontFile(const lnChar* filePath);

    static void SetTextureCacheSize(int count);

    static void SetShaderCacheSize(int count);

    static void SetExceptionMessageBoxEnabled(bool enabled);

    static void SetWin32UserWindowHandle(lnIntPtr hWnd);


private:

    friend struct TypeInfo;
};
class Core
{
public:
    static void Initialize();static void InitializeFileIO();

    static void InitializeAudio();

    static void InitializeInput();

    static void Update();

    static void ResetFrameDelay();

    static bool IsEndRequested();

    static void Terminate();


private:

    friend struct TypeInfo;
};


/**
 * @brief  指定した x y 座標の点が矩形に含まれているかを判別する
 * @param  [in]     x        : X 座標
 * @param  [in]     y        : Y 座標
 * @return 結果を格納する変数
 */
inline bool Rect::Contains(int x, int y)
{
    lnBool result;
    LNRect_Contains(this, x, y, &result);
    return result != LN_FALSE;
}
/**
 * @brief  例外発生時にコールバックされる関数を登録する
 * @param  [in]     handler  : 
 */
inline void Exception::SetHandler(lnExceptionCallback handler)
{
    LNException_SetHandler(handler);
}
/**
 * @brief  例外を発生させる
 * @param  [in]     errCode  : 
 * @param  [in]     message  : 
 * @param  [in]     file     : 
 * @param  [in]     line     : 
 * @param  [in]     func     : 
 */
inline void Exception::Raise(lnResult errCode, const lnChar* message, const lnChar* file, int line, const lnChar* func)
{
    LNException_Raise(errCode, message, file, line, func);
}
/**
 * @brief  最後に発生した例外のエラーコードを取得する
 */
inline void Exception::GetLastErrorCode()
{
    LNException_GetLastErrorCode();
}
/**
 * @brief  最後に発生した例外に対して、デフォルトの処理を行う (メッセージボックスを表示し、LNException_SetHandled(true))
 */
inline void Exception::ProcDefault()
{
    LNException_ProcDefault();
}
/**
 * @brief  整数型の多次元配列を作成する
 * @param  [in]     xSize    : X 要素数
 * @param  [in]     ySize    : Y 要素数
 * @param  [in]     zSize    : Z 要素数
 */
inline  IntTable::IntTable(int xSize, int ySize, int zSize)
    : ReferenceObject(LN_INTERNAL_BLOCK)
{
    LNIntTable_Create(&mHandle, xSize, ySize, zSize);
}
/**
 * @brief  整数型の多次元配列を作成する
 * @param  [in]     xSize    : X 要素数
 * @param  [in]     ySize    : Y 要素数
 * @param  [in]     zSize    : Z 要素数
 * @param  [in]     srcData  : XXXX
 */
inline  IntTable::IntTable(int xSize, int ySize, int zSize, const int* srcData)
    : ReferenceObject(LN_INTERNAL_BLOCK)
{
    LNIntTable_CreateFromSrcData(&mHandle, xSize, ySize, zSize, srcData);
}
/**
 * @brief  整数型テーブルの指定したインデックスに値を設定する
 * @param  [in]     xSize    : X 要素インデックス
 * @param  [in]     ySize    : Y 要素インデックス
 * @param  [in]     zSize    : Z 要素インデックス
 * @param  [in]     value    : 設定する整数値
 * @details
 *         変更前のデータは保持されます。
 */
inline void IntTable::SetValue(int xSize, int ySize, int zSize, int value)
{
    LNIntTable_SetValue(mHandle, xSize, ySize, zSize, value);
}
/**
 * @brief  整数型テーブルの指定したインデックスに値を設定する
 * @param  [in]     xSize    : X 要素インデックス
 * @param  [in]     ySize    : Y 要素インデックス
 * @param  [in]     zSize    : Z 要素インデックス
 * @return 値を格納する整数型変数
 * @details
 *         変更前のデータは保持されます。
 */
inline int IntTable::GetValue(int xSize, int ySize, int zSize)
{
    int value;
    LNIntTable_GetValue(mHandle, xSize, ySize, zSize, &value);
    return value;
}
/**
 * @brief  整数型テーブルの要素数を変更する
 * @param  [in]     xSize    : X 要素数
 * @param  [in]     ySize    : Y 要素数
 * @param  [in]     zSize    : Z 要素数
 * @details
 *         変更前のデータは保持されます。
 */
inline void IntTable::Resize(int xSize, int ySize, int zSize)
{
    LNIntTable_Resize(mHandle, xSize, ySize, zSize);
}
/**
 * @brief  整数型テーブルの X 要素数を取得する
 * @return X 要素数を格納する変数
 */
inline int IntTable::GetXSize()
{
    int xSize;
    LNIntTable_GetXSize(mHandle, &xSize);
    return xSize;
}
/**
 * @brief  整数型テーブルの Y 要素数を取得する
 * @return Y 要素数を格納する変数
 */
inline int IntTable::GetYSize()
{
    int ySize;
    LNIntTable_GetYSize(mHandle, &ySize);
    return ySize;
}
/**
 * @brief  整数型テーブルの Z 要素数を取得する
 * @return Z 要素数を格納する変数
 */
inline int IntTable::GetZSize()
{
    int zSize;
    LNIntTable_GetZSize(mHandle, &zSize);
    return zSize;
}
/**
 * @brief  2D ベクトルに値を設定する
 * @param  [in]     x        : X 値
 * @param  [in]     y        : Y 値
 */
inline void Vector2::Set(float x, float y)
{
    LNVector2_Set(this, x, y);
}
/**
 * @brief  2D ベクトルの長さを計算する
 * @return ベクトルの長さを格納する変数
 */
inline float Vector2::GetLength()
{
    float outLength;
    LNVector2_GetLength(this, &outLength);
    return outLength;
}
/**
 * @brief  2D ベクトルの長さの2乗を計算する
 * @return ベクトルの長さの2乗を格納する変数
 */
inline float Vector2::GetSquareLength()
{
    float outLength;
    LNVector2_GetSquareLength(this, &outLength);
    return outLength;
}
/**
 * @brief  2D ベクトルを正規化する
 * @param  [in]     vec      : 処理の基になる 2D ベクトル
 * @return 演算結果を格納する 2D ベクトル
 */
inline Vector2 Vector2::Normalize(const Vector2& vec)
{
    Vector2 outVec;
    LNVector2_Normalize(&vec, &outVec);
    return outVec;
}
/**
 * @brief  2D ベクトルを正規化する
 * @return 演算結果を格納する 2D ベクトル
 */
inline Vector2 Vector2::Normalize()
{
    Vector2 outVec;
    LNVector2_Normalize(this, &outVec);
    return outVec;
}
/**
 * @brief  3Dベクトルに値を設定する
 * @param  [in]     x        : X 値
 * @param  [in]     y        : Y 値
 * @param  [in]     z        : Z 値
 */
inline void Vector3::Set(float x, float y, float z)
{
    LNVector3_Set(this, x, y, z);
}
/**
 * @brief  3Dベクトルに値を設定する
 * @param  [in]     vec2     : 2D ベクトル
 * @param  [in]     z        : Z 値
 */
inline void Vector3::Set(const Vector2& vec2, float z)
{
    LNVector3_SetVec2(this, &vec2, z);
}
/**
 * @brief  3Dベクトルの長さを計算する
 * @return ベクトルの長さを格納する変数
 */
inline float Vector3::GetLength()
{
    float outLength;
    LNVector3_GetLength(this, &outLength);
    return outLength;
}
/**
 * @brief  3D ベクトルの長さの2乗を計算する
 * @return ベクトルの長さの2乗を格納する変数
 */
inline float Vector3::GetSquareLength()
{
    float outLength;
    LNVector3_GetSquareLength(this, &outLength);
    return outLength;
}
/**
 * @brief  3D ベクトルを正規化する
 * @param  [in]     vec      : 処理の基になる 3D ベクトル
 * @return 演算結果を格納する 3D ベクトル
 */
inline Vector3 Vector3::Normalize(const Vector3& vec)
{
    Vector3 outVec;
    LNVector3_Normalize(&vec, &outVec);
    return outVec;
}
/**
 * @brief  3D ベクトルを正規化する
 * @return 演算結果を格納する 3D ベクトル
 */
inline Vector3 Vector3::Normalize()
{
    Vector3 outVec;
    LNVector3_Normalize(this, &outVec);
    return outVec;
}
/**
 * @brief  2 つの 3D ベクトルの内積を計算する
 * @param  [in]     vec1     : 処理の基になる 3D ベクトル
 * @param  [in]     vec2     : 処理の基になる 3D ベクトル
 * @return ベクトルの内積を格納する変数
 */
inline float Vector3::Dot(const Vector3& vec1, const Vector3& vec2)
{
    float dot;
    LNVector3_Dot(&vec1, &vec2, &dot);
    return dot;
}
/**
 * @brief  2 つの 3D ベクトルの外積を計算する
 * @param  [in]     vec1     : 処理の基になる 3D ベクトル
 * @param  [in]     vec2     : 処理の基になる 3D ベクトル
 * @return 演算結果を格納する 3D ベクトル
 */
inline Vector3 Vector3::Cross(const Vector3& vec1, const Vector3& vec2)
{
    Vector3 outVec;
    LNVector3_Cross(&vec1, &vec2, &outVec);
    return outVec;
}
/**
 * @brief  指定された法線で反射させたベクトルを計算する
 * @param  [in]     vec      : 処理の基になる 3D ベクトル
 * @param  [in]     normal   : 面方向を表す法線 3D ベクトル
 * @return 演算結果を格納する 3D ベクトル
 */
inline Vector3 Vector3::Reflect(const Vector3& vec, const Vector3& normal)
{
    Vector3 outVec;
    LNVector3_Reflect(&vec, &normal, &outVec);
    return outVec;
}
/**
 * @brief  指定された法線と方向から滑りベクトルを計算する
 * @param  [in]     vec      : 処理の基になる 3D ベクトル
 * @param  [in]     normal   : 面方向を表す法線 3D ベクトル
 * @return 演算結果を格納する 3D ベクトル
 */
inline Vector3 Vector3::Slide(const Vector3& vec, const Vector3& normal)
{
    Vector3 outVec;
    LNVector3_Slide(&vec, &normal, &outVec);
    return outVec;
}
/**
 * @brief  2 つの 3D ベクトル間を線形補間する
 * @param  [in]     vec1     : 処理の基になる 3D ベクトル ( t = 0.0 のときの値 )
 * @param  [in]     vec2     : 処理の基になる 3D ベクトル ( t = 1.0 のときの値 )
 * @param  [in]     t        : 補間値 ( 0.0 ～ 1.0 )
 * @return 演算結果を格納する 3D ベクトル
 */
inline Vector3 Vector3::Lerp(const Vector3& vec1, const Vector3& vec2, float t)
{
    Vector3 outVec;
    LNVector3_Lerp(&vec1, &vec2, t, &outVec);
    return outVec;
}
/**
 * @brief  指定された 3D ベクトルを Catmull-Rom 補間する
 * @param  [in]     vec1     : 処理の基になる 3D ベクトル
 * @param  [in]     vec2     : 処理の基になる 3D ベクトル ( t = 0.0 のときの値 )
 * @param  [in]     vec3     : 処理の基になる 3D ベクトル ( t = 1.0 のときの値 )
 * @param  [in]     vec4     : 処理の基になる 3D ベクトル
 * @param  [in]     t        : 補間値 ( 0.0 ～ 1.0 )
 * @return 演算結果を格納する 3D ベクトル
 */
inline Vector3 Vector3::CatmullRom(const Vector3& vec1, const Vector3& vec2, const Vector3& vec3, const Vector3& vec4, float t)
{
    Vector3 outVec;
    LNVector3_CatmullRom(&vec1, &vec2, &vec3, &vec4, t, &outVec);
    return outVec;
}
/**
 * @brief  指定された行列を使用して座標変換する
 * @param  [in]     vec      : 処理の基になる 3D ベクトル
 * @param  [in]     mat      : 処理の基になる 行列
 * @return 演算結果を格納する 3D ベクトル
 * @details
 *         ベクトルを ( x, y, z, 1 ) として座標変換します。
 *         結果の w は出力されません。<br>
 *         結果を w = 1 に射影する ( x y z を w で除算する ) 場合は
 *         Vector3TransformCoord() を使用してください。
 */
inline Vector3 Vector3::Transform(const Vector3& vec, const Matrix& mat)
{
    Vector3 outVec;
    LNVector3_Transform(&vec, &mat, &outVec);
    return outVec;
}
/**
 * @brief  指定された行列を使用して座標変換し、結果を w = 1 に射影する
 * @param  [in]     vec      : 処理の基になる 3D ベクトル
 * @param  [in]     mat      : 処理の基になる 行列
 * @return 演算結果を格納する 3D ベクトル
 * @details
 *         ベクトルを ( x, y, z, 1 ) として座標変換し、
 *         結果を w = 1 に射影 ( x y z を w で除算する ) します。
 */
inline Vector3 Vector3::TransformCoord(const Vector3& vec, const Matrix& mat)
{
    Vector3 outVec;
    LNVector3_TransformCoord(&vec, &mat, &outVec);
    return outVec;
}
/**
 * @brief  任意軸周りの回転を行う
 * @param  [in]     vec      : 処理の基になる Vector3
 * @param  [in]     axis     : 回転軸を表す Vector3
 * @param  [in]     r        : 回転角度 (ラジアン)
 * @return 演算結果を格納する Vector3
 */
inline Vector3 Vector3::RotateAxis(const Vector3& vec, const Vector3& axis, float r)
{
    Vector3 outVec;
    LNVector3_RotateAxis(&vec, &axis, r, &outVec);
    return outVec;
}
/**
 * @brief  行列の回転成分だけを使って座標変換する
 * @param  [in]     vec      : 処理の基になる Vector3
 * @param  [in]     mat      : 処理の基になる Matrix
 * @return 演算結果を格納する Vector3
 */
inline Vector3 Vector3::RotateMatrix(const Vector3& vec, const Matrix& mat)
{
    Vector3 outVec;
    LNVector3_RotateMatrix(&vec, &mat, &outVec);
    return outVec;
}
/**
 * @brief  単位行列を作成する
 * @return 結果を格納する行列
 */
inline Matrix Matrix::Identity()
{
    Matrix mat;
    LNMatrix_Identity(&mat);
    return mat;
}
/**
 * @brief  行列を平行移動する
 * @param  [in]     x        : X 方向の移動量
 * @param  [in]     y        : Y 方向の移動量
 * @param  [in]     z        : Z 方向の移動量
 * @details
 *         与えられた引数から平行移動行列を作り、現在の行列に乗算します。
 */
inline void Matrix::Translate(float x, float y, float z)
{
    LNMatrix_Translate(this, x, y, z);
}
/**
 * @brief  行列を平行移動する
 * @param  [in]     vec      : 移動量
 * @details
 *         与えられた引数から平行移動行列を作り、現在の行列に乗算します。
 */
inline void Matrix::Translate(const Vector3& vec)
{
    LNMatrix_TranslateVec3(this, &vec);
}
/**
 * @brief  X 軸を回転軸にして行列を回転する ( ラジアン単位 )
 * @param  [in]     radian   : 回転角度 ( 軸方向に対して反時計回り )
 * @details
 *         与えられた引数から回転行列を作り、現在の行列に乗算します。
 */
inline void Matrix::RotateX(float radian)
{
    LNMatrix_RotateX(this, radian);
}
/**
 * @brief  Y 軸を回転軸にして行列を回転する ( ラジアン単位 )
 * @param  [in]     radian   : 回転角度
 * @details
 *         与えられた引数から回転行列を作り、現在の行列に乗算します。
 */
inline void Matrix::RotateY(float radian)
{
    LNMatrix_RotateY(this, radian);
}
/**
 * @brief  Z 軸を回転軸にして行列を回転する ( ラジアン単位 )
 * @param  [in]     radian   : 回転角度
 * @details
 *         与えられた引数から回転行列を作り、現在の行列に乗算します。
 */
inline void Matrix::RotateZ(float radian)
{
    LNMatrix_RotateZ(this, radian);
}
/**
 * @brief  行列を回転する ( ラジアン単位 )
 * @param  [in]     xRad     : X 軸を回転軸とした角度
 * @param  [in]     yRad     : Y 軸を回転軸とした角度
 * @param  [in]     zRad     : Z 軸を回転軸とした角度
 * @param  [in]     rotOrder : 回転順序の指定
 * @details
 *         与えられた引数から回転行列を作り、現在の行列に乗算します。
 */
inline void Matrix::Rotate(float xRad, float yRad, float zRad, RotationOrder rotOrder)
{
    LNMatrix_Rotate(this, xRad, yRad, zRad, (lnRotationOrder)rotOrder);
}
/**
 * @brief  行列を回転する ( ラジアン単位 )
 * @param  [in]     vec      : 各軸の回転角度
 * @param  [in]     rotOrder : 回転順序の指定
 * @details
 *         与えられた引数から回転行列を作り、現在の行列に乗算します。
 */
inline void Matrix::Rotate(const Vector3& vec, RotationOrder rotOrder)
{
    LNMatrix_RotateVec3(this, &vec, (lnRotationOrder)rotOrder);
}
/**
 * @brief  任意の軸を回転軸にして行列を回転する ( ラジアン単位 )
 * @param  [in]     axis     : 回転軸を示す Vector3
 * @param  [in]     radian   : 回転角度
 * @details
 *         与えられた引数から回転行列を作り、現在の行列に乗算します。<br>
 *         与えられた軸が正規化されていない場合は正規化したものを使って計算します。
 */
inline void Matrix::RotateAxis(const Vector3& axis, float radian)
{
    LNMatrix_RotateAxis(this, &axis, radian);
}
/**
 * @brief  クォータニオンを使って行列を回転する
 * @param  [in]     qua      : 処理の基になる Quaternion
 * @details
 *         クォータニオンから回転行列を作り、現在の行列に乗算します。
 */
inline void Matrix::RotateQuaternion(const Quaternion& qua)
{
    LNMatrix_RotateQuaternion(this, &qua);
}
/**
 * @brief  行列をスケーリングする
 * @param  [in]     xyz      : X Y Z 全ての方向に適用する拡大率 (X Y Z を同じ値で拡大)
 * @details
 *         与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
 */
inline void Matrix::Scale(float xyz)
{
    LNMatrix_Scale(this, xyz);
}
/**
 * @brief  行列をスケーリングする
 * @param  [in]     x        : X 方向の拡大率
 * @param  [in]     y        : Y 方向の拡大率
 * @param  [in]     z        : Z 方向の拡大率
 * @details
 *         与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
 */
inline void Matrix::Scale(float x, float y, float z)
{
    LNMatrix_ScaleXYZ(this, x, y, z);
}
/**
 * @brief  行列をスケーリングする
 * @param  [in]     scale    : 各方向の拡大率
 * @details
 *         与えられた引数からスケーリング行列を作り、現在の行列に乗算します。<br>
 */
inline void Matrix::Scale(const Vector3& scale)
{
    LNMatrix_ScaleVec3(this, &scale);
}
/**
 * @brief  行列を乗算する
 * @param  [in]     mat1     : 処理の基になる行列
 * @param  [in]     mat2     : 処理の基になる行列
 * @return 結果を格納する行列
 */
inline Matrix Matrix::Multiply(const Matrix& mat1, const Matrix& mat2)
{
    Matrix matOut;
    LNMatrix_Multiply(&mat1, &mat2, &matOut);
    return matOut;
}
/**
 * @brief  逆行列を求める
 * @param  [in]     mat      : 処理の基になる行列
 * @return 演算結果を格納する行列
 */
inline Matrix Matrix::Inverse(const Matrix& mat)
{
    Matrix matOut;
    LNMatrix_Inverse(&mat, &matOut);
    return matOut;
}
/**
 * @brief  逆行列を求める
 * @return 演算結果を格納する行列
 */
inline Matrix Matrix::Inverse()
{
    Matrix matOut;
    LNMatrix_Inverse(this, &matOut);
    return matOut;
}
/**
 * @brief  転置行列を求める
 * @param  [in]     mat      : 処理の基になる行列
 * @return 演算結果を格納する行列
 */
inline Matrix Matrix::Transpose(const Matrix& mat)
{
    Matrix matOut;
    LNMatrix_Transpose(&mat, &matOut);
    return matOut;
}
/**
 * @brief  転置行列を求める
 * @return 演算結果を格納する行列
 */
inline Matrix Matrix::Transpose()
{
    Matrix matOut;
    LNMatrix_Transpose(this, &matOut);
    return matOut;
}
/**
 * @brief  左手座標系ビュー行列を作成する
 * @param  [in]     pos      : 視点の位置を示す Vector3
 * @param  [in]     lookAt   : 注視点を示す Vector3
 * @param  [in]     upDir    : 上方向を示す Vector3
 * @return 演算結果を格納する行列
 */
inline Matrix Matrix::ViewTransformLH(const Vector3& pos, const Vector3& lookAt, const Vector3& upDir)
{
    Matrix matOut;
    LNMatrix_ViewTransformLH(&pos, &lookAt, &upDir, &matOut);
    return matOut;
}
/**
 * @brief  右手座標系ビュー行列を作成する
 * @param  [in]     pos      : 視点の位置を示す Vector3
 * @param  [in]     lookAt   : 注視点を示す Vector3
 * @param  [in]     upDir    : 上方向を示す Vector3
 * @return 演算結果を格納する行列
 */
inline Matrix Matrix::ViewTransformRH(const Vector3& pos, const Vector3& lookAt, const Vector3& upDir)
{
    Matrix matOut;
    LNMatrix_ViewTransformRH(&pos, &lookAt, &upDir, &matOut);
    return matOut;
}
/**
 * @brief  左手座標系射影行列の作成
 * @param  [in]     fovY     : Y 方向への視野角 (ラジアン単位)
 * @param  [in]     aspect   : アスペクト比
 * @param  [in]     nearZ    : 近くのビュー平面の Z 値
 * @param  [in]     farZ     : 遠くのビュー平面の Z 値
 * @return 演算結果を格納する行列
 */
inline Matrix Matrix::PerspectiveFovLH(float fovY, float aspect, float nearZ, float farZ)
{
    Matrix matOut;
    LNMatrix_PerspectiveFovLH(fovY, aspect, nearZ, farZ, &matOut);
    return matOut;
}
/**
 * @brief  右手座標系射影行列の作成
 * @param  [in]     fovY     : Y 方向への視野角 (ラジアン単位)
 * @param  [in]     aspect   : アスペクト比
 * @param  [in]     nearZ    : 近くのビュー平面の Z 値
 * @param  [in]     farZ     : 遠くのビュー平面の Z 値
 * @return 演算結果を格納する行列
 */
inline Matrix Matrix::PerspectiveFovRH(float fovY, float aspect, float nearZ, float farZ)
{
    Matrix matOut;
    LNMatrix_PerspectiveFovRH(fovY, aspect, nearZ, farZ, &matOut);
    return matOut;
}
/**
 * @brief  左手座標系正射影行列の作成
 * @param  [in]     width    : ビューの幅
 * @param  [in]     height   : ビューの高さ
 * @param  [in]     nearZ    : 近くのビュー平面の Z 値
 * @param  [in]     farZ     : 遠くのビュー平面の Z 値
 * @return 演算結果を格納する行列
 */
inline Matrix Matrix::OrthoLH(float width, float height, float nearZ, float farZ)
{
    Matrix matOut;
    LNMatrix_OrthoLH(width, height, nearZ, farZ, &matOut);
    return matOut;
}
/**
 * @brief  右手座標系正射影行列の作成
 * @param  [in]     width    : ビューの幅
 * @param  [in]     height   : ビューの高さ
 * @param  [in]     nearZ    : 近くのビュー平面の Z 値
 * @param  [in]     farZ     : 遠くのビュー平面の Z 値
 * @return 演算結果を格納する行列
 */
inline Matrix Matrix::OrthoRH(float width, float height, float nearZ, float farZ)
{
    Matrix matOut;
    LNMatrix_OrthoRH(width, height, nearZ, farZ, &matOut);
    return matOut;
}
/**
 * @brief  右方向を示す 3D ベクトルの取得
 * @return 結果を格納する Vector3 変数
 */
inline Vector3 Matrix::GetRight()
{
    Vector3 outVec;
    LNMatrix_GetRight(this, &outVec);
    return outVec;
}
/**
 * @brief  上方向を示す 3D ベクトルの取得
 * @return 結果を格納する Vector3 変数
 */
inline Vector3 Matrix::GetUp()
{
    Vector3 outVec;
    LNMatrix_GetUp(this, &outVec);
    return outVec;
}
/**
 * @brief  正面方向を示す 3D ベクトルの取得
 * @return 結果を格納する Vector3 変数
 */
inline Vector3 Matrix::GetFront()
{
    Vector3 outVec;
    LNMatrix_GetFront(this, &outVec);
    return outVec;
}
/**
 * @brief  位置を示す 3D ベクトルの取得
 * @return 結果を格納する Vector3 変数
 */
inline Vector3 Matrix::GetPosition()
{
    Vector3 outVec;
    LNMatrix_GetPosition(this, &outVec);
    return outVec;
}
/**
 * @brief  回転行列からオイラー角を計算する
 * @param  [in]     mat      : 処理の基になる行列
 * @return 各軸の回転角度(ラジアン)を格納する Vector3 変数
 */
inline Vector3 Matrix::ToEuler(const Matrix& mat)
{
    Vector3 outVec;
    LNMatrix_ToEuler(&mat, &outVec);
    return outVec;
}
/**
 * @brief  回転行列からオイラー角を計算する
 * @return 各軸の回転角度(ラジアン)を格納する Vector3 変数
 */
inline Vector3 Matrix::ToEuler()
{
    Vector3 outVec;
    LNMatrix_ToEuler(this, &outVec);
    return outVec;
}
/**
 * @brief  行列をスケーリング、回転、移動成分に分解する
 * @param  [in]     mat      : 処理の基になる行列
 * @param  [out]    scale    : 各軸のスケーリング量を格納する Vector3 変数
 * @param  [out]    rot      : 各軸の回転角度(ラジアン)を格納する Vector3 変数
 * @param  [out]    trans    : 各軸の平行移動量を格納する Vector3 変数
 */
inline void Matrix::Decompose(const Matrix& mat, Vector3* scale, Matrix* rot, Vector3* trans)
{
    LNMatrix_Decompose(&mat, scale, rot, trans);
}
/**
 * @brief  右手系⇔左手系の変換
 * @param  [in]     mat      : 処理の元になる行列
 * @return 演算結果を格納する行列
 */
inline Matrix Matrix::TransformBasis(const Matrix& mat)
{
    Matrix matOut;
    LNMatrix_TransformBasis(&mat, &matOut);
    return matOut;
}
/**
 * @brief  右手系⇔左手系の変換
 * @return 演算結果を格納する行列
 */
inline Matrix Matrix::TransformBasis()
{
    Matrix matOut;
    LNMatrix_TransformBasis(this, &matOut);
    return matOut;
}
/**
 * @brief  クォータニオンに値を設定する
 * @param  [in]     x        : X 値
 * @param  [in]     y        : Y 値
 * @param  [in]     z        : Z 値
 * @param  [in]     w        : W 値
 */
inline void Quaternion::Set(float x, float y, float z, float w)
{
    LNQuaternion_Set(this, x, y, z, w);
}
/**
 * @brief  単位クォータニオンを作成する
 * @return 結果を格納する Quaternion 変数
 */
inline Quaternion Quaternion::Identity()
{
    Quaternion qua;
    LNQuaternion_Identity(&qua);
    return qua;
}
/**
 * @brief  任意の軸を回転軸にして回転するクォータニオンを作成する
 * @param  [in]     axis     : 回転軸を示す Vector3 変数
 * @param  [in]     r        : 回転角度 (ラジアン)
 * @return 演算結果を格納する Quaternion 変数
 * @details
 *         与えられた軸が正規化されていない場合は正規化された軸を使用します。
 */
inline Quaternion Quaternion::RotationAxis(const Vector3& axis, float r)
{
    Quaternion outQua;
    LNQuaternion_RotationAxis(&axis, r, &outQua);
    return outQua;
}
/**
 * @brief  回転行列からクォータニオンを作成する
 * @param  [in]     mat      : 処理の基になる行列
 * @return 演算結果を格納する Quaternion 変数
 */
inline Quaternion Quaternion::RotationMatrix(const Matrix& mat)
{
    Quaternion outQua;
    LNQuaternion_RotationMatrix(&mat, &outQua);
    return outQua;
}
/**
 * @brief  ヨー、ピッチ、およびロールを指定してクォータニオンを作成する
 * @param  [in]     yaw      : y 軸を中心とするヨー (ラジアン単位)
 * @param  [in]     pitch    : x 軸を中心とするピッチ (ラジアン単位)
 * @param  [in]     roll     : z 軸を中心とするロール (ラジアン単位)
 * @return 演算結果を格納する Quaternion 変数
 */
inline Quaternion Quaternion::RotationYawPitchRoll(float yaw, float pitch, float roll)
{
    Quaternion outQua;
    LNQuaternion_RotationYawPitchRoll(yaw, pitch, roll, &outQua);
    return outQua;
}
/**
 * @brief  クォータニオンを正規化する
 * @param  [in]     qua      : 処理の基になるクォータニオン
 * @return 演算結果を格納する Quaternion 変数
 */
inline Quaternion Quaternion::Normalize(const Quaternion& qua)
{
    Quaternion outQua;
    LNQuaternion_Normalize(&qua, &outQua);
    return outQua;
}
/**
 * @brief  クォータニオンを正規化する
 * @return 演算結果を格納する Quaternion 変数
 */
inline Quaternion Quaternion::Normalize()
{
    Quaternion outQua;
    LNQuaternion_Normalize(this, &outQua);
    return outQua;
}
/**
 * @brief  クォータニオンの共役を計算する
 * @param  [in]     qua      : 処理の基になるクォータニオン
 * @return 演算結果を格納する Quaternion 変数
 */
inline Quaternion Quaternion::Conjugate(const Quaternion& qua)
{
    Quaternion outQua;
    LNQuaternion_Conjugate(&qua, &outQua);
    return outQua;
}
/**
 * @brief  クォータニオンの共役を計算する
 * @return 演算結果を格納する Quaternion 変数
 */
inline Quaternion Quaternion::Conjugate()
{
    Quaternion outQua;
    LNQuaternion_Conjugate(this, &outQua);
    return outQua;
}
/**
 * @brief  2 つのクォータニオンの積を計算する
 * @param  [in]     qua1     : 処理の基になるクォータニオン
 * @param  [in]     qua2     : 処理の基になるクォータニオン
 * @return 演算結果を格納する Quaternion 変数
 */
inline Quaternion Quaternion::Multiply(const Quaternion& qua1, const Quaternion& qua2)
{
    Quaternion outQua;
    LNQuaternion_Multiply(&qua1, &qua2, &outQua);
    return outQua;
}
/**
 * @brief  2 つのクォータニオンを球面線形補間する
 * @param  [in]     qua1     : 処理の基になるクォータニオン
 * @param  [in]     qua2     : 処理の基になるクォータニオン
 * @param  [in]     t        : 補間値
 * @return 演算結果を格納する Quaternion 変数
 */
inline Quaternion Quaternion::Slerp(const Quaternion& qua1, const Quaternion& qua2, float t)
{
    Quaternion outQua;
    LNQuaternion_Slerp(&qua1, &qua2, t, &outQua);
    return outQua;
}
/**
 * @brief  現在の時間を乱数シードとして、乱数オブジェクトを作成する
 */
inline  Random::Random()
    : ReferenceObject(LN_INTERNAL_BLOCK)
{
    LNRandom_Create(&mHandle);
}
/**
 * @brief  乱数シードを指定し、乱数オブジェクトを作成する
 * @param  [in]     seed     : シード値
 */
inline  Random::Random(int seed)
    : ReferenceObject(LN_INTERNAL_BLOCK)
{
    LNRandom_CreateSeed(&mHandle, seed);
}
/**
 * @brief  乱数オブジェクトに乱数シードを設定する
 * @param  [in]     seed     : シード値
 * @details
 *         random が NULL の場合、グローバル乱数オブジェクトに設定します。
 */
inline void Random::SetSeed(int seed)
{
    LNRandom_SetSeed(mHandle, seed);
}
/**
 * @brief  整数値乱数の取得
 * @return 整数値を格納する変数
 * @details
 *         random が NULL の場合、グローバル乱数オブジェクトから取得します。
 */
inline int Random::GetInt()
{
    int outValue;
    LNRandom_GetInt(mHandle, &outValue);
    return outValue;
}
/**
 * @brief  範囲を指定して、整数値乱数を取得する ( minValue <= x < maxValue )
 * @param  [in]     minValue : 生成される乱数の下限値
 * @param  [in]     maxValue : 生成される乱数の上限値
 * @return 整数値を格納する変数
 * @details
 *         random が NULL の場合、グローバル乱数オブジェクトから取得します。
 */
inline int Random::GetIntRange(int minValue, int maxValue)
{
    int outValue;
    LNRandom_GetIntRange(mHandle, minValue, maxValue, &outValue);
    return outValue;
}
/**
 * @brief  範囲を指定して、整数値乱数を取得する (center ± width/2)
 * @param  [in]     center   : 生成される乱数の中央値
 * @param  [in]     width    : 生成される乱数の振れ幅
 * @return 整数値を格納する変数
 * @details
 *         random が NULL の場合、グローバル乱数オブジェクトから取得します。
 */
inline int Random::GetIntDeflect(int center, int width)
{
    int outValue;
    LNRandom_GetIntDeflect(mHandle, center, width, &outValue);
    return outValue;
}
/**
 * @brief  ファイルを開く
 * @param  [in]     filePath : ファイル名
 * @param  [in]     mode     : ファイルを開く方法または作成する方法
 * @param  [in]     access   : ファイルにアクセスする方法
 * @details
 *         ファイルの読み込みが終了したら LNFileStream_Close または LNObject_Release でファイルを閉じる必要があります。
 */
inline  FileStream::FileStream(const lnChar* filePath, FileMode mode, FileAccess access)
    : ReferenceObject(LN_INTERNAL_BLOCK)
{
    LNFileStream_Create(&mHandle, filePath, (lnFileMode)mode, (lnFileAccess)access);
}
/**
 * @brief  ファイルを閉じる
 */
inline void FileStream::Close()
{
    LNFileStream_Close(mHandle);
}
/**
 * @brief  ファイルサイズを取得する
 * @return ファイルサイズ (バイト単位) を格納する変数
 */
inline int FileStream::GetSize()
{
    int size;
    LNFileStream_GetSize(mHandle, &size);
    return size;
}
/**
 * @brief  ファイルポインタの位置を取得する
 * @return ファイルポインタの位置 (バイト単位) を格納する変数
 */
inline int FileStream::GetPosition()
{
    int position;
    LNFileStream_GetPosition(mHandle, &position);
    return position;
}
/**
 * @brief  ファイルポインタの位置を設定する
 * @param  [in]     offset   : 読み込んだデータを格納するバッファ
 * @param  [in]     origin   : 起点
 */
inline void FileStream::Seek(int offset, SeekOrigin origin)
{
    LNFileStream_Seek(mHandle, offset, (lnSeekOrigin)origin);
}
/**
 * @brief  ファイルからデータを読み込む
 * @param  [in,out] buffer   : 読み込んだデータを格納するバッファ
 * @param  [in]     readSize : 読み込むサイズ (バイト数)
 * @return 読み込んだサイズ (バイト数) を格納する変数
 */
inline int FileStream::Read(void* buffer, int readSize)
{
    int validSize;
    LNFileStream_Read(mHandle, buffer, readSize, &validSize);
    return validSize;
}
/**
 * @brief  ファイルの存在を調べる
 * @param  [in]     filePath : ファイルパス
 * @return 結果を格納する変数 (LN_TRUE = 存在する / LN_FALSE = 存在しない)
 * @details
 *         ディレクトリと全ての暗号化アーカイブを対象として確認します。
 */
inline bool FileStream::Exists(const lnChar* filePath)
{
    lnBool result;
    LNFileStream_Exists(filePath, &result);
    return result != LN_FALSE;
}
/**
 * @brief  ボタンが現在押されているかを判定する
 * @param  [in]     button   : 仮想ボタン番号 (0～19)
 * @return 状態を格納する変数
 * @details
 *         0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
 */
inline bool Input::IsPress(int button)
{
    lnBool state;
    LNInput_IsPress(button, &state);
    return state != LN_FALSE;
}
/**
 * @brief  仮想ゲームコントローラのボタンが新しく押されたかを判定する
 * @param  [in]     button   : 仮想ボタン番号 (0～19)
 * @return 状態を格納する変数
 * @details
 *         0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
 */
inline bool Input::IsTrigger(int button)
{
    lnBool state;
    LNInput_IsTrigger(button, &state);
    return state != LN_FALSE;
}
/**
 * @brief  仮想ゲームコントローラのボタンが離されたかを判定する
 * @param  [in]     button   : 仮想ボタン番号 (0～19)
 * @return 状態を格納する変数
 * @details
 *         0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
 */
inline bool Input::IsOffTrigger(int button)
{
    lnBool state;
    LNInput_IsOffTrigger(button, &state);
    return state != LN_FALSE;
}
/**
 * @brief  仮想ゲームコントローラのボタンが新しく押された瞬間とリピート状態を判定する
 * @param  [in]     button   : 仮想ボタン番号 (0～19)
 * @return 状態を格納する変数
 * @details
 *         0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
 */
inline bool Input::IsRepeat(int button)
{
    lnBool state;
    LNInput_IsRepeat(button, &state);
    return state != LN_FALSE;
}
/**
 * @brief  仮想ゲームコントローラの指定ボタンの入力をアナログ値(0.0～1.0)で取得する
 * @param  [in]     button   : 仮想ボタン番号 (0～19)
 * @return アナログ値を格納する変数
 * @details
 *         0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
 */
inline float Input::GetPower(int button)
{
    float state;
    LNInput_GetPower(button, &state);
    return state;
}
/**
 * @brief  仮想ゲームコントローラの軸の状態(スティックの左右と上下)の取得
 * @param  [in]     axis     : 軸番号 (0～1)
 * @return 軸の状態を格納する2Dベクトル
 * @details
 *         0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
 */
inline Vector2 Input::GetAxisState(int axis)
{
    Vector2 state;
    LNInput_GetAxisState(axis, &state);
    return state;
}
inline VirtualPad* VirtualPad::GetVirtualPad(int index)
{
    static std::vector< RefPtr<VirtualPad> > pads;
    if (pads.size() < (size_t)(index + 1))
    {
        pads.resize(index);
    }
    if (pads[index] == NULL)
    {
        lnHandle handle;
        LNVirtualPad_GetVirtualPad(index, &handle);
        pads[index]._NewAndAttachHandleAddRef(handle);
    }   
    return pads[index];
}
/**
 * @brief  仮想ゲームコントローラのボタンが現在押されているかを判定する
 * @param  [in]     button   : 仮想ボタン番号 (0～19)
 * @return 状態を格納する変数
 */
inline bool VirtualPad::IsPress(int button)
{
    lnBool state;
    LNVirtualPad_IsPress(mHandle, button, &state);
    return state != LN_FALSE;
}
/**
 * @brief  仮想ゲームコントローラのボタンが新しく押されたかを判定する
 * @param  [in]     button   : 仮想ボタン番号 (0～19)
 * @return 状態を格納する変数
 */
inline bool VirtualPad::IsTrigger(int button)
{
    lnBool state;
    LNVirtualPad_IsTrigger(mHandle, button, &state);
    return state != LN_FALSE;
}
/**
 * @brief  仮想ゲームコントローラのボタンが離されたかを判定する
 * @param  [in]     button   : 仮想ボタン番号 (0～19)
 * @return 状態を格納する変数
 */
inline bool VirtualPad::IsOffTrigger(int button)
{
    lnBool state;
    LNVirtualPad_IsOffTrigger(mHandle, button, &state);
    return state != LN_FALSE;
}
/**
 * @brief  仮想ゲームコントローラのボタンが新しく押された瞬間とリピート状態を判定する
 * @param  [in]     button   : 仮想ボタン番号 (0～19)
 * @return 状態を格納する変数
 */
inline bool VirtualPad::IsRepeat(int button)
{
    lnBool state;
    LNVirtualPad_IsRepeat(mHandle, button, &state);
    return state != LN_FALSE;
}
/**
 * @brief  仮想ゲームコントローラの指定ボタンの入力をアナログ値(0.0～1.0)で取得する
 * @param  [in]     button   : 仮想ボタン番号 (0～19)
 * @return アナログ値を格納する変数
 */
inline float VirtualPad::GetPower(int button)
{
    float state;
    LNVirtualPad_GetPower(mHandle, button, &state);
    return state;
}
/**
 * @brief  仮想ゲームコントローラの軸の状態(スティックの左右と上下)の取得
 * @param  [in]     virtualPad : 仮想ゲームコントローラハンドル
 * @param  [in]     axis     : 軸番号 (0～1)
 * @return 軸の状態を格納する2Dベクトル
 */
inline Vector2 VirtualPad::GetAxisState(const VirtualPad* virtualPad, int axis)
{
    Vector2 state;
    LNVirtualPad_GetAxisState((virtualPad != NULL) ? virtualPad->GetHandle() : NULL, axis, &state);
    return state;
}
/**
 * @brief  仮想ゲームコントローラの新しく押されたデバイスボタン番号の取得
 * @return デバイスボタン番号を格納する構造体
 * @details
 *         キーボード、マウス、ジョイパッド(スティックを含む)の
 *         何らかのボタンが押された場合、そのボタンを識別するための
 *         番号を返します。(デバイスボタン)<br>
 *         この番号は主にキーコンフィグで使用します。<br>
 *         この関数は isTrigger() 同様に動作し、ボタンが押された瞬間だけ
 *         ボタン番号を返します。<br>
 *         それ以外は 0 を返します。
 */
inline int VirtualPad::GetPressedDeviceButton()
{
    int deviceButton;
    LNVirtualPad_GetPressedDeviceButton(mHandle, &deviceButton);
    return deviceButton;
}
/**
 * @brief  デバイスボタン番号に対応するデバイスの種類を取得する
 * @param  [in]     deviceButton : デバイスボタン番号
 * @return デバイスボタンの種類を格納する構造体
 */
inline DeviceButtonType VirtualPad::GetDeviceButtonType(int deviceButton)
{
    lnDeviceButtonType type;
    LNVirtualPad_GetDeviceButtonType(deviceButton, &type);
    return (DeviceButtonType)type;
}
/**
 * @brief  仮想ゲームコントローラのボタンにデバイスボタンを割り当てる
 * @param  [in]     virtualButton : 割り当て先の仮想ボタン番号
 * @param  [in]     deviceButton : 割り当てるデバイスボタン番号
 * @details
 *         この関数はキーコンフィグ用の関数です。<br>
 *         デバイスボタン(キーボードのキーや、ジョイパッドのボタン)を仮想ボタンに割り当てます。<br>
 *         <br>
 *         基本的に LNVirtualPad_GetPressedDeviceButton() とセットで使い、LNVirtualPad_GetDeviceButtonType()から
 *         受け取ったデバイスボタン番号をこの関数の deviceButton に渡します。<br>
 */
inline void VirtualPad::AttachVirtualButton(int virtualButton, int deviceButton)
{
    LNVirtualPad_AttachVirtualButton(mHandle, virtualButton, deviceButton);
}
/**
 * @brief  仮想ゲームコントローラのデバイスボタンの割り当てを解除する
 * @param  [in]     virtualButton : 解除対象の仮想ボタン番号
 * @param  [in]     deviceButton : 解除するデバイスボタン番号
 * @details
 *         解除対象の仮想ボタンに deviceButton で示されるデバイスボタンが割り当てられている場合、解除します。<br>
 *         割り当てられていない場合は何もしません。<br>
 *         deviceButton に 0 を渡すと、virtualButton の全ての割り当てを解除します。
 */
inline void VirtualPad::DetachVirtualButton(int virtualButton, int deviceButton)
{
    LNVirtualPad_DetachVirtualButton(mHandle, virtualButton, deviceButton);
}
/**
 * @brief  BGM を演奏する
 * @param  [in]     filePath : ファイル名
 * @param  [in]     volume   : ボリューム (0 ～ 100)
 * @param  [in]     pitch    : ピッチ (50 ～ 200)
 * @param  [in]     fadeTime : フェードインにかける時間 (ミリ秒)
 */
inline void Audio::PlayBGM(const lnChar* filePath, int volume, int pitch, int fadeTime)
{
    LNAudio_PlayBGM(filePath, volume, pitch, fadeTime);
}
/**
 * @brief  メモリ上の音声データから BGM を演奏する
 * @param  [in]     data     : メモリ上の音声ファイルデータ
 * @param  [in]     dataSize : データサイズ (バイト単位)
 * @param  [in]     volume   : ボリューム (0 ～ 100)
 * @param  [in]     pitch    : ピッチ (50 ～ 200)
 * @param  [in]     fadeTime : フェードインにかける時間 (ミリ秒)
 */
inline void Audio::PlayBGMMem(const void* data, int dataSize, int volume, int pitch, int fadeTime)
{
    LNAudio_PlayBGMMem(data, dataSize, volume, pitch, fadeTime);
}
/**
 * @brief  BGM の演奏を停止する
 * @param  [in]     fadeTime : フェードアウトにかける時間 (ミリ秒)
 */
inline void Audio::StopBGM(int fadeTime)
{
    LNAudio_StopBGM(fadeTime);
}
/**
 * @brief  BGS を演奏する
 * @param  [in]     filePath : ファイル名
 * @param  [in]     volume   : ボリューム (0 ～ 100)
 * @param  [in]     pitch    : ピッチ (50 ～ 200)
 * @param  [in]     fadeTime : フェードインにかける時間 (ミリ秒)
 */
inline void Audio::PlayBGS(const lnChar* filePath, int volume, int pitch, int fadeTime)
{
    LNAudio_PlayBGS(filePath, volume, pitch, fadeTime);
}
/**
 * @brief  メモリ上の音声データから BGS を演奏する
 * @param  [in]     data     : メモリ上の音声ファイルデータ
 * @param  [in]     dataSize : データサイズ (バイト単位)
 * @param  [in]     volume   : ボリューム (0 ～ 100)
 * @param  [in]     pitch    : ピッチ (50 ～ 200)
 * @param  [in]     fadeTime : フェードインにかける時間 (ミリ秒)
 */
inline void Audio::PlayBGSMem(const void* data, int dataSize, int volume, int pitch, int fadeTime)
{
    LNAudio_PlayBGSMem(data, dataSize, volume, pitch, fadeTime);
}
/**
 * @brief  BGS の演奏を停止する
 * @param  [in]     fadeTime : フェードアウトにかける時間 (ミリ秒)
 */
inline void Audio::StopBGS(int fadeTime)
{
    LNAudio_StopBGS(fadeTime);
}
/**
 * @brief  ME を演奏する
 * @param  [in]     filePath : ファイル名
 * @param  [in]     volume   : ボリューム (0 ～ 100)
 * @param  [in]     pitch    : ピッチ (50 ～ 200)
 */
inline void Audio::PlayME(const lnChar* filePath, int volume, int pitch)
{
    LNAudio_PlayME(filePath, volume, pitch);
}
/**
 * @brief  メモリ上の音声データから ME を演奏する
 * @param  [in]     data     : メモリ上の音声ファイルデータ
 * @param  [in]     dataSize : データサイズ (バイト単位)
 * @param  [in]     volume   : ボリューム (0 ～ 100)
 * @param  [in]     pitch    : ピッチ (50 ～ 200)
 */
inline void Audio::PlayMEMem(const void* data, int dataSize, int volume, int pitch)
{
    LNAudio_PlayMEMem(data, dataSize, volume, pitch);
}
/**
 * @brief  ME の演奏を停止する
 */
inline void Audio::StopME()
{
    LNAudio_StopME();
}
/**
 * @brief  SE を演奏する
 * @param  [in]     filePath : ファイル名
 * @param  [in]     volume   : ボリューム (0 ～ 100)
 * @param  [in]     pitch    : ピッチ (50 ～ 200)
 */
inline void Audio::PlaySE(const lnChar* filePath, int volume, int pitch)
{
    LNAudio_PlaySE(filePath, volume, pitch);
}
/**
 * @brief  SE を演奏する (3D サウンド)
 * @param  [in]     filePath : ファイル名
 * @param  [in]     position : 3D 空間上の座標
 * @param  [in]     distance : 減衰距離
 * @param  [in]     volume   : ボリューム (0 ～ 100)
 * @param  [in]     pitch    : ピッチ (50 ～ 200)
 */
inline void Audio::PlaySE3D(const lnChar* filePath, const Vector3& position, float distance, int volume, int pitch)
{
    LNAudio_PlaySE3D(filePath, &position, distance, volume, pitch);
}
/**
 * @brief  SE を演奏する (3D サウンド)
 * @param  [in]     filePath : ファイル名
 * @param  [in]     x        : 3D 空間上の X 座標
 * @param  [in]     y        : 3D 空間上の Y 座標
 * @param  [in]     z        : 3D 空間上の Z 座標
 * @param  [in]     distance : 減衰距離
 * @param  [in]     volume   : ボリューム (0 ～ 100)
 * @param  [in]     pitch    : ピッチ (50 ～ 200)
 */
inline void Audio::PlaySE3D(const lnChar* filePath, float x, float y, float z, float distance, int volume, int pitch)
{
    LNAudio_PlaySE3DXYZ(filePath, x, y, z, distance, volume, pitch);
}
/**
 * @brief  メモリ上の音声データから SE を演奏する
 * @param  [in]     data     : メモリ上の音声ファイルデータ
 * @param  [in]     dataSize : データサイズ (バイト単位)
 * @param  [in]     volume   : ボリューム (0 ～ 100)
 * @param  [in]     pitch    : ピッチ (50 ～ 200)
 */
inline void Audio::PlaySEMem(const void* data, int dataSize, int volume, int pitch)
{
    LNAudio_PlaySEMem(data, dataSize, volume, pitch);
}
/**
 * @brief  メモリ上の音声データから SE を演奏する (3D サウンド)
 * @param  [in]     data     : メモリ上の音声ファイルデータ
 * @param  [in]     dataSize : データサイズ (バイト単位)
 * @param  [in]     position : 3D 空間上の座標
 * @param  [in]     distance : 減衰距離
 * @param  [in]     volume   : ボリューム (0 ～ 100)
 * @param  [in]     pitch    : ピッチ (50 ～ 200)
 */
inline void Audio::PlaySE3DMem(const void* data, int dataSize, const Vector3& position, float distance, int volume, int pitch)
{
    LNAudio_PlaySE3DMem(data, dataSize, &position, distance, volume, pitch);
}
/**
 * @brief  メモリ上の音声データから SE を演奏する (3D サウンド)
 * @param  [in]     data     : メモリ上の音声ファイルデータ
 * @param  [in]     dataSize : データサイズ (バイト単位)
 * @param  [in]     x        : 3D 空間上の X 座標
 * @param  [in]     y        : 3D 空間上の Y 座標
 * @param  [in]     z        : 3D 空間上の Z 座標
 * @param  [in]     distance : 減衰距離
 * @param  [in]     volume   : ボリューム (0 ～ 100)
 * @param  [in]     pitch    : ピッチ (50 ～ 200)
 */
inline void Audio::PlaySE3DMem(const void* data, int dataSize, float x, float y, float z, float distance, int volume, int pitch)
{
    LNAudio_PlaySE3DMemXYZ(data, dataSize, x, y, z, distance, volume, pitch);
}
/**
 * @brief  すべての SE の演奏を停止する
 */
inline void Audio::StopSE()
{
    LNAudio_StopSE();
}
/**
 * @brief  3D 空間の1メートル相当の距離の設定
 * @param  [in]     distance : 距離
 */
inline void Audio::SetMetreUnitDistance(float distance)
{
    LNAudio_SetMetreUnitDistance(distance);
}
/**
 * @brief  再生中のBGMの音量を設定する (フェードアウト中は無効)
 * @param  [in]     volume   : ボリューム (0 ～ 100)
 * @param  [in]     fadeTime : フェードアウトにかける時間 (ミリ秒)
 */
inline void Audio::SetBGMVolume(int volume, int fadeTime)
{
    LNAudio_SetBGMVolume(volume, fadeTime);
}
/**
 * @brief  再生中のBGSの音量を設定する (フェードアウト中は無効)
 * @param  [in]     volume   : ボリューム (0 ～ 100)
 * @param  [in]     fadeTime : フェードアウトにかける時間 (ミリ秒)
 */
inline void Audio::SetBGSVolume(int volume, int fadeTime)
{
    LNAudio_SetBGSVolume(volume, fadeTime);
}
/**
 * @brief  [使用非推奨] BGM、BGS、ME として再生されているサウンドオブジェクトの取得
 * @param  [in]     type     : 取得するサウンドの種類
 * @details
 *         この関数は、BGM、BGS、ME の音量、ピッチの変更、及び
 *         再生位置の取得のために用意されています。
 *         この関数から取得したサウンドハンドルに対して
 *         Release、Play、Stop、Pause を行わないでください。
 */
inline void Audio::GetInternalGameSound(InternalGameSound type)
{
    LNAudio_GetInternalGameSound((lnInternalGameSound)type);
}
/**
 * @brief  リスナーの位置の設定
 * @param  [in]     position : 3D 空間上の座標
 */
inline void SoundListener::SetPosition(const Vector3& position)
{
    LNSoundListener_SetPosition(&position);
}
/**
 * @brief  リスナーの位置の設定
 * @param  [in]     x        : 3D 空間上の X 座標
 * @param  [in]     y        : 3D 空間上の Y 座標
 * @param  [in]     z        : 3D 空間上の Z 座標
 */
inline void SoundListener::SetPosition(float x, float y, float z)
{
    LNSoundListener_SetPositionXYZ(x, y, z);
}
/**
 * @brief  リスナーの正面方向の設定
 * @param  [in]     direction : 向き
 */
inline void SoundListener::SetDirection(const Vector3& direction)
{
    LNSoundListener_SetDirection(&direction);
}
/**
 * @brief  リスナーの正面方向の設定
 * @param  [in]     x        : 向きの X 成分
 * @param  [in]     y        : 向きの Y 成分
 * @param  [in]     z        : 向きの Z 成分
 */
inline void SoundListener::SetDirection(float x, float y, float z)
{
    LNSoundListener_SetDirectionXYZ(x, y, z);
}
/**
 * @brief  リスナーの上方向の設定 (正面方向とは直交であること)
 * @param  [in]     direction : 上方向
 */
inline void SoundListener::SetUpDirection(const Vector3& direction)
{
    LNSoundListener_SetUpDirection(&direction);
}
/**
 * @brief  リスナーの上方向の設定 (正面方向とは直交であること)
 * @param  [in]     x        : 向きの X 成分
 * @param  [in]     y        : 向きの Y 成分
 * @param  [in]     z        : 向きの Z 成分
 */
inline void SoundListener::SetUpDirection(float x, float y, float z)
{
    LNSoundListener_SetUpDirectionXYZ(x, y, z);
}
/**
 * @brief  リスナーの速度の設定
 * @param  [in]     velocity : 速度
 */
inline void SoundListener::Velocity(const Vector3& velocity)
{
    LNSoundListener_Velocity(&velocity);
}
/**
 * @brief  リスナーの速度の設定
 * @param  [in]     x        : 速度の X 成分
 * @param  [in]     y        : 速度の Y 成分
 * @param  [in]     z        : 速度の Z 成分
 */
inline void SoundListener::Velocity(float x, float y, float z)
{
    LNSoundListener_VelocityXYZ(x, y, z);
}
/**
 * @brief  ファイルから音声を作成する
 * @param  [in]     filePath : ファイル名
 * @param  [in]     enable3d : true の場合、3D 音源として作成する
 * @param  [in]     playType : 音声の再生方法
 */
inline  Sound::Sound(const lnChar* filePath, bool enable3d, SoundPlayingType playType)
    : ReferenceObject(LN_INTERNAL_BLOCK)
{
    LNSound_Create(&mHandle, filePath, (enable3d) ? LN_TRUE : LN_FALSE, (lnSoundPlayingType)playType);
}
/**
 * @brief  メモリ上の音声ファイルデータから音声を作成する
 * @param  [in]     data     : メモリ上の音声データへのポインタ
 * @param  [in]     dataSize : データサイズ (バイト単位)
 * @param  [in]     enable3d : true の場合、3D 音源として作成する
 */
inline  Sound::Sound(const void* data, int dataSize, bool enable3d)
    : ReferenceObject(LN_INTERNAL_BLOCK)
{
    LNSound_CreateMem(&mHandle, data, dataSize, (enable3d) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  ボリュームの設定
 * @param  [in]     volume   : ボリューム (0～100)
 */
inline void Sound::SetVolume(int volume)
{
    LNSound_SetVolume(mHandle, volume);
}
/**
 * @brief  ボリュームの取得
 * @return ボリュームを格納する変数
 */
inline int Sound::GetVolume()
{
    int volume;
    LNSound_GetVolume(mHandle, &volume);
    return volume;
}
/**
 * @brief  ピッチの設定
 * @param  [in]     pitch    : ピッチ (50～200)
 */
inline void Sound::SetPitch(int pitch)
{
    LNSound_SetPitch(mHandle, pitch);
}
/**
 * @brief  ピッチの取得
 * @return ピッチを格納する変数
 */
inline int Sound::GetPitch()
{
    int pitch;
    LNSound_GetPitch(mHandle, &pitch);
    return pitch;
}
/**
 * @brief  ループ再生の有効設定
 * @param  [in]     loopEnable : LN_TRUE = ループ再生する / LN_FALSE = しない
 * @param  [in]     begin    : ループ領域の先頭位置 (サンプル数単位)
 * @param  [in]     length   : ループ領域長さ (サンプル数単位)
 * @details
 *         begin と length に 0 を指定すると、全体をループ領域として設定します。
 */
inline void Sound::SetLoopState(bool loopEnable, int begin, int length)
{
    LNSound_SetLoopState(mHandle, (loopEnable) ? LN_TRUE : LN_FALSE, begin, length);
}
/**
 * @brief  ループ再生が有効かを調べる
 * @return ループ再生の有無状態を格納する変数
 */
inline bool Sound::IsLoop()
{
    lnBool enabled;
    LNSound_IsLoop(mHandle, &enabled);
    return enabled != LN_FALSE;
}
/**
 * @brief  サウンドの再生状態を取得する
 * @return 状態を格納する変数
 */
inline SoundPlayingState Sound::GetPlayState()
{
    lnSoundPlayingState state;
    LNSound_GetPlayState(mHandle, &state);
    return (SoundPlayingState)state;
}
/**
 * @brief  音声を再生する
 */
inline void Sound::Play()
{
    LNSound_Play(mHandle);
}
/**
 * @brief  再生を停止する
 */
inline void Sound::Stop()
{
    LNSound_Stop(mHandle);
}
/**
 * @brief  一時停止
 * @param  [in]     pause    : LN_TRUE = 一時停止 / LN_FALSE = 一時停止解除
 */
inline void Sound::Pause(bool pause)
{
    LNSound_Pause(mHandle, (pause) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  音量のフェード
 * @param  [in]     targetVolume : 変更先の音量
 * @param  [in]     time     : フェードにかける時間 (ミリ秒)
 * @param  [in]     fadeState : フェード完了後の動作の指定
 */
inline void Sound::FadeVolume(int targetVolume, int time, SoundFadeState fadeState)
{
    LNSound_FadeVolume(mHandle, targetVolume, time, (lnSoundFadeState)fadeState);
}
/**
 * @brief  再生したサンプル数の取得
 * @return 再生したサンプル数を格納する変数
 */
inline int Sound::GetUnitsPlayed()
{
    int samples;
    LNSound_GetUnitsPlayed(mHandle, &samples);
    return samples;
}
/**
 * @brief  音声データ全体のサンプル数の取得
 * @return 音声データ全体のサンプル数を格納する変数
 */
inline int Sound::GetTotalUnits()
{
    int samples;
    LNSound_GetTotalUnits(mHandle, &samples);
    return samples;
}
/**
 * @brief  サンプリング周波数の取得
 * @return サンプリング周波数を格納する変数
 */
inline int Sound::GetUnitsParSecond()
{
    int frequency;
    LNSound_GetUnitsParSecond(mHandle, &frequency);
    return frequency;
}
/**
 * @brief  3D 音源かを調べる
 * @return 3D 音源かを示す値を格納する変数
 */
inline bool Sound::Is3DSound()
{
    lnBool enabled;
    LNSound_Is3DSound(mHandle, &enabled);
    return enabled != LN_FALSE;
}
/**
 * @brief  3D 音源としての位置を設定する
 * @param  [in]     position : 3D 空間上の座標
 */
inline void Sound::SetEmitterPosition(const Vector3& position)
{
    LNSound_SetEmitterPosition(mHandle, &position);
}
/**
 * @brief  3D 音源としての位置を設定する
 * @param  [in]     x        : 3D 空間上の X 座標
 * @param  [in]     y        : 3D 空間上の Y 座標
 * @param  [in]     z        : 3D 空間上の Z 座標
 */
inline void Sound::SetEmitterPosition(float x, float y, float z)
{
    LNSound_SetEmitterPositionXYZ(mHandle, x, y, z);
}
/**
 * @brief  3D 音源としての速度を設定する
 * @param  [in]     velocity : 速度
 */
inline void Sound::SetEmitterVelocity(const Vector3& velocity)
{
    LNSound_SetEmitterVelocity(mHandle, &velocity);
}
/**
 * @brief  3D 音源としての速度を設定する
 * @param  [in]     x        : 速度の X 成分
 * @param  [in]     y        : 速度の Y 成分
 * @param  [in]     z        : 速度の Z 成分
 */
inline void Sound::SetEmitterVelocity(float x, float y, float z)
{
    LNSound_SetEmitterVelocityXYZ(mHandle, x, y, z);
}
/**
 * @brief  3D 音源の減衰距離 (聴こえなくなる距離) を設定する
 * @param  [in]     distance : 距離
 */
inline void Sound::SetEmitterDistance(float distance)
{
    LNSound_SetEmitterDistance(mHandle, distance);
}
/**
 * @brief  テクスチャの作成
 * @param  [in]     width    : 幅 (ピクセル数)
 * @param  [in]     height   : 高さ (ピクセル数)
 * @details
 *         ミップマップレベル 1、フォーマット A8R8B8G8(透明色付フォーマット) で作成します。
 */
inline  Texture::Texture(int width, int height)
    : ReferenceObject(LN_INTERNAL_BLOCK)
{
    LNTexture_Create(&mHandle, width, height);
}
/**
 * @brief  テクスチャの作成
 * @param  [in]     filePath : テクスチャファイル名
 * @details
 *         サポートしているフォーマットは 
 *         .bmp、.dds、.dib、.jpg、.png、.tga の6種類です。<br>
 *         ミップマップレベル 1 で作成します。
 */
inline  Texture::Texture(const lnChar* filePath)
    : ReferenceObject(LN_INTERNAL_BLOCK)
{
    LNTexture_CreateFromFile(&mHandle, filePath);
}
/**
 * @brief  テクスチャの作成
 * @param  [in]     width    : 幅 (ピクセル数)
 * @param  [in]     height   : 高さ (ピクセル数)
 * @param  [in]     levels   : ミップマップレベル (0で最大レベルまで作成)
 * @param  [in]     format   : サーフェイスフォーマット
 */
inline  Texture::Texture(int width, int height, int levels, TextureFormat format)
    : ReferenceObject(LN_INTERNAL_BLOCK)
{
    LNTexture_CreateEx(&mHandle, width, height, levels, (lnTextureFormat)format);
}
/**
 * @brief  テクスチャの作成
 * @param  [in]     filePath : テクスチャファイル名
 * @param  [in]     colorKey : 透明色として扱う色
 * @param  [in]     levels   : ミップマップレベル (0で最大レベルまで作成)
 * @param  [in]     format   : サーフェイスフォーマット
 * @details
 *         サポートしているフォーマットは 
 *         .bmp、.dds、.dib、.jpg、.png、.tga の6種類です。
 */
inline  Texture::Texture(const lnChar* filePath, lnU32 colorKey, int levels, TextureFormat format)
    : ReferenceObject(LN_INTERNAL_BLOCK)
{
    LNTexture_CreateFromFileEx(&mHandle, filePath, colorKey, levels, (lnTextureFormat)format);
}
/**
 * @brief  テクスチャの幅の取得 (ピクセル単位)
 * @return テクスチャの幅を格納する変数
 */
inline int Texture::GetWidth()
{
    int width;
    LNTexture_GetWidth(mHandle, &width);
    return width;
}
/**
 * @brief  テクスチャの高さの取得 (ピクセル単位)
 * @return テクスチャの高さを格納する変数
 */
inline int Texture::GetHeight()
{
    int height;
    LNTexture_GetHeight(mHandle, &height);
    return height;
}
/**
 * @brief  テクスチャの実際の幅の取得 (ピクセル単位)
 * @return テクスチャの実際の幅を格納する変数
 */
inline int Texture::GetRealWidth()
{
    int width;
    LNTexture_GetRealWidth(mHandle, &width);
    return width;
}
/**
 * @brief  テクスチャの実際の高さの取得 (ピクセル単位)
 * @return テクスチャの実際の高さを格納する変数
 */
inline int Texture::GetRealHeight()
{
    int height;
    LNTexture_GetRealHeight(mHandle, &height);
    return height;
}
/**
 * @brief  テクスチャをクリアする
 * @param  [in]     color    : 塗りつぶす色
 */
inline void Texture::Clear(const Color& color)
{
    LNTexture_Clear(mHandle, &color);
}
/**
 * @brief  テクスチャをクリアする
 * @param  [in]     r        : 塗りつぶす色の R 値
 * @param  [in]     g        : 塗りつぶす色の G 値
 * @param  [in]     b        : 塗りつぶす色の B 値
 * @param  [in]     a        : 塗りつぶす色の A 値
 */
inline void Texture::ClearRGBA(int r, int g, int b, int a)
{
    LNTexture_ClearRGBA(mHandle, r, g, b, a);
}
/**
 * @brief  テクスチャへ文字列を描画する
 * @param  [in]     text     : 描画する文字列
 * @param  [in]     rect     : 描画領域 (はみだした場合にクリッピングされます)
 * @param  [in]     font     : 描画に使用するフォント (0 の場合、デフォルトのフォントを使用する)
 * @param  [in]     length   : 描画文字数 (-1 の場合、文字列全体を描画する)
 * @param  [in]     align    : 描画基準位置の指定
 * @details
 *         レンダリングターゲットと深度バッファテクスチャには描画できません。
 */
inline void Texture::DrawText(const lnChar* text, const Rect& rect, const Font* font, int length, TextAlign align)
{
    LNTexture_DrawText(mHandle, text, &rect, (font != NULL) ? font->GetHandle() : NULL, length, (lnTextAlign)align);
}
/**
 * @brief  テクスチャへ文字列を描画する
 * @param  [in]     text     : 描画する文字列
 * @param  [in]     x        : 描画先の左上 X 座標
 * @param  [in]     y        : 描画先の左上 Y 座標
 * @param  [in]     font     : 描画に使用するフォント (0 の場合、デフォルトのフォントを使用する)
 * @param  [in]     length   : 描画文字数 (-1 の場合、文字列全体を描画する)
 * @param  [in]     align    : 描画基準位置の指定
 * @details
 *         レンダリングターゲットと深度バッファテクスチャには描画できません。
 */
inline void Texture::DrawText(const lnChar* text, int x, int y, const Font* font, int length, TextAlign align)
{
    LNTexture_DrawTextXY(mHandle, text, x, y, (font != NULL) ? font->GetHandle() : NULL, length, (lnTextAlign)align);
}
/**
 * @brief  テクスチャのブロック転送
 * @param  [in]     x        : 転送先 X 座標
 * @param  [in]     y        : 転送先 Y 座標
 * @param  [in]     srcTexture : 転送元テクスチャハンドル
 * @param  [in]     srcRect  : 転送元領域
 * @details
 *         srcTexture で指定されたテクスチャから texture へブロック転送を行います。<br>
 *         レンダリングターゲットと深度バッファテクスチャは
 *         転送元・先のどちらにも指定できません。
 */
inline void Texture::BitBlt(int x, int y, const Texture* srcTexture, const Rect& srcRect)
{
    LNTexture_BitBlt(mHandle, x, y, (srcTexture != NULL) ? srcTexture->GetHandle() : NULL, &srcRect);
}
/**
 * @brief  フォントの作成
 * @details
 *         デフォルトのフォントを使用してフォントを作成します。
 */
inline  Font::Font()
    : ReferenceObject(LN_INTERNAL_BLOCK)
{
    LNFont_Create(&mHandle);
}
/**
 * @brief  ビットマップフォントの作成
 * @param  [in]     filePath : 画像ファイル名
 */
inline  Font::Font(const lnChar* filePath)
    : ReferenceObject(LN_INTERNAL_BLOCK)
{
    LNFont_CreateBitmapFontFromFile(&mHandle, filePath);
}
/**
 * @brief  フォントのコピーの作成
 * @param  [in]     srcFont  : コピー元のフォントハンドル
 */
inline  Font::Font(const Font* srcFont)
    : ReferenceObject(LN_INTERNAL_BLOCK)
{
    LNFont_CreateCopy(&mHandle, (srcFont != NULL) ? srcFont->GetHandle() : NULL);
}
/**
 * @brief  指定フォントをデフォルトフォントに設定する
 * @param  [in]     font     : フォントハンドル
 * @details
 *         テクスチャの DrawText 等でフォントハンドルを NULL にした場合、
 *         このフォントが使われるようになります。
 */
inline void Font::SetDefaultFont(const Font* font)
{
    LNFont_SetDefaultFont((font != NULL) ? font->GetHandle() : NULL);
}
/**
 * @brief  フォント名の設定
 * @param  [in]     fontName : フォント名
 */
inline void Font::SetName(const lnChar* fontName)
{
    LNFont_SetName(mHandle, fontName);
}
/**
 * @brief  フォント名の取得
 * @return フォント名を格納する変数
 */
inline const lnChar* Font::GetName()
{
    const lnChar* fontName;
    LNFont_GetName(mHandle, &fontName);
    return fontName;
}
/**
 * @brief  フォントサイズの設定
 * @param  [in]     size     : 設定するサイズ
 */
inline void Font::SetSize(int size)
{
    LNFont_SetSize(mHandle, size);
}
/**
 * @brief  フォントサイズの取得
 * @return フォントサイズを格納する変数
 */
inline int Font::GetSize()
{
    int size;
    LNFont_GetSize(mHandle, &size);
    return size;
}
/**
 * @brief  フォントカラーの設定
 * @param  [in]     color    : 設定する色
 */
inline void Font::SetColor(const Color& color)
{
    LNFont_SetColor(mHandle, &color);
}
/**
 * @brief  フォントカラーの取得
 * @return 色を格納する変数
 */
inline Color Font::GetColor()
{
    Color color;
    LNFont_GetColor(mHandle, &color);
    return color;
}
/**
 * @brief  エッジカラーの設定
 * @param  [in]     color    : 設定する色
 */
inline void Font::SetEdgeColor(const Color& color)
{
    LNFont_SetEdgeColor(mHandle, &color);
}
/**
 * @brief  エッジカラーの取得
 * @return 色を格納する変数
 */
inline Color Font::GetEdgeColor()
{
    Color color;
    LNFont_GetEdgeColor(mHandle, &color);
    return color;
}
/**
 * @brief  エッジの幅の設定
 * @param  [in]     size     : エッジの幅 (0 でエッジ無効)
 */
inline void Font::SetEdgeSize(int size)
{
    LNFont_SetEdgeSize(mHandle, size);
}
/**
 * @brief  エッジの幅の取得
 * @return エッジの幅を格納する変数
 */
inline int Font::GetEdgeSize()
{
    int size;
    LNFont_GetEdgeSize(mHandle, &size);
    return size;
}
/**
 * @brief  太文字有無の設定
 * @param  [in]     enabled  : 太文字の有無
 */
inline void Font::SetBold(bool enabled)
{
    LNFont_SetBold(mHandle, (enabled) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  太文字有無の取得
 * @return 太文字の有無を格納する変数
 */
inline bool Font::IsBold()
{
    lnBool enabled;
    LNFont_IsBold(mHandle, &enabled);
    return enabled != LN_FALSE;
}
/**
 * @brief  イタリック体有無の設定
 * @param  [in]     enabled  : イタリック体の有無
 */
inline void Font::SetItalic(bool enabled)
{
    LNFont_SetItalic(mHandle, (enabled) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  イタリック体有無の取得
 * @return イタリック体の有無を格納する変数
 */
inline bool Font::IsItalic()
{
    lnBool enabled;
    LNFont_IsItalic(mHandle, &enabled);
    return enabled != LN_FALSE;
}
/**
 * @brief  アンチエイリアス有無の設定
 * @param  [in]     enabled  : アンチエイリアスの有無
 */
inline void Font::SetAntiAlias(bool enabled)
{
    LNFont_SetAntiAlias(mHandle, (enabled) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  アンチエイリアス有無の取得
 * @return アンチエイリアス有無を格納する変数
 */
inline bool Font::IsAntiAlias()
{
    lnBool enabled;
    LNFont_IsAntiAlias(mHandle, &enabled);
    return enabled != LN_FALSE;
}
/**
 * @brief  文字列を描画したときのサイズ (ピクセル単位) の取得
 * @param  [in]     font     : フォントハンドル
 * @param  [in]     text     : サイズ計算に使用する文字列
 * @param  [in]     length   : 文字数 (-1 で文字列全体)
 * @return 結果を格納する変数
 */
inline Size Font::GetTextSize(const Font* font, const lnChar* text, int length)
{
    Size outSize;
    LNFont_GetTextSize((font != NULL) ? font->GetHandle() : NULL, text, length, &outSize);
    return outSize;
}
/**
 * @brief  ノード名の取得
 * @return ノードの名前を格納する変数
 */
inline const lnChar* SceneNode::GetName()
{
    const lnChar* outName;
    LNSceneNode_GetName(mHandle, &outName);
    return outName;
}
/**
 * @brief  ノード名の設定
 * @param  [in]     name     : 新しい名前
 */
inline void SceneNode::SetName(const lnChar* name)
{
    LNSceneNode_SetName(mHandle, name);
}
/**
 * @brief  座標の設定
 * @param  [in]     vec      : 座標
 */
inline void SceneNode::SetPosition(const Vector3& vec)
{
    LNSceneNode_SetPosition(mHandle, &vec);
}
/**
 * @brief  座標の設定
 * @param  [in]     x        : X 座標
 * @param  [in]     y        : Y 座標
 * @param  [in]     z        : Z 座標
 */
inline void SceneNode::SetPosition(float x, float y, float z)
{
    LNSceneNode_SetPositionXYZ(mHandle, x, y, z);
}
/**
 * @brief  座標の取得
 * @return 座標を格納するベクトル
 */
inline Vector3 SceneNode::GetPosition()
{
    Vector3 vec;
    LNSceneNode_GetPosition(mHandle, &vec);
    return vec;
}
/**
 * @brief  座標の取得
 * @param  [out]    x        : X 座標を格納する変数
 * @param  [out]    y        : Y 座標を格納する変数
 * @param  [out]    z        : Z 座標を格納する変数
 */
inline void SceneNode::GetPosition(float* x, float* y, float* z)
{
    LNSceneNode_GetPositionXYZ(mHandle, x, y, z);
}
/**
 * @brief  回転角度の設定 (ラジアン単位)
 * @param  [in]     vec      : 各軸の回転角度
 */
inline void SceneNode::SetAngle(const Vector3& vec)
{
    LNSceneNode_SetAngle(mHandle, &vec);
}
/**
 * @brief  回転角度の設定 (ラジアン単位)
 * @param  [in]     x        : X 軸の回転角度
 * @param  [in]     y        : Y 軸の回転角度
 * @param  [in]     z        : Z 軸の回転角度
 */
inline void SceneNode::SetAngle(float x, float y, float z)
{
    LNSceneNode_SetAngleXYZ(mHandle, x, y, z);
}
/**
 * @brief  回転角度の取得 (ラジアン単位)
 * @return 回転角度を格納するベクトル
 */
inline Vector3 SceneNode::GetAngle()
{
    Vector3 vec;
    LNSceneNode_GetAngle(mHandle, &vec);
    return vec;
}
/**
 * @brief  回転角度の取得 (ラジアン単位)
 * @param  [out]    x        : X 軸の回転角度を格納する変数
 * @param  [out]    y        : Y 軸の回転角度を格納する変数
 * @param  [out]    z        : Z 軸の回転角度を格納する変数
 */
inline void SceneNode::GetAngle(float* x, float* y, float* z)
{
    LNSceneNode_GetAngleXYZ(mHandle, x, y, z);
}
/**
 * @brief  拡大率の設定 (ラジアン単位)
 * @param  [in]     vec      : 拡大率
 */
inline void SceneNode::SetScale(const Vector3& vec)
{
    LNSceneNode_SetScale(mHandle, &vec);
}
/**
 * @brief  拡大率の設定 (ラジアン単位)
 * @param  [in]     x        : X 方向の拡大率
 * @param  [in]     y        : Y 方向の拡大率
 * @param  [in]     z        : Z 方向の拡大率
 */
inline void SceneNode::SetScale(float x, float y, float z)
{
    LNSceneNode_SetScaleXYZ(mHandle, x, y, z);
}
/**
 * @brief  拡大率の取得 (ラジアン単位)
 * @return 拡大率を格納するベクトル
 */
inline Vector3 SceneNode::GetScale()
{
    Vector3 vec;
    LNSceneNode_GetScale(mHandle, &vec);
    return vec;
}
/**
 * @brief  拡大率の取得 (ラジアン単位)
 * @param  [out]    x        : X 方向の拡大率を格納する変数
 * @param  [out]    y        : Y 方向の拡大率を格納する変数
 * @param  [out]    z        : Z 方向の拡大率を格納する変数
 */
inline void SceneNode::GetScale(float* x, float* y, float* z)
{
    LNSceneNode_GetScaleXYZ(mHandle, x, y, z);
}
/**
 * @brief  移動・回転・拡大の原点の設定
 * @param  [in]     vec      : 各軸のオフセット
 */
inline void SceneNode::SetCenter(const Vector3& vec)
{
    LNSceneNode_SetCenter(mHandle, &vec);
}
/**
 * @brief  移動・回転・拡大の原点の設定
 * @param  [in]     x        : X 方向のオフセット
 * @param  [in]     y        : Y 方向のオフセット
 * @param  [in]     z        : Z 方向のオフセット
 */
inline void SceneNode::SetCenter(float x, float y, float z)
{
    LNSceneNode_SetCenterXYZ(mHandle, x, y, z);
}
/**
 * @brief  移動・回転・拡大の原点の取得
 * @return 各軸のオフセットを格納するベクトル
 */
inline Vector3 SceneNode::GetCenter()
{
    Vector3 vec;
    LNSceneNode_GetCenter(mHandle, &vec);
    return vec;
}
/**
 * @brief  移動・回転・拡大の原点の取得
 * @param  [out]    x        : X 方向のオフセットを格納する変数
 * @param  [out]    y        : Y 方向のオフセットを格納する変数
 * @param  [out]    z        : Z 方向のオフセットを格納する変数
 */
inline void SceneNode::GetCenter(float* x, float* y, float* z)
{
    LNSceneNode_GetCenterXYZ(mHandle, x, y, z);
}
/**
 * @brief  座標変換行列の設定
 * @param  [in]     mat      : 座標変換行列
 * @details
 *         単位行列ではない行列が設定されてる場合、
 *         SetPosition()、SetAngle()、SetScale() で設定された値は無効です。
 */
inline void SceneNode::SetMatrix(const Matrix& mat)
{
    LNSceneNode_SetMatrix(mHandle, &mat);
}
/**
 * @brief  座標変換行列の取得
 * @return 座標変換行列を格納する変数
 */
inline Matrix SceneNode::GetMatrix()
{
    Matrix mat;
    LNSceneNode_GetMatrix(mHandle, &mat);
    return mat;
}
/**
 * @brief  回転順序の設定
 * @param  [in]     rotOrder : 回転順序
 */
inline void SceneNode::SetRotateOrder(RotationOrder rotOrder)
{
    LNSceneNode_SetRotateOrder(mHandle, (lnRotationOrder)rotOrder);
}
/**
 * @brief  回転順序の取得
 * @return 回転順序を示す値を格納する変数
 */
inline RotationOrder SceneNode::GetRotateOrder()
{
    lnRotationOrder outOrder;
    LNSceneNode_GetRotateOrder(mHandle, &outOrder);
    return (RotationOrder)outOrder;
}
/**
 * @brief  配置方法の設定
 * @param  [in]     type     : 配置方法
 */
inline void SceneNode::SetPlacementType(PlacementType type)
{
    LNSceneNode_SetPlacementType(mHandle, (lnPlacementType)type);
}
/**
 * @brief  配置方法の取得
 * @return 配置方法を格納する変数
 */
inline PlacementType SceneNode::GetPlacementType()
{
    lnPlacementType outType;
    LNSceneNode_GetPlacementType(mHandle, &outType);
    return (PlacementType)outType;
}
/**
 * @brief  子ノードを追加する
 * @param  [in]     childSceneNode : 子ノードとして追加するノード
 */
inline void SceneNode::AddChild(const SceneNode* childSceneNode)
{
    LNSceneNode_AddChild(mHandle, (childSceneNode != NULL) ? childSceneNode->GetHandle() : NULL);
}
/**
 * @brief  子ノードを取り除く
 * @param  [in]     childSceneNode : 取り外す子ノード
 */
inline void SceneNode::RemoveChild(const SceneNode* childSceneNode)
{
    LNSceneNode_RemoveChild(mHandle, (childSceneNode != NULL) ? childSceneNode->GetHandle() : NULL);
}
/**
 * @brief  フレーム更新
 * @param  [in]     elapsedTime : 経過させる時間
 * @details
 *         エフェクトやアニメーションの状態を elapsedTime 分進めます。
 *         通常、1フレームに1度呼び出します。<br>
 *         自動更新が有効になっている場合はライブラリの
 *         フレーム更新を行った時に自動的に呼ばれるため、
 *         この関数を直接呼び出す必要はありません。<br>
 *         自動更新はデフォルトで有効です。
 */
inline void SceneNode::Update(float elapsedTime)
{
    LNSceneNode_Update(mHandle, elapsedTime);
}
/**
 * @brief  自動更新有無の設定
 * @param  [in]     enabled  : LN_TRUE=有効 / LN_FALSE=無効
 * @details
 *         デフォルト値は LN_TRUE です。
 */
inline void SceneNode::SetEnableAutoUpdate(bool enabled)
{
    LNSceneNode_SetEnableAutoUpdate(mHandle, (enabled) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  自動更新有無の取得
 * @return 自動更新の有無を格納する変数
 */
inline bool SceneNode::IsEnableAutoUpdate()
{
    lnBool outEnabled;
    LNSceneNode_IsEnableAutoUpdate(mHandle, &outEnabled);
    return outEnabled != LN_FALSE;
}
/**
 * @brief  可視状態の設定
 * @param  [in]     enabled  : LN_TRUE=表示する / LN_FALSE=表示しない
 */
inline void VisualNode::SetVisible(bool enabled)
{
    LNVisualNode_SetVisible(mHandle, (enabled) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  不透明度の設定
 * @param  [in]     opacity  : 不透明度 (0.0～1.0)
 */
inline void VisualNode::SetOpacity(float opacity)
{
    LNVisualNode_SetOpacity(mHandle, opacity);
}
/**
 * @brief  乗算色の設定
 * @param  [in]     materialIndex : 設定先のマテリアル番号
 * @param  [in]     color    : 設定する色
 */
inline void VisualNode::SetColorScale(int materialIndex, const ColorF& color)
{
    LNVisualNode_SetColorScale(mHandle, materialIndex, &color);
}
/**
 * @brief  ブレンドカラーの設定
 * @param  [in]     materialIndex : 設定先のマテリアル番号
 * @param  [in]     color    : 設定する色
 */
inline void VisualNode::SetBlendColor(int materialIndex, const ColorF& color)
{
    LNVisualNode_SetBlendColor(mHandle, materialIndex, &color);
}
/**
 * @brief  色調の設定
 * @param  [in]     materialIndex : 設定先のマテリアル番号
 * @param  [in]     tone     : 設定する色調
 */
inline void VisualNode::SetTone(int materialIndex, const Tone& tone)
{
    LNVisualNode_SetTone(mHandle, materialIndex, &tone);
}
/**
 * @brief  UV 変換行列の設定
 * @param  [in]     materialIndex : 設定先のマテリアル番号
 * @param  [in]     matrix   : 設定する行列
 */
inline void VisualNode::SetUVTransform(int materialIndex, const Matrix& matrix)
{
    LNVisualNode_SetUVTransform(mHandle, materialIndex, &matrix);
}
/**
 * @brief  合成方法の設定
 * @param  [in]     mode     : 合成方法
 */
inline void VisualNode::SetBlendMode(BlendMode mode)
{
    LNVisualNode_SetBlendMode(mHandle, (lnBlendMode)mode);
}
/**
 * @brief  カリング方法の設定
 * @param  [in]     mode     : カリング方法
 */
inline void VisualNode::SetCullingMode(CullingMode mode)
{
    LNVisualNode_SetCullingMode(mHandle, (lnCullingMode)mode);
}
/**
 * @brief  深度テストの有効設定
 * @param  [in]     enabled  : 深度テストの有無
 */
inline void VisualNode::SetEnableDepthTest(bool enabled)
{
    LNVisualNode_SetEnableDepthTest(mHandle, (enabled) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  深度書き込みの有効設定
 * @param  [in]     enabled  : 深度書き込みの有無
 */
inline void VisualNode::SetEnableDepthWrite(bool enabled)
{
    LNVisualNode_SetEnableDepthWrite(mHandle, (enabled) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  シェーダの設定
 * @param  [in]     shader   : シェーダハンドル
 */
inline void VisualNode::SetShader(const Shader* shader)
{
    LNVisualNode_SetShader(mHandle, (shader != NULL) ? shader->GetHandle() : NULL);
}
/**
 * @brief  スプライトオブジェクトの作成 (2D空間へ配置する)
 * @param  [in]     texture  : スプライトが表示するテクスチャのハンドル
 */
inline  Sprite::Sprite(const Texture* texture)
    : VisualNode(LN_INTERNAL_BLOCK)
{
    LNSprite_Create(&mHandle, (texture != NULL) ? texture->GetHandle() : NULL);
}
/**
 * @brief  スプライトが表示するテクスチャの設定
 * @param  [in]     texture  : テクスチャハンドル
 */
inline void Sprite::SetTexture(const Texture* texture)
{
    LNSprite_SetTexture(mHandle, (texture != NULL) ? texture->GetHandle() : NULL);
}
/**
 * @brief  スプライトのサイズの設定
 * @param  [in]     width    : 設定する幅
 * @param  [in]     height   : 設定する高さ
 */
inline void Sprite::SetSize(float width, float height)
{
    LNSprite_SetSize(mHandle, width, height);
}
/**
 * @brief  テクスチャの転送矩形の設定
 * @param  [in]     rect     : 転送元矩形
 */
inline void Sprite::SetSrcRect(const Rect& rect)
{
    LNSprite_SetSrcRect(mHandle, &rect);
}
/**
 * @brief  テクスチャの転送矩形の設定
 * @param  [in]     x        : 転送元の左上の X 座標
 * @param  [in]     y        : 転送元の左上の Y 座標
 * @param  [in]     w        : 転送元の幅
 * @param  [in]     h        : 転送元の高さ
 */
inline void Sprite::SetSrcRect(int x, int y, int w, int h)
{
    LNSprite_SetSrcRectXYWH(mHandle, x, y, w, h);
}
/**
 * @brief  テクスチャの転送矩形の取得
 * @return 矩形を格納する変数
 */
inline Rect Sprite::GetSrcRect()
{
    Rect rect;
    LNSprite_GetSrcRect(mHandle, &rect);
    return rect;
}
/**
 * @brief  テクスチャの転送矩形の取得
 * @param  [out]    x        : 転送元の左上の X 座標 を格納する変数
 * @param  [out]    y        : 転送元の左上の Y 座標 を格納する変数
 * @param  [out]    w        : 転送元の幅 を格納する変数
 * @param  [out]    h        : 転送元の高さ を格納する変数
 */
inline void Sprite::GetSrcRect(int* x, int* y, int* w, int* h)
{
    LNSprite_GetSrcRectXYWH(mHandle, x, y, w, h);
}
/**
 * @brief  左右反転有無の設定
 * @param  [in]     enabled  : LN_TRUE=反転する / LN_FALSE=反転しない
 */
inline void Sprite::SetFlipH(bool enabled)
{
    LNSprite_SetFlipH(mHandle, (enabled) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  左右反転有無の取得
 * @return 左右反転の有無を示す値を格納する変数 (LN_TRUE=反転する / LN_FALSE=反転しない)
 */
inline bool Sprite::IsFlipH()
{
    lnBool outEnabled;
    LNSprite_IsFlipH(mHandle, &outEnabled);
    return outEnabled != LN_FALSE;
}
/**
 * @brief  上下反転有無の設定
 * @param  [in]     enabled  : LN_TRUE=反転する / LN_FALSE=反転しない
 */
inline void Sprite::SetFlipV(bool enabled)
{
    LNSprite_SetFlipV(mHandle, (enabled) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  上下反転有無の取得
 * @return 左右反転の有無を示す値を格納する変数
 */
inline bool Sprite::IsFlipV()
{
    lnBool outEnabled;
    LNSprite_IsFlipV(mHandle, &outEnabled);
    return outEnabled != LN_FALSE;
}
/**
 * @brief  スプライトパスオブジェクトの作成
 * @param  [in]     maxSpriteCount : このパスが一度の画面更新で描画できるスプライトの数
 * @param  [in]     coord    : 描画先の座標空間
 */
inline  SpritePath::SpritePath(int maxSpriteCount, Coord coord)
    : VisualNode(LN_INTERNAL_BLOCK)
{
    LNSpritePath_Create(&mHandle, maxSpriteCount, (lnCoord)coord);
}
/**
 * @brief  以降のスプライトの描画で使用するブレンドモードの設定
 * @param  [in]     blendMode : 合成方法
 */
inline void SpritePath::SetPathBlendMode(BlendMode blendMode)
{
    LNSpritePath_SetPathBlendMode(mHandle, (lnBlendMode)blendMode);
}
/**
 * @brief  以降のスプライトの描画で使用する座標変換行列の設定
 * @param  [in]     matrix   : 座標変換行列
 */
inline void SpritePath::SetPathTransform(const Matrix& matrix)
{
    LNSpritePath_SetPathTransform(mHandle, &matrix);
}
/**
 * @brief  スプライトの描画
 * @param  [in]     position : 描画先座標
 * @param  [in]     center   : 位置や回転の中心となるオフセット
 * @param  [in]     size     : 描画先スプライトのサイズ
 * @param  [in]     texture  : 描画するテクスチャハンドル
 * @param  [in]     srcRect  : テクスチャからの転送矩形
 * @param  [in]     vertexColors : 四角形の各頂点の乗算色 (要素数 4 の配列。NULL の場合は考慮しない)
 * @param  [in]     frontAxis : スプライトの正面方法
 * @details
 *         center は 2D/3D で基準位置が異なります。
 *         2D空間へ描画するときはスプライトの左上、
 *         3D空間へ描画するときはスプライトの中央となります。<br>
 *         front は、2Dの時は通常 LN_FRONTAXIS_RZ を指定します。
 */
inline void SpritePath::DrawSprite(const Vector3& position, const Vector3& center, const Vector2& size, const Texture* texture, const Rect& srcRect, const ColorF& vertexColors, FrontAxis frontAxis)
{
    LNSpritePath_DrawSprite(mHandle, &position, &center, &size, (texture != NULL) ? texture->GetHandle() : NULL, &srcRect, &vertexColors, (lnFrontAxis)frontAxis);
}
/**
 * @brief  モデルオブジェクトの作成
 * @param  [in]     filePath : メッシュファイル名
 * @param  [in]     meshCreateFlags : 
 */
inline  Model::Model(const lnChar* filePath, MeshCreateFlags meshCreateFlags)
    : VisualNode(LN_INTERNAL_BLOCK)
{
    LNModel_Create(&mHandle, filePath, (lnMeshCreateFlags)meshCreateFlags);
}
/**
 * @brief  エフェクトオブジェクトの作成 (3D空間へ配置する)
 * @param  [in]     filePath : エフェクトファイル名 (Effekseer 形式)
 */
inline  Effect::Effect(const lnChar* filePath)
    : VisualNode(LN_INTERNAL_BLOCK)
{
    LNEffect_Create(&mHandle, filePath);
}
/**
 * @brief  エフェクトの再生を開始する
 * @param  [in]     overlap  : LN_TRUE = 再生中のエフェクトを停止しない / LN_FALSE = 再生中のエフェクトを停止する
 */
inline void Effect::Play(bool overlap)
{
    LNEffect_Play(mHandle, (overlap) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  エフェクトを停止する
 */
inline void Effect::Stop()
{
    LNEffect_Stop(mHandle);
}
/**
 * @brief  エフェクトが再生中かを確認する
 * @return 再生中かを示す値を格納する変数 (LN_TRUE=再生中 / LN_FALSE=停止中)
 */
inline bool Effect::IsPlaying()
{
    lnBool playing;
    LNEffect_IsPlaying(mHandle, &playing);
    return playing != LN_FALSE;
}
/**
 * @brief  タイルマップの作成
 */
inline  Tilemap::Tilemap()
    : VisualNode(LN_INTERNAL_BLOCK)
{
    LNTilemap_Create(&mHandle);
}
/**
 * @brief  タイルマップのマップデータを設定する
 * @param  [in]     intTable : マップデータとする整数テーブルハンドル
 */
inline void Tilemap::SetMapData(const IntTable* intTable)
{
    LNTilemap_SetMapData(&mHandle, (intTable != NULL) ? intTable->GetHandle() : NULL);
}
/**
 * @brief  タイルマップのタイルセットとして使用するテクスチャを設定する
 * @param  [in]     texture  : 設定するテクスチャハンドル
 */
inline void Tilemap::SetTilesetTexture(const Texture* texture)
{
    LNTilemap_SetTilesetTexture(&mHandle, (texture != NULL) ? texture->GetHandle() : NULL);
}
/**
 * @brief  タイルセットテクスチャの横方向のタイル数を設定する
 * @param  [in]     count    : タイル数
 */
inline void Tilemap::SetTilesetHorizontalTileCount(int count)
{
    LNTilemap_SetTilesetHorizontalTileCount(&mHandle, count);
}
/**
 * @brief  タイルマップのタイルひとつ分のピクセルサイズを設定する
 * @param  [in]     size     : ピクセル数 (正方形。縦横同一サイズ)
 */
inline void Tilemap::SetTilePixelSize(int size)
{
    LNTilemap_SetTilePixelSize(&mHandle, size);
}
/**
 * @brief  シェーダの作成
 * @param  [in]     filePath : シェーダファイル名
 */
inline  Shader::Shader(const lnChar* filePath)
    : ReferenceObject(LN_INTERNAL_BLOCK)
{
    LNShader_CreateFromFile(&mHandle, filePath);
}
/**
 * @brief  デフォルトで生成されるビューペインの取得
 * @return ビューペインハンドルを格納する変数
 */
inline ViewPane* ViewPane::GetDefaultViewPane()
{
    static RefPtr<ViewPane> mGetDefaultViewPane;
    lnHandle viewPane;
    LNViewPane_GetDefaultViewPane(&viewPane);
    if (viewPane == NULL) {
        mGetDefaultViewPane.SafeRelease();
    }
    else if (mGetDefaultViewPane == NULL || mGetDefaultViewPane->GetHandle() != viewPane) {
        mGetDefaultViewPane._NewAndAttachHandleAddRef(viewPane);
    }
    return mGetDefaultViewPane;
}
/**
 * @brief  ViewPane の背景色を設定する
 * @param  [in]     color    : 背景色に設定する色
 */
inline void ViewPane::SetBackgroundColor(const ColorF& color)
{
    LNViewPane_SetBackgroundColor(mHandle, &color);
}
/**
 * @brief  ViewPane に設定されている Layer のリストを取得する
 * @return オブジェクトリストハンドル(Layer)を格納する変数
 */
inline ObjectList<Layer*>* ViewPane::GetLayers()
{
    lnHandle objectList;
    LNViewPane_GetLayers(mHandle, &objectList);
    if (objectList == NULL) {
        mGetLayers.SafeRelease();
    }
    else if (mGetLayers == NULL || mGetLayers->GetHandle() != objectList) {
        mGetLayers._NewAndAttachHandleAddRef(objectList);
    }
    return mGetLayers;
}
/**
 * @brief  【この機能は現在使用できません】
 * @param  [in]     priority : 優先度 (大きいほど手前(後から)に描画されます)
 */
inline void Layer::SetPriority(int priority)
{
    LNLayer_SetPriority(mHandle, priority);
}
/**
 * @brief  【この機能は現在使用できません】
 * @return 優先度を格納する変数
 */
inline int Layer::GetPriority()
{
    int outPriority;
    LNLayer_GetPriority(mHandle, &outPriority);
    return outPriority;
}
/**
 * @brief  フレーム更新
 * @param  [in]     elapsedTime : 経過させる時間
 * @details
 *         エフェクトやアニメーションの状態を elapsedTime 分進めます。
 *         通常、1フレームに1度呼び出します。<br>
 *         自動更新が有効になっている場合はライブラリの
 *         フレーム更新を行った時に自動的に呼ばれるため、
 *         この関数を直接呼び出す必要はありません。<br>
 *         自動更新はデフォルトで有効です。
 */
inline void ViewFilter::Update(float elapsedTime)
{
    LNViewFilter_Update(mHandle, elapsedTime);
}
/**
 * @brief  自動更新有無の設定
 * @param  [in]     enabled  : LN_TRUE=有効 / LN_FALSE=無効
 * @details
 *         デフォルト値は LN_TRUE です。
 */
inline void ViewFilter::SetEnableAutoUpdate(bool enabled)
{
    LNViewFilter_SetEnableAutoUpdate(mHandle, (enabled) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  自動更新有無の取得
 * @return 有無を示す値を格納する変数 (LN_TRUE=有効 / LN_FALSE=無効)
 */
inline bool ViewFilter::IsEnableAutoUpdate()
{
    lnBool outEnabled;
    LNViewFilter_IsEnableAutoUpdate(mHandle, &outEnabled);
    return outEnabled != LN_FALSE;
}
/**
 * @brief  色調を変更するビューフィルタを作成する
 */
inline  ToneFilter::ToneFilter()
    : ViewFilter(LN_INTERNAL_BLOCK)
{
    LNToneFilter_Create(&mHandle);
}
/**
 * @brief  色調を設定する
 * @param  [in]     tone     : 設定する色調
 * @param  [in]     duration : 変化にかける時間 (デフォルトではフレーム単位。タイムスケール依存)
 */
inline void ToneFilter::SetTone(const Tone& tone, float duration)
{
    LNToneFilter_SetTone(mHandle, &tone, duration);
}
/**
 * @brief  トランジションを行うビューフィルタを作成する
 */
inline  TransitionFilter::TransitionFilter()
    : ViewFilter(LN_INTERNAL_BLOCK)
{
    LNTransitionFilter_Create(&mHandle);
}
/**
 * @brief  トランジションの準備として、現在の画面を固定する
 * @details
 *         これ以降、LNTransitionFilter_Transition によるトランジション実行まで
 *         フィルタされた画面は更新されなくなります。
 */
inline void TransitionFilter::Freeze()
{
    LNTransitionFilter_Freeze(mHandle);
}
/**
 * @brief  固定された画面から現在の画面へのトランジションを実行する
 * @param  [in]     duration : 変化にかける時間 (デフォルトではフレーム単位。タイムスケール依存)
 * @param  [in]     maskTexture : トランジションに使用するマスク画像
 * @param  [in]     vague    : マスク画像の境界のあいまいさ
 * @details
 *         maskTexture が NULL の場合は通常のフェードになります。
 */
inline void TransitionFilter::Transition(double duration, const Texture* maskTexture, float vague)
{
    LNTransitionFilter_Transition(mHandle, duration, (maskTexture != NULL) ? maskTexture->GetHandle() : NULL, vague);
}
/**
 * @brief  ブラ―(残像)を適用するビューフィルタを作成する
 */
inline  BlurFilter::BlurFilter()
    : ViewFilter(LN_INTERNAL_BLOCK)
{
    LNBlurFilter_Create(&mHandle);
}
/**
 * @brief  色調を設定する
 * @param  [in]     duration : 適用時間 (デフォルトではフレーム単位。タイムスケール依存)
 * @param  [in]     power    : ブラーの強度 (ブラーイメージを描画する時のアルファ値。0.0～1.0 で指定)
 * @param  [in]     scale    : ブラーの大きさ (長さ)
 * @param  [in]     center   : scale を使用して放射ブラーを作るときの中心座標
 * @param  [in]     baseCamera : center の座標空間の基準となるカメラのハンドル
 * @details
 *         duration が 0 より大きい場合、その時間かけて徐々に強度を 0.0 に戻します。
 *         duration が 0 の場合は強度を自動で変化させません。<br>
 *         scale は元のスクリーンイメージに対して、1フレーム当たりどの程度拡大するかを指定します。<br>
 *         1.0 でスクリーンと等倍 (変化なし)、2.0 でスクリーンの2倍の大きさとなります。
 *         通常は 1.0～1.1 の範囲で指定するのが無難です。
 *         1.0 より小さい値を指定すると内側向きのブラーになります。<br>
 *         center は baseCamera が 2Dカメラであれば 2D 空間上の点、3Dカメラであれば3D空間上の点を指定します。
 *         baseCamera が NULL の場合、center は -1.0 ～ 1.0 で指定します。(0.0 が画面中央です)
 */
inline void BlurFilter::Blur(double duration, float power, float scale, const Vector3& center, const Camera* baseCamera)
{
    LNBlurFilter_Blur(mHandle, duration, power, scale, &center, (baseCamera != NULL) ? baseCamera->GetHandle() : NULL);
}
/**
 * @brief  デフォルトで作成される3Dカメラを取得する
 * @return カメラハンドルを格納する変数
 */
inline Camera* Camera::GetDefault3DCamera()
{
    static RefPtr<Camera> mGetDefault3DCamera;
    lnHandle camera;
    LNCamera_GetDefault3DCamera(&camera);
    if (camera == NULL) {
        mGetDefault3DCamera.SafeRelease();
    }
    else if (mGetDefault3DCamera == NULL || mGetDefault3DCamera->GetHandle() != camera) {
        mGetDefault3DCamera._NewAndAttachHandleAddRef(camera);
    }
    return mGetDefault3DCamera;
}
/**
 * @brief  デフォルトで作成される3Dカメラを取得する
 * @return カメラハンドルを格納する変数
 */
inline Camera* Camera::GetDefault2DCamera()
{
    static RefPtr<Camera> mGetDefault2DCamera;
    lnHandle camera;
    LNCamera_GetDefault2DCamera(&camera);
    if (camera == NULL) {
        mGetDefault2DCamera.SafeRelease();
    }
    else if (mGetDefault2DCamera == NULL || mGetDefault2DCamera->GetHandle() != camera) {
        mGetDefault2DCamera._NewAndAttachHandleAddRef(camera);
    }
    return mGetDefault2DCamera;
}
/**
 * @brief  カメラ姿勢の設定方法
 * @param  [in]     type     : 姿勢の設定方法
 */
inline void Camera::SetCameraType(CameraType type)
{
    LNCamera_SetCameraType(mHandle, (lnCameraType)type);
}
/**
 * @brief  カメラの位置を設定する
 * @param  [in]     position : 座標
 */
inline void Camera::SetPosition(const Vector3& position)
{
    LNCamera_SetPosition(mHandle, &position);
}
/**
 * @brief  カメラの位置を設定する
 * @param  [in]     x        : X 座標
 * @param  [in]     y        : Y 座標
 * @param  [in]     z        : Z 座標
 */
inline void Camera::SetPosition(float x, float y, float z)
{
    LNCamera_SetPositionXYZ(mHandle, x, y, z);
}
/**
 * @brief  カメラの注視点を設定する
 * @param  [in]     at       : 注視点座標
 */
inline void Camera::SetLookAt(const Vector3& at)
{
    LNCamera_SetLookAt(mHandle, &at);
}
/**
 * @brief  カメラの注視点を設定する
 * @param  [in]     x        : 注視点 X 座標
 * @param  [in]     y        : 注視点 Y 座標
 * @param  [in]     z        : 注視点 Z 座標
 */
inline void Camera::SetLookAt(float x, float y, float z)
{
    LNCamera_SetLookAtXYZ(mHandle, x, y, z);
}
/**
 * @brief  カメラの上方向を設定する
 * @param  [in]     up       : 上方向ベクトル
 */
inline void Camera::SetUpDirection(const Vector3& up)
{
    LNCamera_SetUpDirection(mHandle, &up);
}
/**
 * @brief  カメラの上方向を設定する
 * @param  [in]     x        : 上方向ベクトルの X 成分
 * @param  [in]     y        : 上方向ベクトルの Y 成分
 * @param  [in]     z        : 上方向ベクトルの Z 成分
 */
inline void Camera::SetUpDirection(float x, float y, float z)
{
    LNCamera_SetUpDirectionXYZ(mHandle, x, y, z);
}
/**
 * @brief  カメラの回転角度を設定する
 * @param  [in]     angle    : 各軸の回転角度 (ラジアン単位)
 */
inline void Camera::SetAngle(const Vector3& angle)
{
    LNCamera_SetAngle(mHandle, &angle);
}
/**
 * @brief  カメラの回転角度を設定する
 * @param  [in]     x        : X 軸の回転角度 (ラジアン単位)
 * @param  [in]     y        : Y 軸の回転角度 (ラジアン単位)
 * @param  [in]     z        : Z 軸の回転角度 (ラジアン単位)
 */
inline void Camera::SetAngle(float x, float y, float z)
{
    LNCamera_SetAngleXYZ(mHandle, x, y, z);
}
/**
 * @brief  カメラの姿勢行列を設定する
 * @param  [in]     matrix   : 変換行列
 * @details
 *         行列を使用し、位置(0,0,0)、正面(0,0,1)、上方向(0,1,0) を
 *         変換して姿勢を算出します。
 */
inline void Camera::SetMatrix(const Matrix& matrix)
{
    LNCamera_SetMatrix(mHandle, &matrix);
}
/**
 * @brief  カメラの視野角を設定する
 * @param  [in]     fov      : 視野角
 */
inline void Camera::SetFovY(float fov)
{
    LNCamera_SetFovY(mHandle, fov);
}
/**
 * @brief  カメラの最手前クリップ距離と最奥クリップ距離を設定する
 * @param  [in]     nearClip : 手前クリップ距離
 * @param  [in]     farClip  : 奥クリップ距離
 */
inline void Camera::SetNearFarClip(float nearClip, float farClip)
{
    LNCamera_SetNearFarClip(mHandle, nearClip, farClip);
}
/**
 * @brief  カメラのビュー行列を取得する
 * @return ビュー行列を格納する変数
 */
inline Matrix Camera::GetViewMatrix()
{
    Matrix outMatrix;
    LNCamera_GetViewMatrix(mHandle, &outMatrix);
    return outMatrix;
}
/**
 * @brief  カメラのプロジェクション行列を取得する
 * @return プロジェクション行列を格納する変数
 */
inline Matrix Camera::GetProjectionMatrix()
{
    Matrix outMatrix;
    LNCamera_GetProjectionMatrix(mHandle, &outMatrix);
    return outMatrix;
}
/**
 * @brief  ビュー行列とプロジェクション行列の積を取得する
 * @return ビュー行列とプロジェクション行列の積を格納する変数
 */
inline Matrix Camera::GetViewProjectionMatrix()
{
    Matrix outMatrix;
    LNCamera_GetViewProjectionMatrix(mHandle, &outMatrix);
    return outMatrix;
}
/**
 * @brief  ウィンドウタイトルの設定
 * @param  [in]     title    : ウィンドウタイトル文字列
 */
inline void Config::SetWindowTitle(const lnChar* title)
{
    LNConfig_SetWindowTitle(title);
}
/**
 * @brief  ウィンドウサイズの設定
 * @param  [in]     width    : クライアント領域の幅 (default:640)
 * @param  [in]     height   : クライアント領域の高さ (default:480)
 */
inline void Config::SetWindowSize(int width, int height)
{
    LNConfig_SetWindowSize(width, height);
}
/**
 * @brief  バックバッファサイズの設定
 * @param  [in]     width    : バックバッファの幅 (default:0)
 * @param  [in]     height   : バックバッファの高さ (default:0)
 * @details
 *         ウィンドウサイズとは異なるサイズでバックバッファを作成する時、
 *         バックバッファのサイズを指定します。
 *         サイズが 0 の場合、ウィンドウサイズと同じサイズで作成します。
 */
inline void Config::SetBackbufferSize(int width, int height)
{
    LNConfig_SetBackbufferSize(width, height);
}
/**
 * @brief  ウィンドウリサイズの有効設定
 * @param  [in]     enabled  : LN_TRUE の場合、ウィンドウ枠をD&Dすることでサイズ変更が可能 (default:LN_TRUE)
 */
inline void Config::SetWindowResizeEnabled(bool enabled)
{
    LNConfig_SetWindowResizeEnabled((enabled) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  アクセラレータキーとして割り当てるキーの設定
 * @param  [in]     acceleratorKey : アクセラレータの種別
 * @param  [in]     keyCode  : 割り当てるキーコード
 * @param  [in]     isShift  : LN_TRUE を指定すると、Shift キーを修飾子とする
 * @param  [in]     isCtrl   : LN_TRUE を指定すると、Ctrl キーを修飾子とする
 * @param  [in]     isAlt    : LN_TRUE を指定すると、Alt キーを修飾子とする
 * @details
 *         初期状態では以下のように割り当てられています。<br>
 *         - LN_ACCELERATORKEY_TOGGLE_FULLSCREEN<br>
 *         Alt + Enter キー
 *         - LN_ACCELERATORKEY_SHOW_DEBUG_STATISTICS<br>
 *         F3 キー
 */
inline void Config::SetAcceleratorKey(AcceleratorKey acceleratorKey, KeyCode keyCode, bool isShift, bool isCtrl, bool isAlt)
{
    LNConfig_SetAcceleratorKey((lnAcceleratorKey)acceleratorKey, (lnKeyCode)keyCode, (isShift) ? LN_TRUE : LN_FALSE, (isCtrl) ? LN_TRUE : LN_FALSE, (isAlt) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  ログファイルの出力設定
 * @param  [in]     flag     : LN_TRUE=ログファイルを出力する / LN_FALSE=しない (default:LN_FALSE)
 */
inline void Config::SetLogFileEnabled(bool flag)
{
    LNConfig_SetLogFileEnabled((flag) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  コンソールの割り当て設定
 * @param  [in]     flag     : LN_TRUE=割り当てる / LN_FALSE=割り当てない (default:LN_FALSE)
 */
inline void Config::SetConsoleEnabled(bool flag)
{
    LNConfig_SetConsoleEnabled((flag) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  ファイルを開くときにアクセスする暗号化アーカイブの登録
 * @param  [in]     filePath : アーカイブファイル名
 * @param  [in]     password : アーカイブファイルを開くためのパスワード
 */
inline void Config::RegisterArchive(const lnChar* filePath, const lnChar* password)
{
    LNConfig_RegisterArchive(filePath, password);
}
/**
 * @brief  ファイルへのアクセス制限の設定
 * @param  [in]     priority : 制限方法 (default:LN_FILEACCESSPRIORITY_DIRECTORY_FIRST)
 */
inline void Config::SetFileAccessPriority(FileAccessPriority priority)
{
    LNConfig_SetFileAccessPriority((lnFileAccessPriority)priority);
}
/**
 * @brief  DirectMusic の使用設定
 * @param  [in]     mode     : DirectMusic の使用方法 (default:LN_DMINITMODE_NOT_USE)
 */
inline void Config::SetDirectMusicInitMode(DirectMusicInitMode mode)
{
    LNConfig_SetDirectMusicInitMode((lnDirectMusicInitMode)mode);
}
/**
 * @brief  音声データの最大キャッシュ数の設定
 * @param  [in]     count    : 最大数 (default:32)
 */
inline void Config::SetSoundMaxCacheCount(int count)
{
    LNConfig_SetSoundMaxCacheCount(count);
}
/**
 * @brief  音声再生方法の自動選択の音声データバイト数閾値
 * @param  [in]     bytes    : 最大数 (default:100000)
 * @details
 *         再生方法(オンメモリorストリーミング)の自動選択時、
 *         音声データのバイト数がこの閾値よりも大きい場合ストリーミング、
 *         以下の場合はオンメモリで再生されます。
 *         MP3等の圧縮フォーマットではデコード後のサイズでチェックします。
 */
inline void Config::SoundPlayTypeThreshold(int bytes)
{
    LNConfig_SoundPlayTypeThreshold(bytes);
}
/**
 * @brief  フレームレートの設定
 * @param  [in]     frameRate : フレームレート (default:60)
 */
inline void Config::SetFrameRate(int frameRate)
{
    LNConfig_SetFrameRate(frameRate);
}
/**
 * @brief  ライブラリ更新時時のフレーム待ち処理の有効設定
 * @param  [in]     enabled  : LN_TRUE=有効 / LN_FALSE=無効 (default:LN_TRUE)
 */
inline void Config::SetEnableFrameWait(bool enabled)
{
    LNConfig_SetEnableFrameWait((enabled) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  自動更新時のタイムスケールの設定
 * @param  [in]     timeScale : タイムスケール (default:0.0)
 * @details
 *         SceneNode や ViewFilter の自動フレーム更新の際、
 *         前回フレームからの経過時間に乗算される値を設定します。
 *         この値を 1.0 にすると、エフェクト等の遷移時間を時間単位、
 *         フレームレートと同じ値にするとフレーム単位で指定できるようになります。
 *         0.0 の場合、フレームレートの値を使用します。
 */
inline void Config::SetAutoUpdateTimeScale(float timeScale)
{
    LNConfig_SetAutoUpdateTimeScale(timeScale);
}
/**
 * @brief  ライブラリで使用するフォントファイルを登録する
 * @param  [in]     filePath : フォントファイル名
 * @details
 *         登録したフォントファイルはフォントオブジェクト作成時に
 *         ファミリ名を指定することで使用できます。
 */
inline void Config::RegisterFontFile(const lnChar* filePath)
{
    LNConfig_RegisterFontFile(filePath);
}
/**
 * @brief  テクスチャの最大キャッシュ数の設定
 * @param  [in]     count    : 最大数 (default:32)
 */
inline void Config::SetTextureCacheSize(int count)
{
    LNConfig_SetTextureCacheSize(count);
}
/**
 * @brief  シェーダの最大キャッシュ数の設定
 * @param  [in]     count    : 最大数 (default:32)
 */
inline void Config::SetShaderCacheSize(int count)
{
    LNConfig_SetShaderCacheSize(count);
}
/**
 * @brief  例外エラー発生時のメッセージボックス表示設定
 * @param  [in]     enabled  : LN_TRUE の場合、表示する (default:LN_TRUE)
 */
inline void Config::SetExceptionMessageBoxEnabled(bool enabled)
{
    LNConfig_SetExceptionMessageBoxEnabled((enabled) ? LN_TRUE : LN_FALSE);
}
/**
 * @brief  ユーザー定義のウィンドウハンドルの設定
 * @param  [in]     hWnd     : ユーザー定義のウィンドウハンドル (default:NULL)
 */
inline void Config::SetWin32UserWindowHandle(lnIntPtr hWnd)
{
    LNConfig_SetWin32UserWindowHandle(hWnd);
}
inline void Manager::Initialize()
{
    TypeInfo::_RegisterTypeInfo();
    LManager_Initialize();
}
/**
 * @brief  ファイル入出力機能の初期化
 */
inline void Core::InitializeFileIO()
{
    LNCore_InitializeFileIO();
}
/**
 * @brief  音声機能の初期化
 * @details
 *         ファイル入出力機能も同時に初期化されます。
 */
inline void Core::InitializeAudio()
{
    LNCore_InitializeAudio();
}
/**
 * @brief  入力機能の初期化
 */
inline void Core::InitializeInput()
{
    LNCore_InitializeInput();
}
/**
 * @brief  ライブラリのフレーム更新
 * @details
 *         ライブラリの状態を更新し、時間を1フレーム分進めます。
 *         この関数は必ず1フレームに1度だけ呼び出す必要があります。
 */
inline void Core::Update()
{
    LNCore_Update();
}
/**
 * @brief  画面の更新タイミングのリセット
 * @details
 *         時間のかかる処理の後にこのメソッドを呼ぶことで、
 *         極端なフレームスキップが発生しないようにすることができます。
 */
inline void Core::ResetFrameDelay()
{
    LNCore_ResetFrameDelay();
}
/**
 * @brief  アプリケーションを終了するべきかを確認する
 * @return 終了要求の有無を格納する変数
 * @details
 *         ウィンドウのクローズボタンが押された場合等、
 *         アプリケーションを終了するべき時には LN_FALSE を返します。
 */
inline bool Core::IsEndRequested()
{
    lnBool requested;
    LNCore_IsEndRequested(&requested);
    return requested != LN_FALSE;
}
/**
 * @brief  ライブラリの終了処理
 */
inline void Core::Terminate()
{
    LNCore_Terminate();
}


inline ReferenceObject* TypeInfo::IntTableFactory(lnHandle handle)
{
    IntTable* obj = LNOTE_NEW IntTable(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::RandomFactory(lnHandle handle)
{
    Random* obj = LNOTE_NEW Random(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::FileStreamFactory(lnHandle handle)
{
    FileStream* obj = LNOTE_NEW FileStream(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::VirtualPadFactory(lnHandle handle)
{
    VirtualPad* obj = LNOTE_NEW VirtualPad(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::SoundFactory(lnHandle handle)
{
    Sound* obj = LNOTE_NEW Sound(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::TextureFactory(lnHandle handle)
{
    Texture* obj = LNOTE_NEW Texture(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::FontFactory(lnHandle handle)
{
    Font* obj = LNOTE_NEW Font(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::SceneNodeFactory(lnHandle handle)
{
    SceneNode* obj = LNOTE_NEW SceneNode(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::VisualNodeFactory(lnHandle handle)
{
    VisualNode* obj = LNOTE_NEW VisualNode(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::SpriteFactory(lnHandle handle)
{
    Sprite* obj = LNOTE_NEW Sprite(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::SpritePathFactory(lnHandle handle)
{
    SpritePath* obj = LNOTE_NEW SpritePath(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::ModelFactory(lnHandle handle)
{
    Model* obj = LNOTE_NEW Model(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::EffectFactory(lnHandle handle)
{
    Effect* obj = LNOTE_NEW Effect(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::TilemapFactory(lnHandle handle)
{
    Tilemap* obj = LNOTE_NEW Tilemap(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::ShaderFactory(lnHandle handle)
{
    Shader* obj = LNOTE_NEW Shader(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::ViewPaneFactory(lnHandle handle)
{
    ViewPane* obj = LNOTE_NEW ViewPane(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::LayerFactory(lnHandle handle)
{
    Layer* obj = LNOTE_NEW Layer(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::ViewFilterFactory(lnHandle handle)
{
    ViewFilter* obj = LNOTE_NEW ViewFilter(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::ToneFilterFactory(lnHandle handle)
{
    ToneFilter* obj = LNOTE_NEW ToneFilter(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::TransitionFilterFactory(lnHandle handle)
{
    TransitionFilter* obj = LNOTE_NEW TransitionFilter(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::BlurFilterFactory(lnHandle handle)
{
    BlurFilter* obj = LNOTE_NEW BlurFilter(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
inline ReferenceObject* TypeInfo::CameraFactory(lnHandle handle)
{
    Camera* obj = LNOTE_NEW Camera(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}

}