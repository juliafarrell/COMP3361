 /* 
 * File:   main.cpp
 * Author: julia & sam
 *
 * Created on April 12, 2018, 11:16 AM
 */

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "MemoryAllocator.h"

/* This program runs from the command line. an input file that has the
 * page size as a hex integer on the first line
 * 
 * Subsequent lines have one or two hex arguments specifying memory to 
 * allocate and deallocate.
 */
int main(int argc, char** argv) {
    std::string my_file = argv[1];
    MemoryAllocator* ma;
    
    std::vector<uint32_t> page_frames;
    
    std::ifstream ifs;
    std::string cur_line;
    
    ifs.open(my_file);
    if (!ifs) {
        throw std::runtime_error("File '" + my_file + "' could not open properly");
    }
    
    if (ifs.is_open()) {
        uint32_t mem_size;
        getline(ifs, cur_line);
        std::istringstream line(cur_line);
        line >> std::hex >> mem_size;
        std::cout << "#" << std::to_string(mem_size) << std::endl;
        ma = new MemoryAllocator(mem_size);
        
        while (!ifs.eof()) {
            uint32_t arg;
            getline(ifs, cur_line);
            std::istringstream line(cur_line);
            line >> std::hex >> arg;
            
            if(arg == 0) {
                // dealloc
                line >> std::hex >> arg;
                std::cout << "#0 " << std::to_string(arg) << std::endl;
                if(ma->FreePageFrames(arg, page_frames))
                    std::cout << "T ";
                else
                    std::cout << "F ";
                
                std::cout << ma->get_page_frames_free() << std::endl;
            } else if(arg == 1) {
                // alloc
                line >> std::hex >> arg;
                std::cout << "#1 " << std::to_string(arg) << std::endl;
                
                if(ma->AllocatePageFrames(arg, page_frames))
                    std::cout << "T ";
                else
                    std::cout << "F ";
                
                std::cout << ma->get_page_frames_free() << std::endl;
            } else if(arg == 2) {
                // print context
                std::vector<uint32_t> free_pages = ma->get_page_frames_free_addresses();
                std::cout << "#2" << std::endl;
                for(int i = 0; i < free_pages.size(); i++) {
                    std::cout << " " << std::hex << free_pages[i];
                }
                std::cout << std::endl << std::dec;
            }            
        }
    }
    
    
    return 0;
}

