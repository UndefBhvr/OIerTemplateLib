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
	void swap(_Tp &x,_Tp &y){
		_Tp temp=x;
		x=y;
		y=temp;
	}
	template<typename _Tp>
	class vector{
	private:
		_Tp * _m_array;
		size_t sizes,use_len;//used:[start,start+use_len),used in user:[start,start+size)
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
		vector(const vector<_Tp> &_vec);
		~vector();
		
		void push_back(const _Tp &_val);
		void pop_back();
		const _Tp &at(const size_t &_pos) const;
		_Tp &at(const size_t &_pos);
		
		const _Tp &operator[](const size_t &_pos) const;
		_Tp &operator[](const size_t &_pos);
		const vector<_Tp> &operator=(const vector<_Tp> &_vec);
#if _OITL_LANG_VER>=201103L
		const vector<_Tp> &operator=(vector<_Tp> &&_vec) noexcept;
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
		void resize(const size_t &len);
		void insert(const_iterator _pos,const _Tp &_val);
		void erase(const_iterator _pos);
		void erase(const_iterator _begin,const_iterator _end);
	};
	
	template<typename _Tp>
	vector<_Tp>::vector(const size_t &_siz){
		if(_siz==0){
			use_len=1;
		} else{
			use_len=_siz;
		}
		_m_array=new _Tp[use_len];
		sizes=0;
	}
	template<typename _Tp>
	vector<_Tp>::vector(const _Tp * _begin,const _Tp * _end){
		_m_array=new _Tp[1];
		sizes=0;
		use_len=1;
		while(_begin!=_end){
			this->push_back(*_begin);
			++_begin;
		}
	}
#if _OITL_LANG_VER>=201103L
	template<typename _Tp>
	vector<_Tp>::vector(const std::initializer_list<_Tp> &_vec){
		_m_array=new _Tp[1];
		sizes=0;
		use_len=1;
		for(typename std::initializer_list<_Tp>::const_iterator it=_vec.begin();it!=_vec.end();++it){
			this->push_back(*it);
		}
	}
#endif
	template<typename _Tp>
	vector<_Tp>::vector(const vector<_Tp> &_vec){
		use_len=_vec.use_len;
		sizes=_vec.sizes;
		_m_array=new _Tp[use_len];
		for(size_t i=0;i<sizes;++i){
			_m_array[i]=_vec._m_array[i];
		}
	}
	template<typename _Tp>
	vector<_Tp>::~vector(){
		delete[] _m_array;
	}
	template<typename _Tp>
	void vector<_Tp>::push_back(const _Tp &_val){
		if(sizes==use_len){
			resize(use_len<<1);
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
	const _Tp &vector<_Tp>::at(const size_t &_pos) const{
		if(_pos<sizes){
			return (*this)[_pos];
		} else{
			throw std::exception(string(__func__)+":不能访问此位置！\n");
		}
	}
	template<typename _Tp>
	_Tp &vector<_Tp>::at(const size_t &_pos){
		if(_pos<sizes){
			return (*this)[_pos];
		} else{
			throw std::exception(string(__func__)+":不能访问此位置！\n");
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
	template<typename _Tp>
	const vector<_Tp> &vector<_Tp>::operator=(const vector<_Tp> &_vec){
		this->~vector();
		use_len=_vec.use_len;
		sizes=_vec.sizes;
		_m_array=new _Tp[use_len];
		for(size_t i=0;i<sizes;++i){
			_m_array[i]=_vec._m_array[i];
		}
		return (*this);
	}
#if _OITL_LANG_VER>=201103L
	template<typename _Tp>
	const vector<_Tp> &vector<_Tp>::operator=(vector<_Tp> &&_vec) noexcept{
		this->~vector();
		use_len=_vec.use_len;
		sizes=_vec.sizes;
		_m_array=new _Tp[use_len];
		for(size_t i=0;i<sizes;++i){
			_m_array[i]=_vec._m_array[i];
		}
		_vec.~vector();
		return (*this);
	}
#endif
	template<typename _Tp>
	void vector<_Tp>::resize(const size_t &len){
		use_len=len;
		_Tp * tmp=new _Tp[len];
		for(size_t i=0;i<sizes;++i){
			tmp[i]=_m_array[i];
		}
		delete[] _m_array;
		_m_array=tmp;
	}
	template<typename _Tp>
	void vector<_Tp>::insert(const_iterator _pos,const _Tp &_val){
		push_back(back());
		for(iterator it=end();it!=_pos;--it){
			*it=*(it-1);
		}
		_m_array[_pos-begin()]=_val;
	}
	template<typename _Tp>
	void vector<_Tp>::erase(const_iterator _pos){
		for(size_t i=(_pos-begin());i<sizes-1;++i){
			swap(_m_array[i],_m_array[i+1]);
		}
		pop_back();
	}
	template<typename _Tp>
	void vector<_Tp>::erase(const_iterator _begin,const_iterator _end){
		--_end;
		while(_end!=_begin){
			erase(_end);
			--_end;
		}
		erase(_begin);
	}
#if _OITL_LANG_VER<201103L
#undef nullptr
#endif
	
} //namespace oitl

#endif //C++ Header vector.hpp
