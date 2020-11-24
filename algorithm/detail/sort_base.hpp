#include<algorithm>
#include<functional>

namespace oitl
{

namespace __detail
{

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

} //namespace oitl::__detail

} //namespace oitl