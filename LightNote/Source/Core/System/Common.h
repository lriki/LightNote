//==============================================================================
// Common 
//------------------------------------------------------------------------------
///**
//  @file       Common.h
//  @brief      Common
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <vector>

namespace LNote
{
namespace Core
{
namespace System
{
class Manager;
class WindowBase;
class WindowManagerBase;

class Win32Window;
class Win32WindowBase;
class Win32WindowManager;
class IWin32MessageListener;

class GLFWWindow;
class GLFWWindowManager;

typedef std::vector<WindowBase*> WindowBaseArray;

/// ウィンドウシステムに使用するライブラリ
enum WindowSystemAPI
{
	WindowSystemAPI_WinAPI = 0,		///< WindowsAPI
	WindowSystemAPI_GLFW,
};

} // namespace System
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================