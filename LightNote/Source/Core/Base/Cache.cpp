//==============================================================================
// Cache 
//==============================================================================

#include "stdafx.h"
#include "NonCopyable.h"
#include "Cache.h"

namespace LNote
{
namespace Core
{
namespace Base
{

//==============================================================================
// CacheManager::CacheUnusedList
//------------------------------------------------------------------------------
///**
//  @brief      キャッシュシステムのうち、現在使用されていないオブジェクトを管理するクラス
//*/
//==============================================================================
class CacheManager::CacheUnusedList
{
public:
	CacheUnusedList( CacheManager* manager )
        : mManager	( manager )
		, mNodes    ( NULL )
        , mCapacity ( 0 )
        , mSize     ( 0 )
    {}

    ~CacheUnusedList()
    {
        clear();
        SAFE_DELETE_ARRAY( mNodes );
    }

public:

	/// 最大要素数の取得
    int getCapacity() const { return mCapacity - 1; }

	/// 領域の確保
    void setCapacity( int capacityCount )
    {
        clear();

        SAFE_DELETE_ARRAY( mNodes );

        if ( capacityCount == 0 )
        {
            mCapacity = 0;
            return;
        }

        mCapacity = capacityCount + 1;

        mNodes = LN_NEW Node[ mCapacity + 1 ];


        // 空き番号スタック
	    mIndexStack.setCapacity( mCapacity );
	    for ( int i = 0; i < mCapacity; ++i )
        {
		    mIndexStack.push( i );
	    }

        // mCapacity 番は最初の要素の前かつ最後の要素の後にあるダミー要素
		mNodes[ mCapacity ].Next = mNodes[ mCapacity ].Prev = mCapacity;
    }

	/// クリア (全 delete 呼び出し)
    void clear()
    {
        if ( mNodes && mCapacity > 0 )
        {
		    Node* node = &mNodes[ mCapacity ];	// ダミーノード
		    lnU32 index = node->Next;             // ダミーノードの次が開始インデックス
            int i = 0;

		    while ( true )
		    {
			    // ダミーノードを指している場合は終了
			    if ( index == mCapacity )
			    {
				    break;
			    }
                // 削除
				mManager->deleteCachedObject( mNodes[ index ].Value );
                // 次へ
                index = mNodes[ index ].Next;

                ++i;
		    }

            mNodeMap.clear();

            // 空き番号スタック
	        mIndexStack.clear();
	        for ( int i = 0; i < mCapacity; ++i )
            {
		        mIndexStack.push( i );
	        }

            // ノードは new とかしてるわけじゃないので、ダミーの前後だけ付け変えておけばリストは空の状態になる
            mNodes[ mCapacity ].Next = mNodes[ mCapacity ].Prev = mCapacity;
            mSize = 0;
        }
    }

	/// オブジェクトを追加する (RefCount++)
	void addObject(ICacheObject* obj)
    {
        if ( obj && !obj->getCacheObjectInfo().mSharingKey.isEmpty() && mCapacity > 0 )
        {
            // すでに追加されているか調べる
			NodeMap::iterator itr = mNodeMap.find( obj->getCacheObjectInfo().mSharingKey );
            if ( itr != mNodeMap.end() )
            {
                return;
            }

            // キャッシュを調べて古いものがあれば削除
            _checkList();

            int idx = mIndexStack.top();
            mIndexStack.pop();

            Node* dummy     = &mNodes[ mCapacity ];	    // ダミーノード
            Node* prev      = &mNodes[ dummy->Prev ];	// ダミーのひとつ前 ( 最後 )
            Node* new_node  = &mNodes[ idx ];           // その間に入れるノード

            // new_node をリストの末尾に追加する
            new_node->Prev = dummy->Prev;
            new_node->Next = mCapacity;
            dummy->Prev = idx;
            prev->Next = idx;

            new_node->Value = obj;

            mNodeMap.insert( std::pair< SharingKey, Node* >( obj->getCacheObjectInfo().mSharingKey, new_node ) );

            // 削除待ち中
            obj->getCacheObjectInfo().mInCacheList = true;

            ++mSize;
        }
    }

