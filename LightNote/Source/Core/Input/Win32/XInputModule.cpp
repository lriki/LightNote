//==============================================================================
// XInputModule 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Resource/LNResource.h"
#include "XInputModule.h"

namespace LNote
{
namespace Core
{
namespace Input
{

//==============================================================================
// ■ XInputModule
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    XInputModule::XInputModule()
        : mXInputModule     ( NULL )
        , mXInputGetState   ( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    XInputModule::~XInputModule()
    {
        if ( mXInputModule )
        {
            ::FreeLibrary( mXInputModule );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void XInputModule::initialize()
    {
        // モジュール読み込み
		mXInputModule = ::LoadLibrary(XINPUT_DLL);
		LN_THROW(mXInputModule, Base::Win32Exception, GetLastError(), Resource::String::ERR_FailedDXInputInit);

        mXInputGetState = reinterpret_cast< MD_XInputGetState >( ::GetProcAddress( mXInputModule, "XInputGetState" ) );
        mXInputSetState = reinterpret_cast< MD_XInputSetState >( ::GetProcAddress( mXInputModule, "XInputSetState" ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DWORD XInputModule::XInputGetState( DWORD dwUserIndex, XINPUT_STATE* pState )
    {
        return mXInputGetState( dwUserIndex, pState );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DWORD XInputModule::XInputSetState( DWORD dwUserIndex, XINPUT_VIBRATION* pVibration )
    {
        return mXInputSetState( dwUserIndex, pVibration );
    }


} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================