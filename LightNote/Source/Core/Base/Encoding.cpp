//==============================================================================
// Encoding 
//==============================================================================

#include "stdafx.h"
#include <LNUnicodeUtils/LNUnicodeUtils.h>
#include "ReferenceBuffer.h"
#include "Encoding.h"

namespace LNote
{
namespace Core
{
namespace Base
{

//==============================================================================
// Encoding
//==============================================================================

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
Encoding* Encoding::GetSystemMultiByteEncoding()
{
	static SystemMultiByteEncoding systemEncoding;
	return &systemEncoding;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
Encoding* Encoding::GetWideCharEncoding()
{
#if defined(LNOTE_WCHAR_16)
	static UTF16Encoding wideEncoding;
	return &wideEncoding;
#elif defined(LNOTE_WCHAR_32)
	static UTF32Encoding wideEncoding;
	return &wideEncoding;
#else
#error "Invalid wchar_t size."
#endif
	return NULL;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
Encoding* Encoding::GetUTF8Encoding()
{
	static UTF8Encoding encoding;
	return &encoding;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
Encoding* Encoding::GetUTF16Encoding()
{
	static UTF16Encoding encoding;
	return &encoding;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
Encoding* Encoding::GetUTF32Encoding()
{
	static UTF32Encoding encoding;
	return &encoding;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
ReferenceBuffer* Encoding::Convert(
	const void* src, size_t srcByteCount, const Encoding* srcEncoding,
	const Encoding* targetEncoding,
	size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar)
{
	LN_THROW(src != NULL, ArgumentException);
	LN_THROW(srcEncoding != NULL, ArgumentException);
	LN_THROW(targetEncoding != NULL, ArgumentException);
	LN_THROW(srcByteCount >= srcEncoding->GetMinByteCount(), ArgumentException);	// バッファのバイト数は、そのバッファのエンコーディングの最低バイト数以上でなければならない

	/*	変換後に必要な最大のバイト数を求める
		例)
		pEncoding = UTF32 だったら
		変換後最大文字数 = uByteCount / 1文字最低バイト数(4)

		pEncoding = UTF16 だったら
		変換後最大文字数 = uByteCount / 1文字最低バイト数(2)

		pEncoding = UTF8 だったら
		変換後最大文字数 = uByteCount / 1文字最大バイト数(1)

		これに、pThisEncoding の1文字の最大バイト数を掛ける。
	*/
	int nConvertedMaxCharCount = srcByteCount / srcEncoding->GetMinByteCount();
	int nConvertedMaxBytes = nConvertedMaxCharCount * targetEncoding->GetMaxByteCount();
	nConvertedMaxBytes += targetEncoding->GetMinByteCount();	// 終端 \0 の分

	// 変換の中間フォーマットとして wchar_t を使用する。そのための一時バッファ確保
	int nWideMaxBytes = nConvertedMaxCharCount * Encoding::GetWideCharEncoding()->GetMaxByteCount();
	RefPtr<ReferenceBuffer> tmpBuf(LN_NEW ReferenceBuffer());
	tmpBuf->reserve(nWideMaxBytes + sizeof(wchar_t));	// 終端 \0 考慮 (mbstowcs_s は \0 を書き込もうとする)

	// 変換先バッファを、最大要素数で確保
	RefPtr<ReferenceBuffer> targetBuf(LN_NEW ReferenceBuffer());
	targetBuf->reserve(nConvertedMaxBytes/* / targetEncoding->GetMinByteCount()*/);

	// 変換実行
	bool bRes;
	size_t uBytesUsed;
	size_t uCharsUsed;
	bool bUsedDefaultChar;
#if defined(LNOTE_WCHAR_16)
	// ソースフォーマットから中間フォーマットへ
	bRes = srcEncoding->ConvertToUTF16(
		(const byte_t*)src,
		srcByteCount,
		tmpBuf->getPointer(),
		nWideMaxBytes,			// \0 強制格納に備え、1文字分余裕のあるサイズを指定する
		&uBytesUsed,
		&uCharsUsed);
	LN_THROW(bRes, InvalidFormatException);
	// 中間フォーマットからターゲットフォーマットへ
	bRes = targetEncoding->ConvertFromUTF16(
		(const byte_t*)tmpBuf->getPointer(),
		uBytesUsed,
		(byte_t*)targetBuf->getPointer(),
		targetBuf->getSize(),	// \0 強制格納に備え、1文字分余裕のあるサイズを指定する
		&uBytesUsed,
		&uCharsUsed,
		&bUsedDefaultChar);
	LN_THROW(bRes, InvalidFormatException);

#elif defined(LNOTE_WCHAR_32)

#else
#error "Invalid wchar_t size."
#endif

	*bytesUsed = uBytesUsed;
	*charsUsed = uCharsUsed;
	*usedDefaultChar = bUsedDefaultChar;
	targetBuf->resize(uBytesUsed);
	targetBuf.safeAddRef();
	return targetBuf;
}

//==============================================================================
// SystemMultiByteEncoding
//==============================================================================

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
int SystemMultiByteEncoding::GetMinByteCount() const
{
	return 1;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
int SystemMultiByteEncoding::GetMaxByteCount() const
{
#if 1
	// mbstowcs は setlocale() によって動作が変わるが、
	// どんなマルチバイトコードが使われるのか、そのコードの最大バイト数はいくつかは得ることができない。
	// ※ WinAPI では GetCPInfoEx() でコードページごとの最大バイト数を得ることができるが、
	//    setlocale() を使用しても _getmbcp や CP_THREAD_ACP で得られるコードページに変化が無い。
	// 代わりに、最悪のサイズとして UTF-8 の最大サイズを想定する。
	return 6;
#else
#ifdef _WIN32
	CPINFOEX info;
	if (!::GetCPInfoEx(CP_THREAD_ACP, 0, &info)) {
		LN_THROW(0, Win32Exception, ::GetLastError());
	}
	return info.MaxCharSize;
#else
	return 6;	// UTF-8
#endif
#endif
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool SystemMultiByteEncoding::ConvertToUTF16(const byte_t* pInBuffer, size_t uInByteCount, byte_t* pOutBuffer, size_t uOutBufferByteCount, size_t* pBytesUsed, size_t* pCharsUsed) const
{
#ifdef LNOTE_WCHAR_16
	// mbstowcs_s は変換したいサイズを指定することができず、基本的に \0 まで変換することになる。
	// そのため、一度別バッファに移して \0 を付ける
	ReferenceBuffer tmpInBuffer;
	tmpInBuffer.reserve(uInByteCount + sizeof(char));	// NULL 文字分 + 1
	tmpInBuffer.copy(pInBuffer, uInByteCount);
	char* tmpStr = (char*)tmpInBuffer.getPointer();
	tmpStr[uInByteCount] = '\0';
	
	// Multi → Wide
	size_t len;
	errno_t err = mbstowcs_s(
		&len,										// 変換された文字数
		(wchar_t*)pOutBuffer,						// 出力先バッファ
		(uOutBufferByteCount / sizeof(wchar_t)) + 1,// 出力先バッファのサイズ (文字数 = wchar_t としての要素数)
		tmpStr,										// 変換元バッファ
		uOutBufferByteCount / sizeof(wchar_t));		// pOutBuffer に格納する wchar_t の最大数
	LN_THROW(err == 0, ArgumentException);

	// 終端には \0 が強制的に付加される。純粋な文字部分のサイズが欲しいので -1 する。
	// TODO: つまり、出力バッファは ↑の第2引数 +1ワード分 確保しておく必要がある
	len--;

	// mbstowcs じゃ文字数カウントはできないので UnicodeUtils を使う
	int count;
	if (LN::UnicodeUtils::GetUTF16CharCount((LN::UTF16*)pOutBuffer, len, true, &count) != LN::UTFConversionResult_Success){
		return false;
	}

	*pBytesUsed = len * sizeof(wchar_t);
	*pCharsUsed = count;
#else
	LN_THROW(0, NotImplementedException);
#endif
	return true;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool SystemMultiByteEncoding::ConvertToUTF32(const byte_t* pInBuffer, size_t uInByteCount, byte_t* pOutBuffer, size_t uOutBufferByteCount, size_t* pBytesUsed, size_t* pCharsUsed) const
{
#ifdef LNOTE_WCHAR_16
	LN_THROW(0, NotImplementedException);
#else
	// Multi → Wide
	size_t len;
	size_t len;
	errno_t err = mbstowcs_s(&len, (wchar_t*)pOutBuffer, uInByteCount / sizeof(wchar_t), (const char*)pInBuffer, uInByteCount);
	LN_THROW(err == 0, ArgumentException);

	*pBytesUsed = len * sizeof(wchar_t);
	*pCharsUsed = len;	// UTF32 なので要素数 == 文字数
#endif
	return true;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool SystemMultiByteEncoding::ConvertFromUTF16(const byte_t* pInBuffer, size_t uInByteCount, byte_t* pOutBuffer, size_t uOutBufferByteCount, size_t* pBytesUsed, size_t* pCharsUsed, bool* pUsedDefaultChar) const
{
#ifdef LNOTE_WCHAR_16
	// wcsrtombs_s は変換したいサイズを指定することができず、基本的に \0 まで変換することになる。
	// そのため、一度別バッファに移して \0 を付ける
	ReferenceBuffer tmpWideBuffer;
	tmpWideBuffer.reserve(uInByteCount + sizeof(wchar_t));	// NULL 文字分
	tmpWideBuffer.copy(pInBuffer, uInByteCount);
	wchar_t* wideStr = (wchar_t*)tmpWideBuffer.getPointer();
	wideStr[uInByteCount / sizeof(wchar_t)] = L'\0';

	// Wide → Multi
	size_t convertedLen;
	mbstate_t mbstate = 0;
	::memset((void*)&mbstate, 0, sizeof(mbstate));
	errno_t err = wcsrtombs_s(
		&convertedLen,				// 変換された文字数が格納される (Multi への変換なので、バイト数)
		(char*)pOutBuffer,			// 出力先バッファ
		uOutBufferByteCount,		// 出力先バッファのサイズ (バイト数)
		(const wchar_t**)&wideStr,	// 変換元
		_TRUNCATE,					// 出力先バッファに格納できる最大バイト数 (実質↑↑と同じ。_TRUNCATE で \0 まで)
		&mbstate);
	LN_THROW(err == 0, ArgumentException);

	// 終端には \0 が強制的に付加される。純粋な文字部分のサイズが欲しいので -1 する。
	// TODO: つまり、出力バッファは ↑の第2引数 +1ワード分 確保しておく必要がある
	convertedLen--;

	// wcsrtombs_s じゃ文字数カウントはできないので UnicodeUtils を使う
	int count;
	if (LN::UnicodeUtils::GetUTF16CharCount((LN::UTF16*)pInBuffer, uInByteCount / sizeof(LN::UTF16), true, &count) != LN::UTFConversionResult_Success){
		return false;
	}

	*pBytesUsed = convertedLen;
	*pCharsUsed = count;
#else
	LN_THROW(0, NotImplementedException);
#endif
	return true;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool SystemMultiByteEncoding::ConvertFromUTF32(const byte_t* pInBuffer, size_t uInByteCount, byte_t* pOutBuffer, size_t uOutBufferByteCount, size_t* pBytesUsed, size_t* pCharsUsed, bool* pUsedDefaultChar) const
{
#ifdef LN_WCHAR_16
	LN_THROW(0, NotImplementedException);
#else
	// wcsrtombs_s は変換したいサイズを指定することができず、基本的に \0 まで変換することになる。
	// そのため、一度別バッファに移して \0 を付ける
	ReferenceBuffer tmpWideBuffer;
	tmpWideBuffer.reserve(uInByteCount + sizeof(wchar_t));	// NULL 文字分
	tmpWideBuffer.copy(pInBuffer, uInByteCount);
	wchar_t* wideStr = (wchar_t*)tmpWideBuffer.getPointer();
	wideStr[uInByteCount] = L'\0';

	// TODO ↑ uInByteCount+1 が NULL だったら変換しなくていいようにすればパフォーマンス上がるかも

	// Wide → Multi
	size_t convertedLen;
	mbstate_t mbstate = 0;
	::memset((void*)&mbstate, 0, sizeof(mbstate));
	errno_t err = wcsrtombs_s(
		&convertedLen,				// 変換された文字数が格納される (Multi への変換なので、バイト数)
		(char*)pOutBuffer,			// 出力先バッファ
		uOutBufferByteCount,		// 出力先バッファのサイズ (バイト数)
		(const wchar_t**)&wideStr,	// 変換元
		_TRUNCATE,					// 出力先バッファに格納できる最大バイト数 (実質↑↑と同じ。_TRUNCATE で \0 まで)
		&mbstate);
	LN_THROW(err == 0, ArgumentException);

	*pBytesUsed = convertedLen;
	*pCharsUsed = uInByteCount / sizeof(wchar_t);	// UTF32 なので要素数 == 文字数
#endif
	return true;
}


//===============================================================================
// UTF8Encoding
//===============================================================================

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool UTF8Encoding::ConvertToUTF16(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed) const
{
	LN::UTFConversionOptions opt;
	opt.ReplacementChar = '?';
	LN::UTFConversionResult r = LN::UnicodeUtils::ConvertUTF8toUTF16(
		(LN::UTF8*)inBuffer, inByteCount / sizeof(LN::UTF8),
		(LN::UTF16*)outBuffer, outBufferByteCount / sizeof(LN::UTF16),
		&opt);
	if (r != LN::UTFConversionResult_Success) {
		return false;
	}
	*bytesUsed = opt.ConvertedTargetLength * sizeof(LN::UTF16);
	*charsUsed = opt.CharCount;
	return true;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool UTF8Encoding::ConvertToUTF32(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed) const
{
	LN::UTFConversionOptions opt;
	opt.ReplacementChar = '?';
	LN::UTFConversionResult r = LN::UnicodeUtils::ConvertUTF8toUTF32(
		(LN::UTF8*)inBuffer, inByteCount / sizeof(LN::UTF8),
		(LN::UTF32*)outBuffer, outBufferByteCount / sizeof(LN::UTF32),
		&opt);
	if (r != LN::UTFConversionResult_Success) {
		return false;
	}
	*bytesUsed = opt.ConvertedTargetLength * sizeof(LN::UTF32);
	*charsUsed = opt.CharCount;
	return true;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool UTF8Encoding::ConvertFromUTF16(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar) const
{
	LN::UTFConversionOptions opt;
	opt.ReplacementChar = '?';
	LN::UTFConversionResult r = LN::UnicodeUtils::ConvertUTF16toUTF8(
		(LN::UTF16*)inBuffer, inByteCount / sizeof(LN::UTF16),
		(LN::UTF8*)outBuffer, outBufferByteCount / sizeof(LN::UTF8),
		&opt);
	if (r != LN::UTFConversionResult_Success) {
		return false;
	}
	*bytesUsed = opt.ConvertedTargetLength * sizeof(LN::UTF8);
	*charsUsed = opt.CharCount;
	*usedDefaultChar = (opt.IllegalCharCount != 0);
	return true;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool UTF8Encoding::ConvertFromUTF32(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar) const
{
	LN::UTFConversionOptions opt;
	opt.ReplacementChar = '?';
	LN::UTFConversionResult r = LN::UnicodeUtils::ConvertUTF32toUTF8(
		(LN::UTF32*)inBuffer, inByteCount / sizeof(LN::UTF32),
		(LN::UTF8*)outBuffer, outBufferByteCount / sizeof(LN::UTF8),
		&opt);
	if (r != LN::UTFConversionResult_Success) {
		return false;
	}
	*bytesUsed = opt.ConvertedTargetLength * sizeof(LN::UTF8);
	*charsUsed = opt.CharCount;
	*usedDefaultChar = (opt.IllegalCharCount != 0);
	return true;
}


//===============================================================================
// UTF16Encoding
//===============================================================================

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool UTF16Encoding::ConvertToUTF16(const byte_t* pInBuffer, size_t uInByteCount, byte_t* pOutBuffer, size_t uOutBufferByteCount, size_t* pBytesUsed, size_t* pCharsUsed) const
{
	// UTF16 から UTF16 への変換。そのままコピーでよい
	errno_t err = memcpy_s(pOutBuffer, uOutBufferByteCount, pInBuffer, uInByteCount);
	LN_THROW(err == 0, ArgumentException);

	// 文字数はカウントする
	int count;
	if (LN::UnicodeUtils::GetUTF16CharCount((LN::UTF16*)pInBuffer, uInByteCount / sizeof(LN::UTF16), true, &count) != LN::UTFConversionResult_Success){
		return false;
	}

	*pBytesUsed = uInByteCount;
	*pCharsUsed = count;
	return true;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool UTF16Encoding::ConvertToUTF32(const byte_t* pInBuffer, size_t uInByteCount, byte_t* pOutBuffer, size_t uOutBufferByteCount, size_t* pBytesUsed, size_t* pCharsUsed) const
{
	LN_THROW(0, NotImplementedException);
	return true;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool UTF16Encoding::ConvertFromUTF16(const byte_t* pInBuffer, size_t uInByteCount, byte_t* pOutBuffer, size_t uOutBufferByteCount, size_t* pBytesUsed, size_t* pCharsUsed, bool* pUsedDefaultChar) const
{
	*pUsedDefaultChar = false;	// 単なるコピーなのでデフォルト文字はありえない
	return ConvertToUTF16(pInBuffer, uInByteCount, pOutBuffer, uOutBufferByteCount, pBytesUsed, pCharsUsed);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool UTF16Encoding::ConvertFromUTF32(const byte_t* pInBuffer, size_t uInByteCount, byte_t* pOutBuffer, size_t uOutBufferByteCount, size_t* pBytesUsed, size_t* pCharsUsed, bool* pUsedDefaultChar) const
{
	LN_THROW(0, NotImplementedException);
	return true;
}

//===============================================================================
// UTF32Encoding
//===============================================================================

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool UTF32Encoding::ConvertToUTF16(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed) const
{
	LN::UTFConversionOptions opt;
	opt.ReplacementChar = '?';
	LN::UTFConversionResult r = LN::UnicodeUtils::ConvertUTF32toUTF16(
		(LN::UTF32*)inBuffer, inByteCount / sizeof(LN::UTF32),
		(LN::UTF16*)outBuffer, outBufferByteCount / sizeof(LN::UTF16),
		&opt);
	if (r != LN::UTFConversionResult_Success) {
		return false;
	}
	*bytesUsed = opt.ConvertedTargetLength * sizeof(LN::UTF16);
	*charsUsed = opt.CharCount;
	return true;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool UTF32Encoding::ConvertToUTF32(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed) const
{
	// UTF16 から UTF16 への変換。そのままコピーでよい
	errno_t err = memcpy_s(outBuffer, outBufferByteCount, inBuffer, inByteCount);
	LN_THROW(err == 0, ArgumentException);

	// バッファ使用料・文字数はそのまま
	*bytesUsed = inByteCount;
	*charsUsed = inByteCount / sizeof(LN::UTF32);
	return true;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool UTF32Encoding::ConvertFromUTF16(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar) const
{
	LN::UTFConversionOptions opt;
	opt.ReplacementChar = '?';
	LN::UTFConversionResult r = LN::UnicodeUtils::ConvertUTF16toUTF32(
		(LN::UTF16*)inBuffer, inByteCount / sizeof(LN::UTF16),
		(LN::UTF32*)outBuffer, outBufferByteCount / sizeof(LN::UTF32),
		&opt);
	if (r != LN::UTFConversionResult_Success) {
		return false;
	}
	*bytesUsed = opt.ConvertedTargetLength * sizeof(LN::UTF32);
	*charsUsed = opt.CharCount;
	*usedDefaultChar = (opt.IllegalCharCount != 0);
	return true;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool UTF32Encoding::ConvertFromUTF32(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar) const
{
	*usedDefaultChar = false;	// 単なるコピーなので不正文字はなし
	return ConvertToUTF32(inBuffer, inByteCount, outBuffer, outBufferByteCount, bytesUsed, charsUsed);
}


} // namespace Base
} // namespace Core
} // namespace LNote

