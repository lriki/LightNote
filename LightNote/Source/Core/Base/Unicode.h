//==============================================================================
// Unicode 
//------------------------------------------------------------------------------
///**
//  @file       Unicode.h
//  @brief      Unicode
//  @author     Riki
//*/
//==============================================================================

#pragma once

namespace LNote
{
namespace Core
{
namespace Base
{

//==============================================================================
// ■ ICharCodeCodec
//------------------------------------------------------------------------------
///**
//	@brief  
//*/
//==============================================================================
class ICharCodeCodec
{
public:
	virtual ~ICharCodeCodec() {}
	
public:

	/// 1文字の最大バイト数
	virtual int getMaxStride() const = 0;

	/// 1文字の最小バイト数
	virtual int getMinStride() const = 0;

	/// UTF16 へ変換
	virtual bool convertToUTF16( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount ) = 0;

	/// UTF32 へ変換
	virtual bool convertToUTF32( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount ) = 0;




	/*
	/// UTF8 へ変換 (inCount は \0 の分も含まれている。outCharCount は終端\0を含まない文字数が返るが、\0は付加する)
	virtual bool convertToUTF8( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount ) = 0;

	/// UTF16 へ変換
	virtual bool convertToUTF16( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount ) = 0;
	*/
};

//==============================================================================
// ■ UTF8CharCodeCodec
//------------------------------------------------------------------------------
///**
//	@brief  
//*/
//==============================================================================
class UTF8CharCodeCodec
	: public ICharCodeCodec
{
public:
	virtual ~UTF8CharCodeCodec() {}
	
public:
	virtual int getMaxStride() const { return 6; }
	virtual int getMinStride() const { return 1; }
	virtual bool convertToUTF8( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount );
	virtual bool convertToUTF16( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount );
	virtual bool convertToUTF32( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount );
};

//==============================================================================
// ■ UTF16CharCodeCodec
//------------------------------------------------------------------------------
///**
//	@brief		(UTF-16 LN)
//*/
//==============================================================================
class UTF16CharCodeCodec
	: public ICharCodeCodec
{
public:
	virtual ~UTF16CharCodeCodec() {}
	
public:
	virtual int getMaxStride() const { return 2; }
	virtual int getMinStride() const { return 2; }
	virtual bool convertToUTF8( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount );
	virtual bool convertToUTF16( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount );
	virtual bool convertToUTF32( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount );
};

//==============================================================================
// ■ UTF32CharCodeCodec
//------------------------------------------------------------------------------
///**
//	@brief
//*/
//==============================================================================
class UTF32CharCodeCodec
	: public ICharCodeCodec
{
public:
	virtual ~UTF32CharCodeCodec() {}
	
public:
	virtual int getMaxStride() const { return 4; }
	virtual int getMinStride() const { return 4; }
	virtual bool convertToUTF8( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount );
	virtual bool convertToUTF16( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount );
	virtual bool convertToUTF32( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount );
};

//==============================================================================
// ■ ShiftJISCharCodeCodec
//------------------------------------------------------------------------------
///**
//	@brief
//*/
//==============================================================================
class ShiftJISCharCodeCodec
	: public ICharCodeCodec
{
public:
	virtual ~ShiftJISCharCodeCodec() {}
	
public:
	virtual int getMaxStride() const { return 2; }
	virtual int getMinStride() const { return 1; }
	virtual bool convertToUTF8( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount );
	virtual bool convertToUTF16( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount );
	virtual bool convertToUTF32( const lnByte* inBuffer, int inCount, lnByte* outBuffer, int outCount, int* outCharCount );
};

/// エンコーディングの種類
enum EncodingType
{
	ENCODING_UNKNOWN = 0,
	ENCODING_UTF8,
	ENCODING_UTF16,
};

//==============================================================================
// ■ UnicodeString
//------------------------------------------------------------------------------
///**
//	@brief       
//	@note
//				内部エンコーディングは基本的に環境の wchar_t に合わせるが、これで固定はしない。
//				→アーカイブファイル内のファイル名は UTF-16 であるため、
//				　Linux 環境(wchar_t=UTF32)等で何も考えずにアクセスしようとすると失敗する。
//				　このため、一度 UTF-16 に変換する必要がある。
//				getString() は wchar_t* を返すが、上記のような理由から、
//				この関数はあくまでユーティリティという位置づけになる。
//				(環境が UTF-32 でも UTF-32 文字列を返すとは限らない)
//				環境に依存せずに、ある文字コードで比較などを行いたい場合は
//				getStringBuffer() で取得したバッファで memcmp 等を行う必要がある。
//*/
//==============================================================================
class UnicodeString
    : public ReferenceObject
{
public:

	/// 内部エンコーディング (UTF-8 or UTF-16)
	static ICharCodeCodec* InternalCodec;

	static UTF8CharCodeCodec* getUTF8Codec();
	static UTF16CharCodeCodec* getUTF16Codec();
	static UTF32CharCodeCodec* getUTF32Codec();

	/// デフォルトのマルチバイト文字列のコーデック (char 型の文字コード) を設定する
	///		初期値は MSVC:Shift_JIS、GCC:UTF-8
	static void setDefaultMultiByteCodec( ICharCodeCodec* codec );

public:

	/// デフォルトコンストラクタ ( 空の文字列 "" で作成 )
	UnicodeString();

	/// コンストラクタ
	UnicodeString( const lnByte* bytes, ICharCodeCodec* codec );

	virtual ~UnicodeString();

public:

	/// バイト数の取得
    int getByteCount() const { return mByteCount; }

	/// 文字数の取得
    int getCharCount() const { return mCharCount; }

	/// 文字列の格納されているバッファの取得
	//lnByte* getStringBuffer() { return mString; }

	void setDecodeBytes( const char* str, int len = -1 );

	void setDecodeBytes( const wchar_t* str );

	void setDecodeBytes( const lnByte* bytes, int byteCount, ICharCodeCodec* codec );

	//void setString( ICharCodeCodec* internalCodec, const char* nativeMultiByteString );
	//void setString( ICharCodeCodec* internalCodec, const wchar_t* nativeWideString );

	const wchar_t* getString() const { return mString; }

	void getUTF8String( std::string* outStr ) const;

public:

	UnicodeString( const UnicodeString& obj );
	UnicodeString& operator = ( const UnicodeString& obj );
	UnicodeString& operator += ( const UnicodeString& obj );
    bool operator == ( const UnicodeString& obj ) const;
    bool operator != ( const UnicodeString& obj ) const;
    bool operator <  ( const UnicodeString& obj ) const;

public:
	static void ConvertMultiToWide(const char* multi, std::wstring* wide);

private:

	void _dispose();

private:

	wchar_t*	mString;
	int			mCharSize;			///< 文字数 (\0は含まない)
    int         mByteCount;			///< 確保したバッファサイズ (文字数等は関係ない)
	int			mValidByteCount;	///< char、または wchar_t として認識できる文字数 (strlen)
	int			mCharCount;			///< 実文字数 (マルチバイト文字も1文字としたカウント)

public:

	/// 比較
	static int strcmp( const UnicodeString& str1, const UnicodeString& str2 );
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
FILE* ln_wfopen( const wchar_t* filePath, const wchar_t* mode );

} // namespace Base
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================