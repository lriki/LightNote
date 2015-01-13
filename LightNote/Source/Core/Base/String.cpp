//==============================================================================
// String 
//==============================================================================

#include "stdafx.h"
#include <algorithm>
#include "ReferenceBuffer.h"
#include "Encoding.h"
#include "StringUtils.h"
#include "String.h"

namespace LNote
{
namespace Core
{
namespace Base
{

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
template<typename TChar>
void BasicString<TChar>::assignNativeString(const char* str, size_type begin, size_type length, bool* usedDefaultChar)
{
	LN_THROW(str != NULL, Base::ArgumentException);	// std::string の assing は NULL が渡されたときの動作は未定義。VS2013 では制御が返ってこなくなった

	if (length == std_basic_string::npos) {
		length = 0xFFFFFFFF;
	}
	size_type len = std::min(strlen(str), length);
	ConvertFrom(str + begin, len, Encoding::GetSystemMultiByteEncoding(), usedDefaultChar);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
template<typename TChar>
void BasicString<TChar>::assignNativeString(const wchar_t* str, size_type begin, size_type length, bool* usedDefaultChar)
{
	LN_THROW(str != NULL, Base::ArgumentException);	// std::string の assing は NULL が渡されたときの動作は未定義。VS2013 では制御が返ってこなくなった

	if (length == std_basic_string::npos) {
		length = 0xFFFFFFFF;
	}
	size_type len = std::min(wcslen(str), length) * sizeof(wchar_t);
	ConvertFrom(str + begin, len, Encoding::GetWideCharEncoding(), usedDefaultChar);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
template<typename TChar>
void BasicString<TChar>::ConvertFrom(const void* pBuffer, int uByteCount, const Encoding* pEncoding, bool* outUsedDefaultChar)
{
	size_t bytesUsed;
	size_t charsUsed;
	bool usedDefaultChar;

	RefPtr<ReferenceBuffer> tmpBuffer(	
		Encoding::Convert(pBuffer, uByteCount, pEncoding, getThisTypeEncoding(), 
		&bytesUsed, &charsUsed, &usedDefaultChar));
	if (outUsedDefaultChar != NULL) {
		*outUsedDefaultChar = usedDefaultChar;
	}

	assign((const TChar*)tmpBuffer->getPointer(), bytesUsed / sizeof(TChar));
#if 0
	LN_THROW(pBuffer != NULL, ArgumentException);
	LN_THROW(pEncoding != NULL, ArgumentException);

	Encoding* pThisEncoding = getThisTypeEncoding();

	// 同一のエンコーディングであれば普通に格納するだけでOK
	if (pThisEncoding == pEncoding) {
		(*this) = std_basic_string((TChar*)pBuffer, uByteCount / sizeof(TChar));
		return;
	}

	// バッファのバイト数は、そのバッファのエンコーディングの最低バイト数以上でなければならない
	LN_THROW(uByteCount >= pEncoding->GetMinByteCount(), ArgumentException);


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
	int nConvertedMaxCharCount = uByteCount / pEncoding->GetMinByteCount();
	int nConvertedMaxBytes = nConvertedMaxCharCount * pThisEncoding->GetMaxByteCount();
	nConvertedMaxBytes += sizeof(TChar);	// 終端 \0

	// 変換の中間フォーマットとして、wchar_t を使用する。そのための一時バッファ確保
	int nWideMaxBytes = nConvertedMaxCharCount * Encoding::GetWideCharEncoding()->GetMaxByteCount();
	RefPtr<ReferenceBuffer> tmpBuf(LN_NEW ReferenceBuffer());
	tmpBuf->reserve(nWideMaxBytes + sizeof(wchar_t));	// 終端 \0 考慮 (mbstowcs_s は \0 を書き込もうとする)

	// 変換先バッファ(this)を、最大要素数で確保
	std_basic_string::resize(nConvertedMaxBytes / sizeof(TChar));
	TChar* pThisBuf = &((*this)[0]);

	// 変換実行
	bool bRes;
	size_t uBytesUsed;
	size_t uCharsUsed;
	bool bUsedDefaultChar;
#if defined(LNOTE_WCHAR_16)
	// ソースフォーマットから中間フォーマットへ
	bRes = pEncoding->ConvertToUTF16(
		(const byte_t*)pBuffer,
		uByteCount,
		tmpBuf->getPointer(),
		nWideMaxBytes,			// \0 強制格納に備え、1文字分余裕のあるサイズを指定する
		&uBytesUsed,
		&uCharsUsed);
	LN_THROW(bRes, InvalidFormatException);
	// 中間フォーマットからターゲットフォーマットへ
	bRes = pThisEncoding->ConvertFromUTF16(
		(const byte_t*)tmpBuf->getPointer(),
		uBytesUsed,
		(byte_t*)pThisBuf,
		this->size(),			// \0 強制格納に備え、1文字分余裕のあるサイズを指定する
		&uBytesUsed,
		&uCharsUsed,
		&bUsedDefaultChar);
	LN_THROW(bRes, InvalidFormatException);
	this->resize((uBytesUsed / sizeof(TChar)));

#elif defined(LNOTE_WCHAR_32)
	// ソースフォーマットから中間フォーマットへ
	bRes = pEncoding->ConvertToUTF32(pBuffer, uByteCount, tmpBuf->GetPointer(), tmpBuf->GetSize(), &uBytesUsed, &uCharsUsed);
	LN_THROW(bRes, InvalidFormatException);
	// 中間フォーマットからターゲットフォーマットへ
	bRes = pThisEncoding->ConvertFromUTF32(pBuffer, uByteCount, tmpBuf->GetPointer(), tmpBuf->GetSize(), &uBytesUsed, &uCharsUsed, &bUsedDefaultChar);
	LN_THROW(bRes, InvalidFormatException);

#else
#error "Invalid wchar_t size."
#endif
#endif
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
template<typename TChar>
ReferenceBuffer* BasicString<TChar>::ConvertTo(Encoding* encoding, bool* outUsedDefaultChar) const
{
	size_t bytesUsed;
	size_t charsUsed;
	bool usedDefaultChar;

	ReferenceBuffer* buf = Encoding::Convert(
		c_str(), getByteCount(), getThisTypeEncoding(),
		encoding,
		&bytesUsed, &charsUsed, &usedDefaultChar);
	if (outUsedDefaultChar != NULL) {
		*outUsedDefaultChar = usedDefaultChar;
	}

	return buf;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
template<typename TChar>
BasicString<TChar> BasicString<TChar>::SubString(size_type nStartIndex = 0, size_type nLength = npos) const
{
	return StringT(*this, nStartIndex, nLength);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
template<typename TChar>
void BasicString<TChar>::Replace(const TChar* pFrom, const TChar* pTo)
{
	size_type pos = 0;
	size_t from_length = StringUtils::GetLength(pFrom);
	size_t to_length = StringUtils::GetLength(pTo);

	while (pos = this->find(pFrom, pos), pos != std_basic_string::npos)
	{
		this->replace(pos, from_length, pTo);
		pos += to_length;
	}
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
template<typename TChar>
int BasicString<TChar>::IndexOf(const TChar* pStr, int nStartIndex)
{
	size_type pos = this->find_first_of(pStr, nStartIndex);
	if (pos == std_basic_string::npos) {
		return -1;
	}
	return pos;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
template<typename TChar>
void BasicString<TChar>::Split(const TChar* pDelim, StringSplitOptions option, std::vector< BasicString<TChar> >* pOut)
{
	pOut->clear();

	// 最初の区切り文字を探す
	int nTokenStart = 0;
	int nDelimIndex = IndexOf(pDelim, 0);

	if (nDelimIndex >= 0) {
		if (option == StringSplitOptions_None || nDelimIndex > nTokenStart) {
			pOut->push_back(this->SubString(nTokenStart, nDelimIndex - nTokenStart));
		}
	}
	else {
		if (option == StringSplitOptions_None || nTokenStart != this->size()) {
			pOut->push_back(this->SubString(nTokenStart));	// 残り全て
		}
		return;
	}
	// 次のトークン開始位置を指す
	nTokenStart = nDelimIndex + 1;

	while (nTokenStart <= ((int)this->size()))
	{
		nDelimIndex = IndexOf(pDelim, nTokenStart);
		if (nDelimIndex >= 0) {
			if (option == StringSplitOptions_None || nDelimIndex > nTokenStart) {
				pOut->push_back(this->SubString(nTokenStart, nDelimIndex - nTokenStart));
			}
		}
		else {
			if (option == StringSplitOptions_None || nTokenStart != this->size()) {
				pOut->push_back(this->SubString(nTokenStart));	// 残り全て
			}
			break;
		}
		// 次のトークン開始位置を指す
		nTokenStart = nDelimIndex + 1;
	}
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
template<typename TChar>
Encoding* BasicString<TChar>::getThisTypeEncoding() const
{
	if (sizeof(TChar) == sizeof(char))
	{
		// this のエンコーディングはシステム依存である
		return Encoding::GetSystemMultiByteEncoding();
	}
	else if (sizeof(TChar) == sizeof(wchar_t))
	{
		// this のエンコーディングは wchar_t である
		return Encoding::GetWideCharEncoding();
	}
	else if (sizeof(TChar) == sizeof(uint32_t))
	{
		return Encoding::GetUTF32Encoding();
	}
	else
	{
		LN_THROW(0, NotImplementedException);
	}
}

// テンプレートのインスタンス化
// (インスタンス時の型を char、wchar_t に限定することで、テンプレート関数の実装を cpp に隠蔽する)
template class BasicString<char>;
template class BasicString<wchar_t>;
template class BasicString<uint32_t>;

} // namespace Base
} // namespace Core
} // namespace LNote
