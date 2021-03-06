/*
LNUnicodeUtils 1.0.0

The zlib/libpng License
Copyright (c) 2014 lriki

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#ifndef _WIN32
#include <stdint.h>
#endif

namespace LN
{
#ifdef _WIN32
typedef unsigned long	UTF32;
typedef unsigned short	UTF16;
typedef unsigned char	UTF8;
#else
typedef uint32_t		UTF32;
typedef uint16_t		UTF16;
typedef uint8_t			UTF8;
#endif

/// 文字コード変換の処理結果
enum UTFConversionResult
{
	UTFConversionResult_Success = 0,		///< 変換成功
	UTFConversionResult_SourceExhausted,	///< ソースバッファが不正 (バッファ終端でマルチバイト文字が途切れた)
	UTFConversionResult_TargetExhausted,	///< ターゲットバッファが足りない
	UTFConversionResult_SourceIllegal,		///< ソースバッファ内に不正な文字が見つかった (UTF8の不正バイトシーケンスや、UTF16のサロゲートペア不一致(上位があるのに下位が無い)の時は、不正文字置換を有効にしていてもこのエラーで失敗する)
};

/// 文字コード変換の追加情報
struct UTFConversionOptions
{
	UTF32		ReplacementChar;			///< [in] 不正文字を見つけた時、この文字に置換する (ASCII で指定すること。'\0' を指定した状態で不正文字を見つけると、変換は失敗する)

	int			ConvertedSourceLength;		///< [out] 変換に使用したソースバッファのバイト数が格納される
	int			ConvertedTargetLength;		///< [out] 変換されたターゲットバッファのバイト数が格納される
	int			CharCount;					///< [out] 文字数が格納される (マルチバイト文字を1文字とみなした文字数)
	int			IllegalCharCount;			///< [out] 見つけた不正文字の数
};

/**
 *	@brief		Unicode の相互変換等を行うモジュール
 *	@details	UTF-8 は最大 4 バイトとして扱います。
 */
class UnicodeUtils
{
public:
	static const UTF32	SurrogateHighStart	= (UTF32)0xD800;	///< 上位サロゲート値の範囲 min
	static const UTF32	SurrogateHighEnd	= (UTF32)0xDBFF;	///< 上位サロゲート値の範囲 max
	static const UTF32	SurrogateLowStart	= (UTF32)0xDC00;	///< 下位サロゲート値の範囲 min
	static const UTF32	SurrogateLowEnd		= (UTF32)0xDFFF;	///< 下位サロゲート値の範囲 max
	static const int	UTF16HalfShift		= 10;				///< UTF16上位サロゲートに使用するシフト量
	static const UTF32	UTF16HalfBase		= 0x0010000UL;		///< UTF16下位サロゲートに使用するベース値
	static const UTF32	UTF16HalfMask		= 0x3FFUL;			///< UTF32→UTF16変換時の、下位マスク
	static const UTF32	MaxLegalUTF32		= 0x0010FFFF;		///< この値以下が UTF32として有効
	static const UTF32	ReplacementChar		= 0x0000FFFD;		///< 不正文字が見つかった時、これに置換する
	static const UTF32	MaxBMP				= 0x0000FFFF;		///< Basic Multilingual Plane

public:

	/**
	 *	@brief			UTF8 から UTF16 への変換
	 *	@param[in]		sourceStart		: 変換元のバッファの先頭アドレス
	 *	@param[in]		sourceLength	: 変換元のバッファの長さ (文字単位(sourceStartの要素数)。終端 \0 は含まないようにすること)
	 *	@param[in]		targetStart		: 変換結果を格納するバッファの先頭アドレス (終端 \0 は格納されない)
	 *	@param[in]		targetLength	: 変換結果を格納するバッファの長さ (文字単位(targetStartの要素数))
	 *	@param[in,out]	options			: 変換の追加情報
	 */
	static UTFConversionResult ConvertUTF8toUTF16(
		const UTF8*				sourceStart, 
		int						sourceLength,		// 終端 \0 等は含まない
		UTF16*					targetStart,
		int						targetLength,
		UTFConversionOptions*	options);

	/**
	 *	@brief			UTF8 から UTF32 への変換
	 *	@param[in]		sourceStart		: 変換元のバッファの先頭アドレス
	 *	@param[in]		sourceLength	: 変換元のバッファの長さ (文字単位(sourceStartの要素数)。終端 \0 は含まないようにすること)
	 *	@param[in]		targetStart		: 変換結果を格納するバッファの先頭アドレス (終端 \0 は格納されない)
	 *	@param[in]		targetLength	: 変換結果を格納するバッファの長さ (文字単位(targetStartの要素数))
	 *	@param[in,out]	options			: 変換の追加情報
	 */
	static UTFConversionResult ConvertUTF8toUTF32(
		const UTF8*				sourceStart,
		int						sourceLength,
		UTF32*					targetStart,
		int						targetLength,
		UTFConversionOptions*	options);

