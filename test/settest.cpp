#include"../lib/data_structure/balanced_tree/set.hpp"
#include"safe_allocator.hpp"
#include<ctime>


int main()
{
	size_t test;
	printf("Please input the times you want to test:");
	scanf("%zu",&test);
	auto time0=clock();
	// oitl::multiset<int,std::less<int>,oitl_test::safe_allocator<int> > my_set;
	oitl::multiset<int,std::less<int>,std::allocator<int> > my_set;
	srand(time(nullptr));
	while(test--)
	{
		if(test%1000000==0)printf("Times left: %zu millions\n",test/1000000U);
		int a=rand()%6,b=rand()*rand()%20000000;
		if(a==0)my_set.insert(b);
		if(my_set.size()==0)continue;
		if(a==1)
		{
			auto it=my_set.lower_bound(b);
			if(it==my_set.end())--it;
			if(rand()%100)continue;
			my_set.erase(*it);
		}
		if(a==2)my_set.find_by_order(b%my_set.size());
		if(a==3)
		{
			if(my_set.find(b)==my_set.end())continue;
			my_set.order_of_key(b);
		}
		if(a==4)my_set.lower_bound(b);
		if(a==5)my_set.upper_bound(b);
	}
	auto time1=clock();
	printf("Program has returned in %lf seconds\n",(time1-time0)*1./CLOCKS_PER_SEC);
}