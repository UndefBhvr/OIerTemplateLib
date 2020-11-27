#ifndef _OITL_DATA_STRUCTURE_PRIORITY_QUEUE_PAIRING_HEAP_HPP
#define _OITL_DATA_STRUCTURE_PRIORITY_QUEUE_PAIRING_HEAP_HPP//C++ Header pairing_heap.hpp

#include<cstddef>
#include<memory>
#include<utility>
#include<functional>

#ifndef _OITL_DEPENDENCE_FREE
	#include"../../utility/oitl_def.hpp"
	#include"../../utility/oitl_concepts.hpp"
#else //When you want to use this with dependence on only libstdc++
	#define _OITL_LANG_VER __cplusplus
		#ifdef _MSC_VER
			#undef _OITL_LANG_VER
            #define _OITL_LANG_VER _MSVC_LANG
		#endif
#endif // _OITL_DEPENDENCE_FREE

#ifdef _OITL_CONCEPT_AVAILABLE //The support of concept depends on this macro
    #define REQUIRES_OITL_TYPE_CONSTRAINT requires concepts::ordered_associative_container_general_constraint<_Tp, _Cmp, _Alloc>
#else
    #define REQUIRES_OITL_TYPE_CONSTRAINT
#endif // _OITL_CONCEPT_AVAILABLE

namespace oitl
{

#if _OITL_LANG_VER<201103L
    #ifdef nullptr
        #undef nullptr
    #endif
    #define nullptr NULL
#endif



template<
    typename _Tp,
    typename _Cmp=std::less<_Tp>,
    typename _Alloc=std::allocator<_Tp> 
    >

