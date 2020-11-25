#ifndef OITL_CONCEPTS_HPP
#define OITL_CONCEPTS_HPP //C++ Header oitl_concepts.hpp

#ifdef __GNUC__
    #if __cplusplus>=201709L && __GNUC__>=10
        #undef _OITL_CONCEPT_AVAILABLE
        #define _OITL_CONCEPT_AVAILABLE 1
    #endif
#endif

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

} // namespace oitl::concepts

} // namespace oitl

#endif

#endif //C++ Header oitl_concepts.hpp