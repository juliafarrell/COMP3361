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
    uint32_t num_pages_wo_reserved = num_pages - 1;
    memcpy(&mem[pf_free], &num_pages_wo_reserved, sizeof(uint32_t));
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
            // copy the address of the first free page
            memcpy(&free_page, &mem[free_list_head], sizeof(uint32_t));
            // set the first free page to the next page in the list
            memcpy(&new_head, &mem[free_page], sizeof(uint32_t));
            memcpy(&mem[free_list_head], &new_head, sizeof(uint32_t));
            // reduce free pages by one each time
            memcpy(&old_count, &mem[pf_free], sizeof(uint32_t));
            new_count = old_count - 1;
            memcpy(&mem[pf_free], &new_count, sizeof(uint32_t));
            page_frames.push_back(free_page);
        }
        return true;
    }
}

bool MemoryAllocator::FreePageFrames(uint32_t count, std::vector<uint32_t> &page_frames) {
    if (!(count <= page_frames.size())) return false;
    else {
        uint32_t old_head, new_address, index, old_size, new_size;
        for (int i = 0; i < count; i++) {
            memcpy(&old_head, &mem[free_list_head], sizeof(uint32_t));
            index = page_frames.size() - 1;
            new_address = page_frames[index];
            memcpy(&mem[free_list_head], &new_address, sizeof(uint32_t));
            memcpy(&mem[new_address], &old_head, sizeof(uint32_t));
            memcpy(&old_size, &mem[pf_free], sizeof(uint32_t));
            new_size = old_size + 1;
            memcpy(&mem[pf_free], &new_size, sizeof(uint32_t));
            page_frames.pop_back();            
        }
        return true;
    }
}

std::vector<uint32_t> MemoryAllocator::get_page_frames_free_addresses() {
    std::vector<uint32_t> free_addresses;
    uint32_t free_page, count;
    
    memcpy(&count, &mem[pf_free], sizeof(uint32_t));
    
    if(count == 0) return free_addresses;
    
    // get the first free page
    memcpy(&free_page, &mem[free_list_head], sizeof(uint32_t));
    free_addresses.push_back(free_page);
    
    for(int i = 1; i < count; i++) {
        // get the next free page in the list
        memcpy(&free_page, &mem[free_page], sizeof(uint32_t));
        
        // add the page to the vector
        free_addresses.push_back(free_page);
    }
    
    return free_addresses;
}

uint32_t MemoryAllocator::get_page_frames_free() {
    uint32_t me;
    memcpy(&me, &mem[pf_free], sizeof(uint32_t));
    return me;
}
