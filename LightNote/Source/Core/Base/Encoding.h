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
	@brief	文字エンコーディング
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

	/// 文字コードを変換する (不正シーケンスがあったら例外)
	static ReferenceBuffer* Convert(
		const void* src, size_t srcByteCount, const Encoding* srcEncoding, 
		const Encoding* targetEncoding,
		size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar);

public:

	/**
		@brief		1 文字の最小バイト数を取得する
	*/
	virtual int GetMinByteCount() const = 0;

	/**
		@brief		1 文字の最大バイト数を取得する
	*/
	virtual int GetMaxByteCount() const = 0;

	/**
		@brief		このエンコーディングで表現される文字列バッファを、UTF16 に変換する
		@param[in]		inBuffer			: このエンコーディングで表現される文字列バッファ
		@param[in]		inByteCount		: inBuffer のバイト数 (NULL 文字は含まないこと)
		@param[in,out]	outBuffer			: 変換結果の格納先バッファ
		@param[in]		outBufferByteCount	: outBuffer のバイト数
		@param[out]		bytesUsed			: 変換結果のバイト数を格納する変数
		@param[out]		charsUsed			: 変換結果の文字数を格納する変数
		@return		不正な文字列シーケンスが見つかった場合は false を返します。(変換できない文字ではなく、UTF16 のサロゲートペアが片方しかない等、そもそもフォーマットとしておかしい場合)
		@details	この関数は wchar_t のサイズが 2 バイトの環境で使用されます。
	*/
	virtual bool ConvertToUTF16(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed) const = 0;

	/**
		@brief		このエンコーディングで表現される文字列バッファを、UTF32 に変換する
		@param[in]		inBuffer			: このエンコーディングで表現される文字列バッファ
		@param[in]		inByteCount		: inBuffer のバイト数 (NULL 文字は含まないこと)
		@param[in,out]	outBuffer			: 変換結果の格納先バッファ
		@param[in]		outBufferByteCount	: outBuffer のバイト数
		@param[out]		bytesUsed			: 変換結果のバイト数を格納する変数
		@param[out]		charsUsed			: 変換結果の文字数を格納する変数
		@return		不正な文字列シーケンスが見つかった場合は false を返します。(変換できない文字ではなく、UTF16 のサロゲートペアが片方しかない等、そもそもフォーマットとしておかしい場合)
		@details	この関数は wchar_t のサイズが 4 バイトの環境で使用されます。
	*/
	virtual bool ConvertToUTF32(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed) const = 0;

	/**
		@brief		UTF16 文字列を、このエンコーディングで表現される文字列に変換する
		@param[in]		inBuffer			: このエンコーディングで表現される文字列バッファ
		@param[in]		inByteCount		: inBuffer のバイト数 (NULL 文字は含まないこと)
		@param[in,out]	outBuffer			: 変換結果の格納先バッファ
		@param[in]		outBufferByteCount	: outBuffer のバイト数
		@param[out]		bytesUsed			: 変換結果のバイト数を格納する変数
		@param[out]		charsUsed			: 変換結果の文字数を格納する変数
		@param[out]		usedDefaultChar	: 変換できない文字があり、規定の文字を使用した場合は true を返す
		@return		不正な文字列シーケンスが見つかった場合は false を返します。(変換できない文字ではなく、UTF16 のサロゲートペアが片方しかない等、そもそもフォーマットとしておかしい場合)
		@details	この関数は wchar_t のサイズが 2 バイトの環境で使用されます。
	*/
	virtual bool ConvertFromUTF16(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar) const = 0;

	/**
		@brief		UTF32 文字列を、このエンコーディングで表現される文字列に変換する
		@param[in]		inBuffer			: このエンコーディングで表現される文字列バッファ
		@param[in]		inByteCount		: inBuffer のバイト数 (NULL 文字は含まないこと)
		@param[in,out]	outBuffer			: 変換結果の格納先バッファ
		@param[in]		outBufferByteCount	: outBuffer のバイト数
		@param[out]		bytesUsed			: 変換結果のバイト数を格納する変数
		@param[out]		charsUsed			: 変換結果の文字数を格納する変数
		@param[out]		usedDefaultChar	: 変換できない文字があり、規定の文字を使用した場合は true を返す
		@return		不正な文字列シーケンスが見つかった場合は false を返します。(変換できない文字ではなく、UTF16 のサロゲートペアが片方しかない等、そもそもフォーマットとしておかしい場合)
		@details	この関数は wchar_t のサイズが 4 バイトの環境で使用されます。
	*/
	virtual bool ConvertFromUTF32(const byte_t* inBuffer, size_t inByteCount, byte_t* outBuffer, size_t outBufferByteCount, size_t* bytesUsed, size_t* charsUsed, bool* usedDefaultChar) const = 0;

};

/**
	@brief		プログラムのロケールに合わせたマルチバイトコードエンコーディング
	@details	setlocale() にて指定されたロケールの規定文字コードを表します。
				OS 規定のロケールを使用するには、あらかじめ setlocale( LC_ALL, "" ); のようにするにする必要があります。
				例えば、日本語 Windws であれば ShiftJIS、Linux であれば UTF-8 となります。
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
	@brief		UTF8 エンコーディング
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
	@brief		UTF16 エンコーディング
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
	@brief		UTF32 エンコーディング
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

