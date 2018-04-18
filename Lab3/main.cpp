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

#include "MemoryAllocator.h"

/* This program runs from the command line. an input file that has the
 * page size as a hex integer on the first line
 * 
 * Subsequent lines have one or two hex arguments specifying memory to 
 * allocate and deallocate.
 */
int main(int argc, char** argv) {
    std::string my_file = argv[1];
    std::ifstream ifs;
    ifs.open(my_file);
    if (!ifs) {
        throw std::runtime_error("File '" + my_file + "' could not open properly");
    }
    if (ifs.is_open()) {
        std::string cur_line;
        uint8_t mem_size;
        while (!ifs.eof()) {
            getline(ifs, cur_line);
            std::istringstream line(cur_line);
            line >> std::hex >> mem_size;
            
            MemoryAllocator(mem_size);
        }
    }
    
    
    return 0;
}

