#ifndef _OITL_ALGORITHM_MERGE_BASE_HPP
#define _OITL_ALGORITHM_MERGE_BASE_HPP // C++ Header merge_base.hpp

#include<functional>

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

} //namespace oitl

#endif //C++ Header merge_base.hpp
