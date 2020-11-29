#ifndef _OITL_UTILITY_OITL_CONCEPTS_HPP
#define _OITL_UTILITY_OITL_CONCEPTS_HPP //C++ Header oitl_concepts.hpp

#include"oitl_def.hpp"

#ifdef _OITL_CONCEPT_AVAILABLE // The lib is available only if <concepts> is available

#include<concepts>
#include<memory>

namespace oitl
{

namespace concepts
{

template<typename _Alloc>
concept allocator=
requires(_Alloc _alloc)
{
    typename _Alloc::value_type;
    {_alloc.allocate(0ULL)}->std::same_as<typename std::allocator_traits<_Alloc>::pointer>;
    _alloc.deallocate(nullptr,0ULL);
    {_alloc==_alloc}noexcept->std::same_as<bool>;
    {_alloc!=_alloc}noexcept->std::same_as<bool>;
    {_Alloc(_alloc)}noexcept;
};

template<typename _Alloc,typename _Tp>
concept allocator_of_type=
	allocator<_Alloc>
	&&std::same_as<_Tp,typename _Alloc::value_type>;

template<typename _Tp,typename _Cmp,typename _Alloc>
concept ordered_associative_container_general_constraint=
	std::movable<_Tp>
	&&std::relation<_Cmp,_Tp,_Tp>
	&&allocator_of_type<_Alloc,_Tp>;

template<typename _Tp,typename _Func,typename _Alloc>
concept interval_calculating_container_general_constraint=
	std::movable<_Tp>
	&&allocator_of_type<_Alloc,_Tp>
    &&requires(_Func _fn,_Tp _val)
        {
            {_fn(_val,_val)}->std::same_as<_Tp>;
        };

} // namespace oitl::concepts

} // namespace oitl

#endif

#endif //C++ Header oitl_concepts.hpp