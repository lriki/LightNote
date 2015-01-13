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
//  @brief      2文探索で要素を検索するためのコレクション
//	@note
//				想定される最大要素数が少なく(目安100以下) 、 map を使うよりも
//				検索が高速である場合に使用する。
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

	/// 追加 (キーが既に存在する場合は値を上書きする)
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

	/// 検索 (見つからなかった場合は NULL を返す)
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
