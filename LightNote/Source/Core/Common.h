//==============================================================================
// Common 
//------------------------------------------------------------------------------
///**
//  @file       Common.h
//  @brief      Common
//*/
//==============================================================================

#pragma once

#include "Config.h"

#include "../C_API/LFCommon.h"

//-------------------------------------------------------------------------
// flags

// マルチバイト文字セット使用の強制
//#if defined(LNOTE_SPECIFIED_CHARCODESET_MBS)
//#undef UNICODE
//#undef _UNICODE
//#ifndef _MBCS
//#define _MBCS
//#endif
//// UNICODE 文字セット使用の強制
//#elif defined(LNOTE_SPECIFIED_CHARCODESET_UNICODE)
//#ifndef UNICODE
//# define UNICODE
//#endif
//#ifndef _UNICODE
//# define _UNICODE
//#endif
//#undef _MBCS
//#endif

#if defined(LNOTE_IMPORT_DLL)
	#define LNOTE_EXPORT __declspec(dllimport)
#else
	#define LNOTE_EXPORT LNOTEAPI
#endif

// 警告無視
#if defined(LNOTE_DISABLE_WARNING_4996) && (_MSC_VER >= 1200)
	#pragma warning( disable:4996 )
#endif

#ifndef LNOTE_TEST_UNIT
	#define LNOTE_TEST_UNIT
#endif

//-------------------------------------------------------------------------
// constant

#define LN_MUSICTIMEBASE    ( 768 )         // Midi 用ミュージックタイムのベース値
#define LN_INFINITE_TIME    ( 0xFFFFFFFF )  // Infinite timeout

static const unsigned long LN_INVALID_INDEX = 0xffffffff;

//-------------------------------------------------------------------------
// include

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

// tchar.h の扱い
#if defined(LNOTE_MSVC)
	#if defined(LNOTE_INCLUDE_INTERNAL_TCHAR)
		# include "Base/TChar.h"     // _T() のために先に include
	#else
		# include <tchar.h>
	#endif
#else
	#include "Base/TChar.h"
#endif

#ifdef LNOTE_GNUC
	#include <limits.h>
	#include <stdint.h>
#endif

#if defined(LNOTE_INCLUDE_ENV_DEPEND_HEADER)
	#if defined(LNOTE_WIN32)
		#define INITGUID
		#include <winsock2.h>
		#include <mmreg.h>
		#include <msacm.h>
		#include <crtdbg.h>
		#include <locale.h>     // for Framework::init
		#include <numeric>
		#define NOMINMAX
		#include <windows.h>
        #include <initguid.h>
        #include <dxdiag.h>
		#undef min
		#undef max
		#undef NOMINMAX
		#include <process.h>
		#if defined(LNOTE_DIRECTX)
			#include <d3d9.h>
			#include <d3dx9.h>
			#include <xaudio2.h>
			#include <x3daudio.h>
			#define DIRECTINPUT_VERSION 0x0800
			#include <dinput.h>
			#include <xinput.h>
			#define LNOTE_INCLUDED_DIRECTX_HEADER (1)
		#endif
		#if defined(LNOTE_OPENGL)
			#define GLEW_STATIC
			#include "../Dependencies/gl/glew.h"
		#endif
	#elif defined(LNOTE_ANDROID)
		#include <stdint.h>
		#include <unistd.h>
		#include <ctype.h>
		#include <pthread.h>
		#include <jni.h>
		#include <android/log.h>
		#include <GLES2/gl2.h>      // for GLenum
	#endif
	#ifdef LNOTE_MSVC
		#include <assert.h>
	#endif
	#ifdef LNOTE_GNUC
		#include <limits.h>
		#include <stdint.h>
		#include <unistd.h>
		#include <pthread.h>
		#include <wchar.h>
		#include <typeinfo>
		#include "Base/MSVCRT.h"
	#endif
	#ifdef LNOTE_X11
		#include <X11/Xlib.h>
		#include <X11/Xutil.h>
		#include <X11/Xlocale.h>
		#include <X11/keysym.h>
	#endif
#else
	#if defined(LNOTE_WIN32)
		#ifndef NOMINMAX
			#define NOMINMAX
		#endif
		#include <windows.h>
		#undef min
		#undef max
		//#undef NOMINMAX
	#endif
#endif

//-------------------------------------------------------------------------
// typedef

namespace LNote
{
	/// NULL ポインタを表す値
	const class lnNullPtr
	{
	public:
		lnNullPtr() {}
		//template< typename TYPE >
		//operator TYPE*() const { return 0; }
		operator void*() { return 0; }

	} lnNull;



#if defined(LNOTE_ENABLE_SIMPLE_TYPEDEF)
	typedef	lnS8                s8;
	typedef	lnS16               s16;
	typedef	lnS32               s32;
	typedef	lnS64               s64;
	typedef lnU8                u8;
	typedef lnU16               u16;
	typedef lnU32               u32;
	typedef lnU64               u64;
	typedef float				f32;
	typedef double				f64;
	typedef unsigned int		uint;
	const lnNullPtr             nil;
#endif // LNOTE_ENABLE_SIMPLE_TYPEDEF

	typedef int                 LNRESULT;	// TODO: 削除予定
	//typedef LNRESULT            LNResult;

}
