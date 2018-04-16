/* 
 * File:   MemoryAllocator.cpp
 * Author: julia & sam
 * 
 * Created on April 12, 2018, 11:19 AM
 */

#include "MemoryAllocator.h"

MemoryAllocator::MemoryAllocator() {
    throw std::runtime_error("need to define memory size");
}

MemoryAllocator::MemoryAllocator(uint8_t num_pages) {
    // set memory to proper size, assuming page size of 16^4 bytes
    mem.resize(num_pages * 65536);
    mem[pf_total] = num_pages;
    // first page reserved for memory allocator
    mem[pf_free] = num_pages - 1;
    // ?? change to 32 bit thing??
    mem[free_list_head] = 0x10000;
    
    // build linked list of free pages
    // info of next page stored in first 4 bytes
    for (int i = 1; i < num_pages -1; i++) {
        mem[0x10000 * i] = mem[0x10000 * (i+1)];
    }
    mem[num_pages*65536 - 1] = 0xFFFFFFFF;
}

MemoryAllocator::MemoryAllocator(const MemoryAllocator& orig) {
    throw std::runtime_error("no copy constructor");
}

MemoryAllocator::~MemoryAllocator() {
    std::cout << "empty destructor ~MemoryAllocator was called";
}

bool AllocatePageFrames(uint32_t count, std::vector<uint32_t> &page_frames) {
    
}

bool FreePageFrames(uint32_t count, std::vector<uint32_t> &page_frames) {
    
}

uint32_t get_page_frames_free() {
    
}
