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
	LN_THROW(srcByteCount >= srcEncoding->GetMinByteCount(), ArgumentException);	// �o�b�t�@�̃o�C�g���́A���̃o�b�t�@�̃G���R�[�f�B���O�̍Œ�o�C�g���ȏ�łȂ���΂Ȃ�Ȃ�

	/*	�ϊ���ɕK�v�ȍő�̃o�C�g�������߂�
		��)
		pEncoding = UTF32 ��������
		�ϊ���ő啶���� = uByteCount / 1�����Œ�o�C�g��(4)

		pEncoding = UTF16 ��������
		�ϊ���ő啶���� = uByteCount / 1�����Œ�o�C�g��(2)

		pEncoding = UTF8 ��������
		�ϊ���ő啶���� = uByteCount / 1�����ő�o�C�g��(1)

		����ɁApThisEncoding ��1�����̍ő�o�C�g�����|����B
	*/
	int nConvertedMaxCharCount = srcByteCount / srcEncoding->GetMinByteCount();
	int nConvertedMaxBytes = nConvertedMaxCharCount * targetEncoding->GetMaxByteCount();
	nConvertedMaxBytes += targetEncoding->GetMinByteCount();	// �I�[ \0 �̕�

	// �ϊ��̒��ԃt�H�[�}�b�g�Ƃ��� wchar_t ���g�p����B���̂��߂̈ꎞ�o�b�t�@�m��
	int nWideMaxBytes = nConvertedMaxCharCount * Encoding::GetWideCharEncoding()->GetMaxByteCount();
	RefPtr<ReferenceBuffer> tmpBuf(LN_NEW ReferenceBuffer());
	tmpBuf->reserve(nWideMaxBytes + sizeof(wchar_t));	// �I�[ \0 �l�� (mbstowcs_s �� \0 �������������Ƃ���)

	// �ϊ���o�b�t�@���A�ő�v�f���Ŋm��
	RefPtr<ReferenceBuffer> targetBuf(LN_NEW ReferenceBuffer());
	targetBuf->reserve(nConvertedMaxBytes/* / targetEncoding->GetMinByteCount()*/);

	// �ϊ����s
	bool bRes;
	size_t uBytesUsed;
	size_t uCharsUsed;
	bool bUsedDefaultChar;
