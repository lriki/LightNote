//==============================================================================
// RefPtr 
//------------------------------------------------------------------------------
///**
//  @file       RefPtr.h
//  @brief      RefPtr
//  @version    Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "ReferenceObject.h"
#include "Exception.h"

// TODO: namespace LNote へ
#define LRefPtr ::LNote::Core::Base::RefPtr

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Base
{
#define LN_REFPTR_ASSERT    LN_THROW_InvalidOperation

//==============================================================================
// ■ RefPtr
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
template <class T> 
class RefPtr
{
public:

	// デフォルトコンストラクタ
    RefPtr()
        : p( NULL )
    {}

	// コンストラクタ
    explicit RefPtr( T* p_, bool add_ref_ = false )
        : p( p_ )
    {
        if ( p && add_ref_ ) p->addRef();
    }

	// コピーコンストラクタ
	explicit RefPtr( const RefPtr<T>& obj_ )
        : p( obj_.p )
    {
	    if ( p ) p->addRef();
    }

	// デストラクタ
    ~RefPtr()
	{
		release();
	}

public:

	// 関連付け
    void attach( T* p_, bool add_ref_ = false )
    { 
        release();
        p = p_; 
		if ( p && add_ref_ ) p->addRef();
    }

    T* getPtr() const { return p; }

	// convert
    operator T* () const { return p; }

	// =
    T* operator = ( T* p_ )
    {
        LN_SAFE_ADDREF( p_ );
        release();
	    p = p_;
	    return p;
    }

	// *
    T& operator* () const
    {
        LN_REFPTR_ASSERT( p != NULL );
        return *p;
    }

	// ->
    T* operator -> () const
    {
        LN_REFPTR_ASSERT( p != NULL );
        return p;//(T*)this;
    }

	// !
    bool operator ! () const { return ( p == NULL ); }
    
	// == 
    bool operator == ( const T* p_ ) const { return ( p == p_ ); }

	// !=
    bool operator != ( const T* p_ ) const { return ( p != p_ ); }

	// < (for STL cmp)
    bool operator < ( const T* p_ ) const { return p < p_; }

public:

	void addRef() { LN_SAFE_ADDREF( p ); }
    void release() { LN_SAFE_RELEASE( p ); }


	void safeAddRef() { LN_SAFE_ADDREF(p); }
	void safeRelease() { LN_SAFE_RELEASE(p); }

private:

    T* p;
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