//==============================================================================
// LFCommon 
//------------------------------------------------------------------------------
///**
//  @file       LFCommon.h
//  @brief      LFCommon
//*/
//==============================================================================

#pragma once

void LException_SendToWin32Message();

#ifndef LNOTE_FUNC_EXPORT
	#define LNOTE_FUNC_EXPORT
#endif

// デフォルト引数定義の細工
#ifdef LNOTE_DISABLE_AT_DOXY_BUILD
	// Doxygen 実行時は何も定義しない
	// Doxygen 設定 [Preprcessor] > [PREDEFINED] >「LNOTE_DEFAULT_ARG(x)= =x」によって
	// ドキュメント上はデフォルト引数が存在することになる。
/*
	[Preprcessor] > [MACRO_EXPANSION] … ON
	[Preprcessor] > [PREDEFINED] … "LNOTE_DISABLE_AT_DOXY_BUILD"
	[Preprcessor] > [PREDEFINED] … "LNOTE_DEFAULT_ARG(x)= =x"
*/
#else
	#ifdef __cplusplus
		// C++ ビルド時はデフォルト引数を有効にする
		#define LNOTE_DEFAULT_ARG(x) =x
	#else
		// C ビルド時は無効にする
		#define LNOTE_DEFAULT_ARG(x)
	#endif
#endif

#define LNOTE_TYPE_INFO_DECL(className) \
	LNOTEINTERNALAPI void className##_SetTypeUserData(void* data);

#define LNOTE_TYPE_INFO_IMPL(coreClass, className) \
	LNOTEINTERNALAPI void className##_SetTypeUserData(void* data) \
	{ \
		coreClass::mTypeUserData = data; \
	}

//------------------------------------------------------------------------------
// flags

// Debug ビルドフラグ
#if defined( DEBUG ) || defined( _DEBUG )
	#define LNOTE_DEBUG
#endif

// プラットフォーム
#if defined(__ANDROID__)  // Android
	#define LNOTE_ANDROID
	#define LNOTE_OPENGL
	#define LNOTE_GLES
#elif defined(__WIN32__) || defined(_WIN32)// || defined(__CYGWIN__)
	#define LNOTE_WIN32
	#define LNOTE_DIRECTX
	#define LNOTE_OPENGL
#else	// とりあえず
	#define LNOTE_X11
	#define LNOTE_OPENGL
#endif

// コンパイラ
#if defined(__GNUC__)
	#define LNOTE_GNUC
	#undef LNOTE_ENABLE_INLINE_ASM	// インラインアセンブラ無効
#elif defined(_MSC_VER)
	#define LNOTE_MSVC
#else
	#error "invalid compiler"
#endif

#if defined(LNOTE_WIN32) || defined(LNOTE_X11)
	#define LNOTE_GLFW
#endif

// UNICODE 使用フラグ
#if defined (UNICODE) || defined(_UNICODE)
	#define LNOTE_UNICODE
#else
	#define LNOTE_MBS
#endif

// wchar_t の文字コード
#if defined(LNOTE_WIN32)
	#define LNOTE_WCHAR_UTF16
#else
	#define LNOTE_WCHAR_UTF32
#endif

// DLL
#if defined(LNOTE_BUILD_DLL)
	#define LNOTEAPI			__declspec(dllexport)
	#define LNOTEINTERNALAPI	__declspec(dllexport)
#else
	#define LNOTEAPI
	#define LNOTEINTERNALAPI	
#endif

//------------------------------------------------------------------------------
// constant

#define LNOTE_NAME         _T( "LNote" )
#define LNOTE_VERSION_STR  _T( "2.0.0b1" )

#if defined(LNOTE_WIN32)
	#define LN_MAX_PATH     ( 260 )         // 260 (windows の MAX_PATH)
#else
	#define LN_MAX_PATH     ( 255 )         // UNIX 系 OS のファイル名最大長
#endif

//------------------------------------------------------------------------------
// typedef
#if 1
	#include <stdint.h>
	typedef int8_t				lnS8;
	typedef int16_t				lnS16;
	typedef int32_t				lnS32;
	typedef int64_t				lnS64;
	typedef uint8_t				lnU8;
	typedef uint16_t			lnU16;
	typedef uint32_t			lnU32;
	typedef uint64_t			lnU64;
	typedef unsigned int		lnUInt;
#else
#if defined(LNOTE_GNUC)
	#include <stdint.h>
	typedef int8_t				lnS8;
	typedef int16_t				lnS16;
	typedef int32_t				lnS32;
	typedef int64_t				lnS64;
	typedef uint8_t				lnU8;
	typedef uint16_t			lnU16;
	typedef uint32_t			lnU32;
	typedef uint64_t			lnU64;
	typedef unsigned int		lnUInt;
	typedef	pthread_t			lnThreadID;
#else
	typedef	signed char			lnS8;
	typedef	signed short		lnS16;
	typedef	signed int			lnS32;
	typedef	signed long long	lnS64;
	typedef unsigned char		lnU8;
	typedef unsigned short		lnU16;
	typedef unsigned int		lnU32;
	typedef unsigned __int64	lnU64;
	typedef unsigned int		lnUInt;
	typedef	unsigned int		lnThreadID;
#endif
#endif

typedef	unsigned char			lnByte;
typedef float					lnFloat;
typedef intptr_t				LNHandle;
typedef intptr_t				lnIntPtr;

#ifdef LNOTE_UNICODE
	//#define lnChar				wchar_t
	typedef wchar_t				lnChar;
	typedef wchar_t				LNChar;
#else
	//#define lnChar				char
	typedef char				lnChar;
	typedef char				LNChar;
#endif

//------------------------------------------------------------------------------
//
#include <stdio.h>


//------------------------------------------------------------------------------
#define LN_MODULE(x)
#define LN_MODULE_END
#define LN_CLASS(x)
#define LN_STATIC_CLASS(x)
#define LN_CLASS_END
#define LUMINO_INSTANCE_API
#define LUMINO_STATIC_API
#define LUMINO_PROPERTY

#define LN_HANDLE(x) LNHandle

#ifdef __cplusplus
	// C++ ビルド時はデフォルト引数を有効にする
	#define LN_DEFAULT_ARG(x) =x
#else
	// C ビルド時は無効にする
	#define LN_DEFAULT_ARG(x)
#endif
