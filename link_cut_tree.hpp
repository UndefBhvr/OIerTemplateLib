#ifndef LINK_CUT_TREE_HPP
#define LINK_CUT_TREE_HPP //C++ Header link_cut_tree.hpp

#include<utility>
#include<algorithm>
#include<memory>

namespace oitl
{

#if __cplusplus<201103L
	#ifdef nullptr
		#undef nullptr
	#endif
	#define nullptr NULL
#endif

template<typename _Tp,typename _Func,typename _Alloc>
class link_cut_tree;


namespace __tree_base
{

template<typename _Tp,typename _Func,typename _Alloc>
class LCT_splay
{
		LCT_splay(){}
        struct Node;
        Node *__new_node(_Tp);
		void __delete_node(Node*);

	public:
		friend class link_cut_tree<_Tp,_Func,_Alloc>;
		typedef _Tp value_type;
		typedef _Func func_type;
		typedef _Alloc alloc_type;
	#if __cplusplus>=201103L
        typedef typename std::allocator_traits<_Alloc> alloc_traits_type;
	#endif

	private:
	#if __cplusplus>=201103L
        typedef typename alloc_traits_type::template rebind_traits<Node> node_alloc_traits_type;
	#endif
    #if __cplusplus<201703L
		typedef typename _Alloc::template rebind<Node>::other node_alloc_type;
    #else
        typedef typename node_alloc_traits_type::allocator_type node_alloc_type;
    #endif

		node_alloc_type __alloc;
};

template<typename _Tp,typename _Func,typename _Alloc>
struct LCT_splay<_Tp,_Func,_Alloc>::Node:_Func
{
    _Tp val,sum;
    Node* ftr;
    Node* ch[2];
    bool rev;

    Node():
        ftr(nullptr),
        rev(false)
    {
        ch[0]=nullptr;
        ch[1]=nullptr;
    }

    Node(const _Tp& _value):
        val(_value),
        sum(_value),
        ftr(nullptr),
        rev(false)
	{
		ch[0]=nullptr;
        ch[1]=nullptr;
	}

