//==============================================================================
// SortedArray 
//------------------------------------------------------------------------------
///**
//  @file       SortedArray.h
//  @brief      SortedArray
//*/
//==============================================================================

#pragma once

#include <vector>
#include <algorithm>

namespace LNote
{
namespace Core
{
namespace Base
{

//==============================================================================
// SortedArray
//------------------------------------------------------------------------------
///**
//  @brief      2���T���ŗv�f���������邽�߂̃R���N�V����
//	@note
//				�z�肳���ő�v�f�������Ȃ�(�ڈ�100�ȉ�) �A map ���g������
//				�����������ł���ꍇ�Ɏg�p����B
//*/
//==============================================================================
template<class TKey, class TValue>
class SortedArray
{
private:

	struct Pair
	{
		TKey	Key;
		TValue	Value;
	};

	struct PairCompare
	{
		bool operator()(const Pair* left, const Pair* right)  {
			return left->Key < right->Key;
		}
		bool operator()(const Pair* left, const TKey& key)  {
			return left->Key < key;
		}
		bool operator()(const TKey& key, const Pair* right)  {
			return key < right->Key;
		}
	};

	typedef std::vector<Pair*>	PairArray;
	PairArray	mPairArray;

	Pair* findPair(const TKey& key)
	{
		PairCompare pc;
		PairArray::iterator itr = std::lower_bound(mPairArray.begin(), mPairArray.end(), key, pc);
		if (itr != mPairArray.end() && pc(key, *itr))
			return *itr;
		return NULL;
	}

public:
	SortedArray() {}
	~SortedArray() {}

public:

	/// �ǉ� (�L�[�����ɑ��݂���ꍇ�͒l���㏑������)
	void add(const TKey& key, const TValue& value)
	{
		Pair* pair = findPair(key);
		if (pair != NULL) {
			pair->Value = value;
		}
		else {
			pair = LN_NEW Pair();
			pair->Key = key;
			pair->Value = value;
			mPairArray.push_back(pair);
			std::sort(mPairArray.begin(), mPairArray.end(), PairCompare());
		}
	}

	/// ���� (������Ȃ������ꍇ�� NULL ��Ԃ�)
	TValue* find(const TKey& key)
	{
		Pair* pair = findPair(key);
		if (pair != NULL)
			return NULL;
		return &pair->Value;
	}
};

} // namespace Base
} // namespace Core
} // namespace LNote