	/**
	 *	@brief			UTF16 から UTF8 への変換
	 *	@param[in]		sourceStart		: 変換元のバッファの先頭アドレス
	 *	@param[in]		sourceLength	: 変換元のバッファの長さ (文字単位(sourceStartの要素数)。終端 \0 は含まないようにすること)
	 *	@param[in]		targetStart		: 変換結果を格納するバッファの先頭アドレス (終端 \0 は格納されない)
	 *	@param[in]		targetLength	: 変換結果を格納するバッファの長さ (文字単位(targetStartの要素数))
	 *	@param[in,out]	options			: 変換の追加情報
	 */
	static UTFConversionResult ConvertUTF16toUTF8(
		const UTF16*			sourceStart, 
		int						sourceLength, 
		UTF8*					targetStart, 
		int						targetLength,
		UTFConversionOptions*	options);

	/**
	 *	@brief			UTF16 から UTF32 への変換
	 *	@param[in]		sourceStart		: 変換元のバッファの先頭アドレス
	 *	@param[in]		sourceLength	: 変換元のバッファの長さ (文字単位(sourceStartの要素数)。終端 \0 は含まないようにすること)
	 *	@param[in]		targetStart		: 変換結果を格納するバッファの先頭アドレス (終端 \0 は格納されない)
	 *	@param[in]		targetLength	: 変換結果を格納するバッファの長さ (文字単位(targetStartの要素数))
	 *	@param[in,out]	options			: 変換の追加情報
	 */
	static UTFConversionResult ConvertUTF16toUTF32(
		const UTF16*			sourceStart, 
		int						sourceLength, 
		UTF32*					targetStart, 
		int						targetLength,
		UTFConversionOptions*	options);

	/**
	 *	@brief			UTF32 から UTF8 への変換
	 *	@param[in]		sourceStart		: 変換元のバッファの先頭アドレス
	 *	@param[in]		sourceLength	: 変換元のバッファの長さ (文字単位(sourceStartの要素数)。終端 \0 は含まないようにすること)
	 *	@param[in]		targetStart		: 変換結果を格納するバッファの先頭アドレス (終端 \0 は格納されない)
	 *	@param[in]		targetLength	: 変換結果を格納するバッファの長さ (文字単位(targetStartの要素数))
	 *	@param[in,out]	options			: 変換の追加情報
	 */
	static UTFConversionResult ConvertUTF32toUTF8(
		const UTF32*			sourceStart, 
		int						sourceLength, 
		UTF8*					targetStart, 
		int						targetLength,
		UTFConversionOptions*	options);

	/**
	 *	@brief			UTF32 から UTF16 への変換
	 *	@param[in]		sourceStart		: 変換元のバッファの先頭アドレス
	 *	@param[in]		sourceLength	: 変換元のバッファの長さ (文字単位(sourceStartの要素数)。終端 \0 は含まないようにすること)
	 *	@param[in]		targetStart		: 変換結果を格納するバッファの先頭アドレス (終端 \0 は格納されない)
	 *	@param[in]		targetLength	: 変換結果を格納するバッファの長さ (文字単位(targetStartの要素数))
	 *	@param[in,out]	options			: 変換の追加情報
	 */
	static UTFConversionResult ConvertUTF32toUTF16(
		const UTF32*			sourceStart, 
		int						sourceLength, 
		UTF16*					targetStart, 
		int						targetLength,
		UTFConversionOptions*	options);

	/**
	 *	@brief			UTF8 のマルチバイトコードを考慮した文字数を取得する
	 *	@param[in]		sourceStart		: カウント元のバッファの先頭アドレス
	 *	@param[in]		sourceLength	: カウント元のバッファの長さ (文字単位(sourceStartの要素数)。終端 \0 は含まないようにすること)
	 *	@param[in]		isStrict		: 不正シーケンスをチェックしながらカウントする場合は true
	 *	@param[out]		count			: 文字数を格納する変数
	 */
	static UTFConversionResult GetUTF8CharCount(
		const UTF8*				sourceStart,
		int						sourceLength,
		bool					isStrict,
		int*					count);

