//==============================================================================
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

