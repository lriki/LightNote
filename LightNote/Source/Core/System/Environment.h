//==============================================================================
// Environment 
//------------------------------------------------------------------------------
///**
//  @file       Environment.h
//  @brief      Environment
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "Interface.h"

namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// ■ Environment
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class Environment
{
public:

	//----------------------------------------------------------------------
	///**
	//  @brief      システム時刻を取得する (ミリ秒)
	//
	//  @par
	//              Windows では OS 起動時からの経過時間、
	//              X11 ではプログラム起動時からの経過時間を返します。
	//*/
	//----------------------------------------------------------------------
    static lnU32 getSystemTime();

	//----------------------------------------------------------------------
	///**
	//  @brief      カレントディレクトリを取得する
	//
	//  @param[out] buf_ : 文字列を書き込むバッファのアドレス (LN_MAX_PATH 文字分のサイズがあること)
	//
	//  @par
	//              gcc では char で取得した文字列を wchar_t に変換しています。
	//              そのため、char で扱えない文字は正常に取得できない可能性があります。
	//*/
	//----------------------------------------------------------------------
	static void getCurrentDirectory(char* buf);
	static void getCurrentDirectory(wchar_t* buf);

	/// カレントディレクトリを変更する
	static void setCurrentDirectory(const char* path);

	/// カレントディレクトリを変更する
	static void setCurrentDirectory(const wchar_t* path);

	//----------------------------------------------------------------------
	///**
	//  @brief      メッセージボックスの表示 (エラー用)
	//
	//  @par
	//              X11 では title_ は無視され、常に Error と表示される。
	//*/
	//----------------------------------------------------------------------
    static void showMessageBox( Window* parentWindow, const lnChar* message_, const lnChar* title_ );
	static void showMessageBox( Window* parentWindow, const wchar_t* message, const wchar_t* title );

	//----------------------------------------------------------------------
	///**
	//  @brief      1ピクセルの物理的な1辺のサイズを取得する (mm単位)
	//*/
	//----------------------------------------------------------------------
    static float getPhysicalPixelSize();

	/// システム情報の出力
    static bool outputSystemInfo();

#if defined(LNOTE_WIN32)
	/// ウィンドウハンドルの取得 
	static HWND getWindowHandle( Window* window );
#endif

};

//==============================================================================
// ■ MessageBox
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class MessageBox
{
public:

	/// エラーメッセージボックスを表示する
    static void showError( Window* ownerWindow, const wchar_t* text ); 
};

} // namespace System
} // namespace Core
} // namespace LNote
