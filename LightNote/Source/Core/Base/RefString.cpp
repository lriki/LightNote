//==============================================================================
// RefString 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "StringUtils.h"
#include "RefString.h"

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
// ■ RefStringCore クラス
//==============================================================================

//----------------------------------------------------------------------
// ● デフォルトコンストラクタ
//----------------------------------------------------------------------
RefStringCore::RefStringCore()
	: mString(NULL)
	, mSize(0)
{
	mString = LN_NEW char[1];
	mString[0] = '\0';
}

//----------------------------------------------------------------------
// ● コンストラクタ ( 文字列設定 )
//----------------------------------------------------------------------
RefStringCore::RefStringCore(const char* str_)
	: mString(NULL)
{
	mString = StringUtils::copyLength(str_, -1);
	mSize = strlen(mString);
}

//----------------------------------------------------------------------
// ● コンストラクタ ( 長さ指定 )
//----------------------------------------------------------------------
RefStringCore::RefStringCore(const char* str_, int length_)
	: mString(NULL)
{
	mString = StringUtils::copyLength(str_, length_);
	mSize = strlen(mString);
}

//----------------------------------------------------------------------
// ● コンストラクタ ( コピー開始インデックスと長さを指定 )
//----------------------------------------------------------------------
RefStringCore::RefStringCore(const char* str_, int begin_, int length_)
	: mString(NULL)
{
	mString = StringUtils::copyLength(str_, begin_, length_);
	mSize = strlen(mString);
}

//----------------------------------------------------------------------
// ● コンストラクタ ( ワイド文字列から作成・文字列設定 )
//----------------------------------------------------------------------
RefStringCore::RefStringCore(const wchar_t* str_)
	: mString(NULL)
{
	mString = StringUtils::convWideToMulti(str_);
	mSize = strlen(mString);
}

//----------------------------------------------------------------------
// ● コンストラクタ ( ワイド文字列から作成・長さ指定 )
//----------------------------------------------------------------------
RefStringCore::RefStringCore(const wchar_t* str_, int length_)
	: mString(NULL)
{
	wchar_t* s = StringUtils::copyLength(str_, length_);
	mString = StringUtils::convWideToMulti(s);
	SAFE_DELETE_ARRAY(s);
	mSize = strlen(mString);
}

//----------------------------------------------------------------------
// ● コンストラクタ ( ワイド文字列から作成・コピー開始インデックスと長さを指定 )
//----------------------------------------------------------------------
RefStringCore::RefStringCore(const wchar_t* str_, int begin_, int length_)
	: mString(NULL)
{
	wchar_t* s = StringUtils::copyLength(str_, begin_, length_);
	mString = StringUtils::convWideToMulti(s);
	SAFE_DELETE_ARRAY(s);
	mSize = strlen(mString);

}

//----------------------------------------------------------------------
// ● デストラクタ
//----------------------------------------------------------------------
RefStringCore::~RefStringCore()
{
	SAFE_DELETE_ARRAY(mString);
	mSize = 0;
}


void RefStringCore::operator += (const char* str_)
{
	char* new_str = StringUtils::merge(mString, str_);
	SAFE_DELETE_ARRAY(mString);
	mString = new_str;
}

bool RefStringCore::operator == (const RefStringCore& obj_) const
{
	if (mSize == obj_.mSize)
	{
		return (strcmp(mString, obj_.mString) == 0);
	}
	return false;
}

bool RefStringCore::operator == (const char* str_) const
{
	return (strcmp(mString, str_) == 0);
}

bool RefStringCore::operator != (const RefStringCore& obj_) const
{
	return (strcmp(mString, obj_.mString) != 0);
}

bool RefStringCore::operator != (const char* str_) const
{
	return (strcmp(mString, str_) != 0);
}

bool RefStringCore::operator < (const RefStringCore& obj_) const
{
	return (strcmp(mString, obj_.mString) < 0);
}

bool RefStringCore::operator < (const char* str_) const
{
	return (strcmp(mString, str_) < 0);
}

//==============================================================================
// ■ RefWStringCore クラス
//==============================================================================

