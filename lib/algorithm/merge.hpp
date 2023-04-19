#ifndef _OITL_ALGORITHM_MERGE_HPP
#define _OITL_ALGORITHM_MERGE_HPP // C++ Header merge.hpp

#include "detail/merge_base.hpp"

namespace oitl
{
	
	template<typename _RandomAccessIter,typename _Cmp>
	void
	merge(
		_RandomAccessIter BeginIt1,
		_RandomAccessIter EndIt1,
		_RandomAccessIter BeginIt2,
		_RandomAccessIter EndIt2,
		_RandomAccessIter SaveIt,
		const _Cmp &cmp
		){
			typename _RandomAccessIter::difference_type distance1=EndIt1-BeginIt1,distance2=EndIt2-BeginIt2;
			if(distance1==1){
				oitl::__detail::inner_insert(BeginIt1,BeginIt2,EndIt2,SaveIt,cmp);
			} else if(distance2==1){
				oitl::__detail::inner_insert(BeginIt2,BeginIt1,EndIt1,SaveIt,cmp);
			} else{
				oitl::__detail::inner_merge(BeginIt1,EndIt1,BeginIt2,EndIt2,SaveIt,cmp);
			}
		}
	template<typename _RandomAccessIter>
	void
	merge(
		_RandomAccessIter BeginIt1,
		_RandomAccessIter EndIt1,
		_RandomAccessIter BeginIt2,
		_RandomAccessIter EndIt2,
		_RandomAccessIter SaveIt
		){
			typename _RandomAccessIter::difference_type distance1=EndIt1-BeginIt1,distance2=EndIt2-BeginIt2;
			if(distance1==1){
				oitl::__detail::inner_insert(BeginIt1,BeginIt2,EndIt2,SaveIt);
			} else if(distance2==1){
				oitl::__detail::inner_insert(BeginIt2,BeginIt1,EndIt1,SaveIt);
			} else{
				oitl::__detail::inner_merge(BeginIt1,EndIt1,BeginIt2,EndIt2,SaveIt);
			}
		}
	
} //namespace oitl

#endif //C++ Header sort.hpp
