//==============================================================================
// LFScene 
//==============================================================================

#pragma once

#include "stdafx.h"
#include "LFInternal.h"
#include "LfSystem.h"

//==============================================================================
// LNWindow
//==============================================================================

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
LNOTEAPI LNResult LNWindow_GetMainWindow(LNHandle* window)
{
	LN_CHECK_ARG_HANDLE(window);
	*window = FuncLibManager::MainWindow;
	return LN_OK;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
LNOTEAPI LNResult LNWindow_GetSize(LNHandle window, LNSize* size)
{
	LN_CHECK_ARG_HANDLE(window);
	if (!size) return LN_ERR_ARGUMENT;
	*((Geometry::Size*)size) = TO_REFOBJ(System::Window, window)->getSize();
	return LN_OK;
}

#if defined(LNOTE_WIN32)
//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
LNOTEAPI LNResult LNWindow_GetWin32HWND(LNHandle window, HWND* hWnd)
{
	LN_CHECK_ARG_HANDLE(window);
	if (!hWnd) return LN_ERR_ARGUMENT;
	*hWnd = System::Environment::getWindowHandle(TO_REFOBJ(System::Window, window));
	return LN_OK;
}
#endif


