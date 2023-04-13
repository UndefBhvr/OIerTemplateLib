#ifndef _OITL_DATA_STRUCTURE_PRIORITY_QUEUE_PAIRING_HEAP_HPP
#define _OITL_DATA_STRUCTURE_PRIORITY_QUEUE_PAIRING_HEAP_HPP//C++ Header pairing_heap.hpp

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
	
	constexpr const size_t _vector_memory_limit=1000;
	template<typename _Tp>
	class vector{
	private:
		struct per_mem_node{
			per_mem_node * last,* next;
			const size_t id;
			_Tp _m_array[_vector_memory_limit];
			per_mem_node(const size_t &_id,const _Tp& _arr_default_nums=_Tp(),per_mem_node * _creat_from=nullptr):next(nullptr),id(_id){
				for(size_t i=0;i<_vector_memory_limit;++i){
					_m_array[i]=_arr_default_nums;
				}
			}
			
			inline const bool operator==(const per_mem_node &rhs){
				return id==rhs.id;
			}
		};
		per_mem_node * _begin_node,* _end_node;
		size_t _end_top;//from _end_code:[0,_end_top)
	public:
		struct iterator{
			per_mem_node * _now_mem_pos;
			size_t _now_node_pos;
			bool _fail_bit;
			iterator(const per_mem_node * __mem_pos=nullptr,const size_t __node_pos=nullptr):_now_node_pos(__mem_pos),_now_node_pos(__node_pos),_fail_bit(false){}
			const iterator &operator++(){
				if(*_now_mem_pos==*_end_node && _now_node_pos+1>=_end_top){
					_fail_bit=true;
				}
				if(_fail_bit)	return iterator();
				++_now_node_pos;
				if(_now_node_pos>=_vector_memory_limit){
					if(_now_mem_pos->next==nullptr){
						_fail_bit=true;
					} else{
						_now_mem_pos=_now_mem_pos->next;
						_now_node_pos=0;
					}
				}
				if(_fail_bit)	return iterator();
				else	return (*this);
			}
			const iterator &operator++(int){
				const iterator tmp=(*this);
				++(*this);
				return tmp;
			}
			const iterator &operator--(){
				if(*_now_mem_pos==*_begin_node && _now_node_pos==0){
					_fail_bit=true;
				}
				if(_fail_bit)	return iterator();
				if(_now_node_pos){
					--_now_node_pos;
				} else{
					if(_now_mem_pos->last==nullptr){
						_fail_bit=true;
					} else{
						_now_mem_pos=_now_mem_pos->last;
						_now_node_pos=_vector_memory_limit-1;
					}
				}
				if(_fail_bit)	return iterator();
				else	return (*this);
			}
			
		};
	public:
		vector();
		vector(const _Tp * _begin_it,const _Tp * _end_it);
#if _OITL_LANG_VER>=201103L
		vector(const std::initializer_list<_Tp> &list);
#endif
		~vector();
		
		void push_back(const _Tp &_val);
		void pop_back();
		const _Tp &at(const size_t &_pos) const;
		_Tp &at(const size_t &_pos);
		
		const _Tp &operator[](const size_t &_pos) const;
		_Tp &operator[](const size_t &_pos);
	};
	
	template<typename _Tp>
	vector<_Tp>::vector(){
		_begin_node=new per_mem_node(1);
		_end_node=_begin_node;
		_end_top=0;
	}
	template<typename _Tp>
	vector<_Tp>::vector(const _Tp * begin_it,const _Tp * _end_it){
		const _Tp * it=begin_it;
		while(it!=_end_it){
			this->push_back(*it);
			++it;
		}
	}
#if _OITL_LANG_VER>=201103L
	template<typename _Tp>
	vector<_Tp>::vector(const std::initializer_list<_Tp> &list){
		for(typename std::initializer_list<_Tp>::iterator it=list.begin();it!=list.end();++it){
			this->push_back(*it);
		}
	}
#endif
	template<typename _Tp>
	vector<_Tp>::~vector(){
		for(per_mem_node * it=_begin_node;it!=_end_node;){
			if(it==nullptr)	return;
			else if(it->next==nullptr){
				delete it;
				return;
			}
			it=it->next;
			delete it->last;
		}
	}
	template<typename _Tp>
	void vector<_Tp>::push_back(const _Tp &val){
		_end_node->_m_array[_end_top++]=val;
		if(_end_top>_vector_memory_limit){
			per_mem_node * new_arr=new per_mem_node(_end_node->id+1);
			_end_node->next=new_arr;
			new_arr->last=_end_node;
			_end_node=new_arr;
			_end_top=0;
		}
	}
	template<typename _Tp>
	void vector<_Tp>::pop_back(){
		if(_end_top)	--_end_top;
		else{
			if(_end_node->last==nullptr){
				return;//Error!
			} else{
				_end_node=_end_node->last;
				delete _end_node->next;
				_end_node->next=nullptr;
				_end_top=_vector_memory_limit;
			}
		}
	}
	template<typename _Tp>
	_Tp  &vector<_Tp>::operator[](const size_t &_pos){
		if(_pos==0)	return _begin_node->_m_array[0];
		size_t findid=_pos/_vector_memory_limit+(_pos%_vector_memory_limit?1:0);
		per_mem_node * it=_begin_node;
		while(it->id!=findid){
			++it;
		}
		return it->_m_array[_pos%_vector_memory_limit];
	}
	
#if _OITL_LANG_VER<201103L
#undef nullptr
#endif
	
} //namespace oitl

#endif //C++ Header vector.hpp
