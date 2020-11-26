#ifndef _OITL_DATA_STRUCTURE_PRIORITY_QUEUE_LEFTIST_HEAP_HPP
#define _OITL_DATA_STRUCTURE_PRIORITY_QUEUE_LEFTIST_HEAP_HPP //C++ Header leftist_heap.hpp

#include<algorithm>
#include<memory>
#include<utility>
#include<functional>

#ifndef _OITL_DEPENDENCE_FREE
	#include"../../utility/oitl_concepts.hpp"
#endif

namespace oitl
{

#if __cplusplus<201103L
	#ifdef nullptr
		#undef nullptr
	#endif
	#define nullptr NULL
#endif

/**
 * TODO:
 * const_iterator
 * erase a node with an iterator
 */

template<
	typename _Tp,
	typename _Cmp=std::less<_Tp>,
	typename _Alloc=std::allocator<_Tp>
	>

#ifdef _OITL_CONCEPT_AVAILABLE
	requires
		concepts::ordered_associative_container_general_constraint<_Tp,_Cmp,_Alloc>
#endif

class
leftist_heap:_Cmp
{
	private:

        struct Node;
        Node *_root;
        size_t s;
        Node *merge(Node*,Node*);
        void __erase_inner_node(Node*);

	#if __cplusplus>=201103L
		typedef typename std::allocator_traits<_Alloc>::template rebind_traits<Node> _node_alloc_traits_type;
        typedef typename _node_alloc_traits_type::allocator_type _node_alloc_type;
    #else
		typedef typename _Alloc::template rebind<Node>::other _node_alloc_type;
    #endif

		_node_alloc_type _node_allocator;
		inline Node *__get_new_node(_Tp);
		inline void __delete_node(Node*);

		void destroy_inner_nodes(Node*);

    public:

		typedef _Tp value_type;
		typedef _Cmp cmp_type;
		typedef size_t size_type;
		typedef _Alloc allocator_type;
	#if __cplusplus>=201103L
		typedef std::allocator_traits<_Alloc> _alloc_traits_type;
	#endif

        struct iterator;

		leftist_heap():
			_root(nullptr),
			s(0)
		{}
		~leftist_heap()
		{
			destroy_inner_nodes(_root);
		}

        iterator push(const _Tp&);
        value_type &top()const;
        iterator pop();
        iterator join(leftist_heap&);
		bool modify(const iterator&,const value_type&);
        bool empty()const;
        size_type size()const;
};

template<typename _Tp,typename _Cmp,typename _Alloc>
struct leftist_heap<_Tp,_Cmp,_Alloc>::Node
{
    _Tp val;
    Node *ftr,*lc,*rc;
    size_t npl;

    explicit Node(_Tp Value):
        val(Value),
		ftr(nullptr),
        lc(nullptr),
        rc(nullptr),
        npl(0)
	{}

	bool maintain()
	{
		size_t ldp=0,rdp=0;
		if(lc!=nullptr)
		{
			ldp=lc->npl+1;
			lc->ftr=this;
		}
		if(rc!=nullptr)
		{
			rdp=rc->npl+1;
			rc->ftr=this;
		}
        size_type new_npl=std::min(ldp,rdp);
		if(new_npl!=npl)
        {
            npl=new_npl;
		    if(ldp<rdp)std::swap(lc,rc);
            return false;
        }
        return false;
	}
};

template<typename _Tp,typename _Cmp,typename _Alloc>
inline
typename leftist_heap<_Tp,_Cmp,_Alloc>::Node*
leftist_heap<_Tp,_Cmp,_Alloc>::__get_new_node(_Tp __value)
{
#if __cplusplus>=201103L
    Node *new_ptr=_node_alloc_traits_type::allocate(_node_allocator,1);
    _node_alloc_traits_type::construct(_node_allocator,new_ptr,__value);
#else
	Node *new_ptr=_node_allocator.allocate(1);
	_node_allocator.construct(new_ptr,__value);
#endif
    return new_ptr;
}

template<typename _Tp,typename _Cmp,typename _Alloc>
inline
void
leftist_heap<_Tp,_Cmp,_Alloc>::__delete_node(Node *__ptr)
{
#if __cplusplus>=201103L
    _node_alloc_traits_type::destroy(_node_allocator,__ptr);
    _node_alloc_traits_type::deallocate(_node_allocator,__ptr,1);
#else
	_node_allocator.destroy(__ptr);
	_node_allocator.deallocate(__ptr,1);
#endif
}

template<typename _Tp,typename _Cmp,typename _Alloc>
void
leftist_heap<_Tp,_Cmp,_Alloc>::destroy_inner_nodes(Node *ptr)
{
	if(ptr==nullptr)return;
	destroy_inner_nodes(ptr->lc);
	destroy_inner_nodes(ptr->rc);
    __delete_node(ptr);
}

template<typename _Tp,typename _Cmp,typename _Alloc>
typename
leftist_heap<_Tp,_Cmp,_Alloc>::Node*
leftist_heap<_Tp,_Cmp,_Alloc>::merge(Node* first_heap,Node* second_heap)
{
    if(first_heap==nullptr)return second_heap;
    if(second_heap==nullptr)return first_heap;
    if(_Cmp::operator()(second_heap->val,first_heap->val))
	{
		std::swap(first_heap,second_heap);
	}
    second_heap->rc=merge(first_heap,second_heap->rc);
    if(second_heap->lc==nullptr||second_heap->lc->npl<second_heap->rc->npl)
	{
		std::swap(second_heap->lc,second_heap->rc);
	}
    second_heap->maintain();
    return second_heap;
}

template<typename _Tp,typename _Cmp,typename _Alloc>
void
leftist_heap<_Tp,_Cmp,_Alloc>::__erase_inner_node(Node* ptr)
{
    Node *p_lc=ptr->lc,*p_rc=ptr->rc;
    if(p_lc!=nullptr)p_lc->ftr=nullptr;
    if(p_rc!=nullptr)p_rc->ftr=nullptr;
    if(ptr==_root)
    {
        _root=merge(p_lc,p_rc);
        return;
    }
    bool is_lc=ptr->ftr->lc==ptr;
    Node *&ptr_pos=is_lc?ptr->ftr->lc:ptr->ftr->rc;
    ptr_pos=merge(p_lc,p_rc);
    ptr=ptr->ftr;
    while(ptr!=nullptr&&ptr->maintain())ptr=ptr->ftr;
}

template<typename _Tp,typename _Cmp,typename _Alloc>
struct leftist_heap<_Tp,_Cmp,_Alloc>::iterator
{
    private:

