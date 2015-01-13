//==============================================================================
// MouseCursorVisibleCounter 
//------------------------------------------------------------------------------
///**
//  @file       MouseCursorVisibleCounter.h
//  @brief      MouseCursorVisibleCounter
//  @author     Riki
//*/
//==============================================================================

#pragma once

namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// ■ MouseCursorVisibleCounter
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class MouseCursorVisibleCounter
{
public:
	MouseCursorVisibleCounter();
	~MouseCursorVisibleCounter();

public:

	/// この関数が false を返している間は非表示にする (ワーカースレッドでチェック)
	bool checkVisible() const;

	/// マウスカーソルの表示設定 (count は マウスカーソル放置時、消すまでの時間(ms)。0 で常に消す)
	void setMouseCursorVisibleState( bool visible, int time );

	/// マウスカーソルの操作後に呼ぶこと
	void onMoveCursor( bool on_client_area );

private:
	bool    mVisibleMouseCursor;
	lnU32	mMouseVisibleTime;
	lnU32	mStartTime;
	bool	mRequestForced;

};

} // namespace System
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================