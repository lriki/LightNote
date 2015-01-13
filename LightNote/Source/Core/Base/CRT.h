//==============================================================================
// CRT 
//------------------------------------------------------------------------------
///**
//  @file       CRT.h
//  @brief      CRT
//*/
//==============================================================================

#pragma once

#ifdef LNOTE_GNUC

#define EINVAL          22
#define ERANGE          34
#define EILSEQ          42
#define STRUNCATE       80

typedef int errno_t;
typedef size_t rsize_t;

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
inline errno_t memcpy_s(void * _Dst, rsize_t _DstSize, const void * _Src, rsize_t _MaxCount)
{
	if (_MaxCount == 0) {
        return 0;
    }
	if (_Dst == NULL) {
        return EINVAL;
    }
    if (_Src == NULL || _DstSize < _MaxCount)
    {
        memset(_Dst, 0, _DstSize);
        return EINVAL;
    }

    memcpy(_Dst, _Src, _MaxCount);
    return 0;
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
inline int wcsicmp(const wchar_t* s1, const wchar_t* s2) 
{ 
	wchar_t c1 = *s1, c2 = *s2; 
	while (c1 != 0 && c2 != 0) 
	{ 
		if (c1 >= 'a' && c1 <= 'z') {
			c1 -= 'a' + 'A'; 
		}
		if (c2 >= 'a' && c2 <= 'z') {
			c2 -= 'a' + 'A'; 
		}
		if (c2 < c1) { 
			return -1; 
		}
		else if (c2 > c1) {
			return 1; 
		}
		c1 = *(++s1); c2 = *(++s2); 
	} 
	return 0; 
}

//--------------------------------------------------------------------------
// http://msdn.microsoft.com/ja-jp/library/eyktyxsx.aspx
//--------------------------------------------------------------------------
errno_t mbstowcs_s(size_t *pConvertedChars, wchar_t *wcstr, size_t sizeInWords, const char *mbstr, size_t count)
{
	if (wcstr == NULL && sizeInWords > 0)
		return EINVAL;
	if (mbstr == NULL)
		return EINVAL;
	if (wcstr != NULL && sizeInWords == 0)
		return EINVAL;

	// TODO: サイズチェック
	return ERANGE;

	*pConvertedChars = mbstowcs(wcstr, mbstr, count);
}

#endif
