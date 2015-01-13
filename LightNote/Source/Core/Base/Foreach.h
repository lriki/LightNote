//==============================================================================
// Foreach 
//------------------------------------------------------------------------------
///**
//  @file       Foreach.h
//  @brief      Foreach
//*/
//==============================================================================

/*	ln_foreach マクロ
*
*	BOOST C++ の BOOST_FOREACH のミニマル実装です。
*	STL の 各種コンテナの単純なイテレートを簡略化します。
*	例)
		vector<int> vec;
		vec.push_back(1);
		vec.push_back(2);
		vector<int>::iterator itr = vec.begin();
		vector<int>::iterator end = vec.end();
		for (; itr != end; itr++) {
			printf("%d", *itr);
		}
		↓↓↓↓↓
		vector<int> vec;
		vec.push_back(1);
		vec.push_back(2);
		ln_foreach(v, vec) {
			printf("%d", v);
		}
*
*	※ break不可。continue は未評価。あくまで全体の単純なループに使用してください。
*/

#pragma once

namespace LNote
{
namespace Core
{
namespace Base
{
namespace Foreach
{
struct any_itr_base
{
    operator bool() const { return true; }
};

template< typename ITR_T >
struct any_itr
    : any_itr_base
{
    explicit any_itr( ITR_T const &t_ )
      : Item( t_ )
    {}
    mutable ITR_T Item;
};

typedef any_itr_base const &any_itr_t;

template< typename COL_T >
struct col_type
{
    typedef typename COL_T::iterator itr_type;
};

template< typename COL_T >
inline any_itr< typename col_type< COL_T >::itr_type > begin( COL_T& col_ )
{
    return any_itr< typename COL_T::iterator >( col_.begin() );
}

template< typename COL_T >
inline any_itr< typename col_type< COL_T >::itr_type > end( COL_T& col_ )
{
    return any_itr< typename COL_T::iterator >( col_.end() );
}

template< typename COL_T >
inline typename COL_T::iterator& get_itr( any_itr_t any_itr_, COL_T& )
{
    return static_cast< any_itr< typename COL_T::iterator > const& >( any_itr_ ).Item;
}

} // namespace Foreach
} // namespace Base
} // namespace Core
} // namespace LNote

#define ln_foreach( var_, col_ ) \
    if ( LNote::Core::Base::Foreach::any_itr_t _ln_foreach_cur_ = LNote::Core::Base::Foreach::begin( col_ ) ) \
    if ( LNote::Core::Base::Foreach::any_itr_t _ln_foreach_end_ = LNote::Core::Base::Foreach::end( col_ ) ) \
    for ( \
        bool _ln_foreach_continue_ = true; \
        LNote::Core::Base::Foreach::get_itr( _ln_foreach_cur_, col_ ) != LNote::Core::Base::Foreach::get_itr( _ln_foreach_end_, col_ ); \
        ++LNote::Core::Base::Foreach::get_itr( _ln_foreach_cur_, col_ ), _ln_foreach_continue_ = true ) \
    for ( var_ = *LNote::Core::Base::Foreach::get_itr( _ln_foreach_cur_, col_ ); _ln_foreach_continue_; _ln_foreach_continue_ = false )
