
#include "buffer.h"
#include <stdint.h>

class Arena {
private:
	Buffer bf;
	char* base_address; 
	size_t total_size;
	size_t offset_from_base = 0;
	size_t high_water_mark = 0;  
public:
	Arena(size_t s) : bf(s){
		base_address = reinterpret_cast<char*>(bf.get_start_address());
		total_size = bf.get_size_of_memory();
	};
	
	void* allocate(size_t size, uintptr_t alignment) {

		uintptr_t current_address = reinterpret_cast<uintptr_t>(base_address) + offset_from_base;
		uintptr_t aligned_address = (current_address + (alignment - 1)) & ~(alignment - 1);
		uintptr_t aligned_offset = aligned_address - reinterpret_cast<uintptr_t>(base_address);
		size_t allocation_end = aligned_offset + size;
		
		if (allocation_end > total_size) {
			std::cout << "memory limit exceeded!" << std::endl;
			return nullptr;
		}
		offset_from_base = allocation_end;
		if (offset_from_base > high_water_mark) high_water_mark = offset_from_base;



		return reinterpret_cast<void*>(aligned_address);
	}

	size_t get_hwm() {
		return high_water_mark;
	}

	void reset() {
		offset_from_base = 0;
		high_water_mark = 0;
	}
};