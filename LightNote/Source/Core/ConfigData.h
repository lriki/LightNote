//==============================================================================
// Public 
//------------------------------------------------------------------------------
///**
//  @file       Public.h
//  @brief      Public
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Types.h"
#include "System/Common.h"
#include "Geometry/DrawingShapes.h"
#include "Audio/Interface.h"
#include "Graphics/Common/Types.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{

enum AcceleratorKeyType
{
	AcceleratorKeyType_ToggleFullscreen = 0,
	AcceleratorKeyType_ShowDebugStatistics,
	AcceleratorKeyType_Max,
};

struct AcceleratorKeyEntry
{
	//System::ModifierKeys	ModifierKey;	///< Shift または Alt または Ctrl キー
	LNVirtualKeyCode		KeyCode;			///< キーコード
	bool IsShift;
	bool IsCtrl;
	bool IsAlt;
};

//==============================================================================
// ■ ConfigData クラス
//------------------------------------------------------------------------------
///**
//  @brief      ライブラリの初期化情報
//*/
//==============================================================================
class ConfigData
{
public:

	//-----------------------------------------------
	// 全般

    lnU32                   ClassInitializeFlag;    ///< 初期化する機能の指定 (LNClassFlag の組み合わせ)
    lnU32                   ClassUpdateFlag;        ///< 自動更新する機能の指定 (LNClassFlag の組み合わせ)
    bool                    EnableMakeLogFile;      ///< ログファイルを出力するかを示すフラグ
    const lnChar*           LogSaveDir;             ///< ログファイルの保存先ディレクトリ
    LNUpdateMode            UpdateMode;             ///< フレームワークの更新方法
	bool                    ToggleFullscreenEnabled;   ///< Alt + Enter によるフルスクリーン切り替えの有効フラグ
    lnU32                   FrameRate;              ///< フレームレート
	bool					EnableFrameWait;		///< フレーム待ちを行う (false の場合 sleep によるウェイトを行わない)
    bool                    AllInitialize;          ///< Framework::initialize() ですべての機能を初期化する
    bool                    EnableMultipleRunning;  ///< true の場合、多重起動を許可する
    bool                    EnableShowFPS;          ///< true の場合、ウィンドウのタイトルバーに現在の FPS を表示する
	bool					EnableCRTSetDebugFlags;	///< _CrtSetDbgFlag() により、プログラム終了時のメモリリークダンプを有効にする (C++テストや.NETと組み合わせると終了時にエラーが発生するため注意)

	//-----------------------------------------------
	// System 関係
    
    const lnChar*           WindowTitle;            ///< ウィンドウタイトルの文字列
    lnU32                   WindowWidth;            ///< ウィンドウのクライアント領域の幅 (Android では無効)
    lnU32                   WindowHeight;           ///< ウィンドウのクライアント領域の高さ (Android では無効)
    void*                   WindowHandle;           ///< ユーザー定義のウィンドウハンドル (Windows のみ)
    bool                    EnableConsole;          ///< コンソールを割り当てる
	//bool                    EnableDummyWindow;      ///< (WindowHandle == NULL でもウィンドウを作成しない)
	System::WindowSystemAPI	WindowSystemAPI;
	bool					WindowResizeEnabled;	///< リサイズ可能か
	AcceleratorKeyEntry		AcceleratorKeys[AcceleratorKeyType_Max];	///< LNote のキーボードアクセラレータ

	//-----------------------------------------------
	// File 関係

    FileIO::ArchiveOpenDataList	ArchiveList;            ///< (addArchive() を使って追加すること)
    FileIO::FileAccessPriority	FileAccessMode;         ///< ファイルアクセスの優先モード

	//-----------------------------------------------
	// Audio 関係

    lnU32								SoundCacheSize;         ///< 音声ファイルのキャッシュサイズ (ファイル数)
	lnU32								SoundPlayTypeThreshold;	///< オンメモリorストリーミング自動選択の音声データバイト数閾値
    Core::Audio::DirectMusicInitMode	DMInitMode;             ///< DirectMusic の初期化方法

	//-----------------------------------------------
	// Physics 関係
	bool					EnablePhysicsThread;	///< 物理演算を別スレッドで行う (ジョイント結合先のこ往診が2フレーム遅れるので非推奨)

	//-----------------------------------------------
	// Graphics 関係

