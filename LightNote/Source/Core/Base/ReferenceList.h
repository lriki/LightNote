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

	/// �v�f�̐����擾����
	virtual int getCount() const = 0;

	/// �w��C���f�b�N�X�ɗv�f���i�[����
	virtual void setAt(int index, TBasePtr item) = 0;

	/// �w��C���f�b�N�X�̗v�f���擾����
	virtual TBasePtr getAt(int index) = 0;

	/// �v�f�𖖔��ɒǉ�����
	virtual void add(const TBasePtr item) = 0;

	/// �S�Ă̗v�f���폜����
	virtual void clear() = 0;

	/// �w�肵���C���f�b�N�X�̈ʒu�ɗv�f��}������
	virtual void insert(int index, const TBasePtr item) = 0;

	/// item �ƈ�v����ŏ��̗v�f���폜����
	virtual void remove(const TBasePtr item) = 0;

	/// item �ƈ�v����S�Ă̗v�f���폜����
	virtual void removeAll(const TBasePtr item) = 0;

	/// �w�肵���C���f�b�N�X�̗v�f���폜����
	virtual void removeAt(int index) = 0;
};

//==============================================================================
// ReferenceList
//------------------------------------------------------------------------------
///**
//  @brief		�Q�ƃJ�E���g�t���̃��X�g
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

	/// �v�f�̐����擾����
	virtual int getCount() const
	{
		return std::vector<T>::size();
	}

	/// �w��C���f�b�N�X�ɗv�f���i�[����
	virtual void setAt(int index, TBasePtr item)
	{
		(*this)[index] = static_cast<T>(item);
	}

	/// �w��C���f�b�N�X�̗v�f���擾����
	virtual TBasePtr getAt(int index)
	{
		return std::vector<T>::at(index);
	}

	/// �v�f�𖖔��ɒǉ�����
	virtual void add(const TBasePtr item)
	{
		TBasePtr b = const_cast<TBasePtr>(item);
		T t = static_cast<T>(b);
		onAdding(t);
		std::vector<T>::push_back(t);
		onAdded(t);
	}

	/// �S�Ă̗v�f���폜����
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

	/// �w�肵���C���f�b�N�X�̈ʒu�ɗv�f��}������
	virtual void insert(int index, const TBasePtr item)
	{
		TBasePtr b = const_cast<TBasePtr>(item);
		T t = static_cast<T>(b);
		onAdding(t);
		std::vector<T>::insert(std::vector<T>::begin() + index, t);
		onAdded(t);
	}

	/// item �ƈ�v����ŏ��̗v�f���폜����
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

	/// item �ƈ�v����S�Ă̗v�f���폜����
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

	/// �w�肵���C���f�b�N�X�̗v�f���폜����
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
	//Threading::Atomic		mReferenceCount;	///< �Q�ƃJ�E���g
	//void*					mUserData;
};

} // namespace Base
} // namespace Core
} // namespace LNote
