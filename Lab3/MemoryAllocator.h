/* 
 * File:   MemoryAllocator.h
 * Author: julia & sam
 *
 * Created on April 12, 2018, 11:19 AM
 */

#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H

#include <stdexcept>
#include <stdint.h>
#include <vector>
#include <iostream>

class MemoryAllocator {
public:
    MemoryAllocator();
    MemoryAllocator(uint32_t num_pages);
    MemoryAllocator(const MemoryAllocator& orig);
    virtual ~MemoryAllocator();
    bool AllocatePageFrames(uint32_t count, std::vector<uint32_t> &page_frames); 
    bool FreePageFrames(uint32_t count, std::vector<uint32_t> &page_frames);
    std::vector<uint32_t> get_page_frames_free_addresses();
    uint32_t get_page_frames_free();
private:
    std::vector<uint8_t> mem;
    // Index of total number of page frames in mem vector
    // Should hold 32 bit value, so 4 * 8 bits.
    static const uint8_t pf_total = 0;
    // Index of number of free page frames in mem vector
    static const uint8_t pf_free = 4;
    // Index of address for head of the free list in mem vector
    static const uint8_t free_list_head = 8;
};

#endif /* MEMORYALLOCATOR_H */

