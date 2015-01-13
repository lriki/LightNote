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
//  @brief      �V�X�e���E�B���h�E
//  @{
//*/
//==============================================================================

	//----------------------------------------------------------------------
	///**
	//  @brief      ���C���E�B���h�E�̃E�B���h�E�n���h�����擾����
	//	@param[out]	window	: �E�B���h�E�n���h�����i�[����ϐ��̃A�h���X
	//  @return		�������� (LN_OK=���� / ����ȊO=�G���[�R�[�h)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNWindow_GetMainWindow(LNHandle* window);

	//----------------------------------------------------------------------
	///**
	//  @brief      �E�B���h�E�̃T�C�Y���擾���� (�N���C�A���g�̈�)
	//	@param[out]	window	: �E�B���h�E�n���h�����i�[����ϐ�
	//	@param[out]	size	: �E�B���h�E�n���h�����i�[����ϐ��̃A�h���X
	//  @return		�������� (LN_OK=���� / ����ȊO=�G���[�R�[�h)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNWindow_GetSize(LNHandle window, LNSize* size);

#if defined(LNOTE_WIN32) && (defined(_WINDOWS_) || defined(_INC_WINDOWS) || defined(_WINDEF_))
	//----------------------------------------------------------------------
	///**
	//  @brief      Windows �� �E�B���h�E�n���h�����擾����
	//	@param[in]	window	: �E�B���h�E�n���h��
	//	@param[out]	hWnd	: Windows �� �E�B���h�E�n���h�����i�[����ϐ��̃A�h���X
	//  @return		�������� (LN_OK=���� / ����ȊO=�G���[�R�[�h)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNWindow_GetWin32HWND(LNHandle window, HWND* hWnd);
#endif

/** @} */	// defgroup

} // extern "C"

/** @} */	// defgroup
