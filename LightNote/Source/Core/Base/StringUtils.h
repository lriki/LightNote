//==============================================================================
// StringUtils 
//------------------------------------------------------------------------------
///**
//  @file       StringUtils.h
//  @brief      StringUtils
//  @note
//              この中に RefString を使うものを書いたりすると include あたりが
//              ややこしいことになるので書かないこと。
//*/
//==============================================================================

#pragma once

#include <vector>
#include <string>
#include "String.h"

namespace LNote
{

enum LNCharacterCode
{
    LN_CHARACTERCODE_UTF8 = 1,
    LN_CHARACTERCODE_SIFTJIS,
};

namespace Core
{
namespace Base
{

//==============================================================================
// StringUtils
//------------------------------------------------------------------------------
///**
//  @brief		文字列ユーティリティ
//*/
//==============================================================================
class StringUtils
{
public:

	//----------------------------------------------------------------------
	///**
	//  @brief      バッファを確保して文字列をコピーする ( マルチバイト文字列 )
	//
	//  @param[in]  src_str_ : コピー元文字列
	//  @param[in]  length_  : コピーする長さ ( -1 で \0 までコピー )
	//
	//  @return     コピーされた文字列 ( delete[] で解放する必要があります )
	//*/
	//----------------------------------------------------------------------
	static char* copyLength( const char* src_str_, int length_ );

