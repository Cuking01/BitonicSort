#include "../src/cpu.hpp"

#include"test.hpp"

int main()
{
	Tester tester;
	tester.add_test("Bitonic Sort(Native)",bitonic_sort_native<Inc>);
	tester.add_test("std::sort",std_sort);
	tester.test_all(1<<4,1<<20);
	tester.test_all(1<<8,1<<16);
	tester.test_all(1<<12,1<<12);
	tester.test_all(1<<16,1<<8);
	tester.test_all(1<<20,1<<4);
	tester.test_all(1<<24,1);
	//tester.test_all(1<<30,1);
	
}
