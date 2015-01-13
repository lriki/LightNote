//==============================================================================
// Typedef 
//------------------------------------------------------------------------------
///**
//  @file       Typedef.h
//  @brief      Typedef
//*/
//==============================================================================

#pragma once

typedef unsigned char byte_t;

#if defined(LNOTE_GNUC)
	typedef	pthread_t			lnThreadID;
#else
	typedef	unsigned int		lnThreadID;
#endif

#ifdef LNOTE_UNICODE
	#define lnChar              wchar_t
	//#define lnString            std::wstring
	#define lnRefString         ::LNote::Core::Base::RefWString
	#define ln_std_tstring      std::wstring
#else
	#define lnChar              char
	//#define lnString            std::string
	#define lnRefString         ::LNote::Core::Base::RefString
	#define ln_std_tstring      std::string     // Žå‚É“à•”—p
#endif
#define lnWChar				wchar_t

#define lnRefStringA			::LNote::Core::Base::RefString
#define lnRefStringW			::LNote::Core::Base::RefWString
