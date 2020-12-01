#ifndef _OITL_DATA_STRUCTURE_BALANCED_TREE_TREAP_HPP
#define _OITL_DATA_STRUCTURE_BALANCED_TREE_TREAP_HPP //C++ Header treap.hpp

#include<cstdlib>
#include<memory>
#include<utility>
#include<functional>

#if _OITL_LANG_VER>=201103L
#include<random>
#endif

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
    #define REQUIRES_OITL_TYPE_CONSTRAINT\
        requires\
            concepts::ordered_associative_container_general_constraint<_Tp, _Cmp, _Alloc>
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

/**
 * This class can be used normally in some ways
 * But it will be better soon
 * 
 * TODO: 
 * reverse_iterator
 * begin() in O(1) time
 */

template<
    typename _Tp,
    typename _Cmp=std::less<_Tp>,
    typename _Alloc=std::allocator<_Tp> 
    > REQUIRES_OITL_TYPE_CONSTRAINT

class
treap:_Cmp
{
    public:

        typedef _Tp Value_type;
        typedef _Cmp Comparator_type;
        typedef size_t size_type;
        typedef _Alloc allocator_type;
	#if _OITL_LANG_VER>=201103L
        typedef typename std::allocator_traits<_Alloc> alloc_traits_type;
	#endif

    private:

        struct Node;

		struct _const_iterator;

        inline Node *__get_new_node();
        inline Node *__get_new_node(_Tp);
        inline void __emplace_node(Node*,_Tp);
        inline void __delete_node(Node*);

        Node *merge(Node*,Node*);
        void split_val(Node*,const _Tp&,Node*&,Node*&);
        void split_val_equal(Node*,const _Tp&,Node*&,Node*&);

		inline static unsigned long get_pri();
		void destroy_inner_nodes(Node*);

	#if _OITL_LANG_VER>=201103L
        typedef typename alloc_traits_type::template rebind_traits<Node> _node_alloc_traits_type;
        typedef typename _node_alloc_traits_type::allocator_type _node_alloc_type;
    #else
		typedef typename _Alloc::template rebind<Node>::other _node_alloc_type;
	#endif

        _node_alloc_type _node_allocator;

        Node *end_node;
        Node *&root;

    public:

        typedef _const_iterator const_iterator;
		typedef _const_iterator iterator;

        treap():
			_node_allocator(),
            end_node(__get_new_node()),
            root(end_node->lc)
        {}

		~treap();

        std::pair<const_iterator,bool> insert(const _Tp&);
        bool erase(const _Tp&);
		void clear();
        const_iterator begin()const;
        const_iterator end()const;
        const_iterator find(const _Tp&)const;
        const_iterator lower_bound(const _Tp&)const;
        const_iterator upper_bound(const _Tp&)const;
        const_iterator find_by_order(const size_type&)const;
        size_type order_of_key(const _Tp&)const;
        size_type size()const;
};

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
struct treap<_Tp,_Cmp,_Alloc>::Node
{
    _Tp value;
    unsigned long pri;
    size_type s;
    Node *lc,*rc,*ftr;
    
	Node():
		pri(get_pri()),
        s(1),
        lc(nullptr),
        rc(nullptr),
        ftr(nullptr)
    {}

    explicit Node(_Tp val):
        value(val),
        pri(get_pri()),
        s(1),
        lc(nullptr),
        rc(nullptr),
        ftr(nullptr)
    {}

    void maintain()
    {
        s=1;
        if(lc!=nullptr)
        {
            s+=lc->s;
            lc->ftr=this;
        }
        if(rc!=nullptr)
        {
            s+=rc->s;
            rc->ftr=this;
        }
    }

