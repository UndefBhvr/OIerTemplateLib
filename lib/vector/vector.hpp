#ifndef _OITL_DATA_STRUCTURE_VECTOR_HPP
#define _OITL_DATA_STRUCTURE_VECTOR_HPP//C++ Header vector.hpp

#include<cstddef>
#include<memory>
#include<utility>
#include<functional>


#if defined(__has_include) && __has_include("../../utility/oitl_def.hpp")
#include"../../utility/oitl_def.hpp"
#include"../../utility/oitl_concepts.hpp"
#else //When you want to use this with dependence on only libstdc++
#define _OITL_LANG_VER __cplusplus
#ifdef _MSC_VER
#undef _OITL_LANG_VER
#define _OITL_LANG_VER _MSVC_LANG
#endif
#endif // __has_include the necessary headers

#if _OITL_LANG_VER>=201103L
#include<initializer_list>
#include<iterator>
#endif

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
	
	template<typename _Tp>
	class vector{
	private:
		_Tp * _m_array;
		size_t sizes,use_len;//used:[start,start+use_len),used in user:[start,start+size)
	public:
		typedef _Tp value_type;
		typedef _Tp* pointer;
		typedef __gnu_cxx::__normal_iterator<pointer,vector> iterator;
	public:
//		struct iterator{
//		private:
//			_Tp * it;
//		public:
//			typedef _Tp value_type;
//			typedef _Tp& refence;
//			typedef _Tp* pointer;
//			typedef ptrdiff_t difference_type;
//			typedef std::random_access_iterator_tag iterator_category;
//		public:
//			iterator(_Tp * const _it):it(_it){}
//			const iterator &operator++(){
//				++it;
//				return (*this);
//			}
//			const iterator &operator++(int){
//				const iterator tmp=(*this);
//				++it;
//				return tmp;
//			}
//			const iterator &operator--(){
//				--it;
//				return (*this);
//			}
//			const iterator &operator--(int){
//				const iterator tmp=(*this);
//				--it;
//				return tmp;
//			}
//			inline const _Tp &operator*() const{
//				return *it;
//			}
//			inline _Tp &operator*(){
//				return *it;
//			}
//			inline const iterator operator+(const difference_type &_add_num) const{
//				return iterator(it+_add_num);
//			}
//			inline const iterator operator-(const difference_type &_add_num) const{
//				return iterator(it-_add_num);
//			}
//			iterator &operator+=(const difference_type &_add_num){
//				it+=_add_num;
//				return (*this);
//			}
//			iterator &operator-=(const difference_type &_add_num){
//				it-=_add_num;
//				return (*this);
//			}
//			inline const bool operator==(const iterator &rhs) const{
//				return it==rhs.it;
//			}
//			inline const bool operator!=(const iterator &rhs) const{
//				return !((*this)==rhs);
//			}
//			inline const bool operator<(const iterator &rhs) const{
//				return it<rhs.it;
//			}
//			inline const bool operator<=(const iterator &rhs) const{
//				return ((*this)==rhs) || ((*this)<rhs);
//			}
//			inline const bool operator>(const iterator &rhs) const{
//				return it>rhs.it;
//			}
//			inline const bool operator>=(const iterator &rhs) const{
//				return ((*this)==rhs) || ((*this)>rhs);
//			}
//			inline const difference_type operator-(const iterator &rhs) const{
//				return it-rhs.it;
//			}
//		};
	public:
		vector();
		vector(const _Tp * _begin,const _Tp * _end);
#if _OITL_LANG_VER>=201103L
		vector(const std::initializer_list<_Tp> &_vec);
#endif
		~vector();
		
		void push_back(const _Tp &_val);
		void pop_back();
		
		const _Tp &operator[](const size_t &_pos) const;
		_Tp &operator[](const size_t &_pos);

		inline size_t size() const{
			return sizes;
		}
		inline iterator begin() const{
			return iterator(_m_array);
		}
		inline iterator end() const{
			return iterator(_m_array+sizes);
		}
	};
	
	template<typename _Tp>
	vector<_Tp>::vector(){
		_m_array=new _Tp[1];
		sizes=0;
		use_len=1;
	}
	template<typename _Tp>
	vector<_Tp>::vector(const _Tp * _begin,const _Tp * _end){
		sizes=use_len=(_end-_begin);
		++use_len;
		_m_array=new _Tp[use_len];
		for(size_t i=0;_begin!=_end;++_begin,++i){
			_m_array[i]=*_begin;
		}
	}
#if _OITL_LANG_VER>=201103L
	template<typename _Tp>
	vector<_Tp>::vector(const std::initializer_list<_Tp> &_vec){
		vector(_vec.begin(),_vec.end());
	}
#endif
	template<typename _Tp>
	vector<_Tp>::~vector(){
		delete[] _m_array;
	}
	template<typename _Tp>
	void vector<_Tp>::push_back(const _Tp &_val){
		if(sizes==use_len){
			_Tp * tmp=new _Tp[use_len<<=1];
			memcpy(tmp,_m_array,sizes);
			delete[] _m_array;
			_m_array=tmp;
		}
		_m_array[sizes++]=_val;
	}
	template<typename _Tp>
	void vector<_Tp>::pop_back(){
		if(sizes==0){
			return;//error!
		} else{
			--sizes;
		}
	}
	template<typename _Tp>
	const _Tp &vector<_Tp>::operator[](const size_t &_pos) const{
		return _m_array[_pos];
	}
	template<typename _Tp>
	_Tp &vector<_Tp>::operator[](const size_t &_pos){
		return _m_array[_pos];
	}
#if _OITL_LANG_VER<201103L
#undef nullptr
#endif
	
} //namespace oitl

#endif //C++ Header vector.hpp
