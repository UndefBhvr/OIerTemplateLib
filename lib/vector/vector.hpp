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
	inline void swap(_Tp &a,_Tp &b){
		_Tp tmp=a;
		a=b;
		b=tmp;
	}
	template<typename _Tp,typename _Alloc=std::allocator<_Tp>>
	class vector;
	template<typename _Tp,typename _Alloc>
	void swap(vector<_Tp,_Alloc> &lhs,vector<_Tp,_Alloc> &rhs);
	template<typename _Tp,typename _Alloc>
	class vector{
	private:
		_Tp * _m_array;
		size_t sizes,use_len;//used:[start,start+use_len),used in user:[start,start+size)
		_Alloc _allocator;
		friend void swap<_Tp,_Alloc>(vector<_Tp,_Alloc> &lhs,vector<_Tp,_Alloc> &rhs);
	public:
		typedef _Tp value_type;
		typedef _Tp* pointer;
		typedef const _Tp* const_pointer;
		typedef __gnu_cxx::__normal_iterator<pointer,vector> iterator;
		typedef __gnu_cxx::__normal_iterator<const_pointer,vector> const_iterator;
	public:
		vector(const _Tp * _begin,const _Tp * _end);
		vector(const size_t &_siz=1ULL);
#if _OITL_LANG_VER>=201103L
		vector(const std::initializer_list<_Tp> &_vec);
#endif
		vector(const vector<_Tp,_Alloc> &_vec);
		~vector();
		
		void push_back(const _Tp &_val);
		void pop_back();
		const _Tp &at(const size_t &_pos) const;
		_Tp &at(const size_t &_pos);
		
		const _Tp &operator[](const size_t &_pos) const;
		_Tp &operator[](const size_t &_pos);
		const vector<_Tp,_Alloc> &operator=(const vector<_Tp,_Alloc> &_vec);
#if _OITL_LANG_VER>=201103L
		const vector<_Tp,_Alloc> &operator=(vector<_Tp,_Alloc> &&_vec) noexcept;
#endif
		
		inline size_t size() const{
			return sizes;
		}
		inline bool empty() const{
			return size()==0;
		}
		inline iterator begin(){
			return iterator(_m_array);
		}
		inline const_iterator begin() const{
			return const_iterator(_m_array);
		}
		inline iterator end(){
			return iterator(_m_array+sizes);
		}
		inline const_iterator end() const{
			return const_iterator(_m_array+sizes);
		}
		inline const_iterator cbegin() const{
			return const_iterator(_m_array);
		}
		inline const_iterator cend() const{
			return const_iterator(_m_array+sizes);
		}
		inline const _Tp &back() const{
			return _m_array[size()-1];
		}
		inline _Tp &back(){
			return _m_array[size()-1];
		}
		inline size_t capacity() const{
			return use_len;
		}
		inline void shrink_to_fit(){
			use_len=sizes;
		}
		void resize(const size_t &len);
		void insert(const_iterator _pos,const _Tp &_val);
		void erase(const_iterator _pos);
		void erase(const_iterator _begin,const_iterator _end);
	};
	template<typename _Tp,typename _Alloc>
	vector<_Tp,_Alloc>::vector(const size_t &_siz){
		if(_siz==0){
			use_len=1;
		} else{
			use_len=_siz;
		}
		_m_array=_allocator.allocate(use_len);
		sizes=0;
	}
	template<typename _Tp,typename _Alloc>
	vector<_Tp,_Alloc>::vector(const _Tp * _begin,const _Tp * _end){
		_m_array=_allocator.allocate(_end-_begin+1);
		use_len=_end-_begin+1;
		sizes=0;
		while(_begin!=_end){
			_allocator.construct(_m_array+sizes,*_begin);
			++sizes;
			++_begin;
		}
	}
#if _OITL_LANG_VER>=201103L
	template<typename _Tp,typename _Alloc>
	vector<_Tp,_Alloc>::vector(const std::initializer_list<_Tp> &_vec){
		_m_array=_allocator.allocate(_vec.size()+1);
		use_len=_vec.size()+1;
		sizes=0;
		for(typename std::initializer_list<_Tp>::const_iterator it=_vec.begin();it!=_vec.end();++it){
			_allocator.construct(_m_array+sizes,*it);
			++sizes;
		}
	}
