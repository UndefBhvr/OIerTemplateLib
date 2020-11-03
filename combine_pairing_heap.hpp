#ifndef COMBINE_PAIRING_HEAP_HPP
#define COMBINE_PAIRING_HEAP_HPP //C++ Header combone_pairing_heap.hpp

#include<algorithm>
#include<set>
#include<queue>

namespace oitl
{

#if __cplusplus<201103L
	#ifdef nullptr
		#undef nullptr
	#endif
	#define nullptr NULL
#endif

/**
 * There are some problem
 * First modify() doesn't support increase a key
 * Second it depends on STL
 * 
 * TODO:
 * 1.Update modify()
 * 2.Remove the dependence on STL
 */

template<typename _Tp,typename _Cmp=std::less<_Tp>,typename _Alloc=std::allocator<_Tp> >
class combine_pairing_heap:private _Cmp
{
	struct _const_iterator;

	public:

	typedef _Tp value_type;
	typedef _Cmp compare_type;
	typedef _Alloc alloc_type;
    typedef typename _Alloc::size_type size_type;
#if __cplusplus>=201103L
    typedef std::allocator_traits<_Alloc> alloc_traits_type;
#endif
	typedef _const_iterator iterator;
	typedef _const_iterator const_iterator;

    combine_pairing_heap():
        insert_buffer(nullptr),
        tree_pool(nullptr),
        top_node(nullptr)
    {}


    iterator push(_Tp);
    void pop();
    const _Tp &top()const;
    void modify(const const_iterator&,_Tp);
	void join(combine_pairing_heap&);
	size_type size()const;
	bool empty()const;


	private: 

	struct Node;

#if __cplusplus>=201103L
    typedef typename alloc_traits_type::template rebind_traits<Node> node_alloc_traits_type;
    typedef typename node_alloc_traits_type::allocator_type node_alloc_type;
    typedef typename alloc_traits_type::template rebind_traits<Node*>::allocator_type node_ptr_alloc_type;
#else
	typedef typename _Alloc::template rebind<Node>::other node_alloc_type;
    typedef typename _Alloc::template rebind<Node*>::other node_ptr_alloc_type;
#endif
	node_alloc_type _node_allocator;

	struct ptr_cmp:_Cmp
	{
	   bool operator()(const Node *l,const Node *r)const
       {
           return _Cmp::operator()(r->val,l->val);
       }
	};

    Node *insert_buffer,*tree_pool,*top_node;
    size_type _size;
	std::set<Node*,std::less<Node*>,node_ptr_alloc_type> tree_pool_mem;
		
	inline Node *__get_node(_Tp);
	inline void __delete_node(Node*);

	void __clear();
	void move(combine_pairing_heap&);
	void push_to_tree_pool(Node*);
    Node *link(Node*,Node*);
    Node *two_pass(Node*);
    Node *multi_pass(Node*);
    void combine();

};


template<typename _Tp,typename _Cmp,typename _Alloc>
struct combine_pairing_heap<_Tp,_Cmp,_Alloc>::Node
{
	_Tp val;
	Node *left,*sibling,*child;

	Node(_Tp _val,Node *child_ptr=nullptr):
	   val(_val),
	   left(nullptr),
	   sibling(nullptr),
	   child(child_ptr)
	{
	}
};

template<typename _Tp,typename _Cmp,typename _Alloc>
struct combine_pairing_heap<_Tp,_Cmp,_Alloc>::_const_iterator
{
    private:

        Node *real_node;
		friend class combine_pairing_heap;
        _const_iterator(Node* ptr):real_node(ptr) {}