	/// キーに一致するオブジェクトの検索 (返されるオブジェクトの参照カウントは常に 0)             
    ICacheObject* findObject( const SharingKey& key )
    {
        if ( mCapacity == 0 ) return NULL;

		NodeMap::iterator it = mNodeMap.find( key );
        if ( it != mNodeMap.end() )
        {
            // 削除待ちリストから外す
            Node* n = it->second;
            lnU32 idx = mNodes[ n->Next ].Prev;   // 自分のインデックス
            mNodes[ n->Next ].Prev = n->Prev;
            mNodes[ n->Prev ].Next = n->Next;

            // 空きインデックスを戻す
            mIndexStack.push( idx );

            ICacheObject* v = it->second->Value;
            
            mNodeMap.erase( it );

            --mSize;

            // 使用中にする
            v->getCacheObjectInfo().mInCacheList = false;

            return v;
        }
        return NULL;
    }

	/// キャッシュの内容を出力する
    void dump( FILE* stream );

private:

	/// キャッシュのリストを調べて、古いもの(先頭)がある場合は削除
    void _checkList()
    {
        if ( mSize >= mCapacity - 1 )
        {
            Node* dummy     = &mNodes[ mCapacity ];	    // ダミーノード
			lnU32 front_index = dummy->Next;
			Node* front     = &mNodes[ front_index ];	// 先頭のノード取り出し

            // front をリストから外す
            dummy->Next = front->Next;
            mNodes[ front->Next ].Prev = mCapacity;

            // 空いたインデックスをスタックに積む
            mIndexStack.push( front_index );

            // Map からも削除
			mNodeMap.erase( front->Value->getCacheObjectInfo().mSharingKey );

            --mSize;
            
			// ないと思うけど、一応 front がダミーじゃないかチェックしてから解放
			if ( front != dummy )
			{
				mManager->deleteCachedObject( front->Value );
			}            
        }
    }

private:

    struct Node
	{
		int             Next;
		int             Prev;
        ICacheObject*	Value;
	};

