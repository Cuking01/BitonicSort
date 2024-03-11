#include<thrust/sort.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <random>

#include "../src/gpu.cu"
#include "../src/cpu.hpp"
#include "test.hpp"

int main()
{
	Tester tester;

	tester.add_test("thrust::sort(CPU)",thrust_sort_cpu);
	tester.add_test("std::sort",std_sort);
	tester.add_test("thrust::sort(GPU)",thrust_sort_gpu,Test_Categary::GPU);


	tester.test_all(1<<22,1<<2);
	tester.test_all(1<<20,1<<4);
	tester.test_all(1<<18,1<<6);
	tester.test_all(1<<16,1<<8);
}
