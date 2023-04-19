#include<algorithm>
#include<functional>
#include<memory>


#if defined(__has_include) && __has_include("../merge.hpp")
#include "../merge.hpp"
#else //When you want to use this with dependence on only libstdc++
namespace oitl {
	namespace __detail {
#ifndef _DEFINED_LESS
#define _DEFINED_LESS
		template<typename _Tp>
		struct less{
			constexpr bool operator()(const _Tp &lhs,const _Tp &rhs) const{
				return lhs<rhs;
			}
		};
#endif
		template<typename _RandomAccessIter,typename _Cmp>
		void inner_merge(_RandomAccessIter BeginIt1,_RandomAccessIter EndIt1,_RandomAccessIter BeginIt2,_RandomAccessIter EndIt2,_RandomAccessIter SaveIt,const _Cmp &cmp){
			while(BeginIt1!=EndIt1 && BeginIt2!=EndIt2){
				if(cmp(*BeginIt1,*BeginIt2)){
					*SaveIt=*BeginIt1;
					++SaveIt,++BeginIt1;
				} else{
					*SaveIt=*BeginIt2;
					++SaveIt,++BeginIt2;
				}
			}
			while(BeginIt1!=EndIt1){
				*SaveIt=*BeginIt1;
				++SaveIt,++BeginIt1;
			}
			while(BeginIt2!=EndIt2){
				*SaveIt=*BeginIt2;
				++SaveIt,++BeginIt2;
			}
		}
		template<typename _RandomAccessIter>
		inline void inner_merge(_RandomAccessIter BeginIt1,_RandomAccessIter EndIt1,_RandomAccessIter BeginIt2,_RandomAccessIter EndIt2,_RandomAccessIter SaveIt){
			__detail::less<typename _RandomAccessIter::value_type> cmp;
			inner_merge(BeginIt1,EndIt1,BeginIt2,EndIt2,SaveIt,cmp);
		}
		template<typename _RandomAccessIter,typename _Cmp>
		void inner_insert(const _RandomAccessIter &BeginIt1,_RandomAccessIter BeginIt2,_RandomAccessIter EndIt2,_RandomAccessIter SaveIt,const _Cmp &cmp){
			while(!cmp(*BeginIt2,*BeginIt1)){
				if(BeginIt2==EndIt2)	break;
				*(SaveIt++)=*(BeginIt2++);
			}
			*(SaveIt++)=*(BeginIt1);
		}
		template<typename _RandomAccessIter>
		void inner_insert(const _RandomAccessIter &BeginIt1,_RandomAccessIter &BeginIt2,_RandomAccessIter &EndIt2,_RandomAccessIter &SaveIt){
			__detail::less<typename _RandomAccessIter::value_type> cmp;
			inner_insert(BeginIt1,BeginIt2,EndIt2,SaveIt,cmp);
		}
	}
	
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
#endif // __has_include the necessary headers

#ifdef _OITL_CONCEPT_AVAILABLE //The support of concept depends on this macro
#define REQUIRES_OITL_TYPE_CONSTRAINT\
requires\
concepts::interval_calculating_container_general_constraint<_Tp, _Func, _Alloc>
#else
#define REQUIRES_OITL_TYPE_CONSTRAINT
#endif // _OITL_CONCEPT_AVAILABLE

namespace oitl
{
	
	namespace __detail
{
#ifndef _DEFINED_LESS
#define _DEFINED_LESS
	template<typename _Tp>
	struct less{
		constexpr bool operator()(const _Tp &lhs,const _Tp &rhs) const{
			return lhs<rhs;
		}
	};
#endif
	template<typename _RandomAccessIter,typename _Cmp>
	_RandomAccessIter
	_mid_iter(
		_RandomAccessIter first,
		_RandomAccessIter mid,
		_RandomAccessIter last,
		const _Cmp &cmp
		)
{
	_RandomAccessIter small,large;
	if(cmp(*first,*last))small=first,large=last;
	else small=last,large=first;
	if(cmp(*mid,*small))return small;
	if(cmp(*large,*mid))return large;
	return mid;
}
	
