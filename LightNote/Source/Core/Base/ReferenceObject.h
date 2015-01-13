//==============================================================================
// ReferenceObject 
//------------------------------------------------------------------------------
///**
//  @file       ReferenceObject.h
//  @brief      ReferenceObject
//*/
//==============================================================================

#pragma once

#include "NonCopyable.h"
#include "Interface.h"
#include "../Threading/Atomic.h"

namespace LNote
{
namespace Core
{
namespace Base
{

//==============================================================================
// ReferenceObject
//------------------------------------------------------------------------------
///**
//  @brief      参照カウントを持つクラスの基底
//*/
//==============================================================================
class ReferenceObject
    : private NonCopyable
	, public Interface
{
public:

	/// コンストラクタ
	ReferenceObject()
		: mReferenceCount( 1 )
		, mUserData(NULL)
	{}

protected:

	/// デストラクタ
	virtual ~ReferenceObject() {
		if (mInterfaceDeleting != NULL)
			mInterfaceDeleting(this);
	}

public:

	/// 参照カウントの取得
	virtual lnS32 getRefCount() const
    {
        return mReferenceCount.get();
    }

	/// 参照カウントをひとつ増やす
	virtual lnS32 addRef()
    {
        return mReferenceCount.increment();
    }

	/// 参照カウントをひとつ減らす
	virtual lnS32 release()
	{
        int count = mReferenceCount.decrement();
		if ( count <= 0 )
		{
			//if (mInterfaceDeleting != NULL)
			//	mInterfaceDeleting(this);
			 delete this;
		}
        return count;
	}

	/// SAFE_RELEASE() マクロ対応版 release()
	virtual lnS32 Release()
	{
        int count = mReferenceCount.decrement();
		if ( count <= 0 )
		{
			//if (mInterfaceDeleting != NULL)
			//	mInterfaceDeleting(this);
			 delete this;
		}
        return count;
    }


	virtual void setUserData(void* data)
	{
		mUserData = data;
	}

	virtual void* getUserData()
	{
		return mUserData;
	}

public:
	virtual void* getTypeUserData() { return NULL; }

protected:

    Threading::Atomic		mReferenceCount;	///< 参照カウント
	void*					mUserData;

};

/// ReferenceObject の継承先で定義するべき関数をまとめたマクロ
#define LN_REFOBJ_METHODS \
    virtual lnS32 getRefCount() const		{ return ReferenceObject::getRefCount(); } \
	virtual lnS32 addRef()					{ return ReferenceObject::addRef(); } \
    virtual lnS32 release()					{ return ReferenceObject::release(); } \
    virtual void setUserData(void* data)	{ ReferenceObject::setUserData(data); } \
    virtual void* getUserData()				{ return ReferenceObject::getUserData(); }
	//virtual void* getTypeUserData()			{ return ReferenceObject::getTypeUserData(); }

/// a に b を格納するユーティリティ
#define LN_REFOBJ_SET( a, b ) \
{ \
    LN_SAFE_ADDREF( b ); \
    LN_SAFE_RELEASE( a ); \
    a = b; \
}

/// TypeUserData の宣言
#define LN_TYPE_INFO_ACCESS_DECL \
public: \
	static void* mTypeUserData; \
public: \
	virtual void* getTypeUserData() { return mTypeUserData; }
//void SetTypeUserData(void* data) { mTypeUserData = data; }

/// TypeUserData の実装
#define LN_TYPE_INFO_ACCESS_IMPL(classType) \
	void* classType::mTypeUserData = NULL;

} // namespace Base
} // namespace Core
} // namespace LNote
