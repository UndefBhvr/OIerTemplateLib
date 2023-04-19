#ifndef _OITL_ALGORITHM_SORT_HPP
#define _OITL_ALGORITHM_SORT_HPP // C++ Header sort.hpp

#include"detail/sort_base.hpp"

namespace oitl
{
	
	template<typename _RandomAccessIter>
	void
	sort(  
		_RandomAccessIter FirstIter,
		_RandomAccessIter LastIter
		)
{
	__detail::_inner_sort(
		FirstIter,
		LastIter,
		std::less<typename std::iterator_traits<_RandomAccessIter>::value_type>(),
		std::__lg(LastIter-FirstIter)*2
		);
}
	
	template<typename _RandomAccessIter,typename _Cmp>
	void
	sort(  
		_RandomAccessIter FirstIter,
		_RandomAccessIter LastIter,
		_Cmp Compare
		)
{
	__detail::_inner_sort(
		FirstIter,
		LastIter,
		Compare,
		std::__lg(LastIter-FirstIter)*2
		);
}
	template<typename _RandomAccessIter>
	void
	stable_sort(
		_RandomAccessIter FirstIter,
		_RandomAccessIter LastIter
		)
{
	__detail::less<typename _RandomAccessIter::value_type> cmp;
	__detail::_inner_merge_sort(
		FirstIter,
		LastIter,
		cmp
		);
}
	template<typename _RandomAccessIter,typename _Cmp>
	void
	stable_sort(
		_RandomAccessIter FirstIter,
		_RandomAccessIter LastIter,
		const _Cmp &cmp
		)
{
	__detail::_inner_merge_sort(
		FirstIter,
		LastIter,
		cmp
		);
}
	
} //namespace oitl

#endif //C++ Header sort.hpp
