//==============================================================================
// XInputModule 
//------------------------------------------------------------------------------
///**
//  @file       XInputModule.h
//  @brief      XInputModule
//  @author     Riki
//*/
//==============================================================================

#pragma once

namespace LNote
{
namespace Core
{
namespace Input
{

//==============================================================================
// ■ XInputModule
//------------------------------------------------------------------------------
/**
// @brief      XInput DLL モジュール
//*/
//==============================================================================
class XInputModule
{
public:
    XInputModule();
    ~XInputModule();

public:

	/// 初期化
    void initialize();

	/// XInputGetState
    DWORD XInputGetState( DWORD dwUserIndex, XINPUT_STATE* pState );

	/// XInputSetState
    DWORD XInputSetState( DWORD dwUserIndex, XINPUT_VIBRATION* pVibration );


private:

    typedef DWORD ( WINAPI *MD_XInputGetState )(
        DWORD         dwUserIndex,
        XINPUT_STATE* pState
    );

    typedef DWORD ( WINAPI *MD_XInputSetState )(
        DWORD             dwUserIndex,
        XINPUT_VIBRATION* pVibration
    );

private:

    HMODULE             mXInputModule;   ///< XINPUT_DLL ("XInput～.dll")
    MD_XInputGetState   mXInputGetState;
    MD_XInputSetState   mXInputSetState;

};

} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================