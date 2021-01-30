#ifndef _OITL_DATA_STRUCTURE_BALANCED_TREE_SET_HPP
#define _OITL_DATA_STRUCTURE_BALANCED_TREE_SET_HPP //C++ Header set.hpp

#include"balanced_tree/treap.hpp"


namespace oitl_interface
{



template<typename _Tp,typename _Cmp=std::less<_Tp>,typename _Alloc=std::allocator<_Tp> >
class multiset:public oitl::treap<_Tp,_Cmp,_Alloc>
{
	public:
		typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::key_type key_type;
      	typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::value_type value_type;
      	typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::key_compare key_compare;
      	typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::value_compare value_compare;
     	typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::allocator_type allocator_type;
		typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::size_type size_type;
	#if _OITL_LANG_VER>=201103L
        typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::allocator_traits_type allocator_traits_type;
	#endif
		typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::iterator iterator;
		typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::const_iterator const_iterator;
};

template<typename _Tp,typename _Cmp=std::less<_Tp>,typename _Alloc=std::allocator<_Tp> >
class set:public oitl::treap<_Tp,_Cmp,_Alloc>
{
	public:
		typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::key_type key_type;
      	typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::value_type value_type;
      	typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::key_compare key_compare;
      	typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::value_compare value_compare;
     	typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::allocator_type allocator_type;
		typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::size_type size_type;
	#if _OITL_LANG_VER>=201103L
        typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::allocator_traits_type allocator_traits_type;
	#endif
		typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::iterator iterator;
		typedef typename oitl::treap<_Tp,_Cmp,_Alloc>::const_iterator const_iterator;

		std::pair<const_iterator,bool> insert(_Tp);
};

template<typename _Tp,typename _Cmp,typename _Alloc>
std::pair<typename set<_Tp,_Cmp,_Alloc>::const_iterator,bool>
set<_Tp,_Cmp,_Alloc>::insert(_Tp _Value)
{
	if(oitl::treap<_Tp,_Cmp,_Alloc>::find(_Value)
		!=oitl::treap<_Tp,_Cmp,_Alloc>::end())
	{
		return std::make_pair(oitl::treap<_Tp,_Cmp,_Alloc>::end(),false);
	}
	return std::make_pair(oitl::treap<_Tp,_Cmp,_Alloc>::insert(_Value),true);
}


} //namespace oitl

#endif //C++ Header set.hpp