//----------------------------------------------------------------------
// ● デフォルトコンストラクタ
//----------------------------------------------------------------------
RefWStringCore::RefWStringCore()
	: mString(NULL)
	, mSize(0)
{
	mString = LN_NEW wchar_t[1];
	mString[0] = L'\0';
}

//----------------------------------------------------------------------
// ● コンストラクタ ( 文字列設定 )
//----------------------------------------------------------------------
RefWStringCore::RefWStringCore(const wchar_t* str_)
	: mString(NULL)
{
	mString = StringUtils::copyLength(str_, -1);
	mSize = wcslen(mString);
}

//----------------------------------------------------------------------
// ● コンストラクタ ( 長さ指定 )
//----------------------------------------------------------------------
RefWStringCore::RefWStringCore(const wchar_t* str_, int length_)
	: mString(NULL)
{
	mString = StringUtils::copyLength(str_, length_);
	mSize = wcslen(mString);
}

//----------------------------------------------------------------------
// ● コンストラクタ ( コピー開始インデックスと長さを指定 )
//----------------------------------------------------------------------
RefWStringCore::RefWStringCore(const wchar_t* str_, int begin_, int length_)
	: mString(NULL)
{
	mString = StringUtils::copyLength(str_, begin_, length_);
	mSize = wcslen(mString);
}

//----------------------------------------------------------------------
// ● コンストラクタ ( マルチバイト文字列から作成・文字列設定 )
//----------------------------------------------------------------------
RefWStringCore::RefWStringCore(const char* str_)
	: mString(NULL)
{
	mString = StringUtils::convMultiToWide(str_);
	mSize = wcslen(mString);
}

//----------------------------------------------------------------------
// ● コンストラクタ ( マルチバイト文字列から作成・長さ指定 )
//----------------------------------------------------------------------
RefWStringCore::RefWStringCore(const char* str_, int length_)
	: mString(NULL)
{
	char* s = StringUtils::copyLength(str_, length_);
	mString = StringUtils::convMultiToWide(s);
	SAFE_DELETE_ARRAY(s);
	mSize = wcslen(mString);
}

//----------------------------------------------------------------------
// ● コンストラクタ ( マルチバイト文字列から作成・コピー開始インデックスと長さを指定 )
//----------------------------------------------------------------------
RefWStringCore::RefWStringCore(const char* str_, int begin_, int length_)
	: mString(NULL)
{
	char* s = StringUtils::copyLength(str_, begin_, length_);
	mString = StringUtils::convMultiToWide(s);
	SAFE_DELETE_ARRAY(s);
	mSize = wcslen(mString);
}

//----------------------------------------------------------------------
// ● デストラクタ
//----------------------------------------------------------------------
RefWStringCore::~RefWStringCore()
{
	SAFE_DELETE_ARRAY(mString);
	mSize = 0;
}


void RefWStringCore::operator += (const wchar_t* str_)
{
	wchar_t* new_str = StringUtils::merge(mString, str_);
	SAFE_DELETE_ARRAY(mString);
	mString = new_str;
}

bool RefWStringCore::operator == (const RefWStringCore& obj_) const
{
	if (mSize == obj_.mSize)
	{
		return (wcscmp(mString, obj_.mString) == 0);
	}
	return false;
}

bool RefWStringCore::operator == (const wchar_t* str_) const
{
	return (wcscmp(mString, str_) == 0);
}

bool RefWStringCore::operator != (const RefWStringCore& obj_) const
{
	return (wcscmp(mString, obj_.mString) != 0);
}

bool RefWStringCore::operator != (const wchar_t* str_) const
{
	return (wcscmp(mString, str_) != 0);
}

bool RefWStringCore::operator < (const RefWStringCore& obj_) const
{
	return (wcscmp(mString, obj_.mString) < 0);
}

bool RefWStringCore::operator < (const wchar_t* str_) const
{
	return (wcscmp(mString, str_) < 0);
}






