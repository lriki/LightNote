﻿//=============================================================================
//【 X11Window 】
//=============================================================================

#include "stdafx.h"
#include "../../Base/StringUtil.h"
#include "../Manager.h"
#include "X11WindowManager.h"
#include "X11SystemUtil.h"
#include "X11Window.h"

namespace LNote
{
namespace Core
{
namespace System
{

//=============================================================================
// ■ X11Window
//=============================================================================

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	X11Window::X11Window( X11WindowManager* manager )
		: WindowBase  ( manager )
	    , mOrginalWidth     ( 640 )
	    , mOrginalHeight    ( 480 )
	    , mFullScreen       ( false )
	{
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	X11Window::~X11Window()
	{
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void X11Window::initialize( const ConfigData& configData )
	{
	    mTitleText      = configData.TitleText;
	    mClientSize.w   = configData.Width;
	    mClientSize.h   = configData.Height;
	    mFullScreen     = false;
	    mOrginalWidth   = mClientSize.w;
	    mOrginalHeight  = mClientSize.h;

		LN_PRINT_NOT_IMPL_FUNCTION;
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void X11Window::finalize()
	{
		LN_PRINT_NOT_IMPL_FUNCTION;
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	const Geometry::Size& X11Window::getSize()
	{
		LN_PRINT_NOT_IMPL_FUNCTION;;
	    return mClientSize;
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void X11Window::setVisible( bool flag )
	{
		LN_PRINT_NOT_IMPL_FUNCTION;
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void X11Window::setEnableFullScreen( bool flag )
	{
		LN_PRINT_NOT_IMPL_FUNCTION;
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void X11Window::setWindowTextFooter( const lnChar* format, ... )
	{
		LN_PRINT_NOT_IMPL_FUNCTION;
	}
	
	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void X11Window::captureMouse()
	{
		LN_PRINT_NOT_IMPL_FUNCTION;
	}
	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void X11Window::releaseMouseCapture()
	{
		LN_PRINT_NOT_IMPL_FUNCTION;
	}


} // namespace System
} // namespace Core
} // namespace LNote

//=============================================================================
//
//=============================================================================