	REQUIRES_OITL_TYPE_CONSTRAINT

class
pairing_heap:_Cmp
{

    public:

        typedef _Tp value_type;
        typedef _Cmp cmp_type;
        typedef _Alloc allocator_type;
        typedef size_t size_type;
    #if _OITL_LANG_VER>=201103L
        typedef typename std::allocator_traits<_Alloc> alloc_traits_type;
    #endif

    private:

        struct Node;

        inline Node *__get_node(_Tp);
        inline void __delete_node(Node*);

        Node* _root;
        size_t s;
        inline Node* merge(Node*,Node*);
        Node* __pop();
        void erase_all_node(Node *ptr);

    #if _OITL_LANG_VER>=201103L
        typedef typename alloc_traits_type::template rebind_traits<Node> node_alloc_traits_type;
        typedef typename node_alloc_traits_type::allocator_type node_alloc_type;
    #else
        typedef typename _Alloc::template rebind<Node>::other node_alloc_type;
    #endif

        node_alloc_type __alloc;

    public:

        struct iterator;

		pairing_heap():
			_root(nullptr),
			s(0)
		{}
        ~pairing_heap();

        iterator push(const _Tp&);
        _Tp &top()const;
        iterator pop();
        void join(pairing_heap&);
        void modify(const iterator&,_Tp);
        size_t size();
        bool empty();
};

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
struct
pairing_heap<_Tp,_Cmp,_Alloc>::Node
{
    _Tp value;
    Node *left_node,*child,*sibling;
    explicit Node(const _Tp& val):
        value(val),
        left_node(nullptr),
        child(nullptr),
        sibling(nullptr) {}
};

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
inline
typename
pairing_heap<_Tp,_Cmp,_Alloc>::Node*
pairing_heap<_Tp,_Cmp,_Alloc>::__get_node(_Tp value)
{
#if _OITL_LANG_VER>=201103L
    Node *ptr=node_alloc_traits_type::allocate(__alloc,1);
    node_alloc_traits_type::construct(__alloc,ptr,value);
#else
    Node *ptr=__alloc.allocate(1);
    __alloc.construct(ptr,Node(value));
#endif
    return ptr;
};

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
inline
void
pairing_heap<_Tp,_Cmp,_Alloc>::__delete_node(Node *ptr)
{
#if _OITL_LANG_VER>=201103L
    node_alloc_traits_type::destroy(__alloc,ptr);
    node_alloc_traits_type::deallocate(__alloc,ptr,1);
#else
    __alloc.destroy(ptr);
    __alloc.deallocate(ptr,1);
#endif
};

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
struct
pairing_heap<_Tp,_Cmp,_Alloc>::iterator
{
    private:

        Node* __real_node;
        explicit iterator(Node* ptr):__real_node(ptr) {}
        friend class pairing_heap;

    public:

        iterator():__real_node(nullptr){}

        const _Tp &operator*()const
        {
            return __real_node->value;
        }

        operator void*()const
        {
            return __real_node;
        }
};

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
pairing_heap<_Tp,_Cmp,_Alloc>::~pairing_heap()
{
    if(_root!=nullptr)
    {
        erase_all_node(_root);
    }
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
inline
typename
pairing_heap<_Tp,_Cmp,_Alloc>::Node* 
pairing_heap<_Tp,_Cmp,_Alloc>::merge(Node* ptr1,Node* ptr2)
{
    if(ptr1==nullptr)return ptr2;
    if(ptr2==nullptr)return ptr1;
    if(!_Cmp::operator()(ptr1->value,ptr2->value))std::swap(ptr1,ptr2);
    ptr1->left_node=ptr2;
    ptr1->sibling=ptr2->child;
    if(ptr2->child!=nullptr)ptr2->child->left_node=ptr1;
    ptr2->child=ptr1;
    return ptr2;
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
typename
pairing_heap<_Tp,_Cmp,_Alloc>::Node*
pairing_heap<_Tp,_Cmp,_Alloc>::__pop()
{
    if(_root==nullptr)return nullptr;
    Node *son1=_root->child;
    if(son1==nullptr)
    {
        return _root=nullptr;
    }
    Node *son2=son1->sibling;
    _root->child=son2!=nullptr?son2->sibling:nullptr;
    son1->left_node=nullptr;
    if(son2!=nullptr)son2->left_node=nullptr;
    return _root=merge(merge(son1,son2),__pop());
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
void
pairing_heap<_Tp,_Cmp,_Alloc>::erase_all_node(Node *ptr)
{
    if(ptr->child!=nullptr)
    {
        erase_all_node(ptr->child);
    }
    if(ptr->sibling!=nullptr)
    {
        erase_all_node(ptr->sibling);
    }
    __delete_node(ptr);
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
typename
pairing_heap<_Tp,_Cmp,_Alloc>::iterator
pairing_heap<_Tp,_Cmp,_Alloc>::push(const _Tp& Value)
{
    ++s;
    if(_root==nullptr)
    {
        _root=__get_node(Value);
        return iterator(_root);
    }
    Node *ptr=__get_node(Value);
    _root=merge(_root,ptr);
    return iterator(ptr);
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
_Tp&
pairing_heap<_Tp,_Cmp,_Alloc>::top()const
{
    return _root->value;
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
typename
pairing_heap<_Tp,_Cmp,_Alloc>::iterator
pairing_heap<_Tp,_Cmp,_Alloc>::pop()
{
    if(_root==nullptr)return iterator(nullptr);
    --s;
    Node *ptr=_root;
    Node *ret=__pop();
    __delete_node(ptr);
    return iterator(ret);
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
void
pairing_heap<_Tp,_Cmp,_Alloc>::join(pairing_heap& Other_heap)
{
    _root=merge(_root,Other_heap._root);
    s+=Other_heap.s;
    Other_heap.s=0;
    Other_heap._root=nullptr;
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
void
pairing_heap<_Tp,_Cmp,_Alloc>::modify(const iterator& Iter,_Tp Value)
{
    Node* ptr=Iter.__real_node;
    if(_Cmp::operator()(Value,ptr->value))
    {
        if(ptr->left_node->child==ptr)
            ptr->left_node->child=ptr->sibling;
        else
            ptr->left_node->sibling=ptr->sibling;
        if(ptr->sibling!=nullptr)
            ptr->sibling->left_node=ptr->left_node;
        ptr->left_node=ptr->sibling=nullptr;
        Node *changed_node=ptr;
        std::swap(ptr,_root);
        _root=merge(ptr,__pop());
        changed_node->value=Value;
        _root=merge(changed_node,_root);
        return;
    }
    ptr->value=Value;
    if(_root==ptr)
    {
        return;
    }
    if(ptr->left_node->child==ptr)
        ptr->left_node->child=ptr->sibling;
    else
        ptr->left_node->sibling=ptr->sibling;
    if(ptr->sibling!=nullptr)
        ptr->sibling->left_node=ptr->left_node;
    ptr->left_node=ptr->sibling=nullptr;
    _root=merge(_root,ptr);
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
size_t
pairing_heap<_Tp,_Cmp,_Alloc>::size()
{
    return s;
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
bool
pairing_heap<_Tp,_Cmp,_Alloc>::empty()
{
    return _root==nullptr;
}

#if _OITL_LANG_VER<201103L
    #undef nullptr
#endif

} //namespace oitl

#endif //C++ Header pairing_heap.hpp