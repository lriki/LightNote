//==============================================================================
// Unicode 
//------------------------------------------------------------------------------
/*
	[2013/11/4] 内部文字コードは環境依存(wchar_t)にするか？
		Archive のファイルマップのキーは wchar_t。
		速度を意識するなら、環境依存の方がいい。
		例えば UTF16 とした場合は Windows では速度は変わらないが、
		Linux、Mac では UTF32 → UTF16 の変換を挟む必要がある。
		Archive 側のキーを変更するという手もあるが…。

		他にも、環境依存にしないと wchar_t 用の API が使えなくなる。
		(というより、使うために文字コード変換が必要になる)

		あと、デバッガで見づらい。
*/
//==============================================================================

#include "stdafx.h"
//#include "../../Dependencies/ConvertUTF/ConvertUTF.h"
#include <LNUnicodeUtils/LNUnicodeUtils.h>
#include "../Base/StringUtils.h"
#include "ReferenceBuffer.h"
#include "Unicode.h"

namespace LNote
{
namespace Core
{
namespace Base
{
	
//==============================================================================
// ■ UTF8CharCodeCodec
//==============================================================================

#if 0
	static const char gTrailingBytesForUTF8[256] = 
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
	};

	static const UTF32 gOffsetsFromUTF8[6] = 
	{ 
		0x00000000UL, 0x00003080UL, 0x000E2080UL, 
		0x03C82080UL, 0xFA082080UL, 0x82082080UL 
	};
#endif
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool UTF8CharCodeCodec::convertToUTF8( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount )
	{
		// 文字数だけカウントして、後はバッファコピー
		LN::UTFConversionResult r = LN::UnicodeUtils::GetUTF8CharCount(inBuffer, inCount, true, outCharCount);
		memcpy_s(outBuffer, outCount, inBuffer, inCount);
		return (r == LN::UTFConversionResult_Success);

#if 0
		int charCount = 0;
		for (int i = 0 ; i < inCount;)
		{
			unsigned short extraBytesToRead = gTrailingBytesForUTF8[inBuffer[i]];

			// マルチバイトの途中でバッファ終端となった
			if (i + extraBytesToRead >= inCount) return false;
		
			// 不正な文字列が見つかった
			if ( isLegalUTF8( &inBuffer[i], extraBytesToRead + 1 ) ) return false;

			i += extraBytesToRead + 1;
			++charCount; 
		}

		memcpy_s( outBuffer, outCount, inBuffer, inCount );
		*outCharCount = charCount;
		return true;
#endif
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool UTF8CharCodeCodec::convertToUTF16( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount )
	{
		LN::UTFConversionOptions opt;
		opt.ReplacementChar = '?';
		LN::UTFConversionResult r = LN::UnicodeUtils::ConvertUTF8toUTF16(
			inBuffer,
			inCount,
			(LN::UTF16*)outBuffer,
			outCount / sizeof(LN::UTF16),
			&opt);
		return (r == LN::UTFConversionResult_Success);
#if 0
		lnU16* utf16 = (lnU16*)outBuffer;
		ConversionResult result = ConvertUTF8toUTF16(
			&inBuffer, 
			inBuffer + inCount, 
			&utf16, 
			(lnU16*)(outBuffer + outCount), 
			strictConversion, 
			outCharCount );
		return ( result == conversionOK );
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool UTF8CharCodeCodec::convertToUTF32( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount )
	{
		LN::UTFConversionOptions opt;
		opt.ReplacementChar = '?';
		LN::UTFConversionResult r = LN::UnicodeUtils::ConvertUTF8toUTF32(
			inBuffer,
			inCount,
			(LN::UTF32*)outBuffer,
			outCount / sizeof(LN::UTF32),
			&opt);
		return (r == LN::UTFConversionResult_Success);
#if 0
		ConversionResult result = ConvertUTF8toUTF32(
			(const UTF8**)&inBuffer, 
			(const UTF8*)(inBuffer + inCount), 
			(UTF32**)&outBuffer, 
			(UTF32*)(outBuffer + outCount), 
			strictConversion, 
			outCharCount );
		return (result == conversionOK);
#endif
	}

//==============================================================================
// ■ UTF16CharCodeCodec
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool UTF16CharCodeCodec::convertToUTF8( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount )
	{
		LN::UTFConversionOptions opt;
		opt.ReplacementChar = '?';
		LN::UTFConversionResult r = LN::UnicodeUtils::ConvertUTF16toUTF8(
			(LN::UTF16*)inBuffer, inCount / sizeof(LN::UTF16),
			(LN::UTF8*)outBuffer, outCount / sizeof(LN::UTF8),
			&opt);
		return (r == LN::UTFConversionResult_Success);
#if 0
		const lnU16* utf16 = (const lnU16*)inBuffer;
		ConvertUTF16toUTF8( 
			&utf16, 
			(const lnU16*)(inBuffer + inCount), 
			&outBuffer, 
			outBuffer + outCount, 
			strictConversion, 
			outCharCount );
		return true;
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool UTF16CharCodeCodec::convertToUTF16( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount )
	{
		// UTF-16 → Utf-16 なので、文字数だけカウントしてバッファは全コピー
		memcpy_s( outBuffer, outCount, inBuffer, inCount );
		LN::UTFConversionResult r = LN::UnicodeUtils::GetUTF16CharCount(
			(LN::UTF16*)inBuffer, inCount / sizeof(LN::UTF16),
			true, outCharCount);
		return (r == LN::UTFConversionResult_Success);
#if 0
		// 終端 NULL
		if ( outCount < inCount + 1 ) return false;
		outBuffer[inCount] = 0;
		outBuffer[inCount + 1] = 0;

		// 文字数カウント
		ConversionResult result = GetUTF16Length(
			(const lnU16**)&inBuffer,
			(const lnU16*)(inBuffer + inCount),
			strictConversion,
			outCharCount );
		return ( result == conversionOK );
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool UTF16CharCodeCodec::convertToUTF32( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount )
	{
		LN::UTFConversionOptions opt;
		opt.ReplacementChar = '?';
		LN::UTFConversionResult r = LN::UnicodeUtils::ConvertUTF16toUTF32(
			(LN::UTF16*)inBuffer, inCount / sizeof(LN::UTF16),
			(LN::UTF32*)outBuffer, outCount / sizeof(LN::UTF32),
			&opt);
		return (r == LN::UTFConversionResult_Success);
#if 0
		ConversionResult result = ConvertUTF16toUTF32(
			(const UTF16**)&inBuffer, 
			(const UTF16*)(inBuffer + inCount), 
			(UTF32**)&outBuffer, 
			(UTF32*)(outBuffer + outCount), 
			strictConversion, 
			outCharCount );
		return (result == conversionOK);
#endif
	}

//==============================================================================
// ■ UTF32CharCodeCodec
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool UTF32CharCodeCodec::convertToUTF8( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount )
	{
		LN::UTFConversionOptions opt;
		opt.ReplacementChar = '?';
		LN::UTFConversionResult r = LN::UnicodeUtils::ConvertUTF32toUTF8(
			(LN::UTF32*)inBuffer, inCount / sizeof(LN::UTF32),
			(LN::UTF8*)outBuffer, outCount / sizeof(LN::UTF8),
			&opt);
		return (r == LN::UTFConversionResult_Success);
#if 0
		ConversionResult result = ConvertUTF32toUTF8( 
			(const UTF32**)&inBuffer, 
			(const UTF32* )(inBuffer + inCount), 
			&outBuffer, 
			outBuffer + outCount, 
			strictConversion, 
			outCharCount );
		return (result == conversionOK);
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool UTF32CharCodeCodec::convertToUTF16( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount )
	{
		LN::UTFConversionOptions opt;
		opt.ReplacementChar = '?';
		LN::UTFConversionResult r = LN::UnicodeUtils::ConvertUTF32toUTF16(
			(LN::UTF32*)inBuffer, inCount / sizeof(LN::UTF32),
			(LN::UTF16*)outBuffer, outCount / sizeof(LN::UTF16),
			&opt);
		return (r == LN::UTFConversionResult_Success);
#if 0
		const lnU32* utf32 = (const lnU32*)outBuffer;
		lnU16* utf16 = (lnU16*)outBuffer;
		ConversionResult result = ConvertUTF32toUTF16(
			(const UTF32**)&inBuffer,
			(const UTF32* )(inBuffer + inCount), 
			&utf16, 
			(lnU16*)(outBuffer + outCount), 
			strictConversion, 
			outCharCount );
		return (result == conversionOK);
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool UTF32CharCodeCodec::convertToUTF32( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount )
	{
		// そのままコピー
		memcpy_s( outBuffer, outCount, inBuffer, inCount );
		*outCharCount = inCount / sizeof(LN::UTF32);
		return true;
	}

//==============================================================================
// ■ ShiftJISCharCodeCodec
//==============================================================================

	extern "C" unsigned short sjis2utf16_table[];

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool ShiftJISCharCodeCodec::convertToUTF8( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount )
	{
		return false;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool ShiftJISCharCodeCodec::convertToUTF16( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount )
	{
		lnU16* utf16 = (lnU16*)outBuffer;
		int utf16Count = 0;
		int utf16CountMax = outCount / sizeof(lnU16);
		for ( int i = 0 ; i < inCount; ++i )
		{
			// 文字数オーバー
			if ( utf16Count > utf16CountMax ) return false;

			const lnU8* sjis = &inBuffer[i];
			
			// マルチバイト文字の先行バイトである
			if ( ( *sjis >= 0x81 && *sjis <= 0x9f ) || ( *sjis >= 0xe0 && *sjis <= 0xfc ) )
			{
				utf16[utf16Count] = sjis2utf16_table[
					(*(sjis) << 8) | (*(sjis + 1) & 0xff)];
				++i;
				++utf16Count;
			}
			// ASCII
			else
			{
				utf16[utf16Count] = sjis2utf16_table[*sjis];
				++utf16Count;
			}
		}
		utf16[utf16Count] = L'\0';

		*outCharCount = utf16Count;
		return true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool ShiftJISCharCodeCodec::convertToUTF32( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount )
	{
		// 一度 UTF16 に変換
		lnByte* tmpBuf = LN_NEW lnByte[outCount];
		int dummy = 0;
		if ( !convertToUTF16(inBuffer, inCount, tmpBuf, outCount, &dummy ) ) {
			delete[] tmpBuf;
			return false;
		}

		// UTF16 から UTF32 に変換
		UTF16CharCodeCodec utf16Codec;
		bool r = utf16Codec.convertToUTF32( tmpBuf, outCount, outBuffer, outCount, outCharCount );
		delete[] tmpBuf;
		return r;
	}

//==============================================================================
// ■ UnicodeString
//==============================================================================

	static UTF8CharCodeCodec		gUTF8CharCodeCodec;
	static UTF16CharCodeCodec		gUTF16CharCodeCodec;
	static UTF32CharCodeCodec		gUTF32CharCodeCodec;
	static ShiftJISCharCodeCodec	gShiftJISCharCodeCodec;
	
#ifdef LNOTE_WIN32
	static ICharCodeCodec* gDefaultMultiByteCodec = &gShiftJISCharCodeCodec;
#else
	static ICharCodeCodec* gDefaultMultiByteCodec = &gUTF8CharCodeCodec;
#endif
#if defined(LNOTE_WCHAR_16)
	ICharCodeCodec* UnicodeString::InternalCodec = &gUTF16CharCodeCodec;
#elif  defined(LNOTE_WCHAR_32)
	ICharCodeCodec* UnicodeString::InternalCodec = &gUTF32CharCodeCodec;
#else
#error "WCHAR_MAX != 0xffff"
#endif

	UTF8CharCodeCodec* UnicodeString::getUTF8Codec()
	{
		return &gUTF8CharCodeCodec;
	}
	UTF16CharCodeCodec* UnicodeString::getUTF16Codec()
	{
		return &gUTF16CharCodeCodec;
	}
	UTF32CharCodeCodec* UnicodeString::getUTF32Codec()
	{
		return &gUTF32CharCodeCodec;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	UnicodeString::UnicodeString()
		: mString		( NULL )
		, mCharSize		( 0 )
		, mByteCount	( 0 )
		, mCharCount	( 0 )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	UnicodeString::~UnicodeString()
	{
		_dispose();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void UnicodeString::setDecodeBytes( const char* str, int len )
	{
		len = (len <= -1) ? strlen( str ) : len;
		setDecodeBytes( (const lnByte*)str, len, gDefaultMultiByteCodec );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void UnicodeString::setDecodeBytes( const wchar_t* str )
	{
		setDecodeBytes( (const lnByte*)str, wcslen( str ) * sizeof(wchar_t), InternalCodec );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void UnicodeString::setDecodeBytes( const lnByte* bytes, int byteCount, ICharCodeCodec* codec )
	{
		_dispose();

		// 想定最大文字数 + \0
		// (例えばUTF-8の場合、最低は1byte。最悪でも必要となる文字数がわかる)
		int charCount = byteCount / codec->getMinStride() + 1;
		
		// これから確保するバッファのバイト数
		mByteCount = charCount * sizeof(wchar_t);

#if defined(LNOTE_WCHAR_UTF16)
		// UTF16 へ
		mString = LN_NEW wchar_t[charCount];
		codec->convertToUTF16( bytes, byteCount, (lnByte*)mString, mByteCount, &mCharCount );
		mValidByteCount = wcslen( (wchar_t*)mString );
#elif defined(LNOTE_WCHAR_UTF32)
		// UTF32 へ
		mString = LN_NEW wchar_t[charCount];
		codec->convertToUTF32( bytes, byteCount, (lnByte*)mString, mByteCount, &mCharCount );
		mValidByteCount = wcslen( (wchar_t*)mString );
#else
	#error "invalid wchar_t char code."
#endif
		/*
		if ( sizeof(wchar_t) == 2 )
		{
			mString = LN_NEW wchar_t[charCount];
			codec->convertToUTF16( bytes, byteCount, (lnByte*)mString, mByteCount, &mCharCount );
			mValidByteCount = wcslen( (wchar_t*)mString );
		}
		else
		{
			LN_ERR2_ASSERT( 0 );
		}
		*/

#if 0
		// 想定最大文字数 + \0
		int charCount = byteCount / codec->getMinStride() + 1;

		// 新しい文字列を格納するバッファ
		mByteCount = charCount * InternalCodec->getMaxStride();
		mString = LN_NEW lnByte[mByteCount];
		memset( &mString[mByteCount - InternalCodec->getMaxStride() - 1], 0, InternalCodec->getMaxStride() );

		// UTF-8 へ
		if ( InternalCodec == &gUTF8CharCodeCodec )
		{
			mCharSize = 1;
			codec->convertToUTF8( bytes, byteCount, mString, mByteCount, &mCharCount );
			mValidByteCount = strlen( (char*)mString );
		}
		// UTF-16 へ
		else
		{
			mCharSize = 2;
			codec->convertToUTF16( bytes, byteCount, mString, mByteCount, &mCharCount );
			mValidByteCount = wcslen( (wchar_t*)mString );
		}

#if defined(LNOTE_WIN32) && defined(LNOTE_DEBUG)
		mDebug = (wchar_t*)mString;
#endif
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//void UnicodeString::setString( ICharCodeCodec* internalCodec, const char* nativeMultiByteString )
	//{
	//}

	//void UnicodeString::setString( ICharCodeCodec* internalCodec, const wchar_t* nativeWideString )
	//{
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void UnicodeString::getUTF8String( std::string* outStr ) const
	{
		// 想定最大文字数 + \0
		// (例えばUTF-8の場合、最低は1byte。最悪でも必要となる文字数がわかる)
		int charMaxCount = (mCharCount * gUTF8CharCodeCodec.getMaxStride()) + 1;

		// バッファ確保
		LRefPtr<Base::ReferenceBuffer> buf( LN_NEW Base::ReferenceBuffer() );
		buf->reserve( charMaxCount );

		// 変換
		if ( InternalCodec == &gUTF16CharCodeCodec )
		{
			int dummy;
			gUTF16CharCodeCodec.convertToUTF8( 
				(const lnByte*)mString, 
				mByteCount, 
				buf->getPointer(), 
				buf->getSize(),
				&dummy );
		}
		else if ( InternalCodec == &gUTF32CharCodeCodec )
		{
			int dummy;
			gUTF32CharCodeCodec.convertToUTF8( 
				(const lnByte*)mString, 
				mByteCount, 
				buf->getPointer(), 
				buf->getSize(),
				&dummy );
		}
		else {
			LN_THROW_InvalidOperation(0);
		}

		// 出力
		*outStr = (char*)buf->getPointer();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	UnicodeString::UnicodeString( const UnicodeString& obj )
		: mString		( NULL )
		, mByteCount	( 0 )
		, mCharCount	( 0 )
	{
		_dispose();

		mByteCount = obj.mByteCount;
		mCharCount = obj.mCharCount;
		mString = LN_NEW wchar_t[mCharCount + 1];
		memcpy_s( mString, mByteCount, obj.mString, mByteCount );
		mString[mCharCount] = L'\0';
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	UnicodeString& UnicodeString::operator = ( const UnicodeString& obj )
	{
		_dispose();

		mByteCount = obj.mByteCount;
		mCharCount = obj.mCharCount;
		mString = LN_NEW wchar_t[mCharCount + 1];
		memcpy_s( mString, mByteCount, obj.mString, mByteCount );
		mString[mCharCount] = L'\0';
		return *this;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	UnicodeString& UnicodeString::operator += ( const UnicodeString& obj )
	{
		wchar_t* new_str = Base::StringUtils::merge( mString, obj.mString );
        SAFE_DELETE_ARRAY( mString );
        mString = new_str;
		return *this;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool UnicodeString::operator == ( const UnicodeString& obj ) const
	{
		return (UnicodeString::strcmp( *this, obj ) == 0);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool UnicodeString::operator != ( const UnicodeString& obj ) const
	{
		return (UnicodeString::strcmp( *this, obj ) != 0);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool UnicodeString::operator <  ( const UnicodeString& obj ) const
	{
		return (UnicodeString::strcmp( *this, obj ) < 0);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void UnicodeString::_dispose()
	{
		SAFE_DELETE_ARRAY( mString );
		mByteCount = 0;
		mCharCount = 0;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void UnicodeString::ConvertMultiToWide(const char* multi, std::wstring* wide)
	{
		UnicodeString t;
		t.setDecodeBytes(multi);
		*wide = t.getString();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	template<typename T>
	static int strcmpTemplate(const T *s1, const T *s2)
	{
		const T *ss1, *ss2;
		for (ss1 = (const T*)s1, ss2 = (const T*)s2;
			*ss1 == *ss2 && *ss1 != (const T)NULL/*'\0'*/;
			ss1++, ss2++)
		;
		return *ss1 - *ss2;
	}
	int UnicodeString::strcmp( const UnicodeString& str1, const UnicodeString& str2 )
	{
		if ( str1.mCharSize == str2.mCharSize ) {
			if ( str1.mCharSize == 2 ) {
				return strcmpTemplate<lnU16>( (lnU16*)str1.mString, (lnU16*)str2.mString );
			}
			else if ( str1.mCharSize == 4 ) {
				return strcmpTemplate<lnU32>( (lnU32*)str1.mString, (lnU32*)str2.mString );
			}
		}
		return -1;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	FILE* ln_wfopen( const wchar_t* filePath, const wchar_t* mode )
	{
#if defined(LNOTE_WIN32)
		return _wfopen( filePath, mode );
#elif defined(LNOTE_GNUC)
		char* mbspath = StringUtil::convWideToMulti( filePath );
		if ( mbspath == NULL ) {
			return NULL;
		}

		char* mbsmode = StringUtil::convWideToMulti( mode );
		if ( mbsmode == NULL ) {
			delete[] mbspath;
			return NULL;
		}
		
		printf("fopen : %s\n", mbspath);
		FILE* file = fopen( mbspath, mbsmode );

		delete[] mbspath;
		delete[] mbsmode;
		return file;
#endif
	}

} // namespace Base
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================