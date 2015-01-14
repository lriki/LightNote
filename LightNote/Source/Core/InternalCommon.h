//==============================================================================
// InternalCommon 
//------------------------------------------------------------------------------
///**
//  @file       InternalCommon.h
//  @brief      InternalCommon
//*/
//==============================================================================

#pragma once

#include "Config.h"
#include "Common.h"

//-------------------------------------------------------------------------
// macro

#ifndef LN_OPERATOR_NEW
#define LN_OPERATOR_NEW( size ) ::operator new( size )
#endif

#ifndef LN_OPERATOR_DELETE
#define LN_OPERATOR_DELETE( ptr ) { ::operator delete( ptr ); ( ptr ) = 0; }
#endif

#ifndef SAFE_FREE
#define SAFE_FREE( p ) { if ( p ) { free( ( p ) ); ( p ) = NULL; } }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE( p ) { if ( p ) { delete ( p ); ( p ) = NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY( p ) { if ( p ) { delete[] ( p ); ( p ) = NULL; } }
#endif

#ifndef SAFE_ADDREF
#define SAFE_ADDREF( p ) { if ( p ) { ( p )->AddRef(); } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE( p ) { if ( p ) { ( p )->Release(); ( p ) = NULL; } }
#endif

#ifndef LN_SAFE_ADDREF
#define LN_SAFE_ADDREF( p ) { if ( p ) { ( p )->addRef(); } }
#endif

#ifndef LN_SAFE_RELEASE
#define LN_SAFE_RELEASE( p ) { if ( p ) { ( p )->release(); ( p ) = NULL; } }
#endif

#if defined(LNOTE_MSVC)
#define LNOTE_STDCALL __stdcall 
#else
#define LNOTE_STDCALL
#endif

#if defined(LNOTE_MSVC)
#define LNOTE_CDECL __cdecl
#else
#define LNOTE_CDECL
#endif

#define lnToF( x ) ( static_cast< lfloat >( x ) )	// 実数へ
#define lnToI( x ) ( static_cast< int >( x ) )		// 整数へ
#define lnToRev( x ) ( 1.0f / lnToF( x ) )			// 逆数

// 未実装関数
#define LN_PRINT_NOT_IMPL_FUNCTION printf( "%s is not implemented\n", __FUNCTION__ );

#if defined(LNOTE_ANDROID)
#define lnprintf(...)       __android_log_print( ANDROID_LOG_DEBUG, LNOTE_NAME, __VA_ARGS__ )
#define lnwprintf(...)      __android_log_print( ANDROID_LOG_DEBUG, LNOTE_NAME, __VA_ARGS__ )
#else
#define lnprintf            printf
#define lnwprintf           wprintf
#endif

// _p
#if defined(LNOTE_ENABLE_DEBUG_SUPPORT)
#ifndef _p
inline void _ln_printf_(const char* name, const char* v)		{ lnprintf("[ %s = %s ]\n", name, v); }
inline void _ln_printf_(const char* name, unsigned char v)		{ lnprintf("[ %s = %c ]\n", name, v); }
inline void _ln_printf_(const char* name, short v)				{ lnprintf("[ %s = %d ]\n", name, v); }
inline void _ln_printf_(const char* name, unsigned short v)		{ lnprintf("[ %s = %u ]\n", name, v); }
inline void _ln_printf_(const char* name, int v)				{ lnprintf("[ %s = %d ]\n", name, v); }
inline void _ln_printf_(const char* name, unsigned int v)		{ lnprintf("[ %s = %u ]\n", name, v); }
inline void _ln_printf_(const char* name, long v)				{ lnprintf("[ %s = %ld ]\n", name, v); }
inline void _ln_printf_(const char* name, unsigned long v)		{ lnprintf("[ %s = %lu ]\n", name, v); }
inline void _ln_printf_(const char* name, unsigned long long v) { lnprintf("[ %s = %llu ]\n", name, v); }
inline void _ln_printf_(const char* name, float v)				{ lnprintf("[ %s = %f ]\n", name, v); }
inline void _ln_printf_(const char* name, double v)				{ lnprintf("[ %s = %lf ]\n", name, v); }
inline void _ln_printf_(const char* name, bool v)				{ lnprintf("[ %s = %s ]\n", name, (v) ? "true" : "false"); }
inline void _ln_printf_(const char* name, void* v)				{ lnprintf("[ %s = %p ]\n", name, v); }
inline void _ln_printf_(const char* name, const void* v)		{ lnprintf("[ %s = %p ]\n", name, v); }

inline void _ln_printf_(const wchar_t* name, const wchar_t* v)	{ lnwprintf(L"[ %s = %s ]\n", name, v); }
inline void _ln_printf_(const wchar_t* name, const wchar_t  v)	{ lnwprintf(L"[ %s = %c ]\n", name, v); }

// 変数名と値をコンソールに出力するマクロ
#define _p( _var_ )		_ln_printf_( #_var_, ( _var_ ) )
#define _pw( _var_ )	_ln_printf_( _ln_to_w_( #_var_ ), ( _var_ ) );
#endif
#endif // LNOTE_ENABLE_DEBUG_SUPPORT

// 各データクラス用 dump() 実装補助
#define LN_DATA_DUMP_HEADER_UTIL( header, defaultHeader ) \
if (header) { \
	lnprintf(header); \
} \
else { \
	\
	lnprintf(defaultHeader); \
}

//-------------------------------------------------------------------------
// Declaration

namespace tinyxml2
{
	class XMLDocument;
	class XMLElement;
}

namespace LNote
{
	namespace Core
	{
		class Manager;

		namespace Math
		{
			class Vector2;
			class Vector3;
			class Vector4;
			class Matrix;
			class Quaternion;
			class SQTTransform;
			class Random;
		}
		namespace Task
		{
			class Manager;
		}
		namespace System
		{
			class Manager;
		}
		namespace FileIO
		{
			class Manager;
		}
		namespace Input
		{
			class Manager;
		}
		namespace Audio
		{
			class Manager;
		}
		namespace Physics
		{
			class Manager;
		}
		namespace Graphics
		{
			class Manager;
		}
		namespace Effect
		{
			class Manager;
		}
		namespace Scene
		{
			class Manager;
		}
		namespace GUI
		{
			class Manager;
		}
		namespace Game
		{
			class Manager;
		}
		namespace XML
		{
			typedef tinyxml2::XMLDocument	Document;
			typedef tinyxml2::XMLElement	Element;
		}
	}



}

//-------------------------------------------------------------------------
// Base include

#include "Base/Typedef.h"

// 必須のみ
#if defined(LNOTE_COMMON_INCLUDE_REQUIRED_FILES_ONRY)
#include "Base/Foreach.h"
#include "Base/MemoryManager.h"
#include "Base/ReferenceObject.h"
#include "Base/Exception.h"
#else
#include "Base/Foreach.h"
#include "Base/MemoryManager.h"
#include "Base/ReferenceObject.h"
#include "Base/String.h"
#include "Base/RefString.h"
#include "Base/LogFile.h"
#include "Base/Interface.h"
#include "Base/Error.h"
#include "Base/Exception.h"
#include "Base/RefPtr.h"
#endif