//==============================================================================
// ■ RefString クラス
//==============================================================================

    static RefString EMPTY_STRING_A( "" );

	// コンストラクタ ( 受け取った文字列をコピーして作成 )
	RefString::RefString( const char* str_ )
	{
		mImpl = LN_NEW RefStringCore( str_ );
	}
	RefString::RefString( const char* str_, int length_ )
	{
		mImpl = LN_NEW RefStringCore( str_, length_ );
	}
	RefString::RefString( const char* str_, int begin_, int length_ )
	{
		mImpl = LN_NEW RefStringCore( str_, begin_, length_ );
	}

	// ( ワイド文字列から作成 )
	RefString::RefString( const wchar_t* str_ )
	{
		mImpl = LN_NEW RefStringCore( str_ );
	}
	RefString::RefString( const wchar_t* str_, int length_ )
	{
		mImpl = LN_NEW RefStringCore( str_, length_ );
	}
	RefString::RefString( const wchar_t* str_, int begin_, int length_ )
	{
		mImpl = LN_NEW RefStringCore( str_, begin_, length_ );
	}

	// コンストラクタ ( RefWString から作成 )
	RefString::RefString( const RefWString str_ )
	{
		mImpl = LN_NEW RefStringCore( str_.c_str() );
	}

	// 代入演算子 ( char* から )
	RefString& RefString::operator = ( const char* str_ )
	{
		LN_SAFE_RELEASE( mImpl );
        if ( str_ )
        {
		    mImpl = LN_NEW RefStringCore( str_ );
        }
        else
        {
            *this = EMPTY_STRING_A;
        }
		return *this;
	}

	// 代入演算子 ( wchar_t* から )
	RefString& RefString::operator = ( const wchar_t* str_ )
	{
		LN_SAFE_RELEASE( mImpl );
        if ( str_ )
        {
		    mImpl = LN_NEW RefStringCore( str_ );
        }
        else
        {
            *this = EMPTY_STRING_A;
        }
		return *this;
	}

	// 代入演算子 ( RefWString から )
	RefString& RefString::operator = ( const RefWString str_ )
	{
		LN_SAFE_RELEASE( mImpl );
		mImpl = LN_NEW RefStringCore( str_.c_str() );
		return *this;
	}

	// 文字数の取得
	int RefString::size() const
	{
        return ( mImpl ) ? mImpl->size() : 0;
	}

	// 文字列へのポインタの取得
	//char* RefString::c_str()
	//{
	//	return ( mImpl ) ? mImpl->c_str() : NULL;
	//}

	// const 版
    const char* RefString::c_str() const
    {
        return ( mImpl ) ? mImpl->c_str() : "";   // const だからとりあえずこれで OK
    }

#ifdef LNOTE_GNUC
 int _vscprintf (const char * format, va_list pargs) { 
	LN_THROW_NotImpl(0);
      int retval; 
      va_list argcopy; 
      va_copy(argcopy, pargs); 
      retval = vsnprintf(NULL, 0, format, argcopy); 
      va_end(argcopy); 
      return retval; 
   }