#if defined(LNOTE_WCHAR_16)
	// �\�[�X�t�H�[�}�b�g���璆�ԃt�H�[�}�b�g��
	bRes = srcEncoding->ConvertToUTF16(
		(const byte_t*)src,
		srcByteCount,
		tmpBuf->getPointer(),
		nWideMaxBytes,			// \0 �����i�[�ɔ����A1�������]�T�̂���T�C�Y���w�肷��
		&uBytesUsed,
		&uCharsUsed);
	LN_THROW(bRes, InvalidFormatException);
	// ���ԃt�H�[�}�b�g����^�[�Q�b�g�t�H�[�}�b�g��
	bRes = targetEncoding->ConvertFromUTF16(
		(const byte_t*)tmpBuf->getPointer(),
		uBytesUsed,
		(byte_t*)targetBuf->getPointer(),
		targetBuf->getSize(),	// \0 �����i�[�ɔ����A1�������]�T�̂���T�C�Y���w�肷��
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
	// mbstowcs �� setlocale() �ɂ���ē��삪�ς�邪�A
	// �ǂ�ȃ}���`�o�C�g�R�[�h���g����̂��A���̃R�[�h�̍ő�o�C�g���͂������͓��邱�Ƃ��ł��Ȃ��B
	// �� WinAPI �ł� GetCPInfoEx() �ŃR�[�h�y�[�W���Ƃ̍ő�o�C�g���𓾂邱�Ƃ��ł��邪�A
	//    setlocale() ���g�p���Ă� _getmbcp �� CP_THREAD_ACP �œ�����R�[�h�y�[�W�ɕω��������B
	// ����ɁA�ň��̃T�C�Y�Ƃ��� UTF-8 �̍ő�T�C�Y��z�肷��B
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
	// mbstowcs_s �͕ϊ��������T�C�Y���w�肷�邱�Ƃ��ł����A��{�I�� \0 �܂ŕϊ����邱�ƂɂȂ�B
	// ���̂��߁A��x�ʃo�b�t�@�Ɉڂ��� \0 ��t����
	ReferenceBuffer tmpInBuffer;
	tmpInBuffer.reserve(uInByteCount + sizeof(char));	// NULL ������ + 1
	tmpInBuffer.copy(pInBuffer, uInByteCount);
	char* tmpStr = (char*)tmpInBuffer.getPointer();
	tmpStr[uInByteCount] = '\0';
	
	// Multi �� Wide
	size_t len;
	errno_t err = mbstowcs_s(
		&len,										// �ϊ����ꂽ������
		(wchar_t*)pOutBuffer,						// �o�͐�o�b�t�@
		(uOutBufferByteCount / sizeof(wchar_t)) + 1,// �o�͐�o�b�t�@�̃T�C�Y (������ = wchar_t �Ƃ��Ă̗v�f��)
		tmpStr,										// �ϊ����o�b�t�@
		uOutBufferByteCount / sizeof(wchar_t));		// pOutBuffer �Ɋi�[���� wchar_t �̍ő吔
	LN_THROW(err == 0, ArgumentException);

	// �I�[�ɂ� \0 �������I�ɕt�������B�����ȕ��������̃T�C�Y���~�����̂� -1 ����B
	// TODO: �܂�A�o�̓o�b�t�@�� ���̑�2���� +1���[�h�� �m�ۂ��Ă����K�v������
	len--;

	// mbstowcs ���ᕶ�����J�E���g�͂ł��Ȃ��̂� UnicodeUtils ���g��
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
	// Multi �� Wide
	size_t len;
	size_t len;
	errno_t err = mbstowcs_s(&len, (wchar_t*)pOutBuffer, uInByteCount / sizeof(wchar_t), (const char*)pInBuffer, uInByteCount);
	LN_THROW(err == 0, ArgumentException);

	*pBytesUsed = len * sizeof(wchar_t);
	*pCharsUsed = len;	// UTF32 �Ȃ̂ŗv�f�� == ������
#endif
	return true;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool SystemMultiByteEncoding::ConvertFromUTF16(const byte_t* pInBuffer, size_t uInByteCount, byte_t* pOutBuffer, size_t uOutBufferByteCount, size_t* pBytesUsed, size_t* pCharsUsed, bool* pUsedDefaultChar) const
{
#ifdef LNOTE_WCHAR_16
	// wcsrtombs_s �͕ϊ��������T�C�Y���w�肷�邱�Ƃ��ł����A��{�I�� \0 �܂ŕϊ����邱�ƂɂȂ�B
	// ���̂��߁A��x�ʃo�b�t�@�Ɉڂ��� \0 ��t����
	ReferenceBuffer tmpWideBuffer;
	tmpWideBuffer.reserve(uInByteCount + sizeof(wchar_t));	// NULL ������
	tmpWideBuffer.copy(pInBuffer, uInByteCount);
	wchar_t* wideStr = (wchar_t*)tmpWideBuffer.getPointer();
	wideStr[uInByteCount / sizeof(wchar_t)] = L'\0';

	// Wide �� Multi
	size_t convertedLen;
	mbstate_t mbstate = 0;
	::memset((void*)&mbstate, 0, sizeof(mbstate));
	errno_t err = wcsrtombs_s(
		&convertedLen,				// �ϊ����ꂽ���������i�[����� (Multi �ւ̕ϊ��Ȃ̂ŁA�o�C�g��)
		(char*)pOutBuffer,			// �o�͐�o�b�t�@
		uOutBufferByteCount,		// �o�͐�o�b�t�@�̃T�C�Y (�o�C�g��)
		(const wchar_t**)&wideStr,	// �ϊ���
		_TRUNCATE,					// �o�͐�o�b�t�@�Ɋi�[�ł���ő�o�C�g�� (���������Ɠ����B_TRUNCATE �� \0 �܂�)
		&mbstate);
	LN_THROW(err == 0, ArgumentException);

	// �I�[�ɂ� \0 �������I�ɕt�������B�����ȕ��������̃T�C�Y���~�����̂� -1 ����B
	// TODO: �܂�A�o�̓o�b�t�@�� ���̑�2���� +1���[�h�� �m�ۂ��Ă����K�v������
	convertedLen--;

	// wcsrtombs_s ���ᕶ�����J�E���g�͂ł��Ȃ��̂� UnicodeUtils ���g��
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
	// wcsrtombs_s �͕ϊ��������T�C�Y���w�肷�邱�Ƃ��ł����A��{�I�� \0 �܂ŕϊ����邱�ƂɂȂ�B
	// ���̂��߁A��x�ʃo�b�t�@�Ɉڂ��� \0 ��t����
	ReferenceBuffer tmpWideBuffer;
	tmpWideBuffer.reserve(uInByteCount + sizeof(wchar_t));	// NULL ������
	tmpWideBuffer.copy(pInBuffer, uInByteCount);
	wchar_t* wideStr = (wchar_t*)tmpWideBuffer.getPointer();
	wideStr[uInByteCount] = L'\0';

	// TODO �� uInByteCount+1 �� NULL ��������ϊ����Ȃ��Ă����悤�ɂ���΃p�t�H�[�}���X�オ�邩��

	// Wide �� Multi
	size_t convertedLen;
	mbstate_t mbstate = 0;
	::memset((void*)&mbstate, 0, sizeof(mbstate));
	errno_t err = wcsrtombs_s(
		&convertedLen,				// �ϊ����ꂽ���������i�[����� (Multi �ւ̕ϊ��Ȃ̂ŁA�o�C�g��)
		(char*)pOutBuffer,			// �o�͐�o�b�t�@
		uOutBufferByteCount,		// �o�͐�o�b�t�@�̃T�C�Y (�o�C�g��)
		(const wchar_t**)&wideStr,	// �ϊ���
		_TRUNCATE,					// �o�͐�o�b�t�@�Ɋi�[�ł���ő�o�C�g�� (���������Ɠ����B_TRUNCATE �� \0 �܂�)
		&mbstate);
	LN_THROW(err == 0, ArgumentException);

	*pBytesUsed = convertedLen;
	*pCharsUsed = uInByteCount / sizeof(wchar_t);	// UTF32 �Ȃ̂ŗv�f�� == ������
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
	// UTF16 ���� UTF16 �ւ̕ϊ��B���̂܂܃R�s�[�ł悢
	errno_t err = memcpy_s(pOutBuffer, uOutBufferByteCount, pInBuffer, uInByteCount);
	LN_THROW(err == 0, ArgumentException);

	// �������̓J�E���g����
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
	*pUsedDefaultChar = false;	// �P�Ȃ�R�s�[�Ȃ̂Ńf�t�H���g�����͂��肦�Ȃ�
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
	// UTF16 ���� UTF16 �ւ̕ϊ��B���̂܂܃R�s�[�ł悢
	errno_t err = memcpy_s(outBuffer, outBufferByteCount, inBuffer, inByteCount);
	LN_THROW(err == 0, ArgumentException);

	// �o�b�t�@�g�p���E�������͂��̂܂�
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
	*usedDefaultChar = false;	// �P�Ȃ�R�s�[�Ȃ̂ŕs�������͂Ȃ�
	return ConvertToUTF32(inBuffer, inByteCount, outBuffer, outBufferByteCount, bytesUsed, charsUsed);
}


} // namespace Base
} // namespace Core
} // namespace LNote

