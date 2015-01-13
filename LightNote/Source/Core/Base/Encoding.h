//==============================================================================
// Encoding 
//==============================================================================

#pragma once
#include "NonCopyable.h"

namespace LNote
{
namespace Core
{
namespace Base
{

//==============================================================================
// Encoding
//------------------------------------------------------------------------------
/**
	@brief	�����G���R�[�f�B���O
*/
//==============================================================================
class Encoding : private NonCopyable
{
public:
	Encoding() {};
	virtual ~Encoding() {};

public:
	static Encoding* GetSystemMultiByteEncoding();
	static Encoding* GetWideCharEncoding();
	static Encoding* GetUTF8Encoding();
	static Encoding* GetUTF16Encoding();
	static Encoding* GetUTF32Encoding();

	/// �����R�[�h��ϊ����� (�s���V�[�P���X�����������O)
	static ReferenceBuffer* Convert(
		const void* src, size_t srcByteCount, const Encoding* srcEncoding, 
		const Encoding* targetEncoding,
		size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar);

public:

	/**
		@brief		1 �����̍ŏ��o�C�g�����擾����
	*/
	virtual int GetMinByteCount() const = 0;

	/**
		@brief		1 �����̍ő�o�C�g�����擾����
	*/
	virtual int GetMaxByteCount() const = 0;

	/**
		@brief		���̃G���R�[�f�B���O�ŕ\������镶����o�b�t�@���AUTF16 �ɕϊ�����
		@param[in]		inBuffer			: ���̃G���R�[�f�B���O�ŕ\������镶����o�b�t�@
		@param[in]		inByteCount		: inBuffer �̃o�C�g�� (NULL �����͊܂܂Ȃ�����)
		@param[in,out]	outBuffer			: �ϊ����ʂ̊i�[��o�b�t�@
		@param[in]		outBufferByteCount	: outBuffer �̃o�C�g��
		@param[out]		bytesUsed			: �ϊ����ʂ̃o�C�g�����i�[����ϐ�
		@param[out]		charsUsed			: �ϊ����ʂ̕��������i�[����ϐ�
		@return		�s���ȕ�����V�[�P���X�����������ꍇ�� false ��Ԃ��܂��B(�ϊ��ł��Ȃ������ł͂Ȃ��AUTF16 �̃T���Q�[�g�y�A���Е������Ȃ����A���������t�H�[�}�b�g�Ƃ��Ă��������ꍇ)
		@details	���̊֐��� wchar_t �̃T�C�Y�� 2 �o�C�g�̊��Ŏg�p����܂��B
	*/
	virtual bool ConvertToUTF16(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed) const = 0;

	/**
		@brief		���̃G���R�[�f�B���O�ŕ\������镶����o�b�t�@���AUTF32 �ɕϊ�����
		@param[in]		inBuffer			: ���̃G���R�[�f�B���O�ŕ\������镶����o�b�t�@
		@param[in]		inByteCount		: inBuffer �̃o�C�g�� (NULL �����͊܂܂Ȃ�����)
		@param[in,out]	outBuffer			: �ϊ����ʂ̊i�[��o�b�t�@
		@param[in]		outBufferByteCount	: outBuffer �̃o�C�g��
		@param[out]		bytesUsed			: �ϊ����ʂ̃o�C�g�����i�[����ϐ�
		@param[out]		charsUsed			: �ϊ����ʂ̕��������i�[����ϐ�
		@return		�s���ȕ�����V�[�P���X�����������ꍇ�� false ��Ԃ��܂��B(�ϊ��ł��Ȃ������ł͂Ȃ��AUTF16 �̃T���Q�[�g�y�A���Е������Ȃ����A���������t�H�[�}�b�g�Ƃ��Ă��������ꍇ)
		@details	���̊֐��� wchar_t �̃T�C�Y�� 4 �o�C�g�̊��Ŏg�p����܂��B
	*/
	virtual bool ConvertToUTF32(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed) const = 0;

	/**
		@brief		UTF16 ��������A���̃G���R�[�f�B���O�ŕ\������镶����ɕϊ�����
		@param[in]		inBuffer			: ���̃G���R�[�f�B���O�ŕ\������镶����o�b�t�@
		@param[in]		inByteCount		: inBuffer �̃o�C�g�� (NULL �����͊܂܂Ȃ�����)
		@param[in,out]	outBuffer			: �ϊ����ʂ̊i�[��o�b�t�@
		@param[in]		outBufferByteCount	: outBuffer �̃o�C�g��
		@param[out]		bytesUsed			: �ϊ����ʂ̃o�C�g�����i�[����ϐ�
		@param[out]		charsUsed			: �ϊ����ʂ̕��������i�[����ϐ�
		@param[out]		usedDefaultChar	: �ϊ��ł��Ȃ�����������A�K��̕������g�p�����ꍇ�� true ��Ԃ�
		@return		�s���ȕ�����V�[�P���X�����������ꍇ�� false ��Ԃ��܂��B(�ϊ��ł��Ȃ������ł͂Ȃ��AUTF16 �̃T���Q�[�g�y�A���Е������Ȃ����A���������t�H�[�}�b�g�Ƃ��Ă��������ꍇ)
		@details	���̊֐��� wchar_t �̃T�C�Y�� 2 �o�C�g�̊��Ŏg�p����܂��B
	*/
	virtual bool ConvertFromUTF16(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar) const = 0;

