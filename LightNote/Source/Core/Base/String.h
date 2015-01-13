//==============================================================================
// String 
//==============================================================================

#pragma once

#include <string>
#include <vector>

namespace LNote
{
namespace Core
{
namespace Base
{
class Encoding;
class ReferenceBuffer;

/// BasicString::Split() の出力方法
enum StringSplitOptions
{
	StringSplitOptions_None = 0,				///< 出力は空の文字列を含む
	StringSplitOptions_RemoveEmptyEntries,		///< 出力は空の文字列を含まない
};

//==============================================================================
// BasicString
//------------------------------------------------------------------------------
/**
	@brief	
*/
//==============================================================================
template<typename TChar>
class BasicString : public std::basic_string<TChar>
{
public:
	typedef std::basic_string<TChar>	std_basic_string;
	typedef BasicString<TChar>			StringT;

public:

	// override std::basic_string
	BasicString(const std_basic_string& str)	: std_basic_string(str) {}
	BasicString(size_type count, TChar ch)		: std_basic_string(count, ch) {}

	// default & copys
	BasicString()														: std_basic_string() {}
	BasicString(const StringT& str)										: std_basic_string(str) {}
	BasicString(const StringT& str, size_type length)					: std_basic_string(str, length) {}
	BasicString(const StringT& str, size_type begin, size_type length)	: std_basic_string(str, begin, length) {}

	// TChar 割り当て
	BasicString(const TChar* str) : std_basic_string(str) {}
	BasicString(const TChar* str, size_type length) : std_basic_string(str, length) {}
	BasicString(const TChar* str, size_type begin, size_type length) : std_basic_string(str, begin, length) {}

	// operators
	BasicString& operator=(const StringT& right)			{ return static_cast<BasicString&>(std_basic_string::operator=(right)); }
	BasicString& operator=(const std_basic_string& right)	{ return static_cast<BasicString&>(std_basic_string::operator=(right)); }	// = std::string
	BasicString& operator=(const TChar* ptr)				{ return static_cast<BasicString&>(std_basic_string::operator=(ptr)); }
	BasicString& operator+=(const StringT& right)			{ return static_cast<BasicString&>(std_basic_string::operator+=(right)); }
	BasicString& operator+=(const std_basic_string& right)	{ return static_cast<BasicString&>(std_basic_string::operator+=(right)); }	// += std::string
	BasicString& operator+=(const TChar *ptr)				{ return static_cast<BasicString&>(std_basic_string::operator+=(ptr)); }
	BasicString& operator+=(TChar ch)						{ return static_cast<BasicString&>(std_basic_string::operator+=(ch)); }

public:
	// char 割り当て (Encoding::getSystemEncoding() 使用。変換不可文字は ? へ。不正シーケンスは例外)
	void assignNativeString(const char* str, bool* usedDefaultChar = NULL) { assignNativeString(str, 0, std_basic_string::npos, usedDefaultChar); }
	void assignNativeString(const char* str, size_type length, bool* usedDefaultChar = NULL) { assignNativeString(str, 0, length, usedDefaultChar); }
	void assignNativeString(const char* str, size_type begin, size_type length, bool* usedDefaultChar = NULL);

	// wchar_t 割り当て (Encoding::getWideCharEncoding() 使用。変換不可文字は ? へ。不正シーケンスは例外)
	void assignNativeString(const wchar_t* str, bool* usedDefaultChar = NULL) { assignNativeString(str, 0, std_basic_string::npos, usedDefaultChar); }
	void assignNativeString(const wchar_t* str, size_type length, bool* usedDefaultChar = NULL) { assignNativeString(str, 0, length, usedDefaultChar); }
	void assignNativeString(const wchar_t* str, size_type begin, size_type length, bool* usedDefaultChar = NULL);

	/**
		@brief		指定したエンコーディングを使用し、文字列を変換して設定する
		@param[in]	buffer			: 変換元文字列が格納されているバッファ
		@param[in]	byteCount		: pBuffer のバイト数 (-1 で \0 まで)
		@param[in]	encoding		: 変換元文字エンコーディング
		@param[in]	usedDefaultChar	: 変換不可文字をデフォルト文字 ('?') に変換したかどうか
	*/
	void ConvertFrom(const void* buffer, int byteCount, const Encoding* encoding, bool* usedDefaultChar = NULL);

	/**
		@brief		指定したエンコーディングを使用し、変換した文字列バッファを取得する
		@param[in]	encoding		: 変換先文字エンコーディング
		@param[in]	usedDefaultChar	: 変換不可文字をデフォルト文字 ('?') に変換したかどうか
		@return		\0終端文字は付加されません。getSize() により使用バイト数を確認できます。
					使用後、Release() で開放する必要があります。
	*/
	ReferenceBuffer* ConvertTo(Encoding* encoding, bool* usedDefaultChar = NULL) const;

	/**
		@brief		部分文字列を取得する
		@param[in]	startIndex	: 文字列の 0 から始まる開始文字位置
		@param[in]	length		: 部分文字列の文字数 (省略した場合は末尾まで)
	*/
	StringT SubString(size_type startIndex = 0, size_type length = npos) const;

	/// 文字列の置換をおこなう (すべて)
	void Replace(const TChar* from, const TChar* to);

	/// 文字列を検索し、見つかった最初の文字のインデックスを返す (見つからなかった場合は -1)
	int IndexOf(const TChar* str, int startIndex = 0);

	/// 文字列をデリミタで分割する
	void Split(const TChar* delim, StringSplitOptions option, std::vector< BasicString<TChar> >* out);

	/// 文字列を構成するバイト数を取得する
	int getByteCount() const { return size() * sizeof(TChar); }

private:
	Encoding* getThisTypeEncoding() const;
};

typedef BasicString<lnChar>		String;
typedef BasicString<char>		StringA;
typedef BasicString<wchar_t>	StringW;

} // namespace Base
} // namespace Core

typedef Core::Base::BasicString<lnChar>		lnString;
typedef Core::Base::BasicString<char>		lnStringA;
typedef Core::Base::BasicString<wchar_t>	lnStringW;

} // namespace LNote

