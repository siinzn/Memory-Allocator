#pragma once
#include "buffer.h"
#include <stdint.h>


class PoolAllocator {
private:
    char* base_address;
    static const size_t BLOCK_SIZE = 32; // Made static for default arg use
    static const size_t BLOCK_COUNT = 128;
    Buffer bf;

public:
    PoolAllocator(size_t s = BLOCK_SIZE * BLOCK_COUNT) : bf(s) {
        base_address = reinterpret_cast<char*>(bf.get_start_address());
        init();
    }

    struct LinkedList {
        struct LinkedList* next;
    };
    LinkedList* free_list_header;

    void init() {
        uintptr_t current_address = reinterpret_cast<uintptr_t>(base_address);
        for (size_t i = 0; i < BLOCK_COUNT; i++) { 
            LinkedList* block = reinterpret_cast<LinkedList*>(current_address);
            if (i < BLOCK_COUNT - 1)
                block->next = reinterpret_cast<LinkedList*>(current_address + BLOCK_SIZE);
            else
                block->next = nullptr;
            current_address += BLOCK_SIZE;
        }
        free_list_header = reinterpret_cast<LinkedList*>(base_address);
    }

    void* allocate() { 
        if (free_list_header == nullptr) return nullptr;

        LinkedList* allocated_block = free_list_header;
        free_list_header = free_list_header->next;
        return reinterpret_cast<void*>(allocated_block);
    }

    void deallocate(void* user_block) {
        if (user_block == nullptr) return;

        LinkedList* userBlock = reinterpret_cast<LinkedList*>(user_block);
        userBlock->next = free_list_header;
        free_list_header = userBlock;
    }
};