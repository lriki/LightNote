//==============================================================================
// ReferenceObjectArray 
//------------------------------------------------------------------------------
///**
//  @file       ReferenceObjectArray.h
//  @brief      ReferenceObjectArray
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <vector>
#include <algorithm>
#include "ReferenceObject.h"

namespace LNote
{
namespace Core
{
namespace Base
{

//==============================================================================
// ■ ReferenceObjectArray
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
template<class T>
class ReferenceObjectArray
{
public:
	typedef std::vector<T>	ObjectArray;
	
private:
	ObjectArray	mObjectArray;
	ObjectArray	mRegisterList;
	ObjectArray	mUnregisterList;

public:
	ReferenceObjectArray() {}
	~ReferenceObjectArray() { clear(); }

public:

	// 追加
	void registerObject( T obj )
	{
		LN_ERR2_ASSERT( obj );
		LN_ERR2_ASSERT( std::find( mRegisterList.begin(), mRegisterList.end(), obj ) == mRegisterList.end() );
		LN_ERR2_ASSERT( std::find( mObjectArray.begin(), mObjectArray.end(), obj ) == mObjectArray.end() );
		mRegisterList.push_back( obj );
		obj->addRef();
	}

	// 削除
	void unregisterObject( T obj )
	{
		LN_ERR2_ASSERT( obj );

		// 登録リストに入ってたらこの時点で削除してしまう
		typename ObjectArray::iterator itr = std::find( mRegisterList.begin(), mRegisterList.end(), obj );
		if ( itr != mRegisterList.end() )
		{
			mRegisterList.erase( itr );
			return;
		}
		
		LN_ERR2_ASSERT( std::find( mUnregisterList.begin(), mUnregisterList.end(), obj ) == mUnregisterList.end() );
		LN_ERR2_ASSERT( std::find( mObjectArray.begin(), mObjectArray.end(), obj ) != mObjectArray.end() );
		mUnregisterList.push_back( obj );
		obj->addRef();
	}

	/// (同期フェーズ)
	void commit()
	{
		typename ObjectArray::iterator itr, end;

		// 追加
		if ( !mRegisterList.empty() )
		{
			itr = mRegisterList.begin();
			end = mRegisterList.end();
			for ( ; itr != end; ++itr )
			{
				mObjectArray.push_back( *itr );
				// 参照カウントは mRegisterList から外す分と mObjectArray に
				// 追加する分で ±0 なので操作はしない
			}
			mRegisterList.clear();
		}

		// 削除
		if ( !mUnregisterList.empty() )
		{
			itr = mUnregisterList.begin();
			end = mUnregisterList.end();
			for ( ; itr != end; ++itr )
			{
				typename ObjectArray::iterator pos = std::find( mObjectArray.begin(), mObjectArray.end(), (*itr) );
				if ( pos != mObjectArray.end() )
				{
					(*pos)->release();	// mUnregisterList から外す分
					(*pos)->release();	// mRegisterList から外す分
					mObjectArray.erase( pos );
				}
			}
			mUnregisterList.clear();
		}

		// このクラスからしか参照していないものを削除
		itr = mObjectArray.begin();
		end = mObjectArray.end();
		for ( ; itr != end; )
		{
			if ( (*itr)->getRefCount() == 1 )
			{
				(*itr)->release();
				itr = mObjectArray.erase( itr );
				end = mObjectArray.end();
			}
			else
			{
				++itr;
			}
		}
	}

	/// 追加予約中配列取得
	ObjectArray& getRegistOrderObjectArray() { return mRegisterList; }

	/// 配列取得
	ObjectArray& getObjectArray() { return mObjectArray; }

	/// すべてクリア (終了処理用。clear() を呼ぶスレッド以外は停止していること)
	void clear()
	{
		typename ObjectArray::iterator itr, end;
		
		itr = mRegisterList.begin();
		end = mRegisterList.end();
		for ( ; itr != end; ++itr ) (*itr)->release();
		mRegisterList.clear();

		itr = mUnregisterList.begin();
		end = mUnregisterList.end();
		for ( ; itr != end; ++itr ) (*itr)->release();
		mUnregisterList.clear();

		itr = mObjectArray.begin();
		end = mObjectArray.end();
		for ( ; itr != end; ++itr ) (*itr)->release();
		mObjectArray.clear();
	}
};

} // namespace Base
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================