	/**
		@brief		UTF32 ��������A���̃G���R�[�f�B���O�ŕ\������镶����ɕϊ�����
		@param[in]		inBuffer			: ���̃G���R�[�f�B���O�ŕ\������镶����o�b�t�@
		@param[in]		inByteCount		: inBuffer �̃o�C�g�� (NULL �����͊܂܂Ȃ�����)
		@param[in,out]	outBuffer			: �ϊ����ʂ̊i�[��o�b�t�@
		@param[in]		outBufferByteCount	: outBuffer �̃o�C�g��
		@param[out]		bytesUsed			: �ϊ����ʂ̃o�C�g�����i�[����ϐ�
		@param[out]		charsUsed			: �ϊ����ʂ̕��������i�[����ϐ�
		@param[out]		usedDefaultChar	: �ϊ��ł��Ȃ�����������A�K��̕������g�p�����ꍇ�� true ��Ԃ�
		@return		�s���ȕ�����V�[�P���X�����������ꍇ�� false ��Ԃ��܂��B(�ϊ��ł��Ȃ������ł͂Ȃ��AUTF16 �̃T���Q�[�g�y�A���Е������Ȃ����A���������t�H�[�}�b�g�Ƃ��Ă��������ꍇ)
		@details	���̊֐��� wchar_t �̃T�C�Y�� 4 �o�C�g�̊��Ŏg�p����܂��B
	*/
	virtual bool ConvertFromUTF32(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar) const = 0;

};

/**
	@brief		�v���O�����̃��P�[���ɍ��킹���}���`�o�C�g�R�[�h�G���R�[�f�B���O
	@details	setlocale() �ɂĎw�肳�ꂽ���P�[���̋K�蕶���R�[�h��\���܂��B
				OS �K��̃��P�[�����g�p����ɂ́A���炩���� setlocale( LC_ALL, "" ); �̂悤�ɂ���ɂ���K�v������܂��B
				�Ⴆ�΁A���{�� Windws �ł���� ShiftJIS�ALinux �ł���� UTF-8 �ƂȂ�܂��B
*/
class SystemMultiByteEncoding : public Encoding
{
public:
	SystemMultiByteEncoding() {};
	virtual ~SystemMultiByteEncoding() {};

public:
	// override Encoding
	virtual int GetMinByteCount() const;
	virtual int GetMaxByteCount() const;
	virtual bool ConvertToUTF16(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed) const;
	virtual bool ConvertToUTF32(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed) const;
	virtual bool ConvertFromUTF16(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar) const;
	virtual bool ConvertFromUTF32(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar) const;
};

/**
	@brief		UTF8 �G���R�[�f�B���O
*/
class UTF8Encoding : public Encoding
{
public:
	UTF8Encoding() {};
	virtual ~UTF8Encoding() {};

public:
	// override Encoding
	virtual int GetMinByteCount() const { return 1; }
	virtual int GetMaxByteCount() const { return 6; }
	virtual bool ConvertToUTF16(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed) const;
	virtual bool ConvertToUTF32(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed) const;
	virtual bool ConvertFromUTF16(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar) const;
	virtual bool ConvertFromUTF32(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar) const;
};

/**
	@brief		UTF16 �G���R�[�f�B���O
*/
class UTF16Encoding : public Encoding
{
public:
	UTF16Encoding() {};
	virtual ~UTF16Encoding() {};

public:
	// override Encoding
	virtual int GetMinByteCount() const { return 2; }
	virtual int GetMaxByteCount() const { return 4; }
	virtual bool ConvertToUTF16(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed) const;
	virtual bool ConvertToUTF32(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed) const;
	virtual bool ConvertFromUTF16(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar) const;
	virtual bool ConvertFromUTF32(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar) const;
};

/**
	@brief		UTF32 �G���R�[�f�B���O
*/
class UTF32Encoding : public Encoding
{
public:
	UTF32Encoding() {};
	virtual ~UTF32Encoding() {};

public:
	// override Encoding
	virtual int GetMinByteCount() const { return 4; }
	virtual int GetMaxByteCount() const { return 4; }
	virtual bool ConvertToUTF16(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed) const;
	virtual bool ConvertToUTF32(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed) const;
	virtual bool ConvertFromUTF16(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar) const;
	virtual bool ConvertFromUTF32(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar) const;
};



} // namespace Base
} // namespace Core
} // namespace LNote