#endif

	// 書式を指定して文字列を作成する
    void RefString::format( const char* format_, ... )
    {
        va_list args;
        va_start( args, format_ );

        size_t size = _vscprintf( format_, args ) + 1;

        //char* tmp_buf = static_cast< char* >( malloc( size * sizeof( char ) ) );
        char* tmp_buf = LN_NEW char[ size ];
        vsnprintf( tmp_buf, size, format_, args );
        tmp_buf[ size - 1 ] = '\0';

        LN_SAFE_RELEASE( mImpl );
		mImpl = LN_NEW RefStringCore( tmp_buf );

        //SAFE_FREE( tmp_buf );
        SAFE_DELETE_ARRAY( tmp_buf );
        va_end( args );
    }

	// クリア
    void RefString::clear()
    {
        *this = EMPTY_STRING_A;
    }

	// 空文字列かを判定する
    bool RefString::empty() const
    {
        return ( mImpl && mImpl->size() != 0 ) ? false : true;
    }

    void RefString::operator += ( const char* str_ )
    {
        RefStringCore*	impl = mImpl;
        
		mImpl = LN_NEW RefStringCore( impl->c_str() );
        ( *mImpl ) += str_;

        LN_SAFE_RELEASE( impl );
    }

    bool RefString::operator == ( const RefString& obj_ ) const
    {
        if ( mImpl == obj_.mImpl ) return true;
        return ( *mImpl == *obj_.mImpl );
    }

    bool RefString::operator == ( const char* str_ ) const
    {
        if ( !mImpl ) return (strcmp( "", str_ ) != 0);
        return ( *mImpl == str_ );
    }

    bool RefString::operator != ( const RefString& obj_ ) const
    {
        if ( mImpl != obj_.mImpl ) return true;
        return ( *mImpl != *obj_.mImpl );
    }

    bool RefString::operator != ( const char* str_ ) const
    {
        if ( !mImpl ) return (strcmp( "", str_ ) != 0);
        return ( *mImpl != str_ );
    }

    bool RefString::operator < ( const RefString& obj_ ) const
    {
        return ( *mImpl < *obj_.mImpl );
    }

    bool RefString::operator < ( const char* str_ ) const
    {
        return ( *mImpl < str_ );
    }

    char& RefString::operator [] ( int idx_ )
    {
        return mImpl->c_str()[ idx_ ];
    }

    const char& RefString::operator [] ( int idx_ ) const
    {
        return mImpl->c_str()[ idx_ ];
    }

    RefString::operator std::string () const
    {
        return std::string( mImpl->c_str() );
    }

	// 参照カウント用
	#define CLASSNAME RefString
	#define IMPLCLASS RefStringCore
    #define NOTCMP
	#include "RefClassTemplate.h"


