/* 
 * File:   MemoryAllocator.cpp
 * Author: julia & sam
 * 
 * Created on April 12, 2018, 11:19 AM
 */

#include <cstring>
#include "MemoryAllocator.h"

MemoryAllocator::MemoryAllocator() {
    throw std::runtime_error("need to define memory size");
}

MemoryAllocator::MemoryAllocator(uint32_t num_pages) {
    // set memory to proper size, assuming page size of 16^4 bytes
    mem.resize(num_pages * 65536);
    memcpy(&mem[pf_total], &num_pages, sizeof(uint32_t));
    // first page reserved for memory allocator
    memcpy(&mem[pf_free], &num_pages - 1, sizeof(uint32_t));
    uint32_t page_size = 0x10000;
    memcpy(&mem[free_list_head], &page_size, sizeof(uint32_t));
    
    // build linked list of free pages
    // info of next page stored in first 4 bytes
    uint32_t index; 
    uint32_t next_index;
    for (int i = 1; i < (num_pages - 1); i++) {
        index = page_size * i;
        next_index = index + page_size;
        memcpy(&mem[index], &next_index, sizeof(uint32_t));
    }
    // set last page pointer to 0xFFFFFFFF
    index = page_size * (num_pages - 1);
    next_index = 0xFFFFFFFF;
    memcpy(&mem[index], &next_index, sizeof(uint32_t));
}

MemoryAllocator::MemoryAllocator(const MemoryAllocator& orig) {
    throw std::runtime_error("no copy constructor");
}

MemoryAllocator::~MemoryAllocator() {
    std::cout << "empty destructor ~MemoryAllocator was called";
}

bool MemoryAllocator::AllocatePageFrames(uint32_t count, std::vector<uint32_t> &page_frames) {
    uint32_t num_free, free_page, new_head, old_count, new_count;
    // pull number of free pages
    memcpy(&num_free, &mem[pf_free], sizeof(uint32_t)); 
    if (num_free < count) return false;
    else {
        for (int i = 0; i < count; i++) {
            memcpy(&free_page, &mem[free_list_head + (i * 0x10000)], sizeof(uint32_t));
            page_frames.push_back(free_page);
        }
        // pull address of the next free page
        memcpy(&new_head, &mem[free_list_head + (count * 0x10000)], sizeof(uint32_t));
        // set free list head to next free page
        memcpy(&mem[free_list_head], &new_head, sizeof(uint32_t));
        // set number of free page frames without count
        memcpy(&old_count, &mem[pf_free], sizeof(uint32_t));
        new_count = old_count - count;
        memcpy(&mem[pf_free], &new_count, sizeof(uint32_t));
        return true;
    }
}

bool MemoryAllocator::FreePageFrames(uint32_t count, std::vector<uint32_t> &page_frames) {
    if (!count <= page_frames.size()) return false;
    else {
        uint32_t head, points_to;
        // TODO this or this minus one?
        uint8_t index = page_frames.size() - count;
        for (int i = 0; i < count; i++) {
            memcpy(&head, &mem[free_list_head], sizeof(uint32_t));
            memcpy(&points_to, &mem[head], sizeof(uint32_t));
            
        }
        return true;
    }
}

uint32_t MemoryAllocator::get_page_frames_free() {
    
}
