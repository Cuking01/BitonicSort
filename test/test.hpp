#pragma once

#include<chrono>
#include<string>
#include<string.h>
#include<iostream>
#include<random>
#include<utility>
#include<string>
#include<type_traits>
#include<string_view>

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

enum class Test_Categary
{
	CPU,
	GPU
};

struct Tester
{
	static constexpr uint64_t max_len=1ull<<30;
	int*init_data=new int[max_len];
	int*sorted_data=new int[max_len];
	int*data=new int[max_len];
	int*gpu_data;

	uint64_t len;
	uint64_t k;

	Tester()
	{
		#ifdef __CUDACC__
		
			cudaMalloc((void**)&gpu_data,max_len*sizeof(int));
		#endif
	}

	struct Test_Unit
	{
		std::string name;
		void(*func)(int*arr,uint64_t len);
		Test_Categary categary;

		Test_Unit(std::string name,void(*func)(int*,uint64_t),Test_Categary categary):
			name(std::move(name)),func(func),categary(categary)
		{}
	};

	std::vector<Test_Unit> test_units;

	void add_test(std::string name,void(*func)(int*,uint64_t),Test_Categary test_c=Test_Categary::CPU)
	{
		test_units.emplace_back(std::move(name),func,test_c);
	}

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
	void test_cpu(Func&&fun,std::string_view name)
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

#ifdef __CUDACC__

	template<typename Func>
	void test_gpu(Func&&fun,std::string_view name) try
	{
		std::cout<<name;
		Clock clock;
		puts("****");
		for(uint64_t cnt=k;cnt--;)
		{
			cudaMemcpy(gpu_data,init_data,len*sizeof(int),cudaMemcpyHostToDevice);
			fun(gpu_data,len);
			cudaMemcpy(data,gpu_data,len*sizeof(int),cudaMemcpyDeviceToHost);
			cudaDeviceSynchronize();
		}
		
		if(!check())
			std::cout<<"(Error)";
	}
	catch(thrust::system_error&e)
	{
		printf("%s\n",e.what());
	}
	catch(std::exception&e)
	{
		printf("%s\n",e.what());
	}
	catch(...)
	{
		puts("*******");
	}

#endif

	void test_all(uint64_t len,uint64_t k=1)
	{	
		this->len=len;
		this->k=k;

		printf(">>>>>>>>>test len=%llu  k=%llu<<<<<<<<<\n\n",len,k);
		init();
		
		for(auto&[name,func,categary]:test_units)
		{
			#ifdef __CUDACC__
				puts("****");
				if(categary==Test_Categary::GPU)
					test_gpu(func,name);
				else
					test_cpu(func,name);
			#else
				test_cpu(func,name);
			#endif
		}
		

		printf("\n\n");
	}
};
