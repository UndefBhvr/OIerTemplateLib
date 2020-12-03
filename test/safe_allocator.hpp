#include<cassert>
#include<memory>
#include<unordered_set>

namespace oitl_test
{

template<typename _Tp>
class safe_allocator:public std::allocator<_Tp>
{
	private:
	
		std::unordered_set<_Tp*> ptr_set;

	public:

	_Tp *allocate(size_t __n)
	{
		auto ptr=std::allocator<_Tp>::allocate(__n);
		ptr_set.insert(ptr);
		return ptr;
	}

	void deallocate(_Tp *ptr,size_t __n)
	{
		assert(ptr_set.find(ptr)!=ptr_set.end());
		ptr_set.erase(ptr);
		std::allocator<_Tp>::deallocate(ptr,__n);
	}

	~safe_allocator()
	{
		assert(ptr_set.empty());
	}
};

}