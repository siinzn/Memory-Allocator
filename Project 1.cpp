#include <iostream>
#include "arena.h"
#include <chrono>



int main()
{
    Arena a1(1000000);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i{ 0 }; i < 100000; i++) {
        a1.allocate(8, 8);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    a1.reset();

    std::cout << "Duration for Custom Allocator : " << duration.count() << " microseconds" << std::endl;
    return 0;
}