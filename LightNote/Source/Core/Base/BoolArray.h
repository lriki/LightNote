//==============================================================================
// BoolArray
//------------------------------------------------------------------------------
///**
//  @file       BoolArray.h
//  @brief      BoolArray
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
// BoolArray
//------------------------------------------------------------------------------
///**
//  @brief      省メモリ bool配列
//*/
//==============================================================================
class BoolArray
{
public:

	BoolArray()
		: mFlags		( NULL )
		, mArraySize	( 0 )
		, mFlagNum		( 0 )
	{}

	~BoolArray()
	{
		SAFE_DELETE_ARRAY( mFlags );
	}

public:

	/// 要素数の変更 (最初に呼ぶこと)
	void resize( lnU32 size_ )
	{
		SAFE_DELETE_ARRAY( mFlags );

		mFlagNum = size_;
		mArraySize = mFlagNum / 32 + 1;
		mFlags = new lnU32[ mArraySize ];

		clear();
	}

	/// 値をすべて false にする
    void clear()
    {
        memset( mFlags, 0, sizeof( lnU32 ) * mArraySize );
    }

	/// 設定
	void set( lnU32 idx_, bool b_ )
	{
		// ( idx_ >> 5 )   は idx_ / 32
		// ( idx_ & 0x1f ) は idx_ % 32

		if ( b_ )
		{
			*( mFlags + ( idx_ >> 5 ) ) |= ( 0x01 << ( idx_ & 0x1f ) );
		}
		else
		{
			*( mFlags + ( idx_ >> 5 ) ) &= ~( 0x01 << ( idx_ & 0x1f ) );
		}
	}

	/// 取得
	bool get( lnU32 idx_ ) const
	{
		return ( ( *( mFlags + ( idx_ >> 5 ) ) ) & ( 0x01 << ( idx_ & 0x1f ) ) ) != 0;
	}

private:

	lnU32*		mFlags;
	lnU32		mArraySize;
	lnU32		mFlagNum;
};

} // namespace Base
} // namespace Core
} // namespace LNote