	/**
	 *	@brief			UTF16 のサロゲートペアを考慮した文字数を取得する
	 *	@param[in]		sourceStart		: カウント元のバッファの先頭アドレス
	 *	@param[in]		sourceLength	: カウント元のバッファの長さ (文字単位(sourceStartの要素数)。終端 \0 は含まないようにすること)
	 *	@param[in]		isStrict		: 不正なサロゲートペアをチェックしながらカウントする場合は true
	 *	@param[out]		count			: 文字数を格納する変数
	 */
	static UTFConversionResult GetUTF16CharCount(
		const UTF16*			sourceStart,
		int						sourceLength,
		bool					isStrict,
		int*					count);

	/**
	 *	@brief			UTF8 の文字数を取得する (UTF8 型単位)
	 *	@details		\0 終端文字までの文字数を取得します。マルチバイトコードは考慮しません。
	 *					strlen と同等の結果を返します。
	 */
	static size_t GetUTF8Length(const UTF8* str);

	/**
	 *	@brief			UTF16 の文字数を取得する (UTF16 要素数)
	 *	@details		\0 終端文字までの文字数を取得します。サロゲートペアは考慮しません。
	 *					wchar_t が 2バイト環境での wcslen と同等の結果を返します。
	 */
	static size_t GetUTF16Length(const UTF16* str);

	/**
	 *	@brief			UTF32 の文字数を取得する (UTF32 要素数)
	 *	@details		\0 終端文字までの文字数を取得します。
	 *					wchar_t が 4バイト環境での wcslen と同等の結果を返します。
	 */
	static size_t GetUTF32Length(const UTF32* str);

	/**
	 *	@brief			UTF8 文字の後続バイト数を取得する
	 *	@param[in]		sourceStart			: チェック対象の先頭文字のアドレス
	 *	@param[in]		sourceEnd			: バッファの終端文字のアドレス (有効な領域の次のアドレスを指す (\0 終端文字列なら \0 を指す))
	 *	@param[in]		strict				: true の場合、不正文字のチェックを行う (不正文字が見つかった場合は UTFConversionResult_SourceIllegal を返す)
	 *	@param[out]		outExtraByteCount	: 後続バイト数 (ASCII 文字等、後続が無い場合は 0 が格納される)
	 */
	static UTFConversionResult CheckUTF8TrailingBytes(const UTF8* sourceStart, const UTF8* sourceEnd, bool strict, int* outExtraByteCount);

	/**
	 *	@brief			UTF16 のサロゲートペアであるかを確認する
	 *	@param[in]		sourceStart		: チェック対象の先頭文字のアドレス
	 *	@param[in]		sourceEnd		: バッファの終端文字のアドレス (有効な領域の次のアドレスを指す (\0 終端文字列なら \0 を指す))
	 *	@param[in]		strict			: true の場合、不正文字のチェックを行う (不正文字が見つかった場合は UTFConversionResult_SourceIllegal を返す)
	 *	@param[out]		outSurrogate	: true が格納された場合、sourceStart と その次の文字はサロゲートペアである
	 */
	static UTFConversionResult CheckUTF16Surrogate(const UTF16* sourceStart, const UTF16* sourceEnd, bool strict, bool* outSurrogate);

private:

	/// 1文字分の変換 (UTF8 → UTF32)
	static UTFConversionResult ConvertCharUTF8toUTF32(
		const UTF8**			sourceStart,
		const UTF8*				sourceEnd,
		UTFConversionOptions*	options,
		UTF32*					outChar);

	/// 1文字分の変換 (UTF32 → UTF8)
	static UTFConversionResult ConvertCharUTF32toUTF8(
		UTF32					ch,
		UTF8**					targetStart, 
		UTF8*					targetEnd,
		UTFConversionOptions*	options);

	/// 1文字分の変換 (UTF16 → UTF32)
	static UTFConversionResult ConvertCharUTF16toUTF32(
		const UTF16**			sourceStart,
		const UTF16*			sourceEnd,
		UTFConversionOptions*	options,
		UTF32*					outChar);

	/// 1文字分の変換 (UTF32 → UTF16)
	static UTFConversionResult ConvertCharUTF32toUTF16(
		UTF32					ch,
		UTF16**					targetStart, 
		UTF16*					targetEnd, 
		UTFConversionOptions*	options);

	/// 正規の UTF8 バイト列かをチェックする
	static bool IsLegalUTF8(const UTF8 *source, int length);

	/// 不正文字を許容するか？
	static bool IsStrictConversion(const UTFConversionOptions* options) { return (options->ReplacementChar == 0); }

	/// オプション構造体の初期化
	static void InitOptions(UTFConversionOptions* options);
};

} // namespace LN