        Node *real_node;
		friend class leftist_heap;
        explicit iterator(Node* ptr):real_node(ptr) {}

    public:
		iterator():real_node(nullptr){}

        const _Tp &operator*()const
        {
            return real_node->val;
        }
        operator void*()const
		{
			return (void*)real_node;
		}
};

template<typename _Tp,typename _Cmp,typename _Alloc>
typename
leftist_heap<_Tp,_Cmp,_Alloc>::iterator
leftist_heap<_Tp,_Cmp,_Alloc>::push(const _Tp& Value)
{
    Node* new_node=new Node(Value);
    _root=merge(new_node,_root);
    ++s;
    return iterator(new_node);
}

template<typename _Tp,typename _Cmp,typename _Alloc>
typename
leftist_heap<_Tp,_Cmp,_Alloc>::value_type&
leftist_heap<_Tp,_Cmp,_Alloc>::top()const
{
    return _root->val;
}

template<typename _Tp,typename _Cmp,typename _Alloc>
typename
leftist_heap<_Tp,_Cmp,_Alloc>::iterator
leftist_heap<_Tp,_Cmp,_Alloc>::pop()
{
    Node* ptr=_root;
	if(ptr->lc!=nullptr)ptr->lc->ftr=nullptr;
	if(ptr->rc!=nullptr)ptr->rc->ftr=nullptr;
    _root=merge(ptr->lc,ptr->rc);
    __delete_node(ptr);
    --s;
    return iterator(_root);
}

template<typename _Tp,typename _Cmp,typename _Alloc>
typename
leftist_heap<_Tp,_Cmp,_Alloc>::iterator
leftist_heap<_Tp,_Cmp,_Alloc>::join(leftist_heap<_Tp,_Cmp,_Alloc>& Other_heap)
{
    _root=merge(Other_heap._root,_root);
    s+=Other_heap.s;
    Other_heap._root=nullptr;
    Other_heap.s=0;
    return iterator(_root);
}

template<typename _Tp,typename _Cmp,typename _Alloc>
bool
leftist_heap<_Tp,_Cmp,_Alloc>::modify(const iterator& Iterator,const value_type& Value)
{
    Node *ptr=Iterator.real_node;
    if(ptr==nullptr)return false;
    __erase_inner_node(ptr);
    ptr->val=Value;
    ptr->ftr=nullptr;
    ptr->lc=nullptr;
    ptr->rc=nullptr;
    ptr->maintain();
    _root=merge(_root,ptr);
    return true;
}

template<typename _Tp,typename _Cmp,typename _Alloc>
typename
leftist_heap<_Tp,_Cmp,_Alloc>::size_type
leftist_heap<_Tp,_Cmp,_Alloc>::size()const
{
    return s;
}

template<typename _Tp,typename _Cmp,typename _Alloc>
bool
leftist_heap<_Tp,_Cmp,_Alloc>::empty()const
{
    return _root==nullptr;
}

#if __cplusplus<201103L
	#undef nullptr
#endif

} //namespace oitl

#endif //C++ Header leftist_heap.hpp