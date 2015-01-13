//==============================================================================
// STLUtil 
//------------------------------------------------------------------------------
///**
//  @file       STLUtil.h
//  @brief      STLUtil
//*/
//==============================================================================

#pragma once

#include <vector>
#include <list>
#include <map>
#include <algorithm>

namespace LNote
{
namespace Core
{
namespace Base
{

//==============================================================================
// STLUtils
//------------------------------------------------------------------------------
///**
// @brief       
//*/
//==============================================================================
class STLUtils
{
public:

	/// vector から等しい要素をすべて削除する
	template < class T > static bool remove(std::vector<T>& vec, const T& value)
	{
		size_t n = vec.size();
		vec.erase(std::remove(vec.begin(), vec.end(), value), vec.end());
		return n != vec.size();
	}

	/// vector から条件と等しい要素をすべて削除する
    template < class VTYPE_,class PR_ > static void remove_if( VTYPE_& vector_, const PR_& pred_ )
    {
        vector_.erase( std::remove_if( vector_.begin(), vector_.end(), pred_ ), vector_.end() );
    }

	/// std::vector に要素が含まれているかチェックする
	template <class T> static bool contains( const std::vector<T>& vec, const T& value )
    {
		return std::find( vec.begin(), vec.end(), value ) != vec.end();
    }

	/// std::list に要素が含まれているかチェックする
	template <class T> static bool contains(const std::list<T>& list, const T& value)
	{
		return std::find(list.begin(), list.end(), value) != list.end();
	}

	/// std::map に要素が含まれているかチェックする
	template <class K, class V> static bool contains( const std::map<K, V>& map, const K& key )
    {
		std::map<K, V>::const_iterator itr = map.find( key );
		return (itr != map.end());
    }

};

} // namespace Base
} // namespace Core
} // namespace LNote
