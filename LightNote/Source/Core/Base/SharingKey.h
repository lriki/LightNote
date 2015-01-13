//==============================================================================
// SharingKey 
//------------------------------------------------------------------------------
///**
//  @file       SharingKey.h
//  @brief      SharingKey
//*/
//==============================================================================

#pragma once

#include "../FileIO/FileUtil.h"
#include "StringUtils.h"
#include "Hash.h"

namespace LNote
{
namespace Core
{
namespace Base
{

//==============================================================================
// SharingKey
//------------------------------------------------------------------------------
///**
//	@brief      ハッシュ値、文字列 ( char、wchar_t ) をまとめてひとつのキーとして扱うクラス
//*/
//==============================================================================
class SharingKey
{
public:
	static const SharingKey NULL_KEY;

public:

	/// 空のキー
    SharingKey()
        : mHash( 0 )
    {
        mReqType = 0;
    }

	/// ハッシュ値 (整数値)
    explicit SharingKey( lnU32 key )
        : mHash( key )
    {
        mReqType = ( key != 0 ) ? 1 : 0;
    }

	/// 文字列
    explicit SharingKey( const lnChar* key )
        : mHash( 0 )
        , mString( key )
    {
        FileIO::Path::normalizePath( &mString );
        mReqType = ( mString.size() > 0 ) ? 2 : 0;
    }

	/// RefString
    explicit SharingKey( RefTString key )
        : mHash( 0 )
        , mString( key.c_str() )
    {
        FileIO::Path::normalizePath( &mString );
        mReqType = ( mString.size() > 0 ) ? 2 : 0;
    }

	/// デストラクタ
    virtual ~SharingKey() {}

public:

	/// キーが空か判定する
    bool isEmpty() const 
    {
        return ( mHash == 0 && mString.size() == 0 );
    }

	/// キーが文字列かを判定する
    bool isString() const
    {
        return ( mHash == 0 && mString.size() > 0 );
    }

	/// 数値を取得する
    lnU32 getKeyHash() const { return mHash; }

	/// 文字列を取得する
    const lnChar* getKeyString() const { return ( mString.size() > 0 ) ? mString.c_str() : NULL; }

public:

    bool operator == ( const SharingKey& key ) const
    {
        if ( mReqType == key.mReqType )
        {
            if ( mHash != 0 )
            {
                return ( mHash == key.mHash );
            }
            if ( mString.size() > 0 && key.mString.size() > 0 )
            {
                return ( mString == key.mString );
            }
        }
        return false;
    };

    

    bool operator < ( const SharingKey& key ) const
    {
        if ( mReqType == key.mReqType )
        {
            if ( mHash != 0 )
            {
                return ( mHash < key.mHash );
            }
            if ( mString.size() > 0 )
            {
                return ( mString < key.mString );
            }
        }
        return mReqType < key.mReqType;
        //return mReqType == key_.mReqType; // ハマった…

        // map,set が検索を行う時は
        //「X1 < X2」「X2 < X1」の両方が成立しなかったとき「X1==X2」とみなす
        // ってのが標準的な動作らしい
    };

	/// copy
    SharingKey( const SharingKey& key )
    {
		mHash		= key.mHash;
		mString		= key.mString;
		mReqType	= key.mReqType;
    }

	/// copy
	SharingKey& operator = ( const SharingKey& key )
	{
		mHash		= key.mHash;
		mString		= key.mString;
		mReqType	= key.mReqType;
		return *this;
	}

private:

    lnU32		mHash;
    RefTString	mString;
    lnU32		mReqType;

public:

    unsigned getGenericHash( unsigned m ) const
    {
        unsigned h = 0;
        int s = mString.size();
        if ( s > 0 )
        {
            const unsigned char* p = reinterpret_cast< const unsigned char* >( mString.c_str() );
            for ( int i = 0; i < s; ++i )
            {
		        h *= 137;
		        h += p[ i ];
	        }
            h %= m;
        }
        return h;
    }
    
};

// class SharingKey 
template<> inline unsigned GenericHash< SharingKey >::value( const SharingKey& v, unsigned m ) const
{
    return v.getGenericHash( m );
}

} // namespace Base
} // namespace Core

const Core::Base::SharingKey    lnNullKey;
typedef Core::Base::SharingKey  lnSharingKey;

} // namespace LNote
