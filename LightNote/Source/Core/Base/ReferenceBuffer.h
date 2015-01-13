//==============================================================================
// ReferenceBuffer 
//------------------------------------------------------------------------------
///**
//  @file       ReferenceBuffer.h
//  @brief      ReferenceBuffer
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
// ReferenceBuffer
//------------------------------------------------------------------------------
///**
//  @brief      参照カウント付のメモリバッファ
//*/
//==============================================================================
class ReferenceBuffer
    : public ReferenceObject
{
public:

	ReferenceBuffer()
		: mBuffer		( NULL )
        , mSize			( 0 )
		, mUnManaged	( false )
	{}

	ReferenceBuffer( lnByte* data, size_t size, bool unManaged )
		: mBuffer		( data )
        , mSize			( size )
		, mUnManaged	( unManaged )
	{}

	virtual ~ReferenceBuffer()
    {
		_dispose();
    }

public:

	/// メモリ割り当て
	void reserve(size_t size)
    {
		_dispose();
        mBuffer = LN_NEW lnByte[size];
        mSize = size;
    }

	/// メモリ割り当て + コピー
	void reserve(const lnByte* data, size_t size)
    {
		_dispose();
        mBuffer = LN_NEW lnByte[size];
        mSize = size;
		memcpy( mBuffer, data, mSize );
		mUnManaged = false;
    }

	/// コピー
	void copy(const byte_t* pData, size_t uSize)
	{
		memcpy(mBuffer, pData, uSize);
	}

	/// 0クリア
	void clear()
	{
		if ( mBuffer ) {
			memset( mBuffer, 0, mSize );
		}
	}

	/// アドレス取得
	lnByte* getPointer() const
    {
        return mBuffer;
    }

	/// サイズ取得
	size_t getSize() const
    {
        return mSize;
    }

	/// サイズ変更
	void resize(size_t newSize)
	{
		assert(newSize <= mSize);	// 縮小方向のみとりあえず許可
		mSize = newSize;
	}

	/// ラッピング
	void setBytesBuffer( lnByte* data, size_t size, bool unManaged )
	{
		_dispose();
		mBuffer = data;
		mSize = size;
		mUnManaged = unManaged;
	}

private:

	void _dispose()
	{
		if ( !mUnManaged ) {
			SAFE_DELETE( mBuffer );
		}
	}

private:

    lnByte*     mBuffer;
    size_t      mSize;
	bool		mUnManaged;
};

} // namespace Base
} // namespace Core
} // namespace LNote