    void reverse();
    void push_down();
    void push_all();
    void maintain();
    bool is_root();
    void rotate();
    void splay();
};

template<typename _Tp,typename _Func,typename _Alloc>
void
LCT_splay<_Tp,_Func,_Alloc>::Node::reverse()
{
    rev=!rev;
}

template<typename _Tp,typename _Func,typename _Alloc>
void
LCT_splay<_Tp,_Func,_Alloc>::Node::push_down()
{
    if(!rev)return;
    rev=false;
    std::swap(ch[0],ch[1]);
    if(ch[0]!=nullptr)ch[0]->reverse();
    if(ch[1]!=nullptr)ch[1]->reverse();
}

template<typename _Tp,typename _Func,typename _Alloc>
void
LCT_splay<_Tp,_Func,_Alloc>::Node::push_all()
{
    if(!is_root())this->ftr->push_all();
    push_down();
}

template<typename _Tp,typename _Func,typename _Alloc>
void
LCT_splay<_Tp,_Func,_Alloc>::Node::maintain()
{
    sum=val;
    if(ch[0]!=nullptr)sum=_Func::operator()(ch[0]->sum,sum);
    if(ch[1]!=nullptr)sum=_Func::operator()(sum,ch[1]->sum);
}

template<typename _Tp,typename _Func,typename _Alloc>
bool
LCT_splay<_Tp,_Func,_Alloc>::Node::is_root()
{
    return ftr==nullptr||(ftr->ch[0]!=this&&ftr->ch[1]!=this);
}

template<typename _Tp,typename _Func,typename _Alloc>
void
LCT_splay<_Tp,_Func,_Alloc>::Node::rotate()
{
    Node *nftr=ftr,*gftr=ftr->ftr;
    bool is_rc=nftr->ch[1]==this;
    bool is_rf=gftr!=nullptr?gftr->ch[1]==nftr:0;
    ftr=gftr;
    if(!nftr->is_root())gftr->ch[is_rf]=this;
    nftr->ch[is_rc]=this->ch[!is_rc];
    if(this->ch[!is_rc]!=nullptr)this->ch[!is_rc]->ftr=nftr;
    nftr->ftr=this;
    this->ch[!is_rc]=nftr;
    nftr->maintain();
    maintain();
}

template<typename _Tp,typename _Func,typename _Alloc>
void
LCT_splay<_Tp,_Func,_Alloc>::Node::splay()
{
    push_all();
    while(!is_root())
    {
        Node *nftr=ftr,*gftr=ftr->ftr;
        if(nftr->is_root())rotate();
        else
        {
            if((gftr->ch[0]==nftr)!=(nftr->ch[0]==this))rotate();
            else nftr->rotate();
            rotate();
        }
    }
}

template<typename _Tp,typename _Func,typename _Alloc>
typename
LCT_splay<_Tp,_Func,_Alloc>::Node*
LCT_splay<_Tp,_Func,_Alloc>::__new_node(_Tp __val)
{
	Node *ptr;
#if __cplusplus>=201103L
    ptr=node_alloc_traits_type::allocate(__alloc,1);
	node_alloc_traits_type::construct(__alloc,ptr,__val);
#else
    ptr=__alloc.allocate(1);
	__alloc.construct(ptr,__val);
#endif
	return ptr;
}

template<typename _Tp,typename _Func,typename _Alloc>
void
LCT_splay<_Tp,_Func,_Alloc>::__delete_node(Node *__ptr)
{
#if __cplusplus>=201103L
    node_alloc_traits_type::destroy(__alloc,__ptr);
	node_alloc_traits_type::deallocate(__alloc,__ptr,1);
#else
	__alloc.destroy(__ptr);
    __alloc.deallocate(__ptr,1);
#endif
}

} //namespace __tree_base


template<typename _Tp,typename _Func,typename _Alloc=std::allocator<_Tp> >
class link_cut_tree:private __tree_base::LCT_splay<_Tp,_Func,_Alloc>
{
    private:
		typedef typename __tree_base::LCT_splay<_Tp,_Func,_Alloc> splay_base;
    	typedef typename splay_base::Node Node;

        void access(Node*);
        void make_root(Node*);
        Node* find_root(Node*);
        bool split(Node*,Node*);
    public:
		typedef typename splay_base::value_type value_type;
		typedef typename splay_base::func_type func_type;
		typedef typename splay_base::alloc_type allocator_type;
	#if __cplusplus>=201103L
		typedef typename splay_base::alloc_traits_type allocator_traits_type;
	#endif

        struct iterator;

        iterator make_node(_Tp);
		bool erase_node(iterator&);
        bool link(const iterator&,const iterator&);
        bool cut(const iterator&,const iterator&);
        std::pair<_Tp,bool> query(const iterator&,const iterator&);
        bool modify(const iterator&,_Tp);
};

template<typename _Tp,typename _Func,typename _Alloc>
struct link_cut_tree<_Tp,_Func,_Alloc>::iterator
{
    private:
        Node* ptr;
        friend class link_cut_tree;
        iterator(Node* p):ptr(p) {}
    public:
		iterator():ptr(nullptr) {}
        iterator(const iterator& Iterator):
            ptr(Iterator.ptr)
            {}
        iterator &operator=(const iterator& rhs)
        {
            ptr=rhs.ptr;
            return *this;
        }

