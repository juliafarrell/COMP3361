/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Process.h
 * Author: julia
 *
 * Created on April 9, 2018, 9:55 AM
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

class Process {
public:
    std::string fileName;
    std::ifstream ifs;
    Process();
    Process(std::string inputTraceFile);
    Process(const Process& orig);
    virtual ~Process();
    /**
     * Run allocates an empty array used for processing commands in the trace
     * file. Each command in the trace file is handled with separate helper
     * functions.
     * @return standard output stream of trace data
     */
    void Run();
private:
    std::vector<uint8_t> memBank;
    void memsize(uint32_t size);
    void diff(std::vector<uint32_t> args, uint32_t numBytes);
    void store(std::vector<uint32_t> args, uint32_t numBytes);
    void replicate(uint32_t value, uint32_t count, uint32_t address);
    void duplicate(uint32_t count, uint32_t sourceAddr, uint32_t destAddr);
    void print(uint32_t count, uint32_t address);
};

#endif /* PROCESS_H */

