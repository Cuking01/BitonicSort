#pragma once

#include<algorithm>
#include<concepts>
#include<cstdint>

struct Inc
{
	static void swap(int*a,int*b)
	{
		int min=std::min(*a,*b);
		int max=std::max(*a,*b);
		*a=min;
		*b=max;
	}
};
struct Dec
{
	static void swap(int*a,int*b)
	{
		int min=std::min(*a,*b);
		int max=std::max(*a,*b);
		*a=max;
		*b=min;
	}
};

template<typename T>
concept Order_Like=std::same_as<T,Inc>||std::same_as<T,Dec>;

template<Order_Like Order>
void divide(int*arr,uint64_t len)
{
	if(len==1)return;
	len>>=1;
	for(uint64_t i=0;i<len;i++)
		Order::swap(arr+i,arr+len+i);
	divide<Order>(arr,len);
	divide<Order>(arr+len,len);
}


//len必须是2的幂次
template<Order_Like Order>
void bitonic_sort_native(int*arr,uint64_t len)
{
	if(len==1)return;

	len>>=1;
	bitonic_sort_native<Dec>(arr,len);
	bitonic_sort_native<Inc>(arr+len,len);
	divide<Order>(arr,len<<1);
}