//==============================================================================
// ■ RefWString クラス
//==============================================================================

    static RefString EMPTY_STRING_W( L"" );

	// コンストラクタ ( 受け取った文字列をコピーして作成 )
	RefWString::RefWString( const wchar_t* str_ )
	{
		mImpl = LN_NEW RefWStringCore( str_ );
	}
	RefWString::RefWString( const wchar_t* str_, int length_ )
	{
		mImpl = LN_NEW RefWStringCore( str_, length_ );
	}
	RefWString::RefWString( const wchar_t* str_, int begin_, int length_ )
	{
		mImpl = LN_NEW RefWStringCore( str_, begin_, length_ );
	}

	// ( マルチバイト文字列から作成 )
	RefWString::RefWString( const char* str_ )
	{
		mImpl = LN_NEW RefWStringCore( str_ );
	}
	RefWString::RefWString( const char* str_, int length_ )
	{
		mImpl = LN_NEW RefWStringCore( str_, length_ );
	}
	RefWString::RefWString( const char* str_, int begin_, int length_ )
	{
		mImpl = LN_NEW RefWStringCore( str_, begin_, length_ );
	}

	/// コンストラクタ ( LString から作成 )
	RefWString::RefWString( const RefString str_ )
	{
		mImpl = LN_NEW RefWStringCore( str_.c_str() );
	}

	// 代入演算子 ( wchar_t* から )
	RefWString& RefWString::operator = ( const wchar_t* str_ )
	{
		LN_SAFE_RELEASE( mImpl );
		if ( str_ )
        {
		    mImpl = LN_NEW RefWStringCore( str_ );
        }
        else
        {
            *this = EMPTY_STRING_W;
        }
		return *this;
	}

	// 代入演算子 ( char* から )
	RefWString& RefWString::operator = ( const char* str_ )
	{
		LN_SAFE_RELEASE( mImpl );
        if ( str_ )
        {
		    mImpl = LN_NEW RefWStringCore( str_ );
        }
        else
        {
            *this = EMPTY_STRING_W;
        }
		return *this;
	}

	// 代入演算子 ( LString から )
	RefWString& RefWString::operator = ( const RefString str_ )
	{
		LN_SAFE_RELEASE( mImpl );
        mImpl = LN_NEW RefWStringCore( str_.c_str() );
		return *this;
	}

	// 文字数の取得
	int RefWString::size() const
	{
		return ( mImpl ) ? mImpl->size() : 0;
	}

	// 文字列へのポインタの取得
	//wchar_t* RefWString::c_str()
	//{
	//	return ( mImpl ) ? mImpl->c_str() : NULL;
	//}

	// const 版
    const wchar_t* RefWString::c_str() const
    {
        return ( mImpl ) ? mImpl->c_str() : L"";
    }

	// 書式を指定して文字列を作成する
    void RefWString::format( const wchar_t* format_, ... )
    {
		static const int TmpBufSize = 512;
		wchar_t tmpBuf[TmpBufSize];

        va_list args;
        va_start( args, format_ );

        //size_t size = _vscwprintf( format_, args ) + 1;

        //wchar_t* tmp_buf = static_cast< wchar_t* >( malloc( size * sizeof( wchar_t ) ) );
        //wchar_t* tmp_buf = LN_NEW wchar_t[ size ];
        //vswprintf( tmp_buf, size, format_, args );
        //tmp_buf[ size - 1 ] = L'\0';
		int validSize = vswprintf( tmpBuf, TmpBufSize, format_, args );
		LN_THROW_InvalidOperation((validSize < TmpBufSize));

        LN_SAFE_RELEASE( mImpl );
		//mImpl = LN_NEW RefWStringCore( tmp_buf );
		mImpl = LN_NEW RefWStringCore( tmpBuf );

        //SAFE_FREE( tmp_buf );
        //SAFE_DELETE_ARRAY( tmp_buf );
        va_end( args );

        /*
        RefStringCore ts = format_;    // to char
        std::string sts = ts.c_str();
        //std::wstring ts = format_;
        StringUtil::replaceString( &sts, "%s", "%ls" );
        StringUtil::replaceString( &sts, "%c", "%lc" );

        

        _p( sts.c_str() );
        _p( vsnprintf( NULL, 0, sts.c_str(), args ) );

        
        */
    }

	// クリア
    void RefWString::clear()
    {
        *this = EMPTY_STRING_W;
    }

	// 空文字列かを判定する
    bool RefWString::empty() const
    {
        return ( mImpl && mImpl->size() != 0 ) ? false : true;
    }

    void RefWString::operator += ( const wchar_t* str_ )
    {
        RefWStringCore*	impl = mImpl;
        
		mImpl = LN_NEW RefWStringCore( impl->c_str() );
        ( *mImpl ) += str_;

        LN_SAFE_RELEASE( impl );
    }

    bool RefWString::operator == ( const RefWString& obj_ ) const
    {
        if ( mImpl == obj_.mImpl ) return true;
        return ( *mImpl == *obj_.mImpl );
    }

    bool RefWString::operator == ( const wchar_t* str_ ) const
    {
        if ( !mImpl ) return (wcscmp( L"", str_ ) == 0);
        return ( *mImpl == str_ );
    }

    bool RefWString::operator != ( const RefWString& obj_ ) const
    {
        if ( mImpl != obj_.mImpl ) return true;
        return ( *mImpl != *obj_.mImpl );
    }

    bool RefWString::operator != ( const wchar_t* str_ ) const
    {
        if ( !mImpl ) return (wcscmp( L"", str_ ) != 0);
        return ( *mImpl != str_ );
    }

    bool RefWString::operator < ( const RefWString& obj_ ) const
    {
        return ( *mImpl < *obj_.mImpl );
    }

    bool RefWString::operator < ( const wchar_t* str_ ) const
    {
        return ( *mImpl < str_ );
    }

    wchar_t& RefWString::operator [] ( int idx_ )
    {
        return mImpl->c_str()[ idx_ ];
    }

    const wchar_t& RefWString::operator [] ( int idx_ ) const
    {
        return mImpl->c_str()[ idx_ ];
    }

    RefWString::operator std::wstring () const
    {
        return std::wstring( mImpl->c_str() );
    }

	// 参照カウント用
	#define CLASSNAME RefWString
	#define IMPLCLASS Base::RefWStringCore
    #define NOTCMP
	#include "RefClassTemplate.h"


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================
