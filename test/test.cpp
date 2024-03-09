#include "../src/cpu.hpp"

#include<algorithm>

void std_sort(int*arr,uint64_t len)
{
	std::sort(arr,arr+len);
}

#include<chrono>
#include<string>
#include<string.h>
#include<iostream>
#include<random>

struct Clock
{
	std::chrono::steady_clock::time_point start;
	Clock():start(std::chrono::steady_clock::now()){}
	~Clock()
	{
		auto end=std::chrono::steady_clock::now();
		std::chrono::duration<double> duration=end-start;
		std::cout<<' '<<duration<<"\n";
	}
};


struct Tester
{
	static constexpr uint64_t max_len=1ull<<30;
	int*init_data=new int[max_len];
	int*sorted_data=new int[max_len];
	int*data=new int[max_len];

	uint64_t len;
	uint64_t k;

	void init()
	{
		std::mt19937 rnd(114514);
		for(int i=0;i<len;i++)
			init_data[i]=rnd();
		memcpy(sorted_data,init_data,len*sizeof(int));
		std::sort(sorted_data,sorted_data+len);
	}

	bool check()
	{
		return memcmp(data,sorted_data,len*sizeof(int))==0;
	}

	template<typename Func>
	void test(Func&&fun,std::string_view name)
	{
		std::cout<<name;
		Clock clock;
		for(uint64_t cnt=k;cnt--;)
		{
			memcpy(data,init_data,len*sizeof(int));
			fun(data,len);
		}
		
		if(!check())
			std::cout<<"(Error)";
	}

	void test_all(uint64_t len,uint64_t k=1)
	{	
		this->len=len;
		this->k=k;

		printf(">>>>>>>>>test len=%llu  k=%llu<<<<<<<<<\n\n",len,k);
		init();
		
		test(bitonic_sort_native<Inc>,"Bitonic Sort(Native)");
		test(std_sort,"std::sort");


		printf("\n\n");
	}
};



int main()
{
	Tester tester;
	tester.test_all(1<<16,1<<8);
	tester.test_all(1<<20,1<<4);
	tester.test_all(1<<24,1);
	
}
