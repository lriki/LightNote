//==============================================================================
// ReferenceList 
//------------------------------------------------------------------------------
///**
//  @file       ReferenceList.h
//  @brief      ReferenceList
//*/
//==============================================================================

#pragma once

#include <vector>
#include "STLUtils.h"
#include "Interface.h"
#include "ReferenceObject.h"

namespace LNote
{
namespace Core
{
namespace Base
{

template<typename TBasePtr>
class IList
	: public Interface
{
public:
	virtual ~IList() {}

public:

	/// 要素の数を取得する
	virtual int getCount() const = 0;

	/// 指定インデックスに要素を格納する
	virtual void setAt(int index, TBasePtr item) = 0;

	/// 指定インデックスの要素を取得する
	virtual TBasePtr getAt(int index) = 0;

	/// 要素を末尾に追加する
	virtual void add(const TBasePtr item) = 0;

	/// 全ての要素を削除する
	virtual void clear() = 0;

	/// 指定したインデックスの位置に要素を挿入する
	virtual void insert(int index, const TBasePtr item) = 0;

	/// item と一致する最初の要素を削除する
	virtual void remove(const TBasePtr item) = 0;

	/// item と一致する全ての要素を削除する
	virtual void removeAll(const TBasePtr item) = 0;

	/// 指定したインデックスの要素を削除する
	virtual void removeAt(int index) = 0;
};

//==============================================================================
// ReferenceList
//------------------------------------------------------------------------------
///**
//  @brief		参照カウント付きのリスト
//*/
//==============================================================================
template<typename T, typename TBasePtr = ReferenceObject*>
class ReferenceList
	: public std::vector<T>
	, public ReferenceObject
	//, public IList<TBasePtr>
{
public:
	ReferenceList()
		//: mReferenceCount(1)
		//, mUserData(NULL)
	{}

	virtual ~ReferenceList() 
	{

		//if (mInterfaceDeleting != NULL)
		//	mInterfaceDeleting(this);
	}

public:

	/// 要素の数を取得する
	virtual int getCount() const
	{
		return std::vector<T>::size();
	}

	/// 指定インデックスに要素を格納する
	virtual void setAt(int index, TBasePtr item)
	{
		(*this)[index] = static_cast<T>(item);
	}

	/// 指定インデックスの要素を取得する
	virtual TBasePtr getAt(int index)
	{
		return std::vector<T>::at(index);
	}

	/// 要素を末尾に追加する
	virtual void add(const TBasePtr item)
	{
		TBasePtr b = const_cast<TBasePtr>(item);
		T t = static_cast<T>(b);
		onAdding(t);
		std::vector<T>::push_back(t);
		onAdded(t);
	}

	/// 全ての要素を削除する
	virtual void clear()
	{
		ln_foreach(T& item, *this)
		{
			onRemoving(item);
		}
		ln_foreach(T& item, *this)
		{
			onRemoved(item);
		}
		std::vector<T>::clear();
	}

	/// 指定したインデックスの位置に要素を挿入する
	virtual void insert(int index, const TBasePtr item)
	{
		TBasePtr b = const_cast<TBasePtr>(item);
		T t = static_cast<T>(b);
		onAdding(t);
		std::vector<T>::insert(std::vector<T>::begin() + index, t);
		onAdded(t);
	}

	/// item と一致する最初の要素を削除する
	virtual void remove(const TBasePtr item)
	{
		std::vector<T>::iterator itr = std::vector<T>::begin();
		std::vector<T>::iterator end = std::vector<T>::end();
		for (; itr != end; ++itr)
		{
			if (*itr == item) {
				T& item = *itr;
				onRemoving(item);
				std::vector<T>::erase(itr);
				onRemoved(item);
				return;
			}
		}
	}

	/// item と一致する全ての要素を削除する
	virtual void removeAll(const TBasePtr item)
	{
		//STLUtils::remove(*this, item);
		std::vector<T>::iterator itr = std::vector<T>::begin();
		std::vector<T>::iterator end = std::vector<T>::end();
		for (; itr != end; ++itr)
		{
			if (*itr == item) {
				T& item = *itr;
				onRemoving(item);
				itr = std::vector<T>::erase(itr);
				end = std::vector<T>::end();
				onRemoved(item);
			}
		}
	}

	/// 指定したインデックスの要素を削除する
	virtual void removeAt(int index)
	{
		T& item = *(std::vector<T>::begin() + index);
		onRemoving(item);
		std::vector<T>::erase(std::vector<T>::begin() + index);
		onRemoved(item);
	}

protected:

	virtual void onAdding(T& item) {}
	virtual void onAdded(T& item) {}
	virtual void onRemoving(T& item) {}
	virtual void onRemoved(T& item) {}

public:
	//virtual lnS32 getRefCount() const
	//{
	//	return mReferenceCount.get();
	//}

	//virtual lnS32 addRef()
	//{
	//	return mReferenceCount.increment();
	//}

	//virtual lnS32 release()
	//{
	//	int count = mReferenceCount.decrement();
	//	if (count <= 0)
	//	{
	//		//if (mInterfaceDeleting != NULL)
	//		//	mInterfaceDeleting(this);
	//		delete this;
	//	}
	//	return count;
	//}

	//virtual void setUserData(void* data)
	//{
	//	mUserData = data;
	//}

	//virtual void* getUserData()
	//{
	//	return mUserData;
	//}

	//virtual void* getTypeUserData()
	//{
	//	return NULL;
	//}

private:
	//Threading::Atomic		mReferenceCount;	///< 参照カウント
	//void*					mUserData;
};

} // namespace Base
} // namespace Core
} // namespace LNote