	typedef std::map<SharingKey, Node*>    NodeMap;

private:
	CacheManager*	mManager;
    Node*           mNodes;         ///< 解放待ちリスト
    int             mCapacity;      ///< 最大要素数
    int             mSize;          ///< 現在の要素数
    Stack<int>		mIndexStack;    ///< 空き番号スタック
    NodeMap         mNodeMap;       ///< キーとノードの対応表
};

//==============================================================================
// CacheObject
//==============================================================================
#if 0
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	CacheObject::CacheObject()
		: mCacheManager		( NULL )
		, mInCacheList		( false )
		, mIsStockObject	( false )
	{}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	CacheObject::~CacheObject()
	{
		LN_SAFE_RELEASE( mCacheManager );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	lnS32 CacheObject::release()
	{
		// キャッシュ管理されていないかストックオブジェクトの場合は普通に release
		if ( mCacheManager == NULL || mIsStockObject )
		{
			return ReferenceObject::release();
		}

		lnS32 count = mReferenceCount.decrement();
		LN_ERR2_ASSERT( count >= 0 );
		if ( count <= 0 )
		{
			mCacheManager->addCacheUnusedList( this );
		}
		return count;
	}
#endif

//==============================================================================
// ICacheObject::CacheObjectInfo
//==============================================================================

	ICacheObject::CacheObjectInfo::CacheObjectInfo()
		: mCacheManager(NULL)
		, mCacheMemorySize(0)
		, mInCacheList(false)
		, mIsStockObject(false)
	{
	}

	ICacheObject::CacheObjectInfo::~CacheObjectInfo()
	{
		LN_SAFE_RELEASE(mCacheManager);
	}

//==============================================================================
// CacheManager
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	CacheManager::CacheManager()
		: mCacheUnusedList		( NULL )
		, mFreeMemorySize		( 0 )
		, mMaxMemorySize		( 0 )
		, mClearingCacheList	( false )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	CacheManager::~CacheManager()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void CacheManager::initialize( int capacityCount, size_t maxMemorySize )
	{
		LN_ERR2_ASSERT( capacityCount >= 0 );

		mCacheUnusedList = LN_NEW CacheUnusedList( this );
		mCacheUnusedList->setCapacity( capacityCount );
		mMaxMemorySize = ( capacityCount == 0 ) ? 0 : maxMemorySize;

		// TODO:
		LN_ERR2_ASSERT( mMaxMemorySize == 0 && "メモリサイズは未実装" );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void CacheManager::finalize()
	{
		clearStockList();
		clearCacheList();
		SAFE_DELETE( mCacheUnusedList );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void CacheManager::registerStockObject( const SharingKey& key, ICacheObject* obj )
	{
		registerCachedObject( key, obj );

		Threading::ScopedLock lock( mMutex );
		mStockObjectArray.push_back( obj );
		obj->addRef();
		obj->getCacheObjectInfo().mIsStockObject = true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void CacheManager::registerCachedObject( const SharingKey& key, ICacheObject* obj )
	{
		Threading::ScopedLock lock( mMutex );

		LN_ERR2_ASSERT( !key.isEmpty() );
		LN_ERR2_ASSERT( obj->getCacheObjectInfo().mSharingKey.isEmpty() );
		if ( mCacheUnusedList == NULL ) return;

        mCacheUsingMap.insert( CacheUsingPair( key, obj ) );
		obj->getCacheObjectInfo().mCacheManager = this;
		obj->getCacheObjectInfo().mCacheManager->addRef();
		obj->getCacheObjectInfo().mSharingKey = key;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ICacheObject* CacheManager::findCacheAddRef( const SharingKey& key )
	{
		ICacheObject* obj = NULL;
		Threading::ScopedLock lock( mMutex );

		if ( !key.isEmpty() && mCacheUnusedList )
        {
            // まずは今使ってる物を検索
            CacheUsingMap::iterator itr = mCacheUsingMap.find( key );
            if ( itr != mCacheUsingMap.end() )
            {
                obj = itr->second;
                obj->addRef();
            }
            // キャッシングする場合は削除待ちリストも検索
			else if ( mCacheUnusedList->getCapacity() > 0 )
            {
                obj = mCacheUnusedList->findObject( key );
                if ( obj )
                {
                    // 使用中オブジェクトのマップに入れておく
					mCacheUsingMap.insert(CacheUsingPair(obj->getCacheObjectInfo().mSharingKey, obj));
					obj->addRef();
                }
            }
        }

		return obj;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void CacheManager::clearCacheList()
	{
		Threading::ScopedLock lock( mMutex );
		mClearingCacheList = true;
		if ( mCacheUnusedList )
		{
			mCacheUnusedList->clear();
		}
		mClearingCacheList = false;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void CacheManager::clearStockList()
	{
		Threading::ScopedLock lock( mMutex );
		mClearingCacheList = true;
		ln_foreach( ICacheObject* obj, mStockObjectArray )
		{
			obj->release();
		}
		mStockObjectArray.clear();
		mClearingCacheList = false;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void CacheManager::addCacheUnusedList( ICacheObject* obj )
	{
		LN_ERR2_ASSERT( obj );

		bool addedObj = false;
		if ( !mClearingCacheList )
		{
			Threading::ScopedLock lock( mMutex );
			if ( mCacheUnusedList != NULL ){
				// 使用中の場合、使用中オブジェクト対応表から外して、
				// キャッシングする場合は解放待ちリストに追加する
				if (!obj->getCacheObjectInfo().mInCacheList) {
					// メモリサイズチェック
					size_t size = obj->getCacheObjectInfo().mCacheMemorySize;
					if ( mMaxMemorySize == 0 || 
						 (size > 0 && size <= mMaxMemorySize) )	// TODO: メモリサイズ対応の時、この条件間違ってないかチェック必要かも
					{
						if ( mCacheUnusedList->getCapacity() > 0 )
						{
							mCacheUsingMap.erase(obj->getCacheObjectInfo().mSharingKey);
							mCacheUnusedList->addObject( obj );
							mFreeMemorySize += size;
							addedObj = true;
						}
					}
				}
			}
		}

		// 追加できなかったら削除する
		if ( !addedObj ) {
			deleteCachedObject( obj );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void CacheManager::deleteCachedObject( ICacheObject* obj )
	{
		LN_ERR2_ASSERT( obj );
		LN_ERR2_ASSERT(obj->getCacheObjectInfo().mCacheManager);

		mFreeMemorySize -= obj->getCacheObjectInfo().mCacheMemorySize;
		delete obj;

		LN_ERR2_ASSERT( mFreeMemorySize >= 0 );
	}

} // namespace Base
} // namespace Core
} // namespace LNote
