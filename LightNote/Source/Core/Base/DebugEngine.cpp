﻿//==============================================================================
//  DebugEngine 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Base
{

//==============================================================================
// ■ DebugEngine
//==============================================================================


    /*
    そもそもデバッグ関係の処理が Error Exception Log 等に分かれてるところが気に入らない
        LN_LOG_INIT_BEGIN 等の一掃
        Base/Debug フォルダにまとめる。

    メインスレッドの例外しか正常に捕捉できていない。
        スレッドごとの例外の管理 (発生スレッドの特定。ThreadObject と連携)



    作業：
        Exception のコンストラクタで DebugEngine に処理を回し、
        ここでメッセージボックス表示、ログ出力などを行う。
    */
    
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================