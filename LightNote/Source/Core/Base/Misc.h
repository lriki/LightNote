//==============================================================================
// Misc 
//------------------------------------------------------------------------------
///**
//  @file       Misc.h
//  @brief      Misc
//  @author     Riki
//*/
//==============================================================================

#pragma once

namespace LNote
{
namespace Core
{
namespace Threading{ class Mutex; }

namespace Base
{

//==============================================================================
// ■ Singleton
//------------------------------------------------------------------------------
///**
//	@brief      シングルトンクラスの基底
//*/
//==============================================================================
template <class T>
class Singleton
{
public:
	static inline T& getInstance()
	{
		static T instance;
		return instance;
	}

protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	Singleton(const Singleton& rhs) {}
	void operator=(const Singleton& rhs) {}
};

//==============================================================================
// ■ TempBuffer
//------------------------------------------------------------------------------
///**
//	@brief      
//*/
//==============================================================================
class TempBuffer
{
public:

    TempBuffer();

    ~TempBuffer();

public:

	/// size_ 分の領域を確保して、先頭アドレスを返す (既に確保されている場合は確保しない)
    lnByte* request( lnU32 size_ );

	/// 排他制御付の requestBuffer()
    lnByte* lock( lnU32 size_ );

	/// lookBuffer() に対するアンロック
    void unlock();

	/// 確保したメモリ領域取得
    lnByte* getBuffer() { return mBuffer; }

private:

    lnByte* mBuffer;
    lnU32   mSize;
    Threading::Mutex* mMutex;
};

//==============================================================================
// ■ TempBufferLock
//------------------------------------------------------------------------------
///**
//	@brief      
//*/
//==============================================================================
class TempBufferLock
{
public:

	/// コンストラクタ
    TempBufferLock( TempBuffer& target_obj_, lnU32 request_size_ )
        : mObject( target_obj_ )
    {
        mObject.lock( request_size_ );
    }

	/// デストラクタ
    ~TempBufferLock()
    {
        mObject.unlock();
    }

	/// 確保したメモリ領域取得
    lnByte* getBuffer() { return mObject.getBuffer(); }

private:

    TempBuffer& mObject;
};

//==============================================================================
// ■ FinallyDeletePtr
//------------------------------------------------------------------------------
///**
//	@brief      finally { delete obj; } の代用クラス
//
//  @note
//              TYPE_ はポインタ型であること。
//*/
//==============================================================================
template < class TYPE_ >
class FinallyDeletePtr
    : Base::NonCopyable
{
public:

    FinallyDeletePtr( TYPE_ obj_ ) : mObject( obj_ ) { }

    ~FinallyDeletePtr() { SAFE_DELETE( mObject ); }

    TYPE_& operator * () { return *mObject; }

    TYPE_ operator -> () { return mObject; }

    TYPE_ returnObject()
    {
        TYPE_ obj = mObject;
        mObject = NULL;
        return obj;
    }

private:

    TYPE_  mObject;
};

//==============================================================================
// ■ FinallyReleasePtr
//------------------------------------------------------------------------------
///**
//	@brief      finally { obj->release; } の代用クラス
//*/
//==============================================================================
template < class T >
class FinallyReleasePtr
    : Base::NonCopyable
{
public:

    FinallyReleasePtr() : p( NULL ) { }

    explicit FinallyReleasePtr( T* obj ) : p( obj ) { }

    ~FinallyReleasePtr() { LN_SAFE_RELEASE( p ); }

    operator T*() const { return p; }

    T** operator&() { return &p; }

    T& operator * () { return *p; }

    T* operator -> () { return p; }

    T* getPtr() { return p; }

    T* returnObject()
    {
        T* t = p;
        p = NULL;
        return t;
    }

private:

    T*  p;
};

#if 0
template < class TYPE_ >
class FinallyReleasePtr
    : Base::NonCopyable
{
public:

    FinallyReleasePtr( TYPE_ obj_ ) : mObject( obj_ ) { }

    ~FinallyReleasePtr() { LN_SAFE_RELEASE( mObject ); }

    TYPE_& operator * () { return *mObject; }

    TYPE_ operator -> () { return mObject; }

    TYPE_ getObjectPtr() { return mObject; }

    TYPE_ returnObject()
    {
        TYPE_ obj = mObject;
        mObject = NULL;
        return obj;
    }

private:

    TYPE_  mObject;
};
#endif

//==============================================================================
// ■ 
//==============================================================================

/// リトルエンディアン環境かをチェックする
inline bool isLittleEndian()
{
    static unsigned short x = 1;   // 0x0001
    return ( *(unsigned char *)&x ) != 0;
}

/// 整数の桁数を調べる
inline int getNumberDigits( int x_ ) { int i = 0; while( x_ != 0 ) { ++i; x_/=10; } return i; }

/// 整数の桁数を調べる
inline int getNumberDigits( lnU32 x_ ) { int i = 0; while( x_  > 0 ) { ++i; x_/=10; } return i; }

/// byte 配列の一致判定
template<typename T>
inline bool equalArrays(const T* ary1, size_t count1, const T* ary2, size_t count2 )
{
	if ( count1 != count2 ) {
		return false;
	}
	for ( int i = 0; i < count1; ++i ) {
		if ( ary1[i] != ary2[i] ) {
			return false;
		}
	}
	return true;
}

} // namespace Base
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================