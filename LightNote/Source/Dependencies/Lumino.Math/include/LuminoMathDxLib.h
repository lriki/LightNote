
#ifndef LUMINO_MATH_DXLIB_H
#define LUMINO_MATH_DXLIB_H

#ifndef __DXLIB
#error please include after dxlib.h
#endif

#define LN_MATH_VECTOR3_CAST_OPERATORS	operator const DxLib::VECTOR& () { return *(reinterpret_cast<const DxLib::VECTOR*>(this)); }
#define LN_MATH_MATRIX_CAST_OPERATORS	operator const DxLib::MATRIX& () { return *(reinterpret_cast<const DxLib::MATRIX*>(this)); }

#include "LuminoMath.h"
using namespace Lumino;

#ifdef _MSC_VER
	#if _MSC_VER >= 1800	// Visual C++ 12.0 (Visual Studio 2013)
		#ifdef _DEBUG
			#pragma comment(lib, "LuminoMath-msvc120-static_d")
		#else
			#pragma comment(lib, "LuminoMath-msvc120-static")
		#endif
	#elif _MSC_VER >= 1700	// Visual C++ 11.0 (Visual Studio 2012)
		#ifdef _DEBUG
			#pragma comment(lib, "LuminoMath-msvc110-static_d")
		#else
			#pragma comment(lib, "LuminoMath-msvc110-static")
		#endif
	#elif _MSC_VER >= 1600	// Visual C++ 10.0 (Visual Studio 2010)
		#ifdef _DEBUG
			#pragma comment(lib, "LuminoMath-msvc100-static_d")
		#else
			#pragma comment(lib, "LuminoMath-msvc100-static")
		#endif
	#elif _MSC_VER >= 1500	// Visual C++ 9.0 (Visual Studio 2008)
		#ifdef _DEBUG
			#pragma comment(lib, "LuminoMath-msvc90-static_d")
		#else
			#pragma comment(lib, "LuminoMath-msvc90-static")
		#endif
	#elif _MSC_VER >= 1400	// Visual C++ 8.0 (Visual Studio 2005)
		#ifdef _DEBUG
			#pragma comment(lib, "LuminoMath-msvc80-static_d")
		#else
			#pragma comment(lib, "LuminoMath-msvc80-static")
		#endif
	#else
	#endif
#endif

#endif // LUMINO_MATH_DXLIB_H