        _Tp &operator*()const {return ptr->val;}
		operator bool()const {return ptr!=nullptr;}
		bool operator==(const iterator& rhs)const
		{
			return ptr==rhs.ptr;
		}
		bool operator!=(const iterator& rhs)const
		{
			return ptr!=rhs.ptr;
		}
};

template<typename _Tp,typename _Func,typename _Alloc>
void
link_cut_tree<_Tp,_Func,_Alloc>::access(Node* ptr)
{
    for(Node* now_rc_ptr=nullptr;ptr!=nullptr;ptr=ptr->ftr)
        {
            ptr->splay();
            ptr->ch[1]=now_rc_ptr;
            ptr->maintain();
			now_rc_ptr=ptr;
        }
}

template<typename _Tp,typename _Func,typename _Alloc>
void
link_cut_tree<_Tp,_Func,_Alloc>::make_root(Node* ptr)
{
    access(ptr);
    ptr->splay();
    ptr->reverse();
}

template<typename _Tp,typename _Func,typename _Alloc>
typename
link_cut_tree<_Tp,_Func,_Alloc>::Node*
link_cut_tree<_Tp,_Func,_Alloc>::find_root(Node* ptr)
{
    access(ptr);
    ptr->splay();
    while(ptr->ch[0]!=nullptr)ptr->push_down(),ptr=ptr->ch[0];
    ptr->splay();
    return ptr;
}

template<typename _Tp,typename _Func,typename _Alloc>
bool
link_cut_tree<_Tp,_Func,_Alloc>::split(Node* first_ptr,Node* second_ptr)
{
    make_root(first_ptr);
    if(find_root(second_ptr)!=first_ptr)return 0;
    second_ptr->splay();
    return 1;
}

template<typename _Tp,typename _Func,typename _Alloc>
typename
link_cut_tree<_Tp,_Func,_Alloc>::iterator
link_cut_tree<_Tp,_Func,_Alloc>::make_node(_Tp Value)
{
    iterator res(__tree_base::LCT_splay<_Tp,_Func,_Alloc>::__new_node(Value));
    return res;
}

template<typename _Tp,typename _Func,typename _Alloc>
bool
link_cut_tree<_Tp,_Func,_Alloc>::erase_node(iterator &Iterator)
{
	Node *ptr=Iterator.ptr;
	if(ptr==nullptr)return false;
    __tree_base::LCT_splay<_Tp,_Func,_Alloc>::__delete_node(ptr);
	return true;
}

template<typename _Tp,typename _Func,typename _Alloc>
bool
link_cut_tree<_Tp,_Func,_Alloc>::link(const iterator& First_iter,const iterator& Second_iter)
{
    Node* first_ptr=First_iter.ptr;
    Node* second_ptr=Second_iter.ptr;
    make_root(first_ptr);
    if(find_root(second_ptr)==first_ptr)return false;
    first_ptr->ftr=second_ptr;
    return true;
}

template<typename _Tp,typename _Func,typename _Alloc>
bool
link_cut_tree<_Tp,_Func,_Alloc>::cut(const iterator& First_iter,const iterator& Second_iter)
{
    Node* first_ptr=First_iter.ptr;
    Node* second_ptr=Second_iter.ptr;
    make_root(first_ptr);
    if(find_root(second_ptr)!=first_ptr||second_ptr->ftr!=first_ptr||second_ptr->ch[0]!=nullptr)
	{
		return false;
	}
    second_ptr->ftr=nullptr;
    first_ptr->ch[0]=nullptr;
    first_ptr->maintain();
    return true;
}

template<typename _Tp,typename _Func,typename _Alloc>
std::pair<_Tp,bool>
link_cut_tree<_Tp,_Func,_Alloc>::query(const iterator &First_iter,const iterator &Second_iter)
{
    Node* first_ptr=First_iter.ptr;
    Node* second_ptr=Second_iter.ptr;
    if(!split(first_ptr,second_ptr))
	{
		return std::make_pair(second_ptr->sum,false);
	}
    return std::make_pair(second_ptr->sum,true);
}

template<typename _Tp,typename _Func,typename _Alloc>
bool
link_cut_tree<_Tp,_Func,_Alloc>::modify(const iterator &Iterator,_Tp Value)
{
    Node* ptr=Iterator.ptr;
    if(ptr==nullptr)return false;
    ptr->splay();
    ptr->val=Value;
    ptr->maintain();
    return true;
}

#if __cplusplus<201103L
	#undef nullptr
#endif

} //namespace oitl

#endif //C++ Header link_cut_tree.hpp