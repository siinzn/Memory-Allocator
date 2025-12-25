#include <vector>
#include <iostream>
#include <chrono>

std::vector<void*> bm_ptr;
int bm() {
	auto start = std::chrono::high_resolution_clock::now();
	int* ptr;
	//to reserve a cetain amount in vector before allocation
	bm_ptr.reserve(100000);
	for (int i{ 0 }; i < 100000; i++) {
		int* ptr = new int(42);
		bm_ptr.push_back(ptr);
	}
	for (void* p : bm_ptr) delete p;

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << "Duration for Benchmark Allocator : " << duration.count() << " microseconds" << std::endl;

	return 0;
}
