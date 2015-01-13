//==============================================================================
// Parser 
//------------------------------------------------------------------------------
///**
//  @file       Parser.h
//  @brief      Parser
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Base
{
class TokenAnalyzer;

enum TokenType
{
    TOKEN_TYPE_UNKNOWN = 0,
	TokenType_NewlLine,			///< LF, CR, CR+LF
	TokenType_WhitespaceBlock,	///< 空白文字の連続
    TOKEN_TYPE_IDENTIFIER,      ///< 識別子
    TOKEN_TYPE_DIGIT,           ///< 数値
    TOKEN_TYPE_OPERATOR,
    TokenType_Keyword,
};

struct TokenCode
{
    TokenType       Type;
    const lnChar*   Begin;  
    const lnChar*   End;    ///< 終端の文字を指すポインタ (1 文字の場合は Begin と同じ)
    lnU32           Length;
	int				UserType;	///< ユーザーデータ
};

struct OperatorCode
{
    lnChar*     Code;
    lnU32       Length;
    int         Type;		///< ユーザーデータ
};

struct KeywordCode
{
    lnChar*     Code;
    lnU32       Length;
    int         Type;		///< ユーザーデータ
};

//==============================================================================
// ■ Parser
//------------------------------------------------------------------------------
///**
//  @brief      簡易高速構文解析器
//*/
//==============================================================================
class Parser
{
public:
	static const lnChar CodeLF = _T( '\n' );
	static const lnChar CodeCR = _T( '\r' );

public:

    Parser();

	/// 解析 (code はこのクラス及び token_analyzer が必要なくなってから解放すること)
    bool analyze( const lnChar* code, TokenAnalyzer* token_analyzer );

private:
    const OperatorCode* _findOperatorCode( const lnChar* c );
	const KeywordCode*	_findKeywordCode( const lnChar* c );
    void				_sendToken( TokenType type, const lnChar* c, lnU32 length, int operatorType = 0 );
    void				_sendToken( TokenType type, const lnChar* begin, const lnChar* end, int operatorType = 0 );

private:

    TokenAnalyzer*  mTokenAnalyzer;
};

//==============================================================================
// ■ TokenAnalyzer
//------------------------------------------------------------------------------
///**
//  @brief      トークン解析用インターフェイス
//*/
//==============================================================================
class TokenAnalyzer
{
protected:

	/// (文字数の降順の配列を定義すること)
    virtual const OperatorCode* getOperatorList() = 0;

	/// (文字数の降順の配列を定義すること)
	virtual const KeywordCode* getKeywordList() { return NULL; }

	// とりあえず virtual は外しておく。必要になったら改めて付けてオーバーライド。
    /*virtual*/ bool isSpace( lnChar c );
    /*virtual*/ bool isAlpha( lnChar c );
    /*virtual*/ bool isDigit( lnChar c );

	/// 識別子の開始文字
	bool isIdentifierStart( lnChar c );

	/// 識別子
	bool isIdentifierPart( lnChar c );

    virtual void analyzeToken( const TokenCode& token ) = 0;
    
    friend class Parser;
};

//==============================================================================
// ■ OneLineCSVAnalyzer
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class OneLineCSVAnalyzer
    : public TokenAnalyzer
{
public:

    OneLineCSVAnalyzer();

public:

	/// 要素数の取得
    int getElementCount() const { return mElementArray.size(); }

	/// 整数として取得
    int getInt( int index ) const;

protected:

    virtual const OperatorCode* getOperatorList();
    virtual void analyzeToken( const TokenCode& token );

private:

    enum IdentifierSearchingSequence
    {
        IDENT_SEQ_IDLE,
        IDENT_SEQ_COMMA,    ///< ,
    };

    IdentifierSearchingSequence     mIdentifierSearchingSequence;
    std::vector< TokenCode >        mElementArray;
};

//==============================================================================
// ■ ParserUtil
//------------------------------------------------------------------------------
///**
//  @brief
//  @note
//              変換に失敗した場合は例外が発生する。
//*/
//==============================================================================
class ParserUtil
{
public:

    static float parseFloat( const lnChar* str );
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================