	template<typename _RandomAccessIter,typename _Cmp>
	_RandomAccessIter
	_unstable_partition(
		_RandomAccessIter first,
		_RandomAccessIter last,
		const _Cmp &cmp
		)
{
	_RandomAccessIter it=first+(last-first)/2;
	it=_mid_iter(first,it,last-1,cmp);
	std::swap(*it,*first);
	it=first;
	++first;
	while(true)
	{
		while(true)
		{
			if(first==last)goto end;
			else if(cmp(*first,*it))++first;
			else break;
		}
		--last;
		while(true)
		{
			if(first==last)goto end;
			else if(cmp(*it,*last))--last;
			else break;
		}
		std::iter_swap(first,last);
		++first;
	}
	end:
	std::iter_swap(it,--first);
	return first;
}
	
	template<typename _RandomAccessIter,typename _Cmp>
	void
	_final_insertion_sort(
		_RandomAccessIter first,
		_RandomAccessIter last,
		const _Cmp &cmp
		)
{
	_RandomAccessIter lim=first;
	while(lim!=last)
	{
		_RandomAccessIter now=lim;
		while(now!=first)
		{
			if(cmp(*now,*(now-1)))
			{
				std::iter_swap(now,now-1);
				--now;
			}
			else break;
		}
		++lim;
	}
}
	
	template<typename _RandomAccessIter,typename _Cmp>
	void
	_max_heapify(
		_RandomAccessIter first,
		_RandomAccessIter last,
		_RandomAccessIter pos,
		const _Cmp &cmp
		)
{
	typedef typename std::iterator_traits<_RandomAccessIter>::difference_type pos_type;
	pos_type dis=last-first,now=pos-first;
	while(now*2<dis)
	{
		pos_type large=now,left=now*2+1,right=now*2+2;
		if(left<dis&&cmp(first[large],first[left]))large=left;
		if(right<dis&&cmp(first[large],first[right]))large=right;
		if(large==now)break;
		std::iter_swap(first+now,first+large);
		now=large;
	}
}
	
	template<typename _RandomAccessIter,typename _Cmp>
	void
	_heap_sort(
		_RandomAccessIter first,
		_RandomAccessIter last,
		const _Cmp &cmp
		)
{
	typename std::iterator_traits<_RandomAccessIter>::difference_type distance=last-first;
	_RandomAccessIter now=first+distance/2;
	while(now!=first)
	{
		_max_heapify(first,last,now,cmp);
		--now;
	}
	while(last!=first)
	{
		_max_heapify(first,last,first,cmp);
		std::iter_swap(first,--last);
	}
}
	
	template<typename _RandomAccessIter,typename _Cmp>
	void
	_inner_sort(
		_RandomAccessIter first,
		_RandomAccessIter last,
		const _Cmp &cmp,
		typename std::iterator_traits<_RandomAccessIter>::difference_type limit
		)
{
	typedef typename std::iterator_traits<_RandomAccessIter>::difference_type dist_type;
	dist_type distance=last-first;
	if(distance<2)return;
	if(distance<=16)
	{
		_final_insertion_sort(first,last,cmp);
		return;
	}
	static dist_type dep=0;
	if(dep>=limit)
	{
		_heap_sort(first,last,cmp);
		return;
	}
	_RandomAccessIter it=_unstable_partition(first,last,cmp);
	++dep;
	_inner_sort(first,it,cmp,limit);
	_inner_sort(++it,last,cmp,limit);
	--dep;
}
	
	template<typename _RandomAccessIter,typename _Cmp>
	void
	_inner_merge_sort(
		_RandomAccessIter first,
		_RandomAccessIter last,
		const _Cmp &cmp
		){
			typename _RandomAccessIter::difference_type distance=last-first;
			if(distance<2)	return;
			if(distance<16){
				_final_insertion_sort(first,last,cmp);
				return;
			}
			_RandomAccessIter mid=first+(distance>>1);
			_inner_merge_sort(first,mid,cmp);
			_inner_merge_sort(mid,last,cmp);
			std::_Temporary_buffer<_RandomAccessIter,typename _RandomAccessIter::value_type> _tmpBuf(first,distance);
			::oitl::merge(first,mid,mid,last,_RandomAccessIter(_tmpBuf.begin()),cmp);
			_RandomAccessIter it=_RandomAccessIter(_tmpBuf.begin());
			while(first!=last){
				*first=*it;
				++first,++it;
			}
			
	}
	
} //namespace oitl::__detail
	
} //namespace oitl
