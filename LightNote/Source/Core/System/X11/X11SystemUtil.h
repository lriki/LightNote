//=============================================================================
//【 X11SystemUtil 】
//-----------------------------------------------------------------------------
///**
//  @file       X11SystemUtil.h
//  @brief      X11SystemUtil
//  @author     Riki
//*/
//=============================================================================

#pragma once

#include "../Interface.h"

namespace LNote
{
namespace Core
{
namespace System
{
#if 0
/// メッセージをライブラリ用のイベントメッセージに変換する
void translateMessageToLN( LNEvent* lnev_, const XEvent& xev_, bool* handled_ );

/// ライブラリで必要なメッセージかをチェックする (クローズボタン用の ClientMessage は通さないので注意)
bool checkNecessaryMessage( const XEvent& xev_ );

//---------------------------------------------------------------------
///**
//  @brief      XK_～ → LN_KEY_～ へのキーコード変換
//
//  @par
//              使っていく中で、LNVirtualKeyCode のように直で変換できない
//              ものがあった場合はここに書いていく
//*/	
//---------------------------------------------------------------------
lnU32 convVirtualKeyCode( lnU32 win_vk_ );
#endif
} // namespace System
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================