	//----------------------------------------------------------------------
	///**
	//	@brief		バッファを確保して文字列をコピーする ( ワイド文字列 )
	//
	//  @par
	//              copyLength() のワイド文字列版です。
	//*/
	//----------------------------------------------------------------------
	static wchar_t* copyLength( const wchar_t* src_str_, int length_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      バッファを確保して文字列をコピーする ( マルチバイト文字列 )
	//
	//  @param[in]  src_str_ : コピー元文字列
	//  @param[in]  begin_   : コピー元の先頭インデックス
	//  @param[in]  length_  : コピーする長さ ( -1 で \0 までコピー )
	//
	//  @return     コピーされた文字列 ( delete[] で解放する必要があります )
	//
	//  @par
	//              begin_ がコピー元文字列の長さを超えている場合は空の文字列を返します。
	//*/
	//----------------------------------------------------------------------
	static char* copyLength( const char* src_str_, int begin_, int length_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      バッファを確保して文字列をコピーする ( ワイド文字列 )
	//
	//  @par
	//              copyLength() のワイド文字列版です。
	//*/
	//----------------------------------------------------------------------
	static wchar_t* copyLength( const wchar_t* src_str_, int begin_, int length_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      バッファを確保して文字列をコピーする ( 範囲指定・マルチバイト文字列 )
	//
	//  @param[in]  src_str_ : コピー元文字列
	//  @param[in]  begin_	 : コピー元の先頭位置
	//  @param[in]  end_	 : コピー元の終端位置
	//
	//  @return     コピーされた文字列 ( delete[] で解放する必要があります )
	//
	//  @par
	//              src_str_[ end_ ] のひとつ前の文字までコピーします。	        <br>
	//                                                                          <br>
	//                  copyRange( "abcdef", 1, 3 ) => "bc"			            <br>
	//                                                                          <br>
	//              begin_ と end_ の差が 0 以下の場合は空の文字列を返します。
	//*/
	//----------------------------------------------------------------------
	static char* copyRange( const char* src_str_, int begin_, int end_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      バッファを確保して文字列をコピーする ( 範囲指定・ワイド文字列 )
	// 
	//  @par
	//              copyRange() のワイド文字列版です。
	//*/
	//----------------------------------------------------------------------
	static wchar_t* copyRange( const wchar_t* src_str_, int begin_, int end_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      ふたつの文字列を結合した新しい文字列を作成する ( char )
	//
	//  @return     作成された文字列 ( delete[] で解放する必要があります )  
	//*/
	//----------------------------------------------------------------------
	static char* merge( const char* src1_, const char* src2_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      ふたつの文字列を結合した新しい文字列を作成する ( char )
	//
	//  @return     作成された文字列 ( delete[] で解放する必要があります )  
	//*/
	//----------------------------------------------------------------------
	static wchar_t* merge( const wchar_t* src1_, const wchar_t* src2_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      マルチバイト文字列をワイド文字列に変換した後の文字数取得
	//
	//  @param[in]  multi_str_ : 変換元のマルチバイト文字列
	//
	//  @retval     -1 : 失敗
	//
	//  @return     ワイド文字での文字数 ( NULL 終端文字の分は含まない )
	//*/
	//----------------------------------------------------------------------
	static int getMultiToWideLength( const char* multi_str_ );

	//----------------------------------------------------------------------
	///**
	//  @brief		マルチバイト文字列をワイド文字列に変換
	//
	//  @param[out]	out_str_ : 変換後のワイド文字列を格納するバッファへのポインタ
	//  @param[in]	size_	 : out_str_ のバッファのサイズ ( out_str_ に書き込める文字数(\0を含む分) )
	//  @param[in]	in_str_  : null で終わるマルチバイト文字列のアドレス
	//
	//  @retval     true  : 成功
	//  @retval     false : 失敗
	//
	//  @par
	//              バッファサイズが小さい場合は、変換できた分 + 終端 null
	//              をバッファサイズに格納します。                              <br>
	//                                                                          <br>
	//              size_ = 4  in_str_ = "あいうえお" → out_str_ = "あいう\0"	
	//*/
	//----------------------------------------------------------------------
	static bool convMultiToWide( wchar_t* out_str_, const int size_, const char* in_str_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      マルチバイト文字列をワイド文字列に変換
	//
	//  @param[in]  multi_str_ : null で終わるマルチバイト文字列のアドレス
	//
	//  @retval     NULL : 失敗
	//
	//  @return     変換された文字列 ( delete[] で解放する必要があります )
	//*/
	//----------------------------------------------------------------------
	static wchar_t* convMultiToWide( const char* multi_str_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      ワイド文字列をマルチバイト文字列に変換した後の文字数取得
	//
	//  @param[in]  wide_str_ : 変換元のワイド文字列
	//
	//  @retval     -1 : 失敗
	//
	//  @return     マルチバイト文字での文字数 ( NULL 終端文字の分は含みません )
	//*/
	//----------------------------------------------------------------------
	static int getWideToMultiLength( const wchar_t* wide_str_ );

	//----------------------------------------------------------------------
	///**
	//  @brief  ワイド文字列をマルチバイト文字列に変換
	//
	//  @param[out] out_str_ : 変換後のマルチバイト文字列を格納するバッファへのポインタ
	//  @param[in]  size_	 : out_str_ のバッファのサイズ ( 文字数 )
	//  @param[in]  in_str_  : null で終わるワイド文字列のアドレス
	//
	//  @retval     true  : 成功
	//  @retval     false : 失敗
	// 
	//  @par
	//              バッファサイズが小さい場合は、変換できた分 + 終端 null
	//              をバッファサイズに格納します。                              <br>
	//                                                                          <br>
	//              size_ = 4  in_str_ = "あいうえお" → out_str_ = "あいう\0"	
	//*/
	//----------------------------------------------------------------------
	static bool convWideToMulti( char* out_str_, int size_, const wchar_t* in_str_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      ワイド文字列をマルチバイト文字列に変換
	//
	//  @param[in]  wide_str_ : null で終わるワイド文字列のアドレス
	//
	//  @retval     NULL : 失敗
	//
	//  @return     変換された文字列 ( delete[] で解放する必要があります )
	//*/
	//----------------------------------------------------------------------
	static char* convWideToMulti( const wchar_t* wide_str_ );


    

	//----------------------------------------------------------------------
	///**
	//  @brief      2 バイト文字列の先行バイトかを調べる
	//
	//  @param[in]  code_ : チェックする文字へのポインタ
	//
	//  @retval     true  : 2 バイト文字列
	//  @retval     false : 1 バイト文字列
	//
	//  @par
	//              Unicode ビルド時は常に false を返します。
	//*/
	//----------------------------------------------------------------------
    inline static bool checkMultiByteChar( const char* code_ )
    {
#ifdef LNOTE_UNICODE
        return false;
#else
        return  ( (unsigned char)*code_ >= 0x81 && (unsigned char)*code_ <= 0x9f ) || ( (unsigned char)*code_ >= 0xe0 && (unsigned char)*code_ <= 0xfc );
#endif
    }

	//----------------------------------------------------------------------
	///**
	//  @brief      マルチバイト文字を 1 として文字数をカウントする (unicode の場合はそのまま)
	//*/
	//----------------------------------------------------------------------
    static int strlenMB( const lnChar* str_ );

	/// 文字列中の英大文字を小文字に変換
    static char* toLower(char *s)
    {
        char *p;                     /* 作業用ポインタ */
                                     /* for (  )ループの初期化の式で、pは文字列の
                                        先頭アドレスを指すように初期化される */
        for (p = s; *p; p++)         /* pがヌルでなければ */
            *p = tolower(*p);        /* pの指す中身を小文字に変換 */
        return (s);                  /* 文字列の先頭アドレスを返す */
    }

	/// 文字列中の英小文字を大文字に変換
    static char* toUpper(char *s)
    {
        char *p;                     /* 作業用ポインタ */
                                     /* for (  )ループの初期化の式で、pは文字列の
                                        先頭アドレスを指すように初期化される */
        for (p = s; *p; p++)         /* pがヌルでなければ */
            *p = toupper(*p);        /* pの指す中身を大文字に変換 */
        return (s);                  /* 文字列の先頭アドレスを返す */
    }

	//----------------------------------------------------------------------
	///**
	//  @brief      ワイルドカード('*', '?')を使って文字列を比較する
	//
	//  @param[in]  ptn_  : ワイルドカードを含む文字列
	//  @param[in]  str_  : 比較する文字列
	//
	//  @retval     true  : 一致した
	//  @retval     false : 一致しなかった
	//*/
	//----------------------------------------------------------------------
    static bool match( const lnChar* ptn_, const lnChar* str_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      文字列の比較 (大文字小文字を考慮しない)
	//*/
	//----------------------------------------------------------------------
    static bool strcmpI( const char* str1_, const char* str2_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      文字列の比較 (文字数指定で大文字小文字を考慮しない)
	//
	//  @note
	//              CgFX の解析で使ってるので wchar_t とは分ける。
	//*/
	//----------------------------------------------------------------------
    static bool strcmpNI( const char* str1_, const char* str2_, int n_ );

	template<typename T>
	static bool equals(const T* str1, const T* str2);

	template<typename T>
	static bool strcmpN(const T* str1, const T* str2, int n);
   
	//----------------------------------------------------------------------
	///**
	//  @brief      文字列の置換を行う
	//
	//  @param[in]  str_  : 置換対象の文字列
	//  @param[in]  from_ : 検索文字列
	//  @param[in]  to_   : 置換後の文字列
	//*/
	//----------------------------------------------------------------------
    static void replaceString( std::string* str_,  const char*    from_, const char*    to_ );

#if LNOTE_WCHAR_T_SUPPORT
    static void replaceString( std::wstring* str_, const wchar_t* from_, const wchar_t* to_ );
#endif

	//----------------------------------------------------------------------
	///**
	//  @brief      連続するスペース文字 (isspaceで判定) を置換する
	//  @note
	//              置換後の文字列はスペースでもOK
	//*/
	//----------------------------------------------------------------------
    static void replaceSpace( ln_std_tstring* out_, const lnChar* in_, const lnChar* to_ );

	/// 空白 (半角スペース、全角スペース、タブ) を全て削除する
	static void deleteSpace( ln_std_tstring* str_ );

	/// 改行 (\r \n) をすべて削除する
	static void deleteNewLine( ln_std_tstring* str );

	/// 文字をカウントする
	static int countChar( const lnString& str, int offset, lnChar c );

	/// 文字をカウントする
	static int countChar(const wchar_t* str, int offset, wchar_t c);

	/// 連続するスペース文字の数を返す
    static int getSpaceLength( const lnChar* str_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      文字列の先頭と終端のみのホワイトスペースを削除した string を返す
	//*/
	//----------------------------------------------------------------------
    static void trimHFSpace( ln_std_tstring* out_, const ln_std_tstring& in_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      指定した文字の間の文字列を返す (begin_ と end_ の文字は含まない)
	//*/
	//----------------------------------------------------------------------
    static bool trimRange( ln_std_tstring* out_, const lnChar* in_, lnChar begin_, lnChar end_ );


	//----------------------------------------------------------------------
	///**
	//  @brief      文字列分割
	//
	//  @par
	//              str_ の最後にデリミタ文字がある場合は、そのデリミタ文字は無視されます。<br>
	//              "*=MirrorObject.fx;" => "*=MirrorObject.fx"
	//*/
	//----------------------------------------------------------------------
    static std::vector< ln_std_tstring > split( ln_std_tstring str_, ln_std_tstring delim_ );

	// こっちの split はバグあり。
    static void split( std::vector< ln_std_tstring >* out_, const lnChar* str_, const lnChar* delim_ );


	/// 指定した文字をすべて削除する
    static void removeChar( ln_std_tstring* str_, lnChar code_ );

	/// 行数を返す
    static int getNewLineCount( const lnChar* text_ );

	/// 2つの文字ポインタ間の文字数を返す
    static int getLengthRange( const lnChar* begin , const lnChar* end ) { return (end - begin) / sizeof(lnChar) + 1; }

	/// 数値に変換 (文字数指定。64 文字を超える場合はそこまでで変換)
    static int atoiRange( const lnChar* str, int len );

	/// SJIS → UTF8N への変換 (出力バッファサイズは、入力データ量の6倍以上確保しておくこと)
   // static ResultCode convertSJISToUTF8N( const char* in, int in_bytes, lnU8* out, int out_bytes );
    
	///// UTF8 → UTF32 への変換 (1文字単位)
 //   static LNRESULT convertCodeUTF8ToUTF32BE( const lnU8* utf8code, int* read_len, lnU32* utf32code );

	///// [start] ～ [end - 1] の文字列が改行かを判別し、一致したら文字数を返す ("\r" か "\n" なら 1、"\r\n" なら 2)
	//static int checkNewLineSequence(const char* start, const char* end);
	//
	///// [start] ～ [end - 1] の文字列が改行かを判別し、一致したら文字数を返す ("\r" か "\n" なら 1、"\r\n" なら 2)
	//static int checkNewLineSequence(const wchar_t* start, const wchar_t* end);



	///// [start] から改行を検索する (見つかった改行シーケンスの先頭文字か end を返す)
	//static const char* findNextNewLine(const char* start, const char* end);

	///// [start] から改行を検索する (見つかった改行シーケンスの先頭文字か end を返す)
	//static const wchar_t* findNextNewLine(const wchar_t* start, const wchar_t* end);

	/// [start] ～ [end - 1] から改行を検索する (見つかった改行シーケンスの先頭文字か end を返す)
	template<typename T>
	static const T* findNextNewLine(const T* start, const T* end);

	/// [start] ～ [end - 1] の範囲の先頭が改行かを判別し、一致したら文字数を返す ("\r" か "\n" なら 1、"\r\n" なら 2)
	template<typename T>
	static int checkNewLineSequence(const T* start, const T* end);

	/// 検索
	template <typename T>
	static int indexOf(const T* str1, const T* str2, int startIndex);

	/// 検索
	template <typename T>
	static int indexOf(const T* str1, T ch, int startIndex);
	
	/// 文字数取得
	template <typename T>
	static size_t GetLength(const T* str);

	/// 部分文字列の取得
	template <typename T>
	static BasicString<T> subString(const T* str, size_t startIndex = 0, size_t length = 0);

	/// 文字列分割
	template<typename T>
	static void split(const T* str, const T* delim, StringSplitOptions option, std::vector< BasicString<T> >* out);

	/// 文字列分割
	template<typename T>
	static void split(const T* str, T delim, StringSplitOptions option, std::vector< BasicString<T> >* out);

private:
    StringUtils();
};

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
inline std::vector< ln_std_tstring > StringUtils::split( ln_std_tstring str, ln_std_tstring delim )
{
    std::vector< ln_std_tstring > items;
    std::size_t dlm_idx;
    if ( str.npos == ( dlm_idx = str.find_first_of( delim ) ) )
    {
        items.push_back(str.substr(0, dlm_idx));
    }
    while ( str.npos != ( dlm_idx = str.find_first_of( delim ) ) )
    {
        if ( str.npos == str.find_first_not_of( delim ) )
        {
            break;
        }
        items.push_back(str.substr(0, dlm_idx));
        dlm_idx++;
        str = str.erase(0, dlm_idx);
        if ( str.npos == str.find_first_of( delim ) && _T("") != str )
        {
            items.push_back(str);
            break;
        }
    }
    return items;
}

} // namespace Base
} // namespace Core
} // namespace LNote
