//=============================================================================
//【 X11WindowManager 】
//=============================================================================

#include "stdafx.h"
#include "X11Window.h"
#include "X11WindowManager.h"

namespace LNote
{
namespace Core
{
namespace System
{

//=============================================================================
// ■ X11WindowManager
//=============================================================================

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	X11WindowManager::X11WindowManager( Manager* manager )
		: WindowManagerBase	( manager )
		, mMainWindow		( NULL )
		, mConsoleAlloced	( false )
		, mSystemMouseShown	( true )
	{
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	X11WindowManager::~X11WindowManager()
	{
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void X11WindowManager::preInitialize( const ConfigData& configData )
	{
		mConfigData = configData;
		LN_PRINT_NOT_IMPL_FUNCTION;
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void X11WindowManager::initialize()
	{
		/*
		Win32Window::SettingData data;
		data.TitleText		= mSettingData.TitleText;
		data.Width			= mSettingData.Width;
		data.Height			= mSettingData.Height;
		data.Windowed		= mSettingData.Windowed;
		data.WinClassName	= NULL;
		data.IconResourceID	= NULL;
		Win32Window* window = LN_NEW Win32Window( this );
		window->initialize( data );
		mMainWindow = window;
		*/
		LN_PRINT_NOT_IMPL_FUNCTION;
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void X11WindowManager::finalize()
	{
		LN_PRINT_NOT_IMPL_FUNCTION;
		/*
		if ( mMainWindow ) mMainWindow->finalize();
		LN_SAFE_RELEASE( mMainWindow );
		*/
		WindowManagerBase::finalize();
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void X11WindowManager::processMessage()
	{
		LN_PRINT_NOT_IMPL_FUNCTION;
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	IWindow* X11WindowManager::getMainWindow() const 
	{ 
		return mMainWindow; 
	}

} // namespace System
} // namespace Core
} // namespace LNote

//=============================================================================
//
//=============================================================================