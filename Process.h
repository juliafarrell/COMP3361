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

class Process {
public:
    std::string fileName;
    Process();
    Process(std::string inputTraceFile);
    Process(const Process& orig);
    virtual ~Process();
    void Run();
private:

};

#endif /* PROCESS_H */

