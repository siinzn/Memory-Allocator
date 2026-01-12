#pragma once
class Buffer {
private:
    size_t MemorySize;
    char* dynamicArray;
public:
    Buffer(size_t s) : MemorySize(s) {
        dynamicArray = new char[s];
    }

    size_t get_size_of_memory() {
        return MemorySize;
    }

    void* get_start_address() {
        return static_cast<void*>(dynamicArray);
    }

    ~Buffer() {
        delete[] dynamicArray;
        dynamicArray = nullptr;
    }
};

