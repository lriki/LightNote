//==============================================================================
// TChar 
//------------------------------------------------------------------------------
///**
//  @file       TChar.h
//  @brief      TChar
//  @author     Riki
//*/
//==============================================================================

#pragma once

#ifndef _INC_TCHAR

#include <stdio.h>
#include <wchar.h>

#if defined(LNOTE_WIN32)
    #define snprintf  sprintf_s
	//#define ln_wfopen _wfopen
#endif

#if WCHAR_MAX <= 0xffff
	#define LNOTE_WCHAR_16
#elif WCHAR_MAX <= 0xffffffff
	#define LNOTE_WCHAR_32
#else
	#error "size of wchar_t is too large."
#endif

#ifdef LNOTE_GNUC
//inline int _vscprintf(const char *format, va_list argptr)
//{
//    return(vsnprintf(0, 0, format, argptr));
//}
//
//inline int _vscwprintf(const wchar_t *format, va_list argptr)
//{
//    return(vswprintf(0, 0, format, argptr));
//}
#endif

#ifdef LNOTE_UNICODE
//-------------------------------------------------------------------------
// wchar_t
//-------------------------------------------------------------------------

#define __T(x)          L ## x

#define _tprintf        wprintf
#define _stprintf_s     swprintf

#define _tstoi          _wtoi

#define _tcslen         wcslen
#define _tcscat         wcscat
#define _tcscpy         wcscpy
#define _tcsncpy        wcsncpy
#define _tcscmp         wcscmp

#define _tfopen         ln_wfopen
#define _ftprintf       fwprintf

// 式指定文字列内の文字数を返す(GNUC番は↑で定義)
#define _vsctprintf     _vscwprintf

// 書式付き出力を書き込む
#define _vstprintf_s    vswprintf   // vswnprintf のようにNULL, 0 を渡して文字数をカウントできないので注意



#else
//-------------------------------------------------------------------------
// char
//-------------------------------------------------------------------------

#if defined(LNOTE_MSVC)
    #define _ln_stricmp_ stricmp
#else
    #define _ln_stricmp_ strcasecmp
#endif

#define __T(x)          x

#define _tprintf        printf
#define _stprintf_s     snprintf

#define _tstoi          atoi
#define _tcstod         strtod

#define _tcslen         strlen
#define _tcscat         strcat
#define _tcscpy         strcpy
#define _tcsncpy        strncpy
#define _tcscmp         strcmp
#define _tcsicmp        _ln_stricmp_
#define _tcsncmp        strncmp

#define _istdigit       isdigit

#define _tfopen         fopen
#define _ftprintf       fprintf

// 式指定文字列内の文字数を返す(GNUC番は↑で定義)
#define _vsctprintf     _vscprintf

/// 書式付き出力を書き込む
#define _vstprintf_s    vsnprintf


//#define _vsctprintf( format_, args_ )   vsnprintf( NULL, 0, format_, args_ )

#endif


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

#define _T(x)           __T(x)
#define _TEXT(x)        __T(x)

#endif

#define __WT(x)			L ## x
#define _WT(x)			__WT(x)



template<class T> inline const T _ln_tchar_cast( const char c, const wchar_t wc ) { return c; }
template<> inline const wchar_t _ln_tchar_cast<wchar_t>( const char c, const wchar_t wc ){ return wc; }
template<class T> inline const T* _ln_tstring_cast( const char* c, const wchar_t* wc ){ return c; }
template<> inline const wchar_t* _ln_tstring_cast<wchar_t>( const char* c, const wchar_t* wc ){ return wc; }

#define _lnTC( c )	_ln_tchar_cast<CHAR_T>(c,L##c)
#define _lnTS( s )	_ln_tstring_cast<CHAR_T>(s,L##s)

inline size_t	ln_tcslen( const char* s )		{ return strlen( s ); }
inline size_t	ln_tcslen( const wchar_t* s )	{ return wcslen( s ); }

// _tcscpy
inline bool ln_tcscpy( char* strDest, size_t strDestSize, const char* strSource, size_t strSourceSize )		
{ 
	if ( strDestSize < strSourceSize ) return false;
	strcpy( strDest, strSource );
	return true;
}
inline bool ln_tcscpy( wchar_t* strDest, size_t strDestSize, const wchar_t* strSource, size_t strSourceSize )	
{ 
	if ( strDestSize < strSourceSize ) return false;
	wcscpy( strDest, strSource ); 
	return true;
}

// _tcsncpy
inline bool ln_tcsncpy( char* strDest, size_t strDestSize, const char* strSource, size_t count )		
{ 
	if ( count > strDestSize ) return false;
	strncpy( strDest, strSource, count );
	return true;
}
inline bool ln_tcsncpy( wchar_t* strDest, size_t strDestSize, const wchar_t* strSource, size_t count )	
{ 
	if ( count > strDestSize ) return false;
	wcsncpy( strDest, strSource, count ); 
	return true;
}

// _tcscat  dest = src1 + src2   size は文字数
inline bool ln_tcscat( char* dest, size_t destSize, const char* src1, size_t src1Size, const char* src2, size_t src2Size )
{ 
	if ( destSize < src1Size + src2Size ) return false;
	strncat( dest, src1, src1Size );
	strncat( dest + src1Size, src2, src2Size );
	return true;
}
inline bool ln_tcscat( wchar_t* dest, size_t destSize, const wchar_t* src1, size_t src1Size, const wchar_t* src2, size_t src2Size )
{ 
	if ( destSize < src1Size + src2Size ) return false;
	wcsncat( dest, src1, src1Size );
	wcsncat( dest + src1Size, src2, src2Size );
	return true;
}

//==============================================================================
//
//==============================================================================