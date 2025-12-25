#include <iostream>
#include "arena.h"
#include "benchmark.h"
#include <chrono>
#include <vector>



int main()
{
    //arena 
    Arena a1(1000000);
    volatile std::uintptr_t sink;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i{ 0 }; i < 100000; i++) {
        void* ptr = a1.allocate(8, 8);
        sink = reinterpret_cast<std::uintptr_t>(ptr);
    }

    a1.reset();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Duration for Custom Allocator : " << duration.count() << " microseconds" << std::endl;


    //benchmark 
    bm();
    return 0;
}