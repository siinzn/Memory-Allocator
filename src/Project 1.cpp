#include <iostream>
#include "arena.h"
#include "benchmark.h"
#include <chrono>
#include <vector>
#include "stack.h"
#include "pool.h"

int main()
{
    //arena 
    std::cout << "Linear Allocation - " << std::endl;

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


    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Stack Allocation - " << std::endl;

    Stack s(10000);
    //block a
    
    void* b_a = s.allocate(200, 16);
    std::cout << "Block A : " << b_a << std::endl;
    std::cout << "Offset after A : " << s.get_offset() << std::endl;

    void* b_b = s.allocate(100, 8);
    std::cout << "Block B : " << b_b << std::endl;
    std::cout << "Offset after B : " << s.get_offset() << std::endl;

    void* b_c = s.allocate(200, 4);
    std::cout << "Block C : " << b_c << std::endl;
    std::cout << "Offset after C : " << s.get_offset() << std::endl;


    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Freeing" << std::endl;


    s.free(b_c);
    std::cout << "Block B : " << b_b << std::endl;
    std::cout << "Offset after C is removed : " << s.get_offset() << std::endl;
    s.free(b_b);
    std::cout << "Block A : " << b_a << std::endl;
    std::cout << "Offset after B is removed : " << s.get_offset() << std::endl;
    s.free(b_a);
    std::cout << "Offset after A is removed : " << s.get_offset() << std::endl;


    std::cout << "------------------------------------------------" << std::endl;

    void* test2a = s.allocate(200, 16);
    std::cout << "Test 2a : " << test2a << std::endl;
    void* test2b = s.allocate(100, 8);
    std::cout << "Test 2b : " << test2b << std::endl;
    s.free(test2b);
    std::cout << "Freed test 2b : " << std::endl;
    void* test2c = s.allocate(100, 8);
    std::cout << "Test 2c : " << test2c << std::endl;

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Pool Allocation - " << std::endl;

    PoolAllocator pool;
    void* ptrs[128]; 
    int allocated = 0;

    for (int i = 0; i < 128; i++) {
        ptrs[i] = pool.allocate();
        if (ptrs[i] == nullptr) {
            std::cout << "  Unexpected nullptr at index " << i << std::endl;
            break;
        }
        allocated++;
    }
    void* extra = pool.allocate();
    std::cout << "  Allocated " << allocated << " blocks" << std::endl;

    if (extra == nullptr && allocated == 128) {
        std::cout << "  PASS: Returned nullptr when exhausted" << std::endl;
    }
    else {
        std::cout << "  FAIL: Should return nullptr after exhaustion" << std::endl;
    }


    void* a = pool.allocate();
    void* b = pool.allocate();
    std::cout << "  Allocated A: " << a << std::endl;
    std::cout << "  Allocated B: " << b << std::endl;

    pool.deallocate(a);
    void* c = pool.allocate();
    std::cout << "  Freed A, allocated C: " << c << std::endl;

    if (c == a) {
        std::cout << "  PASS: C reused A's address (free list pop works)" << std::endl;
    }
    else {
        std::cout << "  FAIL: Expected C to equal A" << std::endl;
    }

    // Cleanup
    pool.deallocate(b);
    pool.deallocate(c);

    return 0;
}