    public:
		_const_iterator():real_node(nullptr){}

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
inline
typename
combine_pairing_heap<_Tp,_Cmp,_Alloc>::Node*
combine_pairing_heap<_Tp,_Cmp,_Alloc>::__get_node(_Tp __value)
{
#if __cplusplus>=201103L
    Node *new_ptr=node_alloc_traits_type::allocate(_node_allocator,1);
    node_alloc_traits_type::construct(_node_allocator,new_ptr,__value,new_ptr);
#else
	Node *new_ptr=_node_allocator.allocate(1);
	_node_allocator.construct(new_ptr,Node(__value,new_ptr));
#endif
    return new_ptr;
}

template<typename _Tp,typename _Cmp,typename _Alloc>
inline
void
combine_pairing_heap<_Tp,_Cmp,_Alloc>::__delete_node(Node *__ptr)
{
#if __cplusplus>=201103L
    node_alloc_traits_type::destroy(_node_allocator,__ptr);
    node_alloc_traits_type::deallocate(_node_allocator,__ptr,1);
#else
	_node_allocator.destroy(__ptr);
	_node_allocator.deallocate(__ptr,1);
#endif
}

template<typename _Tp,typename _Cmp,typename _Alloc>
void
combine_pairing_heap<_Tp,_Cmp,_Alloc>::__clear()
{
	insert_buffer=nullptr;
	tree_pool=nullptr;
	_size=0;
	top_node=nullptr;
	tree_pool_mem.clear();
}

template<typename _Tp,typename _Cmp,typename _Alloc>
void
combine_pairing_heap<_Tp,_Cmp,_Alloc>::move(combine_pairing_heap &other)
{
	insert_buffer=other.insert_buffer;
	tree_pool=other.tree_pool;
	_size=other._size;
	top_node=other.top_node;
	tree_pool_mem.swap(other.tree_pool_mem);
	other.__clear();
}

template<typename _Tp,typename _Cmp,typename _Alloc>
void
combine_pairing_heap<_Tp,_Cmp,_Alloc>::push_to_tree_pool(Node *ptr)
{
	ptr->left=nullptr;
	ptr->sibling=tree_pool;
	if(tree_pool!=nullptr)tree_pool->left=ptr;
	tree_pool=ptr;
	tree_pool_mem.insert(ptr);
	if(top_node==nullptr||_Cmp::operator()(top_node->val,ptr->val))top_node=ptr;
	if((1ULL<<tree_pool_mem.size())>_size)combine();
}


template<typename _Tp,typename _Cmp,typename _Alloc>
typename
combine_pairing_heap<_Tp,_Cmp,_Alloc>::Node*
combine_pairing_heap<_Tp,_Cmp,_Alloc>::link(Node *l,Node *r)
{
    if(l==nullptr)return r;
    if(r==nullptr)return l;
    if(_Cmp::operator()(l->val,r->val))std::swap(l,r);
    r->sibling=l->child;
    if(l->child!=nullptr)l->child->left=r;
    r->left=l;
    l->child=r;
    return l;
}

template<typename _Tp,typename _Cmp,typename _Alloc>
typename
combine_pairing_heap<_Tp,_Cmp,_Alloc>::Node*
combine_pairing_heap<_Tp,_Cmp,_Alloc>::two_pass(Node *head)
{
    if(head==nullptr||head->sibling==nullptr)return head;
    Node *other=head->sibling,*rest=head->sibling->sibling;
    head->left=nullptr;
    head->sibling=nullptr;
    other->left=nullptr;
    other->sibling=nullptr;
    if(rest!=nullptr)rest->left=nullptr;
    return link(link(head,other),two_pass(rest));
}

template<typename _Tp,typename _Cmp,typename _Alloc>
typename
combine_pairing_heap<_Tp,_Cmp,_Alloc>::Node*
combine_pairing_heap<_Tp,_Cmp,_Alloc>::multi_pass(Node *head)
{
    if(head==nullptr||head->sibling==nullptr)return head;
    static std::queue<Node*,std::deque<Node*,node_ptr_alloc_type> > ptr_que;
    Node *now=head,*other;
    while(now!=nullptr)
    {
        other=now->sibling;
        now->sibling=nullptr;
        now->left=nullptr;
        ptr_que.push(now);
        now=other;
    }
    while(ptr_que.size()>1)
    {
        now=ptr_que.front();
        ptr_que.pop();
        other=ptr_que.front();
        ptr_que.pop();
        ptr_que.push(link(now,other));
    }
    Node *res=ptr_que.front();
    ptr_que.pop();
    return res;
}

template<typename _Tp,typename _Cmp,typename _Alloc>
void
combine_pairing_heap<_Tp,_Cmp,_Alloc>::combine()
{
    for(Node *i=insert_buffer;i!=nullptr;i=i->sibling)i->child=nullptr;
    insert_buffer=multi_pass(insert_buffer);
    static std::vector<Node*,node_ptr_alloc_type> root_table;
    root_table.clear();
    for(Node *i=tree_pool,*other;i!=nullptr;i=other)
    {
        other=i->sibling;
        i->left=nullptr;
        i->sibling=nullptr;
        root_table.push_back(i);
    }
    std::sort(root_table.begin(),root_table.end(),ptr_cmp());
    tree_pool=nullptr;
    typedef std::vector<Node*,node_ptr_alloc_type> table_type;
    for(typename table_type::const_iterator iter=root_table.begin();iter!=root_table.end();++iter)
    {
        Node *ptr=*iter;
        tree_pool=link(tree_pool,ptr);
    }
    tree_pool=link(tree_pool,insert_buffer);
    insert_buffer=nullptr;
	tree_pool_mem.clear();
	tree_pool_mem.insert(tree_pool);
	top_node=tree_pool;
}

template<typename _Tp,typename _Cmp,typename _Alloc>
typename
combine_pairing_heap<_Tp,_Cmp,_Alloc>::iterator
combine_pairing_heap<_Tp,_Cmp,_Alloc>::push(_Tp Value)
{
    Node *ptr=__get_node(Value);
    if(insert_buffer!=nullptr)insert_buffer->left=ptr;
    ptr->sibling=insert_buffer;
    insert_buffer=ptr;
    if(top_node==nullptr||_Cmp::operator()(top_node->val,Value))top_node=ptr;
    ++_size;
    return iterator(ptr);
}

template<typename _Tp,typename _Cmp,typename _Alloc>
void
combine_pairing_heap<_Tp,_Cmp,_Alloc>::pop()
{
    if(top_node->child==top_node)
    {
        if(top_node->left!=nullptr)top_node->left->sibling=top_node->sibling;
        if(top_node->sibling!=nullptr)top_node->sibling->left=top_node->left;
        if(insert_buffer==top_node)insert_buffer=top_node->sibling;
    }
    else
    {
        Node *son=top_node->child;
        if(son!=nullptr)son->left=nullptr;
        son=two_pass(son);
        if(top_node->left!=nullptr)top_node->left->sibling=top_node->sibling;
        if(top_node->sibling!=nullptr)top_node->sibling->left=top_node->left;
        if(tree_pool==top_node)tree_pool=top_node->sibling;
        if(son!=nullptr)
        {
            son->sibling=tree_pool;
            tree_pool=son;
        }
    }
	--_size;
	Node *dead_node=top_node;
    combine();
    __delete_node(dead_node);
}

template<typename _Tp,typename _Cmp,typename _Alloc>
const _Tp&
combine_pairing_heap<_Tp,_Cmp,_Alloc>::top()const
{
    return top_node->val;
}

template<typename _Tp,typename _Cmp,typename _Alloc>
void
combine_pairing_heap<_Tp,_Cmp,_Alloc>::modify(const iterator& Iterator,_Tp Value)
{
	Node *ptr=(Node*)Iterator.real_node;
	if(_Cmp::operator()(top_node->val,Value))top_node=ptr;
	ptr->val=Value;
	if(ptr->child==ptr||tree_pool_mem.find(ptr)!=tree_pool_mem.end())
	{
		return;
	}
	if(ptr->child!=nullptr)
	{
		Node *replace=ptr->child;
		ptr->child=ptr->child->sibling;
		if(ptr->child!=nullptr)ptr->child->left=ptr;
		if(ptr->left!=nullptr)
		{
			if(ptr==ptr->left->child)ptr->left->child=replace;
			else ptr->left->sibling=replace;
		}
		if(ptr->sibling!=nullptr)
		{
			ptr->sibling->left=replace;
		}
		replace->left=ptr->left;
		replace->sibling=ptr->sibling;
	}
	else
	{
		if(ptr->left!=nullptr)
		{
			if(ptr==ptr->left->child)ptr->left->child=ptr->sibling;
			else ptr->left->sibling=ptr->sibling;
		}
		if(ptr->sibling!=nullptr)
		{
			ptr->sibling->left=ptr->left;
		}
	}
	push_to_tree_pool(ptr);
}

template<typename _Tp,typename _Cmp,typename _Alloc>
void
combine_pairing_heap<_Tp,_Cmp,_Alloc>::join(combine_pairing_heap &other)
{
	if(other._size==0)return;
	if(_size==0)move(other);
	if(_size>=other._size)
	{
		other.combine();
		_size+=other._size;
		push_to_tree_pool(other.tree_pool);
		other.__clear();
	}
	else
	{
		combine();
		other._size+=_size;
		other.push_to_tree_pool(tree_pool);
		__clear();
		move(other);
	}
}

template<typename _Tp,typename _Cmp,typename _Alloc>
typename
combine_pairing_heap<_Tp,_Cmp,_Alloc>::size_type
combine_pairing_heap<_Tp,_Cmp,_Alloc>::size()const
{
	return _size;
}

template<typename _Tp,typename _Cmp,typename _Alloc>
bool
combine_pairing_heap<_Tp,_Cmp,_Alloc>::empty()const
{
	return _size==0;
}

#if __cplusplus<201103L
	#undef nullptr
#endif

} //namespace oitl

#endif //C++ Header combine_pairing_heap.hpp