    LNGraphicsAPI           GraphicsAPI;            ///< 描画に使用するグラフィックス API
    LNFontNameList          FontFilenameList;       ///< 使用するフォントファイル名のリスト (システムにインストールされていないフォントを使うとき用)
    Geometry::Size			BackbufferSize;			///< (0, 0) でウィンドウサイズに合わせてリサイズ。それ以外はアス比固定
	Graphics::BackbufferResizeMode	BackbufferResizeMode;
	bool                    EnableFPUPreserve;      ///< true の場合、DirectX の初期化で浮動小数点演算ユニットの演算精度を落とさないようにする
    bool                    EnableFullscreen;       ///< 起動時フルスクリーンの有効設定
	int						TextureCacheSize;
	int						ShaderCacheSize;

	//-----------------------------------------------
	// Scene
	double					AutoUpdateTimeScale;

	//-----------------------------------------------
	// Android
#if defined(LNOTE_ANDROID)
    android_app*            AndroidApp;
#endif

public:

	//-------------------------------------------------------------
	/// デフォルト値
    ConfigData()
    {
        ClassInitializeFlag     = LN_CLASS_ALL;
        ClassUpdateFlag         = LN_CLASS_ALL;
        EnableMakeLogFile       = false;
        LogSaveDir              = NULL;
        UpdateMode              = LN_UPDATEMODE_THREADED;//LN_UPDATEMODE_ALWAYS;//
		ToggleFullscreenEnabled = true;
        FrameRate               = 60;
		EnableFrameWait			= true;
        AllInitialize           = true;
        EnableMultipleRunning   = false;
        EnableShowFPS           = false;
		EnableCRTSetDebugFlags	= true;

        WindowTitle             = LNOTE_NAME;
        WindowWidth             = 640;
        WindowHeight            = 480;
        WindowHandle            = NULL;
        EnableConsole           = false;
        //EnableDummyWindow       = false;
		WindowResizeEnabled		= true;

		memset(AcceleratorKeys, 0, sizeof(AcceleratorKeys));
#ifdef LNOTE_DEBUG
		AcceleratorKeys[AcceleratorKeyType_ToggleFullscreen].KeyCode = LN_KEY_ENTER;
		AcceleratorKeys[AcceleratorKeyType_ToggleFullscreen].IsShift = false;
		AcceleratorKeys[AcceleratorKeyType_ToggleFullscreen].IsCtrl = false;
		AcceleratorKeys[AcceleratorKeyType_ToggleFullscreen].IsAlt = true;
		AcceleratorKeys[AcceleratorKeyType_ShowDebugStatistics].KeyCode = LN_KEY_F3;
		AcceleratorKeys[AcceleratorKeyType_ShowDebugStatistics].IsShift = false;
		AcceleratorKeys[AcceleratorKeyType_ShowDebugStatistics].IsCtrl = false;
		AcceleratorKeys[AcceleratorKeyType_ShowDebugStatistics].IsAlt = false;
#endif

        FileAccessMode          = FileIO::FileAccessPriority_DirectoryFirst;

        SoundCacheSize          = 32;
		SoundPlayTypeThreshold	= 100000;
        DMInitMode              = Core::Audio::DMINITMODE_NOTUSE;

		EnablePhysicsThread		= true;

        GraphicsAPI             = LN_GRAPHICSAPI_DIRECTX9;
        BackbufferSize          = LSize( 0, 0 );
		BackbufferResizeMode	= Graphics::BackbufferResizeMode_ScalingWithLetterBox;
        EnableFPUPreserve       = false;
        EnableFullscreen        = false;
		TextureCacheSize		= 32;
		ShaderCacheSize			= 32;

		AutoUpdateTimeScale		= 0.0;

#ifdef LNOTE_DEBUG
        EnableMakeLogFile       = true;
        EnableConsole           = true;
        EnableShowFPS           = true;
		EnableCRTSetDebugFlags  = true;
#endif  

#if defined(LNOTE_WIN32)
		WindowSystemAPI			= System::WindowSystemAPI_WinAPI;
#elif defined(LNOTE_ANDROID)
        AndroidApp              = NULL;
#endif
    }

public:

	void setAcceleratorKeys()
	{

	}

	/// アーカイブの追加
    void addArchive( const lnChar* directory_, const lnChar* filename_, const lnChar* key_ )
    {
        FileIO::ArchiveOpenData data;
        data.Directory   = directory_;
        data.ArchiveName = filename_;
        data.ArchiveKey  = key_;
        ArchiveList.push_back( data );
    }

	/// フォントファイルの追加
    void addFontFile( const lnChar* filename_ )
    {
        FontFilenameList.push_back( ln_std_tstring( filename_ ) );
    }

};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================