	inline void rotate()
	{
		Node *nftr=ftr,*gftr=ftr->ftr;
		if(gftr!=nullptr)
		{
			if(nftr==gftr->lc)gftr->lc=this;
			else gftr->rc=this;
		}
		if(this==nftr->lc)
		{
			nftr->lc=rc;
			rc=nftr;
		}
		else
		{
			nftr->rc=lc;
			lc=nftr;
		}
		nftr->maintain();
		maintain();
		if(gftr!=nullptr)gftr->maintain();
	}
};

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
inline
typename treap<_Tp,_Cmp,_Alloc>::Node*
treap<_Tp,_Cmp,_Alloc>::__get_new_node()
{
#if _OITL_LANG_VER>=201103L
    Node *new_ptr=_node_alloc_traits_type::allocate(_node_allocator,1);
    _node_alloc_traits_type::construct(_node_allocator,new_ptr);
#else
	Node *new_ptr=_node_allocator.allocate(1);
	_node_allocator.construct(new_ptr,Node());
#endif
    return new_ptr;
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
inline
typename treap<_Tp,_Cmp,_Alloc>::Node*
treap<_Tp,_Cmp,_Alloc>::__get_new_node(_Tp __value)
{
#if _OITL_LANG_VER>=201103L
    Node *new_ptr=_node_alloc_traits_type::allocate(_node_allocator,1);
    _node_alloc_traits_type::construct(_node_allocator,new_ptr,__value);
#else
	Node *new_ptr=_node_allocator.allocate(1);
	_node_allocator.construct(new_ptr,Node(__value));
#endif
    return new_ptr;
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
inline
void
treap<_Tp,_Cmp,_Alloc>::__delete_node(Node *__ptr)
{
#if _OITL_LANG_VER>=201103L
    _node_alloc_traits_type::destroy(_node_allocator,__ptr);
    _node_alloc_traits_type::deallocate(_node_allocator,__ptr,1);
#else
	_node_allocator.destroy(__ptr);
	_node_allocator.deallocate(__ptr,1);
#endif
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
typename treap<_Tp,_Cmp,_Alloc>::Node*
treap<_Tp,_Cmp,_Alloc>::merge(Node *lef_tree,Node *rig_tree)
{
    if(lef_tree==nullptr)return rig_tree;
    if(rig_tree==nullptr)return lef_tree;
    if(lef_tree->pri>rig_tree->pri)
    {
        lef_tree->rc=merge(lef_tree->rc,rig_tree);
        lef_tree->maintain();
        return lef_tree;
    }
    else
    {
        rig_tree->lc=merge(lef_tree,rig_tree->lc);
        rig_tree->maintain();
        return rig_tree;
    }
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
void
treap<_Tp,_Cmp,_Alloc>::split_val(Node *subtree_root,const _Tp &val,Node *&lef_tree,Node *&rig_tree)
{
    if(subtree_root==nullptr)
    {
        lef_tree=nullptr;
        rig_tree=nullptr;
        return;
    }
    if(_Cmp::operator()(subtree_root->value,val))
    {
        lef_tree=subtree_root;
        split_val(lef_tree->rc,val,lef_tree->rc,rig_tree);
    }
    else
    {
        rig_tree=subtree_root;
        split_val(rig_tree->lc,val,lef_tree,rig_tree->lc);
    }
    subtree_root->maintain();
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
void
treap<_Tp,_Cmp,_Alloc>::split_val_equal(Node *subtree_root,const _Tp &val,Node *&lef_tree,Node *&rig_tree)
{
    if(subtree_root==nullptr)
    {
        lef_tree=nullptr;
        rig_tree=nullptr;
        return;
    }
    if(!_Cmp::operator()(val,subtree_root->value))
    {
        lef_tree=subtree_root;
        split_val_equal(lef_tree->rc,val,lef_tree->rc,rig_tree);
    }
    else
    {
        rig_tree=subtree_root;
        split_val_equal(rig_tree->lc,val,lef_tree,rig_tree->lc);
    }
    subtree_root->maintain();
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
struct treap<_Tp,_Cmp,_Alloc>::_const_iterator
{
		friend class treap<_Tp,_Cmp,_Alloc>;

    private:

        const Node *ptr;

        explicit _const_iterator(Node *ptr):
            ptr(ptr)
        {}

    public:

        _const_iterator():ptr(nullptr){}

        const _Tp &operator*()const
        {
            return ptr->value;
        }

		operator void*()const
		{
			return (void*)ptr;
		}

        _const_iterator &operator++()
        {
            if(ptr->rc!=nullptr)
            {
                ptr=ptr->rc;
                while(ptr->lc!=nullptr)ptr=ptr->lc;
                return *this;
            }
            else if(ptr->ftr!=nullptr)
            {
                while(ptr->ftr!=nullptr&&ptr==ptr->ftr->rc)ptr=ptr->ftr;
                if(ptr->ftr!=nullptr&&ptr==ptr->ftr->lc)
                {
                    ptr=ptr->ftr;
                    return *this;
                }
            }
			return *this;
        }
        _const_iterator operator++(int)
        {
            Node *ptr0=ptr;
            ++*this;
            return iterator(ptr0);
        }
        _const_iterator &operator--()
        {
            if(ptr->lc!=nullptr)
            {
                ptr=ptr->lc;
                while(ptr->rc!=nullptr)ptr=ptr->rc;
                return *this;
            }
            else if(ptr->ftr!=nullptr)
            {
                while(ptr->ftr!=nullptr&&ptr==ptr->ftr->lc)ptr=ptr->ftr;
                if(ptr->ftr!=nullptr&&ptr==ptr->ftr->rc)
                {
                    ptr=ptr->ftr;
                    return *this;
                }
            }
			return *this;
        }
        _const_iterator operator--(int)
        {
            Node *ptr0=ptr;
            --*this;
            return const_iterator(ptr0);
        }
};

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
inline
unsigned long
treap<_Tp,_Cmp,_Alloc>::get_pri()
{
#if _OITL_LANG_VER>=201103L
    static std::mt19937 rand_mker;
	return rand_mker();
#else
	#if RAND_MAX<0x3fffffff
    	return (rand()*1ULL<<32ULL)+rand();
	#else
		return rand();
	#endif
#endif
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
void
treap<_Tp,_Cmp,_Alloc>::destroy_inner_nodes(Node *ptr)
{
	if(ptr==nullptr)return;
	destroy_inner_nodes(ptr->lc);
	destroy_inner_nodes(ptr->rc);
    __delete_node(ptr);
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
treap<_Tp,_Cmp,_Alloc>::~treap()
{
	destroy_inner_nodes(end_node);
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
std::pair<typename treap<_Tp,_Cmp,_Alloc>::const_iterator,bool>
treap<_Tp,_Cmp,_Alloc>::insert(const _Tp& Value)
{
    if(find(Value)!=end())return std::make_pair(end(),false);
    Node *ptr=root,*last=end_node;
    Node *new_ptr=__get_new_node(Value);
    bool is_lc=true;
    while(ptr!=NULL&&ptr->pri>new_ptr->pri)
    {
        ++ptr->s;
        last=ptr;
        if(_Cmp::operator()(Value,ptr->value))ptr=ptr->lc,is_lc=true;
        else if(_Cmp::operator()(ptr->value,Value))ptr=ptr->rc,is_lc=false;
    }
    split_val(ptr,Value,new_ptr->lc,new_ptr->rc);
    new_ptr->maintain();
    if(is_lc)last->lc=new_ptr;
    else last->rc=new_ptr;
    last->maintain();
    return std::make_pair(const_iterator(new_ptr),true);
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
bool
treap<_Tp,_Cmp,_Alloc>::erase(const _Tp& Value)
{
    if(find(Value)==end())return false;
    Node *ptr=root;
    bool is_lc=true;
    while(1)
    {
        --ptr->s;
        if(_Cmp::operator()(Value,ptr->value))ptr=ptr->lc,is_lc=true;
        else if(_Cmp::operator()(ptr->value,Value))ptr=ptr->rc,is_lc=false;
        else break;
    }
    Node *fptr=ptr->ftr;
    if(is_lc)fptr->lc=merge(ptr->lc,ptr->rc);
    else fptr->rc=merge(ptr->lc,ptr->rc);
    fptr->maintain();
    __delete_node(ptr);
    return true;
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
void
treap<_Tp,_Cmp,_Alloc>::clear()
{
    destroy_inner_nodes(root);
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
typename treap<_Tp,_Cmp,_Alloc>::const_iterator
treap<_Tp,_Cmp,_Alloc>::begin()const
{
    if(root==nullptr)return const_iterator(end_node);
    Node *now=root;
    while(now->lc!=nullptr)now=now->lc;
    return const_iterator(now);
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
typename treap<_Tp,_Cmp,_Alloc>::const_iterator
treap<_Tp,_Cmp,_Alloc>::end()const
{
    return const_iterator(end_node);
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
typename treap<_Tp,_Cmp,_Alloc>::const_iterator
treap<_Tp,_Cmp,_Alloc>::find(const _Tp& Value)const
{
    Node *now=root;
    while(now!=nullptr)
    {
        if(_Cmp::operator()(Value,now->value))now=now->lc;
        else if(_Cmp::operator()(now->value,Value))now=now->rc;
        else return const_iterator(now);
    }
    return const_iterator(end_node);
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
typename treap<_Tp,_Cmp,_Alloc>::const_iterator
treap<_Tp,_Cmp,_Alloc>::lower_bound(const _Tp& Value)const
{
    Node *now=root,*ans=nullptr;
    while(now!=nullptr)
    {
        if(!_Cmp::operator()(now->value,Value))
        {
            ans=now;
            now=now->lc;
        }
        else
        {
            now=now->rc;
        }
    }
    if(ans==nullptr)ans=end_node;
    return const_iterator(ans);
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
typename treap<_Tp,_Cmp,_Alloc>::const_iterator
treap<_Tp,_Cmp,_Alloc>::upper_bound(const _Tp& Value)const
{
    Node *now=root,*ans=nullptr;
    while(now!=nullptr)
    {
        if(_Cmp::operator()(Value,now->value))
        {
            ans=now;
            now=now->lc;
        }
        else
        {
            now=now->rc;
        }
    }
    if(ans==nullptr)ans=end_node;
    return const_iterator(ans);
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
typename treap<_Tp,_Cmp,_Alloc>::const_iterator
treap<_Tp,_Cmp,_Alloc>::find_by_order(const size_type& Order)const
{
    size_type rest_order=Order;
    Node *ans=root;
    while(ans!=nullptr)
    {
        size_type ls=0;
        if(ans->lc!=nullptr)ls=ans->lc->s;
        if(ls>rest_order)
        {
            ans=ans->lc;
        }
        else if(ls<rest_order)
        {
            rest_order-=ls+1;
            ans=ans->rc;
        }
        else return const_iterator(ans);
    }
    return const_iterator(ans);
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
typename treap<_Tp,_Cmp,_Alloc>::size_type
treap<_Tp,_Cmp,_Alloc>::order_of_key(const _Tp& Value)const
{
    size_type ans=0;
    Node *now=root;
    while(now!=nullptr)
    {
        size_type ls=0;
        if(now->lc!=nullptr)ls=now->lc->s;
        if(_Cmp::operator()(now->value,Value))
        {
            ans+=ls+1;
            now=now->rc;
        }
        else
        {
            now=now->lc;
        }
    }
    return ans;
}

template<typename _Tp,typename _Cmp,typename _Alloc> REQUIRES_OITL_TYPE_CONSTRAINT
typename treap<_Tp,_Cmp,_Alloc>::size_type
treap<_Tp,_Cmp,_Alloc>::size()const
{
    if(root==nullptr)return 0;
    return root->s;
}

#if _OITL_LANG_VER<201103L
	#undef nullptr
#endif

} //namespace oitl

#endif //C++ Header treap.hpp