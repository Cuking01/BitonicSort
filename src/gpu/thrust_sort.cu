#include<thrust/sort.h>
#include<stdint.h>

void thrust_sort_cpu(int*arr,uint64_t len)
{
	thrust::sort(arr,arr+len);
}

void thrust_sort_gpu(int*arr,uint64_t len)
{
	thrust::sort(thrust::device_ptr<int>(arr),thrust::device_ptr<int>(arr+len));
}
