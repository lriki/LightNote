﻿//==============================================================================
// LFFramework 
//==============================================================================

#pragma once

#include "stdafx.h"
#include "LFInternal.h"
#include "LFBase.h"
#include "LFFramework.h"

//==============================================================================
// LNConfig
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetWindowTitle(const LNChar* title)
	{
		FuncLibManager::ConfigData.WindowTitle = title;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNConfig_SetWindowSize(int width, int height)
	{
		FuncLibManager::ConfigData.WindowWidth = width;
		FuncLibManager::ConfigData.WindowHeight = height;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNConfig_SetBackbufferSize(int width, int height)
	{
		FuncLibManager::ConfigData.BackbufferSize = LSize(width, height);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void LNConfig_SetFrameRate( int frameRate )
	{
		FuncLibManager::ConfigData.FrameRate = frameRate;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void LNConfig_SetEnableFrameWait( LNBool enabled )
	{
		FuncLibManager::ConfigData.EnableFrameWait = (enabled != LN_FALSE);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetAutoUpdateTimeScale(float timeScale)
	{
		FuncLibManager::ConfigData.AutoUpdateTimeScale = timeScale;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNConfig_SetWindowResizeEnabled(LNBool enabled)
	{
		FuncLibManager::ConfigData.WindowResizeEnabled = LNOTE_BOOL_TO_LNBOOL(enabled);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNConfig_SetBackbufferResizeMode(LNBackbufferResizeMode mode)
	{
		FuncLibManager::ConfigData.BackbufferResizeMode = (Graphics::BackbufferResizeMode)mode;
	}


#if 0
	//----------------------------------------------------------------------
	///**
	//	@brief      システムキー入力によるフルスクリーン切り替えの有効設定
	//	@param[in]  enabled	: LN_TRUE=許可 / LN_FALSE=禁止 (default:LN_TRUE)
	//	@par
	//				許可する場合、Alt+Enter キーでフルスクリーンと
	//				ウィンドウモードを切り替えられるようになります。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetToggleFullscreenEnabled(LNBool enabled);
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNConfig_SetToggleFullscreenEnabled(LNBool enabled)
	{
		FuncLibManager::ConfigData.ToggleFullscreenEnabled = LNOTE_BOOL_TO_LNBOOL(enabled);
	}
#endif

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNConfig_SetAcceleratorKey(LNAcceleratorKey acceleratorKey, LNKeyCode keyCode, LNBool isShift, LNBool isCtrl, LNBool isAlt)
	{
		FuncLibManager::ConfigData.AcceleratorKeys[acceleratorKey].KeyCode = (LNVirtualKeyCode)keyCode;
		FuncLibManager::ConfigData.AcceleratorKeys[acceleratorKey].IsShift = LNOTE_BOOL_TO_LNBOOL(isShift);
		FuncLibManager::ConfigData.AcceleratorKeys[acceleratorKey].IsCtrl = LNOTE_BOOL_TO_LNBOOL(isCtrl);
		FuncLibManager::ConfigData.AcceleratorKeys[acceleratorKey].IsAlt = LNOTE_BOOL_TO_LNBOOL(isAlt);
	}

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
	void LNConfig_SetLogFileEnabled(LNBool flag)
    {
        FuncLibManager::ConfigData.EnableMakeLogFile = (flag != LN_FALSE);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNConfig_SetConsoleEnabled(LNBool flag)
    {
        FuncLibManager::ConfigData.EnableConsole = (flag != LN_FALSE);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNConfig_RegisterArchive(const LNChar* filePath, const LNChar* password)
	{
		lnRefString dir = FileIO::Path::getDirectoryPath( filePath );
		lnRefString file = FileIO::Path::getFileName( filePath );
		FuncLibManager::ConfigData.addArchive( dir.c_str(), file.c_str(), password );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNConfig_SetFileAccessPriority(LNFileAccessPriority priority)
	{
		FuncLibManager::ConfigData.FileAccessMode = (FileIO::FileAccessPriority)priority;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void LNConfig_SetDirectMusicInitMode( LNDirectMusicInitMode mode )
    {
        FuncLibManager::ConfigData.DMInitMode = (Core::Audio::DirectMusicInitMode)mode;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void LNConfig_SetSoundMaxCacheCount( int count )
	{
		FuncLibManager::ConfigData.SoundCacheSize = count;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNConfig_SoundPlayTypeThreshold(int bytes)
	{
		FuncLibManager::ConfigData.SoundPlayTypeThreshold = bytes;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void LNConfig_RegisterFontFile( const LNChar* filePath )
	{
		FuncLibManager::ConfigData.addFontFile( filePath );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetTextureCacheSize(int count)
	{
		FuncLibManager::ConfigData.TextureCacheSize = count;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetShaderCacheSize(int count)
	{
		FuncLibManager::ConfigData.ShaderCacheSize = count;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetExceptionMessageBoxEnabled(LNBool enabled)
	{
		FuncLibManager::ExceptionMessageBox = enabled;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNConfig_SetWin32UserWindowHandle(intptr_t hWnd)
	{
#ifdef LNOTE_WIN32
		FuncLibManager::ConfigData.WindowHandle = (HWND)hWnd;
#endif
	}

//==============================================================================
// LNCore
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCore_Initialize()
	{
		LN_FUNC_TRY_BEGIN;
		FuncLibManager::preInitialize();
		FuncLibManager::CoreManager->initialize( FuncLibManager::ConfigData );
		FuncLibManager::postInitialize();
		return ::LN_OK;
		LN_FUNC_TRY_END;
		return LNException_GetLastErrorCode();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCore_InitializeFileIO()
	{
		LN_FUNC_TRY_BEGIN;
		FuncLibManager::preInitialize();
		FuncLibManager::CoreManager->initializeFileManager( FuncLibManager::ConfigData );
		FuncLibManager::postInitialize();
		return ::LN_OK;
		LN_FUNC_TRY_END;
		return LNException_GetLastErrorCode();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCore_InitializeAudio()
    {
		LN_FUNC_TRY_BEGIN;
		FuncLibManager::preInitialize();
		FuncLibManager::CoreManager->initializeAudioManager( FuncLibManager::ConfigData );
		FuncLibManager::postInitialize();
		return ::LN_OK;
		LN_FUNC_TRY_END;
		return LNException_GetLastErrorCode();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCore_InitializeInput()
	{
		LN_FUNC_TRY_BEGIN;
		FuncLibManager::preInitialize();
		FuncLibManager::CoreManager->initializeInputManager( FuncLibManager::ConfigData );
		FuncLibManager::postInitialize();
		return ::LN_OK;
		LN_FUNC_TRY_END;
		return LNException_GetLastErrorCode();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCore_Update()
	{
		LN_FUNC_TRY_BEGIN;
		FuncLibManager::CoreManager->update();
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCore_ResetFrameDelay()
	{
		FuncLibManager::CoreManager->resetDelayTime();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCore_IsEndRequested(LNBool* requested)
	{
		*requested = LNOTE_BOOL_TO_LNBOOL(FuncLibManager::CoreManager->isEndRequested());
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
	void LNCore_Terminate()
    {
        FuncLibManager::finalize();
    }

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
	void* LNCore_GetInternalObject()
	{
		return FuncLibManager::CoreManager;
	}

#if 0
//==============================================================================
// LFFramework 
//------------------------------------------------------------------------------
///**
//  @file       LFFramework.h
//  @brief      LFFramework
//  @defgroup   group_lib_framework Framework
//  @{
//*/
//==============================================================================

#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

extern "C" {

//==============================================================================
// LNConfig
//------------------------------------------------------------------------------
///**
//  @defgroup   group_config LNConfig
//  @brief      初期設定
//  @{
//*/
//==============================================================================

	//----------------------------------------------------------------------
	///**
	//	@brief      ウィンドウタイトルの設定
	//	@param[in]  title	: ウィンドウタイトル文字列
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetWindowTitle(const LNChar* title);

	//----------------------------------------------------------------------
	///**
	//	@brief      ウィンドウサイズの設定
	//	@param[in]  width	: クライアント領域の幅 (default:640)
	//	@param[in]  height	: クライアント領域の高さ (default:480)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetWindowSize(int width, int height);

	//----------------------------------------------------------------------
	///**
	//	@brief      バックバッファサイズの設定
	//	@param[in]  width	: バックバッファの幅 (default:0)
	//	@param[in]  height	: バックバッファの高さ (default:0)
	//	@details	ウィンドウサイズとは異なるサイズでバックバッファを作成する時、
	//				バックバッファのサイズを指定します。
	//				サイズが 0 の場合、ウィンドウサイズと同じサイズで作成します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetBackbufferSize(int width, int height);

	//----------------------------------------------------------------------
	///**
	//	@brief      ウィンドウリサイズの有効設定
	//	@param[in]  enabled	: LN_TRUE の場合、ウィンドウ枠をD&Dすることでサイズ変更が可能 (default:LN_TRUE)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetWindowResizeEnabled(LNBool enabled);

	//----------------------------------------------------------------------
	///**
	//	@brief      アクセラレータキーとして割り当てるキーの設定
	//	@param[in]  acceleratorKey	: アクセラレータの種別
	//	@param[in]  keyCode			: 割り当てるキーコード
	//	@param[in]  isShift			: LN_TRUE を指定すると、Shift キーを修飾子とする
	//	@param[in]  isCtrl			: LN_TRUE を指定すると、Ctrl キーを修飾子とする
	//	@param[in]  isAlt			: LN_TRUE を指定すると、Alt キーを修飾子とする
	//	@details	初期状態では以下のように割り当てられています。<br>
	//				- LN_ACCELERATORKEY_TOGGLE_FULLSCREEN<br>
	//					Alt + Enter キー
	//				- LN_ACCELERATORKEY_SHOW_DEBUG_STATISTICS<br>
	//					なし
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetAcceleratorKey(LNAcceleratorKey acceleratorKey, LNKeyCode keyCode, LNBool isShift, LNBool isCtrl, LNBool isAlt);

	//----------------------------------------------------------------------
	///**
	//  @brief      ログファイルの出力設定
	//  @param[in]  flag    : LN_TRUE=ログファイルを出力する / LN_FALSE=しない (default:LN_FALSE)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetLogFileEnabled(LNBool flag);

	//----------------------------------------------------------------------
	///**
	//  @brief      コンソールの割り当て設定
	//  @param[in]  flag    : LN_TRUE=割り当てる / LN_FALSE=割り当てない (default:LN_FALSE)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetConsoleEnabled(LNBool flag);

	//----------------------------------------------------------------------
	///**
	//  @brief		ファイルを開くときにアクセスする暗号化アーカイブの登録
	//  @param[in]  filePath	: アーカイブファイル名
	//	@param[in]  password	: アーカイブファイルを開くためのパスワード
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_RegisterArchive(const LNChar* filePath, const LNChar* password);

	//----------------------------------------------------------------------
	///**
	//  @brief		ファイルへのアクセス制限の設定
	//  @param[in]  priority	: 制限方法 (default:LN_FILEACCESSPRIORITY_DIRECTORY_FIRST)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetFileAccessPriority(LNFileAccessPriority priority);

	//----------------------------------------------------------------------
	///**
	//  @brief		DirectMusic の使用設定
	//  @param[in]  mode    : DirectMusic の使用方法 (default:LN_DMINITMODE_NOT_USE)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetDirectMusicInitMode(LNDirectMusicInitMode mode);

	//----------------------------------------------------------------------
	///**
	//  @brief		音声データの最大キャッシュ数の設定
	//  @param[in]  count   : 最大数 (default:32)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetSoundMaxCacheCount(int count);

	//----------------------------------------------------------------------
	///**
	//  @brief		音声再生方法の自動選択の音声データバイト数閾値
	//  @param[in]  bytes   : 最大数 (default:100000)
	//	@details	再生方法(オンメモリorストリーミング)の自動選択時、
	//				音声データのバイト数がこの閾値よりも大きい場合ストリーミング、
	//				以下の場合はオンメモリで再生されます。
	//				MP3等の圧縮フォーマットではデコード後のサイズでチェックします。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SoundPlayTypeThreshold(int bytes);
	
	//----------------------------------------------------------------------
	///**
	//  @brief		フレームレートの設定
	//  @param[in]  frameRate   : フレームレート (default:60)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetFrameRate(int frameRate);

	//----------------------------------------------------------------------
	///**
	//  @brief		ライブラリ更新時時のフレーム待ち処理の有効設定
	//  @param[in]  enabled   : LN_TRUE=有効 / LN_FALSE=無効 (default:LN_TRUE)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetEnableFrameWait(LNBool enabled);

	//----------------------------------------------------------------------
	///**
	//  @brief		自動更新時のタイムスケールの設定
	//  @param[in]  timeScale   : タイムスケール (default:0.0)
	//	@details	SceneNode や ViewFilter の自動フレーム更新の際、
	//				前回フレームからの経過時間に乗算される値を設定します。
	//				この値を 1.0 にすると、エフェクト等の遷移時間を時間単位、
	//				フレームレートと同じ値にするとフレーム単位で指定できるようになります。
	//				0.0 の場合、フレームレートの値を使用します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetAutoUpdateTimeScale(float timeScale);

	//----------------------------------------------------------------------
	///**
	//  @brief		ライブラリで使用するフォントファイルを登録する
	//  @param[in]  filePath	: フォントファイル名
	//	@details	登録したフォントファイルはフォントオブジェクト作成時に
	//				ファミリ名を指定することで使用できます。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_RegisterFontFile(const LNChar* filePath);

	//----------------------------------------------------------------------
	///**
	//  @brief		テクスチャの最大キャッシュ数の設定
	//  @param[in]  count		: 最大数 (default:32)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetTextureCacheSize(int count);

	//----------------------------------------------------------------------
	///**
	//  @brief		シェーダの最大キャッシュ数の設定
	//  @param[in]  count		: 最大数 (default:32)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetShaderCacheSize(int count);

	//----------------------------------------------------------------------
	///**
	//  @brief		例外エラー発生時のメッセージボックス表示設定
	//  @param[in]  enabled		: LN_TRUE の場合、表示する (default:LN_TRUE)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetExceptionMessageBoxEnabled(LNBool enabled);

	//----------------------------------------------------------------------
	///**
	//  @brief		ユーザー定義のウィンドウハンドルの設定
	//  @param[in]  hWnd	: ユーザー定義のウィンドウハンドル (default:NULL)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNConfig_SetWin32UserWindowHandle(intptr_t hWnd);
	/*DeclOpt
	Ruby_Enabled = false;
	DeclOpt*/

/** @} */	// defgroup

//==============================================================================
///**
//  @defgroup   group_core LNCore
//  @brief      ライブラリ管理
//  @{
//*/
//==============================================================================

	//----------------------------------------------------------------------
	///**
	//  @brief      ライブラリの初期化
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	ライブラリのすべての機能を使用できるように初期化を行います。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCore_Initialize();
	/*DeclOpt
	Cpp_MethodOverride = true;
	HSP_Override = true;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイル入出力機能の初期化
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCore_InitializeFileIO();
	/*DeclOpt
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief      音声機能の初期化
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//  @details    ファイル入出力機能も同時に初期化されます。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCore_InitializeAudio();
	/*DeclOpt
	HSP_Override = true;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief      入力機能の初期化
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCore_InitializeInput();
	/*DeclOpt
	HSP_Override = true;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief      ライブラリのフレーム更新
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	ライブラリの状態を更新し、時間を1フレーム分進めます。
	//				この関数は必ず1フレームに1度だけ呼び出す必要があります。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCore_Update();
	/*DeclOpt
	HSP_Override = true;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief		画面の更新タイミングのリセット
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	時間のかかる処理の後にこのメソッドを呼ぶことで、
	//				極端なフレームスキップが発生しないようにすることができます。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCore_ResetFrameDelay();

	//----------------------------------------------------------------------
	///**
	//  @brief		アプリケーションを終了するべきかを確認する
	//	@param[out]	requested	: 終了要求の有無を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	ウィンドウのクローズボタンが押された場合等、
	//				アプリケーションを終了するべき時には LN_FALSE を返します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNCore_IsEndRequested(LNBool* requested);

	//----------------------------------------------------------------------
	///**
	//  @brief     ライブラリの終了処理
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI void LNCore_Terminate();
	/*DeclOpt
	//CS_MethodOverride = true;
	DeclOpt*/

/** @} */	// defgroup

	LNOTEINTERNALAPI void* LNCore_GetInternalObject();

} // extern "C"

/** @} */	// defgroup

#endif

