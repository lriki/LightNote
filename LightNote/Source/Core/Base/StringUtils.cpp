//==============================================================================
// StringUtils 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <algorithm>
#include "StringUtils.h"

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
// ■ StringUtils クラス
//==============================================================================

	//----------------------------------------------------------------------
	// ● バッファを確保して文字列をコピーする ( マルチバイト文字列 )
	//----------------------------------------------------------------------
	char* StringUtils::copyLength( const char* src_str_, int length_ )
	{
        if ( src_str_ )
        {
		    if ( length_ < 0 )
		    {
			    length_ = strlen( src_str_ );
		    }
		    char* str = LN_NEW char[ length_ + 1 ];
		    memcpy( str, src_str_, sizeof( char ) * length_ );
		    str[ length_ ] = '\0';
		    return str;
        }
        else
        {
            char* str = LN_NEW char[ 1 ];
            *str = '\0';
            return str;
        }
	}

	//----------------------------------------------------------------------
	// ● バッファを確保して文字列をコピーする ( ワイド文字列 )
	//----------------------------------------------------------------------
	wchar_t* StringUtils::copyLength( const wchar_t* src_str_, int length_ )
	{
        if ( src_str_ )
        {
		    if ( length_ < 0 )
		    {
			    length_ = wcslen( src_str_ );
		    }
		    wchar_t* str = LN_NEW wchar_t[ length_ + 1 ];
		    memcpy( str, src_str_, sizeof( wchar_t ) * length_ );
		    str[ length_ ] = L'\0';
		    return str;
        }
        else
        {
            wchar_t* str = LN_NEW wchar_t[ 1 ];
            *str = L'\0';
            return str;
        }
	}

	//----------------------------------------------------------------------
	// ● バッファを確保して文字列をコピーする ( マルチバイト文字列 )
	//----------------------------------------------------------------------
	char* StringUtils::copyLength( const char* src_str_, int begin_, int length_ )
	{
		if ( begin_ >= static_cast< int >( strlen( src_str_ ) ) )
		{
			length_ = 0;
		}
		return copyLength( src_str_ + begin_, length_ );
	}

	//----------------------------------------------------------------------
	// ● バッファを確保して文字列をコピーする ( ワイド文字列 )
	//----------------------------------------------------------------------
	wchar_t* StringUtils::copyLength( const wchar_t* src_str_, int begin_, int length_ )
	{
		if ( begin_ >= static_cast< int >( wcslen( src_str_ ) ) )
		{
			length_ = 0;
		}
		return copyLength( src_str_ + begin_, length_ );
	}

	//----------------------------------------------------------------------
	// ● バッファを確保して文字列をコピーする ( 範囲指定・マルチバイト文字列 )
	//----------------------------------------------------------------------
	char* StringUtils::copyRange( const char* src_str_, int begin_, int end_ )
	{
		int len = ( end_ - begin_ ) + 1;
		if ( len < 0 )
		{
			len = 0;
		}
		char* str = LN_NEW char[ len + 1 ];
		memcpy( str, &( src_str_[ begin_ ] ), sizeof( char ) + len );
		str[ len ] = '\0';
		return str;
	}

	//----------------------------------------------------------------------
	// ● バッファを確保して文字列をコピーする ( 範囲指定・ワイド文字列 )
	//----------------------------------------------------------------------
	wchar_t* StringUtils::copyRange( const wchar_t* src_str_, int begin_, int end_ )
	{
		int len = ( end_ - begin_ ) + 1;
		if ( len < 0 )
		{
			len = 0;
		}
		wchar_t* str = LN_NEW wchar_t[ len + 1 ];
		memcpy( str, &( src_str_[ begin_ ] ), sizeof( wchar_t ) + len );
		str[ len ] = L'\0';
		return str;
	}

	//----------------------------------------------------------------------
	// ● ふたつの文字列を結合した新しい文字列を作成する ( char )
	//----------------------------------------------------------------------
    char* StringUtils::merge( const char* src1_, const char* src2_ )
    {
        int len1 = strlen( src1_ );
        char* new_str = LN_NEW char[ len1 + strlen( src2_ ) + 1 ];
        strcpy( new_str, src1_ );
        strcpy( ( new_str + len1 ), src2_ );
        return new_str;
    }

	//----------------------------------------------------------------------
	// ● ふたつの文字列を結合した新しい文字列を作成する ( char )
	//----------------------------------------------------------------------
    wchar_t* StringUtils::merge( const wchar_t* src1_, const wchar_t* src2_ )
    {
        int len1 = wcslen( src1_ );
        wchar_t* new_str = LN_NEW wchar_t[ len1 + wcslen( src2_ ) + 1 ];
        wcscpy( new_str, src1_ );
        wcscpy( ( new_str + len1 ), src2_ );
        return new_str;
    }

	//----------------------------------------------------------------------
	// ● マルチバイト文字列をワイド文字列に変換した後の文字数取得
	//----------------------------------------------------------------------
	int StringUtils::getMultiToWideLength( const char* multi_str_ )
	{
	    if ( !multi_str_ )
        {
            return -1;
        }
	    
#if defined(LNOTE_GNUC)
        mbstate_t state;
        ::memset( &state, 0, sizeof( state ) );
        return static_cast< int >( mbsrtowcs( NULL, &multi_str_, 0, &state ) );
#else
        size_t size = 0;
        mbstowcs_s( &size, NULL, 0, multi_str_, 0 );
        return static_cast< int >( size ) - 1;
#endif
    }

	//----------------------------------------------------------------------
	// ● マルチバイト文字列をワイド文字列に変換
	//----------------------------------------------------------------------
    bool StringUtils::convMultiToWide( wchar_t* out_str_, const int size_, const char* in_str_ )
    {
        if ( !out_str_ || size_ <= 0 || !in_str_ )
        {
            return false;
        }
#if defined(LNOTE_GNUC)
        mbstate_t state;
        ::memset( &state, 0, sizeof( state ) );
        mbsrtowcs( out_str_, &in_str_, size_, &state );
#else
        // ワイド文字に変換 ( 3つめの引数は wchar での文字数 )
        errno_t err = mbstowcs_s( NULL, out_str_, size_, in_str_, _TRUNCATE );

        if ( err == ERANGE )
        {
            out_str_[ 0 ] = L'\0';
        }
#endif
        return true;
    }

	//----------------------------------------------------------------------
	// ● マルチバイト文字列をワイド文字列に変換
	//----------------------------------------------------------------------
	wchar_t* StringUtils::convMultiToWide( const char* multi_str_ )
	{
        if ( !multi_str_ )
        {
            return NULL;
        }

		int len = StringUtils::getMultiToWideLength( multi_str_ ) + 1;
		wchar_t* str = LN_NEW wchar_t[ len ];
		StringUtils::convMultiToWide( str, len, multi_str_ );
		return str;
	}

	//----------------------------------------------------------------------
	// ● ワイド文字列をマルチバイト文字列に変換した後の文字数取得
	//----------------------------------------------------------------------
    int StringUtils::getWideToMultiLength( const wchar_t* wide_str_ )
    {
        if ( !wide_str_ )
        {
            return -1;
        }
#if defined(LNOTE_GNUC)
        mbstate_t state;
        ::memset( &state, 0, sizeof( state ) );
        return static_cast< int >( wcsrtombs( NULL, &wide_str_, 0, &state ) );
#else
        size_t size = 0;
        wcstombs_s(
            &size,		// [out] 変換された文字数
            NULL,		// [out] 変換結果のマルチバイト文字列用のバッファのアドレス
            0,			// [in]  mbstr (第二引数) バッファのサイズ (バイト数)
            wide_str_,	// [in]	 変換されるワイド文字列
            0 );		// [in]	 mbstr (第二引数)バッファに格納される最大バイト数または _TRUNCATE
        return static_cast< int >( size ) - 1;
#endif
	}
	
	//----------------------------------------------------------------------
	// ● ワイド文字列をマルチバイト文字列に変換
	//----------------------------------------------------------------------
	bool StringUtils::convWideToMulti( char* out_str_, int size_, const wchar_t* in_str_ )
	{
        if ( !out_str_ || size_ <= 0 || !in_str_ )
        {
            return false;
        }
#if defined(LNOTE_GNUC)
	    mbstate_t state;
        ::memset( &state, 0, sizeof( state ) );
        wcsrtombs( out_str_, &in_str_, size_, &state );
        return true;
#else
        errno_t err = wcstombs_s( NULL, out_str_, size_, in_str_, _TRUNCATE );

        if ( err == ERANGE )
        {
            out_str_[ 0 ] = '\0';
        }
        return true;
#endif
	}

	//----------------------------------------------------------------------
	// ● ワイド文字列をマルチバイト文字列に変換
	//----------------------------------------------------------------------
	char* StringUtils::convWideToMulti( const wchar_t* wide_str_ )
	{
        if ( !wide_str_ ) { 
	return NULL; }

		int len = StringUtils::getWideToMultiLength( wide_str_ ) + 1;
		char* str = LN_NEW char[ len ];
		StringUtils::convWideToMulti( str, len, wide_str_ );
		return str;
	}

    
	//----------------------------------------------------------------------
	// ● マルチバイト文字を 1 として文字数をカウントする (unicode の場合はそのまま)
	//----------------------------------------------------------------------
    int StringUtils::strlenMB( const lnChar* str_ )
    {
#ifdef LNOTE_UNICODE
        return _tcslen( str_ );
#else
        //int max_len = _tcslen( str_ );
        int count = 0;
        //int i = 0;
        //for ( ; i < max_len; )
        while ( *str_ )
        {
            if ( checkMultiByteChar( str_ ) )
            {
                str_ += 2;
                //i += 2;
            }
            else
            {
                ++str_;
                //++i
            }
            ++count;
        }
        return count;
#endif
    }

	//----------------------------------------------------------------------
	// ● ワイルドカード('*', '?')を使って文字列を比較する
	//----------------------------------------------------------------------
    bool StringUtils::match( const lnChar* ptn_, const lnChar* str_ )
    {
        switch( *ptn_ )
	    {
		    case _T( '\0' ):
			    return _T( '\0' ) == *str_;
		    case _T( '*' ):
			    return match( ptn_ + 1, str_ )
			        || ( ( _T( '\0' ) != *str_ ) && match( ptn_, str_ + 1 ) );
		    case _T( '?' ):
			    return ( _T( '\0' ) != *str_ )
			        && match( ptn_ + 1, str_ + 1 );
		    default:
			    return ( (unsigned char)*ptn_ == (unsigned char)*str_ )
			        && match( ptn_ + 1, str_ + 1 );
	    }
    }
    
	//----------------------------------------------------------------------
	// ● 文字列の比較 (大文字小文字を考慮しない)
	//----------------------------------------------------------------------
    bool StringUtils::strcmpI( const char* str1_, const char* str2_ )
    {
#if defined(LNOTE_MSVC)
        return stricmp( str1_, str2_ ) == 0;
#else
        return strcasecmp( str1_, str2_ ) == 0;
#endif
    }

	//----------------------------------------------------------------------
	// ● 文字列の比較 (文字数指定で大文字小文字を考慮しない)
	//----------------------------------------------------------------------
    bool StringUtils::strcmpNI( const char* str1_, const char* str2_, int n_ )
    {
        for ( int i = 0; ( (*str1_) && (*str2_) && (i < n_) ); ++i )
        {
            if ( toupper( *str1_ ) != toupper( *str2_ ) )
            {
                return false;
            }
            ++str1_;
            ++str2_;
        }
        return true;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	template<typename T>
	bool StringUtils::equals(const T* str1, const T* str2)
	{
		for (; (*str1) && (*str2);)
		{
			if (*str1 != *str2) {
				return false;
			}
			++str1;
			++str2;
		}
		return true;
	}
	template bool StringUtils::equals<char>(const char* str1, const char* str2);
	template bool StringUtils::equals<wchar_t>(const wchar_t* str1, const wchar_t* str2);
	template bool StringUtils::equals<uint32_t>(const uint32_t* str1, const uint32_t* str2);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	template<typename T>
	bool StringUtils::strcmpN(const T* str1, const T* str2, int n)
	{
		for (int i = 0; ((*str1) && (*str2) && (i < n)); ++i)
		{
			if (*str1 != *str2) {
				return false;
			}
			++str1;
			++str2;
		}
		return true;
	}
	template bool StringUtils::strcmpN<char>(const char* str1, const char* str2, int n);
	template bool StringUtils::strcmpN<wchar_t>(const wchar_t* str1, const wchar_t* str2, int n);
	template bool StringUtils::strcmpN<uint32_t>(const uint32_t* str1, const uint32_t* str2, int n);


	//----------------------------------------------------------------------
	// ● 文字列の置換を行う
	//----------------------------------------------------------------------
    void StringUtils::replaceString( std::string* str_, const char* from_, const char* to_ )
    {
        std::string::size_type pos = 0;
        int from_length = strlen( from_ );
        int to_length   = strlen( to_ );

        while ( pos = str_->find( from_, pos ), pos != std::string::npos )
        {
            str_->replace( pos, from_length, to_ );
            pos += to_length;
        }
    }
#if LNOTE_WCHAR_T_SUPPORT
    void StringUtils::replaceString( std::wstring* str_, const wchar_t* from_, const wchar_t* to_ )
    {
        std::wstring::size_type pos = 0;
        int from_length = wcslen( from_ );
        int to_length   = wcslen( to_ );

        while ( pos = str_->find( from_, pos ), pos != std::wstring::npos )
        {
            str_->replace( pos, from_length, to_ );
            pos += to_length;
        }
    }
#endif
    /*
    void StringUtils::replaceString( ln_std_tstring* str_, const lnChar* from_, const lnChar* to_ )
    {
        ln_std_tstring::size_type pos = 0;
        int from_length = _tcslen( from_ );
        int to_length   = _tcslen( to_ );

        while ( pos = str_->find( from_, pos ), pos != ln_std_tstring::npos )
        {
            str_->replace( pos, from_length, to_ );
            pos += to_length;
        }
    }
    */

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void StringUtils::replaceSpace( ln_std_tstring* out_, const lnChar* in_, const lnChar* to_ )
    {
        ln_std_tstring out = in_;
        ln_std_tstring::size_type pos = 0;
        int to_length   = _tcslen( to_ );
        
        while ( pos < out.size() )
        {
            if ( isspace( out[pos] ) )
            {
                int len = getSpaceLength( &out[pos] );
                out.replace( pos, len, to_ );
                pos += to_length;
            }
            else
            {
                ++pos;
            }
        }

        *out_ = out;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void StringUtils::deleteSpace( ln_std_tstring* str_ )
	{
		size_t pos;
		while((pos = str_->find_first_of(_T(" 　\t"))) != ln_std_tstring::npos)
		{
			str_->erase(pos, 1);
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void StringUtils::deleteNewLine( ln_std_tstring* str )
	{
		size_t pos;
		while((pos = str->find_first_of(_T("\r\n"))) != ln_std_tstring::npos)
		{
			str->erase(pos, 1);
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	int StringUtils::countChar( const lnString& str, int offset, lnChar c )
	{
		int count = 0;
		const lnChar* cc = str.c_str();
		for ( ; *cc; ++cc ) {
			if ( *cc == c ) {
				++count;
			}
		}
		return count;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	int StringUtils::countChar(const wchar_t* str, int offset, wchar_t c)
	{
		int count = 0;
		for (; *str; ++str) {
			if (*str == c) {
				++count;
			}
		}
		return count;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    int StringUtils::getSpaceLength( const lnChar* str_ )
    {
        int count = 0;
        while ( *str_ != _T('\0') && isspace( *str_ ) )
        {
            ++count;
            ++str_;
        }
        return count;
    }

	//----------------------------------------------------------------------
	// ● 文字列の先頭と終端のみのホワイトスペースを削除した string を返す
	//----------------------------------------------------------------------
    void StringUtils::trimHFSpace( ln_std_tstring* out_, const ln_std_tstring& in_ )
    {
        const lnChar* s = in_.c_str();
        int end = in_.size();
        int count = 0;
        while ( --end >= 0 && s[ end ] == _T( ' ' ) )
        {
            ++count;
        }

        int begin = 0;
        while ( s[ begin ] != _T( '\0' ) && s[ begin ] == _T( ' ' ) )
        {
            ++begin;
        }

        *out_ = ln_std_tstring( s, begin, end + 1 );
    }
#if 0
    int Trim(char *s) {
    int i;
    int count = 0;

    /* 空ポインタか? */
    if ( s == NULL ) { /* yes */
        return -1;
    }

    /* 文字列長を取得する */
    i = strlen(s);

    /* 末尾から順に空白でない位置を探す */
    while ( --i >= 0 && s[i] == ' ' ) count++;

    /* 終端ナル文字を付加する */
    s[i+1] = '\0';

    /* 先頭から順に空白でない位置を探す */
    i = 0;
    while ( s[i] != '\0' && s[i] == ' ' ) i++;
    strcpy(s, &s[i]);

    return i + count;
}
#endif

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool StringUtils::trimRange( ln_std_tstring* out_, const lnChar* in_, lnChar begin_, lnChar end_ )
    {
        const lnChar* c = in_;

        int begin_pos = 0;
        while ( true )
        {
            if ( *c == begin_ )
            {
                break;
            }
            else if ( *c == _T('\0') )
            {
                return false;
            }
            ++c;
            ++begin_pos;
        }

        int end_pos = begin_pos;
        ++c;
        ++end_pos;
        while ( true )
        {
            if ( *c == end_ )
            {
                break;
            }
            else if ( *c == _T('\0') )
            {
                return false;
            }
            ++c;
            ++end_pos;
        }

        *out_ = ln_std_tstring( in_, begin_pos + 1, end_pos - begin_pos - 1 );
        return true;
    }
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void StringUtils::split( std::vector< ln_std_tstring >* out_, const lnChar* str_, const lnChar* delim_ )
    {
        *out_ = split( str_, delim_ );
    }

    class RemoveCharPred
    {
    public:
        RemoveCharPred(lnChar code_)
            : mCode(code_)
        {}
        bool operator()(lnChar c) const
        {
            return c == mCode;
        }
    public:
        lnChar mCode;
    };

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void StringUtils::removeChar( ln_std_tstring* str_, lnChar code_ )
    {
        str_->erase(std::remove_if(str_->begin(), str_->end(), RemoveCharPred(code_)), str_->end());
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    int StringUtils::getNewLineCount( const lnChar* s )
    {
        int count = 0;
        for (; *s; ++s)
        {
			if (*s == _T('\r')) {
				if (s[1] == _T('\n')) {
					++count;	// CR+LF
					++s;
				}
				else {
					++count;	// CR
				}
			}
			else if (*s == _T('\n')) {
				++count;		// LF
			}
        }
        return count;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    int StringUtils::atoiRange( const lnChar* str, int len )
    {
        lnChar buf[64] = { 0 };
        _tcsncpy( buf, str, (len > 63) ? 63 : len );
        return _tstoi( buf );
    }

#if 0
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ResultCode StringUtils::convertSJISToUTF8N( const char* in, int in_bytes, lnU8* out, int out_bytes )
    {
#if defined(LNOTE_MSVC)

        // 一度Unicodeに
        char* new_wide = LN_NEW char[out_bytes];
        memset(new_wide, 0, out_bytes);
        if (::MultiByteToWideChar( 932, 0, in, in_bytes, (wchar_t*)new_wide, out_bytes / sizeof(wchar_t) ) == FALSE )
        {
            SAFE_DELETE_ARRAY( new_wide );
            return LN_ERR_SYSTEM;
        }

        // Unicodeから再度UTF8に
        if (::WideCharToMultiByte( CP_UTF8, 0, (wchar_t*)new_wide, in_bytes, (LPSTR)out, out_bytes, NULL, NULL ) == FALSE )
        {
            SAFE_DELETE_ARRAY( new_wide );
            return LN_ERR_SYSTEM;
        }

        SAFE_DELETE_ARRAY( new_wide );
        return LN_OK;
#else
        return LN_ERR_NOTIMPL;
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    LNRESULT StringUtils::convertCodeUTF8ToUTF32BE( const lnU8* utf8code, int* read_len, lnU32* utf32code )
    {
        const lnU8* c = utf8code;
        lnU32 c0 = *c;

        ++c;

        // U+0000～U+007F
        if ((c0 & 0x80) == 0)
        {
            *utf32code = c0;
            *read_len = 1;
        }
        // U+0080～U+07FF
        else if ((c0 & 0xE0) == 0xC0)
        {
            lnU32 c1 = *c++;
            if ((c1 & 0xc0) != 0x80) return LN_ERR_INVALIDCALL;

            *utf32code = ((c0 & 0x1F) << 6) | (c1 & 0x3F);
            *read_len = 2;
        }
        // U+0800～U+FFFF
        else if ((c0 & 0xF0) == 0xE0)
        {
            lnU32 c1 = *c++;
            if ((c1 & 0xc0) != 0x80) return LN_ERR_INVALIDCALL;
            lnU32 c2 = *c++;
            if ((c2 & 0xc0) != 0x80) return LN_ERR_INVALIDCALL;

            *utf32code = ((c0 & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
            *read_len = 3;
        }
        // U+10000～U+10FFFF
        else if (0xF0 <= c0 && c0 <= 0xF4)
        {
            lnU32 c1 = *c++;
            if ((c1 & 0xc0) != 0x80) return LN_ERR_INVALIDCALL;
            lnU32 c2 = *c++;
            if ((c2 & 0xc0) != 0x80) return LN_ERR_INVALIDCALL;
            lnU32 c3 = *c++;
            if ((c3 & 0xc0) != 0x80) return LN_ERR_INVALIDCALL;

            // 上位5bit
            c0 = ((c0 & 0x7) << 2) | ((c1 >> 4) & 0x3);
            if (c0 > 0x10) return LN_ERR_INVALIDCALL;

            *utf32code = (c0 << 16) | ((c1 & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
            *read_len = 4;
        }
        else
        {
            return LN_ERR_INVALIDCALL;
        }
        return LN_OK;
    }
#endif

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	template<typename T>
	int StringUtils::checkNewLineSequence(const T* start, const T* end)
	{
		if (start < end)
		{
			if (start[0] == '\r') {
				if ((start + 1) < end && start[1] == '\n') {
					return 2;	// CR+LF
				}
				else {
					return 1;	// CR
				}
			}
			else if (start[0] == '\n') {
				return 1;		// LF
			}
		}
		return 0;
	}
	template int StringUtils::checkNewLineSequence<char>(const char* start, const char* end);
	template int StringUtils::checkNewLineSequence<wchar_t>(const wchar_t* start, const wchar_t* end);
	template int StringUtils::checkNewLineSequence<uint32_t>(const uint32_t* start, const uint32_t* end);


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//int StringUtils::checkNewLineSequence(const wchar_t* start, const wchar_t* end)
	//{
	//	if (start < end)
	//	{
	//		if (start[0] == L'\r') {
	//			if ((start + 1) < end && start[1] == L'\n') {
	//				return 2;	// CR+LF
	//			}
	//			else {
	//				return 1;	// CR
	//			}
	//		}
	//		else if (start[0] == L'\n') {
	//			return 1;		// LF
	//		}
	//	}
	//	return 0;
	//}

	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
	//const char* StringUtils::findNextNewLine(const char* start, const char* end)
	//{
	//	while (start < end)
	//	{
	//		int nlLen = checkNewLineSequence(start, end);
	//		if (nlLen > 0) {
	//			return start;	// 見つかった
	//		}
	//		++start;
	//	}
	//	return end;	// 見つからなかった
	//}

	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
	//const wchar_t* StringUtils::findNextNewLine(const wchar_t* start, const wchar_t* end)
	//{
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	template<typename T>
	const T* StringUtils::findNextNewLine(const T* start, const T* end)
	{
		while (start < end)
		{
			int nlLen = checkNewLineSequence(start, end);
			if (nlLen > 0) {
				return start;	// 見つかった
			}
			++start;
		}
		return end;	// 見つからなかった
	}
	template const char* StringUtils::findNextNewLine<char>(const char* start, const char* end);
	template const wchar_t* StringUtils::findNextNewLine<wchar_t>(const wchar_t* start, const wchar_t* end);
	template const uint32_t* StringUtils::findNextNewLine<uint32_t>(const uint32_t* start, const uint32_t* end);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	template <typename T>
	int StringUtils::indexOf(const T* str1, const T* str2, int startIndex)
	{
		if (GetLength(str1) <= startIndex) {
			return -1;
		}
		int i = startIndex;
		str1 += startIndex;
		while (*str1)
		{
			if (equals<T>(str1, str2))
				return i;
			++i;
			++str1;
		}
		return i;
	}
	template int StringUtils::indexOf<char>(const char* str1, const char* str2, int startIndex);
	template int StringUtils::indexOf<wchar_t>(const wchar_t* str1, const wchar_t* str2, int startIndex);
	template int StringUtils::indexOf<uint32_t>(const uint32_t* str1, const uint32_t* str2, int startIndex);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	template <typename T>
	int StringUtils::indexOf(const T* str1, T ch, int startIndex)
	{
		const T s[2] = { ch, 0 };
		return indexOf(str1, s, startIndex);
	}
	template int StringUtils::indexOf<char>(const char* str1, char ch, int startIndex);
	template int StringUtils::indexOf<wchar_t>(const wchar_t* str1, wchar_t ch, int startIndex);
	template int StringUtils::indexOf<uint32_t>(const uint32_t* str1, uint32_t ch, int startIndex);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	template <typename T>
	size_t StringUtils::GetLength(const T* str)
	{
		const T *s;
		for (s = str; *s; ++s);
		return (s - str);
	}
	template size_t StringUtils::GetLength<char>(const char* str);
	template size_t StringUtils::GetLength<wchar_t>(const wchar_t* str);
	template size_t StringUtils::GetLength<uint32_t>(const uint32_t* str);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	template <typename T>
	BasicString<T> StringUtils::subString(const T* str, size_t startIndex, size_t length)
	{
		return BasicString<T>(str, startIndex, (length == 0) ? BasicString<T>::npos : length);
	}
	template BasicString<char> StringUtils::subString(const char* str, size_t startIndex, size_t length);
	template BasicString<wchar_t> StringUtils::subString(const wchar_t* str, size_t startIndex, size_t length);
	template BasicString<uint32_t> StringUtils::subString(const uint32_t* str, size_t startIndex, size_t length);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	template<typename T>
	void StringUtils::split(const T* str, const T* delim, StringSplitOptions option, std::vector< BasicString<T> >* out)
	{
		int len = GetLength(str);
		out->clear();

		// 最初の区切り文字を探す
		int nTokenStart = 0;
		int nDelimIndex = indexOf(str, delim, 0);

		if (nDelimIndex >= 0) {
			if (option == StringSplitOptions_None || nDelimIndex > nTokenStart) {
				out->push_back(subString(str, nTokenStart, nDelimIndex - nTokenStart));
			}
		}
		else {
			if (option == StringSplitOptions_None || nTokenStart != len) {
				out->push_back(subString(str, nTokenStart));	// 残り全て
			}
			return;
		}
		// 次のトークン開始位置を指す
		nTokenStart = nDelimIndex + 1;

		while (nTokenStart <= len)
		{
			nDelimIndex = indexOf(str, delim, nTokenStart);
			if (nDelimIndex >= 0) {
				if (option == StringSplitOptions_None || nDelimIndex > nTokenStart) {
					out->push_back(subString(str, nTokenStart, nDelimIndex - nTokenStart));
				}
			}
			else {
				if (option == StringSplitOptions_None || nTokenStart != len) {
					out->push_back(subString(str, nTokenStart));	// 残り全て
				}
				break;
			}
			// 次のトークン開始位置を指す
			nTokenStart = nDelimIndex + 1;
		}
	}
	template void StringUtils::split<char>(const char* str, const char* delim, StringSplitOptions option, std::vector< BasicString<char> >* out);
	template void StringUtils::split<wchar_t>(const wchar_t* str, const wchar_t* delim, StringSplitOptions option, std::vector< BasicString<wchar_t> >* out);
	template void StringUtils::split<uint32_t>(const uint32_t* str, const uint32_t* delim, StringSplitOptions option, std::vector< BasicString<uint32_t> >* out);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	template<typename T>
	void StringUtils::split(const T* str, T delim, StringSplitOptions option, std::vector< BasicString<T> >* out)
	{
		const T s[2] = { delim, 0 };
		split(str, s, option, out);
	}
	template void StringUtils::split<char>(const char* str, char delim, StringSplitOptions option, std::vector< BasicString<char> >* out);
	template void StringUtils::split<wchar_t>(const wchar_t* str, wchar_t delim, StringSplitOptions option, std::vector< BasicString<wchar_t> >* out);
	template void StringUtils::split<uint32_t>(const uint32_t* str, uint32_t delim, StringSplitOptions option, std::vector< BasicString<uint32_t> >* out);


} // namespace Base
} // namespace Core
} // namespace LNote
