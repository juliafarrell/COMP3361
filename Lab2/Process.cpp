/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Process.cpp
 * Author: julia
 * 
 * Created on April 9, 2018, 9:55 AM
 */

#include "Process.h"

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>


using namespace std;

Process::Process() {
    cout << "No trace file";
    fileName = "no file :(";
}

Process::Process(string inputTraceFile) {
    // open file
    fileName = inputTraceFile;
    ifs.open(fileName);
    // check if file opened
    if (!ifs) {
        throw runtime_error("File '" + inputTraceFile + "' could not open properly");
    }
    // TODO: other things??
}

Process::Process(const Process& orig) {
    throw invalid_argument("No copy constructor allowed, please use a trace file");
}

Process::~Process() {
    ifs.close();
}

/* This method parses the cmd file and routes each command to the correct method
 * In addition it prints each line.
 */
void Process::Run() {
    while (ifs.is_open()) {
        string curLine, func, arg;
        uint32_t argHex;
        uint32_t lineNo = 1;
        while(!ifs.eof()) {
            getline(ifs, curLine);
            
            cout << lineNo++ << ":" << curLine << endl;
            
            std::istringstream line(curLine);
            std::vector<uint32_t> args;
            
            line >> func;
            
            // get arguments in hex form, pushing onto uint32_t vector
            while (line >> std::hex >> argHex) {
                //cout << "calling push_back on: " << to_string(argHex) << std::endl;
                args.push_back(argHex);
            }
            
            char v = curLine[0];
            
            if (func == string("memsize") && args.size() == 1) {
                this->memsize(args[0]);
            } 
            else if (func == string("store") && args.size() >= 2) {
                uint32_t numBytes = args.size() - 1;
                this->store(args, numBytes);
            }
            else if (func == string("diff") && args.size() >= 2) {
                uint32_t numBytes = args.size() - 1;
                this->diff(args, numBytes);
            }
            else if (func == string("replicate") && args.size() == 3) {
                uint32_t val = args[0];
                this->replicate(val, args[1], args[2]);
            }
            else if (func == string("duplicate") && args.size() == 3) {
                this->duplicate(args[0], args[1], args[2]);
            }
            else if (func == string("print") && args.size() == 2) {
                this->print(args[0], args[1]);
            }
            // if the line is empty or its a comment its valid. Else error.
            else if(curLine.empty() || curLine[0] == v) {
                
            }
            else {
                throw invalid_argument("Invalid command: " + curLine);
            }
        }
    }
}

void Process::memsize(uint32_t size) {
    memBank.resize(size);
}
   
/* Prints all values different than what is in memory
 */
void Process::diff(std::vector<uint32_t> args, uint32_t numBytes) {
    uint32_t curVal, expVal;
    uint32_t address = args[numBytes];
    for (uint32_t i = 0; i < numBytes; i++) {
        expVal = args[i];
        curVal = memBank.at(address + i);
        if (curVal != expVal) {
           cerr << "- at: " << std::hex << address + i << " current: " << curVal << " expected: " << expVal << std::dec << endl;
        }
    }
}
    
void Process::store(std::vector<uint32_t> args, uint32_t numBytes) {
    uint32_t address = args[numBytes];
    for (uint32_t i = 0; i < numBytes; i++) {
        memBank.at(address + i) = args[i];
    }
}
    
void Process::replicate(uint32_t value, uint32_t count, uint32_t address) {
    for (uint32_t i = 0; i < count; i++) {
        memBank.at(address + i) = value;
    }
}
    
void Process::duplicate(uint32_t count, uint32_t sourceAddr, uint32_t destAddr) {
    for (uint32_t i = 0; i < count; i++) {
        memBank.at(destAddr + i) = memBank.at(sourceAddr + i);
    }
}
    
void Process::print(uint32_t count, uint32_t address) {
    int bytesThisLine = 0;
    for (uint32_t i = 0; i < count; i++) {
        if (bytesThisLine == 16){
            cout << endl;
            bytesThisLine = 0;
        } else {
            if(bytesThisLine == 0) {
                cout << " ";
            }
            uint32_t b = memBank.at(address+i);
            if(!b) {
                b = 0;
            }
            cout << setfill('0') << setw(2) << std::hex << b << std::dec << " ";
            bytesThisLine++;
        }
    }
    if(bytesThisLine > 0) {
        cout << endl;
    }
}

