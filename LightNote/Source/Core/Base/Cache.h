//==============================================================================
// Cache 
//------------------------------------------------------------------------------
///**
//  @file       Cache.h
//  @brief      Cache
//*/
//==============================================================================

#pragma once

#include <iostream>
#include <map>
#include "Container/Stack.h"
#include "SharingKey.h"
#include "../Threading/Mutex.h"

namespace LNote
{
namespace Core
{
namespace Base
{
class CacheManager;

#if 0

#define LN_CACHE_REFOBJ_METHODS \
    virtual lnS32 getRefCount() const		{ return Base::ReferenceObject::getRefCount(); } \
    virtual lnS32 addRef()					{ return Base::ReferenceObject::addRef(); } \
	virtual lnS32 release()					{ return Base::CacheObject::release(); } \
	virtual lnS32 Release()					{ return Base::CacheObject::release(); } \
    virtual void setUserData(void* data)	{ Base::ReferenceObject::setUserData(data); } \
    virtual void* getUserData()				{ return Base::ReferenceObject::getUserData(); } \
	virtual void* getTypeUserData()			{ return Base::ReferenceObject::getTypeUserData(); }

//==============================================================================
// CacheObject
//------------------------------------------------------------------------------
///**
//  @brief      キャッシュオブジェクト
//	@note
//				リソースのロードが終わった時点等で 
//				CacheManager::registerCachedObject() で登録すること。
//				登録されたオブジェクトは参照カウントが 0 になったとき、
//				解放待ちリストへ追加される。
//				
//				getCacheMemorySize() は return 0 可。
//				ただし、メモリ計算の対象にはならなくなる。
//
//	@attention
//				ファイルを開いたまま等で、キャッシュに入れないこと。
//				(CacheObject::release() を呼ばないこと)
//				キャッシュから削除されるまでずっと開かれたままになってしまう。
//*/
//==============================================================================
class CacheObject
	: public ReferenceObject
{
public:
	CacheObject();
	virtual ~CacheObject();

public:
	virtual size_t	getCacheMemorySize() const { return 0; }
	virtual lnS32	release();

private:
	friend class CacheManager;
	CacheManager*	mCacheManager;
	SharingKey		mSharingKey;	///< CacheObject 側で持っている方が、少しシンプルに書ける
	bool			mInCacheList;
	bool			mIsStockObject;
};

#endif

//==============================================================================
// ICacheObject
//------------------------------------------------------------------------------
///**
//  @brief      キャッシュオブジェクト
//	@note
//				リソースのロードが終わった時点等で 
//				CacheManager::registerCachedObject() で登録すること。
//				登録されたオブジェクトは参照カウントが 0 になったとき、
//				解放待ちリストへ追加される。
//				
//				getCacheMemorySize() は return 0 可。
//				ただし、メモリ計算の対象にはならなくなる。
//
//	@attention
//				ファイルを開いたまま等で、キャッシュに入れないこと。
//				(CacheObject::release() を呼ばないこと)
//				キャッシュから削除されるまでずっと開かれたままになってしまう。
//*/
//==============================================================================
class ICacheObject
{
protected:
	friend class CacheManager;
	struct CacheObjectInfo
	{
		CacheManager*	mCacheManager;
		SharingKey		mSharingKey;	///< CacheObject 側で持っている方が、少しシンプルに書ける
		size_t			mCacheMemorySize;
		bool			mInCacheList;
		bool			mIsStockObject;

		CacheObjectInfo();
		~CacheObjectInfo();
	};

public:
	ICacheObject() {}
	virtual ~ICacheObject() {}

public:
	virtual CacheObjectInfo&	getCacheObjectInfo() = 0;
	virtual lnS32				addRef() = 0;
	virtual lnS32				release() = 0;
};

#define LN_CACHE_OBJECT_DECL \
protected: \
	CacheObjectInfo mCacheObjectInfo; \
public: \
	virtual CacheObjectInfo& getCacheObjectInfo() { return mCacheObjectInfo; } \
	lnS32 addRef() { return ReferenceObject::addRef(); } \
	lnS32 release() \
	{ \
		if (mCacheObjectInfo.mCacheManager == NULL || mCacheObjectInfo.mIsStockObject) \
		{ \
			return ReferenceObject::release(); \
		} \
		lnS32 count = mReferenceCount.decrement(); \
		LN_ERR2_ASSERT(count >= 0); \
		if (count <= 0) \
		{ \
			mCacheObjectInfo.mCacheManager->addCacheUnusedList(this); \
		} \
		return count; \
	}

//==============================================================================
// CacheManager
//------------------------------------------------------------------------------
///**
//  @brief      キャッシュオブジェクトの管理クラス
//*/
//==============================================================================
class CacheManager
	: public ReferenceObject
{
public:
	CacheManager();
	virtual ~CacheManager();

public:

	/// 初期化(capacityCount:キャッシュリストのサイズ maxMemorySize:最大バイト数(0で考慮しない))
	void initialize( int capacityCount, size_t maxMemorySize );

	/// 終了処理 (これ以降キャッシュにオブジェクトを追加しないようにする)
	void finalize();

	/// オブジェクトをストックとして追加する
	void registerStockObject(const SharingKey& key, ICacheObject* obj);

	/// オブジェクトをキャッシュ管理に追加する (すぐにキャッシュリストに入れるわけではない)
	void registerCachedObject(const SharingKey& key, ICacheObject* obj);

	/// キーに一致するオブジェクトを検索する (参照カウントをインクリメントして返す。キャッシュリストに無くても、registerCachedObject() されたオブジェクトを返す)
	ICacheObject* findCacheAddRef(const SharingKey& key);

	/// キャッシュリストのクリア
	void clearCacheList();

	/// ストックリストのクリア
	void clearStockList();

public:
	void addCacheUnusedList(ICacheObject* obj);
	void deleteCachedObject(ICacheObject* obj);

private:
	class CacheUnusedList;
	typedef std::map<SharingKey, ICacheObject*>		CacheUsingMap;
	typedef std::pair<SharingKey, ICacheObject*>	CacheUsingPair;
	typedef std::vector<ICacheObject*>				CacheObjectArray;

	CacheUsingMap		mCacheUsingMap;
    CacheUnusedList*	mCacheUnusedList;
	CacheObjectArray	mStockObjectArray;
    Threading::Mutex	mMutex;
	size_t				mFreeMemorySize;
	size_t				mMaxMemorySize;
	bool				mClearingCacheList;
};

} // namespace Base
} // namespace Core
} // namespace LNote
