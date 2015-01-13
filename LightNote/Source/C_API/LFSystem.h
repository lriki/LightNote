//==============================================================================
// LFSystem 
//------------------------------------------------------------------------------
///**
//  @file       LFSystem.h
//  @brief      LFSystem
//  @defgroup   group_lib_system System
//  @{
//*/
//==============================================================================

#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

extern "C" {

//==============================================================================
// LNWindow
//------------------------------------------------------------------------------
///**
//  @defgroup   group_Window LNWindow
//  @brief      システムウィンドウ
//  @{
//*/
//==============================================================================

	//----------------------------------------------------------------------
	///**
	//  @brief      メインウィンドウのウィンドウハンドルを取得する
	//	@param[out]	window	: ウィンドウハンドルを格納する変数のアドレス
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNWindow_GetMainWindow(LNHandle* window);

	//----------------------------------------------------------------------
	///**
	//  @brief      ウィンドウのサイズを取得する (クライアント領域)
	//	@param[out]	window	: ウィンドウハンドルを格納する変数
	//	@param[out]	size	: ウィンドウハンドルを格納する変数のアドレス
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNWindow_GetSize(LNHandle window, LNSize* size);

#if defined(LNOTE_WIN32) && (defined(_WINDOWS_) || defined(_INC_WINDOWS) || defined(_WINDEF_))
	//----------------------------------------------------------------------
	///**
	//  @brief      Windows の ウィンドウハンドルを取得する
	//	@param[in]	window	: ウィンドウハンドル
	//	@param[out]	hWnd	: Windows の ウィンドウハンドルを格納する変数のアドレス
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNWindow_GetWin32HWND(LNHandle window, HWND* hWnd);
#endif

/** @} */	// defgroup

} // extern "C"

/** @} */	// defgroup
