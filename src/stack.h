
#include "buffer.h"
#include <stdint.h>
#include <iostream>
#include <cassert>

class Stack {
private:
	Buffer bf;
	char* base_address;
	size_t total_size;
	size_t offset_from_base = 0;
	size_t high_water_mark = 0;
public:
	Stack(size_t s) : bf(s) {
		base_address = reinterpret_cast<char*>(bf.get_start_address());
		total_size = bf.get_size_of_memory();
	};

	struct AllocationHeader
	{
		size_t prev_offset;
	};

	void* allocate(size_t size, uintptr_t alignment) {
		AllocationHeader* header_ptr;
		//where memory starts
		uintptr_t current_address = reinterpret_cast<uintptr_t>(base_address) + offset_from_base + sizeof(AllocationHeader);
		uintptr_t aligned_address = (current_address + (alignment - 1)) & ~(alignment - 1);
		uintptr_t aligned_offset = aligned_address - reinterpret_cast<uintptr_t>(base_address);


		size_t saved_offset = offset_from_base;
		char* header_loc = reinterpret_cast<char*>(aligned_address) - sizeof(AllocationHeader);
		header_ptr = reinterpret_cast<AllocationHeader*>(header_loc);
		header_ptr->prev_offset = saved_offset;
		size_t allocation_end = aligned_offset + size;

		if (allocation_end > total_size) {
			std::cout << "memory limit exceeded!" << std::endl;
			return nullptr;
		}
		offset_from_base = allocation_end;
		if (offset_from_base > high_water_mark) high_water_mark = offset_from_base;

		return reinterpret_cast<void*>(aligned_address);
	}

	void free(void* ptr) {
		if (ptr == nullptr) return;
		assert(reinterpret_cast<char*>(ptr) >= base_address && reinterpret_cast<char*>(ptr) < base_address + offset_from_base);
		char* header_loc = reinterpret_cast<char*>(ptr) - sizeof(AllocationHeader);
		AllocationHeader* header_ptr = reinterpret_cast<AllocationHeader*>(header_loc);
		assert(header_ptr->prev_offset < offset_from_base);
		offset_from_base = header_ptr->prev_offset;
	}

	size_t get_hwm() {
		return high_water_mark;
	}
	size_t get_offset() {
		return offset_from_base;
	}
};
