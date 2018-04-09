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
    std::string memsize(int size);
    std::string diff(int expectedValues, int address);
    std::string store(int values, int address);
    std::string replicate(int value, int count, int address);
    std::string duplicate(int count, int sourceAddr, int destAddr);
    std::string print(int count, int address);
    std::string comment(std::string commentMe);
};

#endif /* PROCESS_H */

