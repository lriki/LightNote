//==============================================================================
// Parser 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "StringUtils.h"
#include "Parser.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Base
{

//==============================================================================
// ■ Parser
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Parser::Parser()
        : mTokenAnalyzer    ( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool Parser::analyze( const lnChar* code, TokenAnalyzer* token_analyzer )
    {
        const lnChar* c = code;
        mTokenAnalyzer = token_analyzer;

        while ( *c )
        {
			//-------------------------------------------------
			// 改行
			if ( *c == CodeLF ) {
				// LF
                _sendToken( TokenType_NewlLine, c, 1 );
				++c;
				continue;
			}
			else if ( *c == CodeCR ) {
				if ( *(c + 1) != _T( '\0' ) && *(c + 1) != CodeLF ) {
					// CR+LF
					_sendToken( TokenType_NewlLine, c, 2 );
					c += 2;
					continue;
				}
				else {
					// CR
					_sendToken( TokenType_NewlLine, c, 1 );
					++c;
					continue;
				}
			}

			//-------------------------------------------------
            // 空白
            if ( mTokenAnalyzer->isSpace( *c ) )
            {
                const lnChar* begin = c;
				++c;
                for ( ; ; ++c )
                {
                    if ( !mTokenAnalyzer->isSpace( *c ) )
                    {
                        _sendToken( TokenType_WhitespaceBlock, begin, c - 1 );
                        break;
                    }
                }
                continue;
            }

			//-------------------------------------------------
            // 演算子、区切り記号
            const OperatorCode* operator_code = _findOperatorCode( c );
            if ( operator_code )
            {
				_sendToken( TOKEN_TYPE_OPERATOR, c, operator_code->Length, operator_code->Type );
                c += operator_code->Length;
                continue;
            }

			//-------------------------------------------------
            // キーワード
            const KeywordCode* keywordCode = _findKeywordCode( c );
            if ( keywordCode )
            {
				_sendToken( TokenType_Keyword, c, keywordCode->Length, keywordCode->Type );
                c += keywordCode->Length;
                continue;
            }

			//-------------------------------------------------
            // 識別子
            if ( mTokenAnalyzer->isIdentifierStart( *c ) )
            {
                const lnChar* begin = c;
				++c;
                for ( ; ; ++c )
                {
                    if ( !mTokenAnalyzer->isIdentifierPart( *c ) )
                    {
                        _sendToken( TOKEN_TYPE_IDENTIFIER, begin, c - 1 );
                        break;
                    }
                }
                continue;
            }

			//-------------------------------------------------
            // 数値
            if ( mTokenAnalyzer->isDigit( *c ) )
            {
                const lnChar* begin = c;
				++c;
                for ( ; ; ++c )
                {
                    if ( !mTokenAnalyzer->isDigit( *c ) )
                    {
                        _sendToken( TOKEN_TYPE_DIGIT, begin, c - 1 );
                        break;
                    }
                }
                continue;
            }

			//-------------------------------------------------
            // 上記以外のナニか (マルチバイト文字の先行バイト等)
			++c;
        }

        return true;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    const OperatorCode* Parser::_findOperatorCode( const lnChar* c )
    {
		// TODO: 二分探索
        const OperatorCode* codes = mTokenAnalyzer->getOperatorList();
		if ( codes ) {
			for (; codes->Code != NULL; ++codes )
			{
				if ( _tcsncmp( c, codes->Code, codes->Length ) == 0 ) return codes;
			}
		}
        return NULL;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	const KeywordCode* Parser::_findKeywordCode( const lnChar* c )
	{
		// TODO: 二分探索
        const KeywordCode* codes = mTokenAnalyzer->getKeywordList();
		if ( codes ) {
			for (; codes->Code != NULL; ++codes )
			{
				if ( _tcsncmp( c, codes->Code, codes->Length ) == 0 ) return codes;
			}
		}
        return NULL;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Parser::_sendToken( TokenType type, const lnChar* c, lnU32 length, int operatorType )
    {
        TokenCode token;
        token.Type			= type;
        token.Begin			= c;
        token.End			= c + length;
        token.Length		= length;
		token.UserType		= operatorType;
        mTokenAnalyzer->analyzeToken( token );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Parser::_sendToken( TokenType type, const lnChar* begin, const lnChar* end, int operatorType )
    {
        TokenCode token;
        token.Type			= type;
        token.Begin			= begin;
        token.End			= end;
        token.Length		= (end - begin) / sizeof(lnChar) + 1;
		token.UserType		= operatorType;
        mTokenAnalyzer->analyzeToken( token );
    }

//==============================================================================
// ■ TokenAnalyzer
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool TokenAnalyzer::isSpace( lnChar c )
    {
        if ( c == _T(' ') ) return true;
        if ( c == _T('\t') ) return true;
        return false;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool TokenAnalyzer::isAlpha( lnChar c )
    {
        if ( _T('A') <= c && c <= _T('Z') ) return true;
        if ( _T('a') <= c && c <= _T('z') ) return true;
        return false;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool TokenAnalyzer::isDigit( lnChar c )
    {
        if ( _T('0') <= c && c <= _T('9') ) return true;
        return false;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool TokenAnalyzer::isIdentifierStart( lnChar c )
	{
		if ( isAlpha( c ) ) {
			return true;
		}
		if ( c == '_' ) {
			return true;
		}
		return false;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool TokenAnalyzer::isIdentifierPart( lnChar c )
	{
		if ( isAlpha( c ) ) return true;
		if ( isDigit( c ) ) return true;
		if ( c == '_' ) return true;
		return false;
	}

//==============================================================================
// ■ OneLineCSVAnalyzer
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    OneLineCSVAnalyzer::OneLineCSVAnalyzer()
        : mIdentifierSearchingSequence( IDENT_SEQ_IDLE )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    int OneLineCSVAnalyzer::getInt( int index ) const
    {
        const TokenCode& token = mElementArray[index];
        return StringUtils::atoiRange( token.Begin, StringUtils::getLengthRange( token.Begin, token.End ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    const OperatorCode* OneLineCSVAnalyzer::getOperatorList()
    {
        static const OperatorCode codes[] = 
        {
            { _T(","),  1, 1 },
            { NULL,     0, 0 }
        };
        return codes;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void OneLineCSVAnalyzer::analyzeToken( const TokenCode& token )
    {
        if ( token.Type == TOKEN_TYPE_IDENTIFIER || token.Type == TOKEN_TYPE_DIGIT )
        {
            // コンマの前に新たな識別子が現れたらエラー
			LN_THROW_InvalidFormat((mIdentifierSearchingSequence != IDENT_SEQ_COMMA));

            mElementArray.push_back( token );
            mIdentifierSearchingSequence = IDENT_SEQ_COMMA;
        }
        else
        {
            mIdentifierSearchingSequence = IDENT_SEQ_IDLE;
        }
    }

//==============================================================================
// ■ ParserUtil
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    float ParserUtil::parseFloat( const lnChar* str )
    {
        lnChar* endptr;
        float f = (float)_tcstod( str, &endptr );
		LN_THROW_Argument(*endptr == _T('\0'), "invalid format");
        return f;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================