#endif
	template<typename _Tp,typename _Alloc>
	vector<_Tp,_Alloc>::vector(const vector<_Tp,_Alloc> &_vec){
		_m_array=_allocator.allocate(_vec.size());
		use_len=_vec.use_len;
		sizes=_vec.sizes;
		for(size_t i=0;i<sizes;++i){
			_allocator.construct(_m_array+i,_vec[i]);
		}
	}
	template<typename _Tp,typename _Alloc>
	vector<_Tp,_Alloc>::~vector(){
		for(size_t i=0;i<use_len;++i){
			_allocator.destroy(_m_array+i);
		}
		_allocator.deallocate(_m_array,use_len);
	}
	template<typename _Tp,typename _Alloc>
	void vector<_Tp,_Alloc>::push_back(const _Tp &_val){
		if(sizes==use_len){
			resize(use_len<<1);
		}
		_allocator.construct(_m_array+sizes,_val);
		++sizes;
	}
	template<typename _Tp,typename _Alloc>
	void vector<_Tp,_Alloc>::pop_back(){
		if(sizes==0){
			return;//error!
		} else{
			--sizes;
			_allocator.destroy(_m_array+sizes);
		}
	}
	template<typename _Tp,typename _Alloc>
	const _Tp &vector<_Tp,_Alloc>::at(const size_t &_pos) const{
		if(_pos<sizes){
			return (*this)[_pos];
		} else{
			throw std::exception(string(__func__)+":不能访问此位置！\n");
		}
	}
	template<typename _Tp,typename _Alloc>
	_Tp &vector<_Tp,_Alloc>::at(const size_t &_pos){
		if(_pos<sizes){
			return (*this)[_pos];
		} else{
			throw std::exception(string(__func__)+":不能访问此位置！\n");
		}
	}
	template<typename _Tp,typename _Alloc>
	const _Tp &vector<_Tp,_Alloc>::operator[](const size_t &_pos) const{
		return _m_array[_pos];
	}
	template<typename _Tp,typename _Alloc>
	_Tp &vector<_Tp,_Alloc>::operator[](const size_t &_pos){
		return _m_array[_pos];
	}
	template<typename _Tp,typename _Alloc>
	const vector<_Tp,_Alloc> &vector<_Tp,_Alloc>::operator=(const vector<_Tp,_Alloc> &_vec){
		if(_vec.sizes<use_len){
			for(size_t i=0ULL;i<sizes;++i){
				_allocator.destroy(_m_array+i);
				if(_vec.sizes>i)_allocator.construct(_m_array+i,_vec[i]);
			}
			for(size_t i=sizes;i<_vec.sizes;++i){
				_allocator.construct(_m_array+i,_vec[i]);
			}
			sizes=_vec.sizes;
		} else{
			this->~vector();
			_m_array=_allocator.allocate(_vec.size());
			use_len=_vec.use_len;
			sizes=_vec.sizes;
			for(size_t i=0;i<sizes;++i){
				_allocator.construct(_m_array+i,_vec[i]);
			}
		}
		return (*this);
	}
#if _OITL_LANG_VER>=201103L
	template<typename _Tp,typename _Alloc>
	const vector<_Tp,_Alloc> &vector<_Tp,_Alloc>::operator=(vector<_Tp,_Alloc> &&_vec) noexcept{
		swap(_m_array,_vec._m_array);
		use_len=_vec.use_len;
		sizes=_vec.sizes;
		_allocator=_vec._allocator;
		_vec.use_len=0;
		_vec.sizes=0;
		return (*this);
	}
#endif
	template<typename _Tp,typename _Alloc>
	void vector<_Tp,_Alloc>::resize(const size_t &len){
		_Tp * tmp=_allocator.allocate(len);
		for(size_t i=0;i<sizes;++i){
			_allocator.construct(tmp+i,_m_array[i]);
			_allocator.destroy(_m_array+i);
		}
		_allocator.deallocate(_m_array,use_len);
		_m_array=tmp;
		use_len=len;
	}
	template<typename _Tp,typename _Alloc>
	void vector<_Tp,_Alloc>::insert(const_iterator _pos,const _Tp &_val){
		push_back(back());
		for(iterator it=end();it!=_pos;--it){
			*it=*(it-1);
		}
		_m_array[_pos-begin()]=_val;
	}
	template<typename _Tp,typename _Alloc>
	void vector<_Tp,_Alloc>::erase(const_iterator _pos){
		for(size_t i=(_pos-begin());i<sizes-1;++i){
			swap(_m_array[i],_m_array[i+1]);
		}
		pop_back();
	}
	template<typename _Tp,typename _Alloc>
	void vector<_Tp,_Alloc>::erase(const_iterator _begin,const_iterator _end){
		size_t distance=_end-_begin;
		for(size_t nowi=_begin-begin();nowi+distance<sizes;++nowi){
			_m_array[nowi]=_m_array[nowi+distance];
		}
		while(distance--){
			pop_back();
		}
		sizes-=distance;
	}
	template<typename _Tp,typename _Alloc>
	void swap(vector<_Tp,_Alloc> &lhs,vector<_Tp,_Alloc> &rhs){
		swap(lhs._m_array,rhs._m_array);
		swap(lhs.use_len,rhs.use_len);
		swap(lhs.sizes,rhs.sizes);
	}
#if _OITL_LANG_VER<201103L
#undef nullptr
#endif
	
} //namespace oitl

#endif //C++ Header vector.hpp
