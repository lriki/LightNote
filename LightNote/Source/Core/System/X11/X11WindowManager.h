//=============================================================================
//【 X11WindowManager 】
//-----------------------------------------------------------------------------
///**
//  @file       X11WindowManager.h
//  @brief      X11WindowManager
//  @author     Riki
//*/
//=============================================================================

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
class X11Window;

//=============================================================================
// ■ X11WindowManager
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class X11WindowManager
    : public WindowManagerBase
{
public:

    /// 初期化データ
	struct ConfigData
	{
        const lnChar*	TitleText;		///< ウィンドウタイトルの文字列
		int				Width;			///< クライアント領域の幅
		int			    Height;			///< クライアント領域の高さ
		bool		    Windowed;		///< ウィンドウモードで初期化する場合 true
    };

public:
    X11WindowManager( Manager* manager );
	virtual ~X11WindowManager();

public:

	/// 前初期化 (call by Main Thread)
	void preInitialize( const ConfigData& configData );

public:
    virtual void				initialize();
	virtual void				finalize();
    virtual void				processMessage();
	virtual IWindow*			getMainWindow() const;
	virtual WindowBaseArray&	getWindowBaseArray() { return mWindowArray; }

private:
	ConfigData					mConfigData;
	X11Window*					mMainWindow;
	bool						mConsoleAlloced;
	bool						mSystemMouseShown;
};

} // namespace System
} // namespace Core
} // namespace LNote

//=============================================================================
//
//=============================================================================