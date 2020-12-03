// #include"lib/utility/oitl_concepts.hpp"
#include<cstdio>
#include"lib/data_structure/priority_queue/pairing_heap.hpp"
// #include"lib/data_structure/priority_queue/elmasry_pairing_heap.hpp"
// #include"lib/data_structure/priority_queue/leftist_heap.hpp"
// #include"lib/data_structure/balanced_tree/test.hpp"

#include<cstdio>
#include<cctype>
// #include<coroutine>
// #include<experimental/coroutine>

// #include<tr1/random>
#include<set>

std::set<int> a;

oitl::pairing_heap<int> heap;
#include<concepts>
#include<type_traits>

template<typename _Tp>
concept is_int=std::same_as<_Tp,int>;

template<typename _Tp>
_Tp gcd(_Tp a,_Tp b)
{
	if(b==0)return a;
	return gcd(b,a%b);
}

template<is_int _Tp>
_Tp gcd(_Tp a,_Tp b)
{
	if(b==0)return a;
	return gcd(b,a%b);
}

template<typename _Tp>
struct remove_reference
{
	typedef _Tp type;
};

template<typename _Tp>
struct remove_reference<_Tp&>
{
	typedef _Tp type;
};

template<typename _Tp>
struct remove_reference<_Tp&&>
{
	typedef _Tp type;
};

template<typename _Tp>
using remove_reference_t=remove_reference<_Tp>::type;

template<typename _Tp>
_Tp&& forward(remove_reference_t<_Tp>& arg)
{
	return static_cast<_Tp&&>(arg);
}

template<typename ..._Args>
struct value_type_of
{
};

// template<typename _Tp>
// struct value_type_of<_Tp::value_type>
// {
// 	typedef typename _Tp::value_type value_type;
// };

int main()
{
	// value_type_of<std::allocator<int> >::type a;
	printf("__cpp_concepts= %ld\n",__cpp_concepts);
	// heap.push(3);
	gcd(3LL,5LL);
}

// char gc()
// {
//     static char buf[1<<15],*p1=buf,*p2=buf;
//     if(p1==p2)
//     {
//         p2=(p1=buf)+fread(buf,1,1<<15,stdin);
//         if(p1==p2)return EOF;
//     }
//     return *p1++;
// }

// #define gc getchar

// #include<iostream>

// template<typename _Tp>
// void read(_Tp &x)
// {
//     x=0;
//     char c=gc();
//     while(!isdigit(c))c=gc();
//     while(isdigit(c))x=x*10-48+c,c=gc();
// 	// std::cin>>x;
// }

// int n,m;
// unsigned long long last,ans;

// int main()
// {
// 	oitl::splay_tree<unsigned long long,std::less<unsigned long long> > my_tree;
//     /*read(n),*/read(m);
//     // for(int i=1;i<=n;++i)
//     // {
//     //     unsigned long long x;
//     //     read(x);
//     //     my_tree.insert((x<<25LL)+i);
//     // }
//     for(int i=0;i<m;++i)
//     {
//         int op;
//         read(op);
//         unsigned long long x;
//         read(x);
// 		last=0;
// 		ans=0;
//         x^=last;
//         if(op==1)
//         {
//             my_tree.insert((x<<25LL)+i);
//         }
//         if(op==2)
//         {
//             my_tree.erase(*my_tree.lower_bound(x<<25LL));
//         }
//         if(op==3)
//         {
//             ans^=(last=my_tree.order_of_key(x<<25LL)+1);
//         }
//         if(op==4)
//         {
//             ans^=(last=((*my_tree.find_by_order(x-1))>>25LL));
//         }
//         if(op==5)
//         {
//             ans^=((last=(*--my_tree.lower_bound(x<<25LL))>>25LL));
//         }
//         if(op==6)
//         {
//             ans^=((last=(*my_tree.upper_bound((x<<25LL)+n+m))>>25LL));
//         }
//     	if(op>2)printf("%llu\n",ans);
// 		// if(op>2)std::cout<<ans<<'\n';
//     }
// }