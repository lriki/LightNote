//==============================================================================
// GLFWWindowManager 
//------------------------------------------------------------------------------
///**
//  @file       GLFWWindowManager.h
//  @brief      GLFWWindowManager
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Common.h"
#include "../Common/WindowManagerBase.h"
#include "../Common/EventListenerList.h"

namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// ■ GLFWWindowManager
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class GLFWWindowManager
    : public WindowManagerBase
{
public:

	/// 初期化データ
	struct SettingData
	{
        bool		    UseConsole;     ///< コンソールを割り当てる場合 true
        const lnChar*	TitleText;		///< ウィンドウタイトルの文字列
		int				Width;			///< クライアント領域の幅
		int			    Height;			///< クライアント領域の高さ
		bool		    Windowed;		///< ウィンドウモードで初期化する場合 true
		bool			Resizable;
    };

public:
    GLFWWindowManager( Manager* manager );
	virtual ~GLFWWindowManager();

public:

	/// 前初期化 (call by Main Thread)
	void preInitialize( const SettingData& setting_data );

public:
    virtual void				initialize();
	virtual void				finalize();
    virtual void				processMessage();
	virtual Window*				getMainWindow() const;
	virtual WindowBaseArray&	getWindowBaseArray() { return mWindowArray; }

private:

	SettingData					mSettingData;
	GLFWWindow*					mMainWindow;
	bool						mConsoleAlloced;
	bool						mSystemMouseShown;
};

} // namespace System
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================