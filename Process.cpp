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

void Process::Run() {
    while (ifs.is_open()) {
        string curLine, func, arg;
        int argHex;
        int lineNo = 1;
        while(!ifs.eof()) {
            getline(ifs, curLine);
            
            cout << lineNo++ << ':' << curLine << endl;
            
            std::istringstream line(curLine);
            std::vector<uint8_t> args;
            
            line >> func;
            
            // get arguments in hex form, pushing onto uint8_t vector
            while (line >> std::hex >> argHex) {
                cout << "calling push_back on: " << std::to_string(argHex) << std::endl;
                args.push_back(argHex);
            }
            
            
            if (func == string("memsize") && args.size() == 1) {
                this->memsize(args[0]);
            } 
            else if (func == string("store") && args.size() >= 2) {
                int numBytes = args.size() - 1;
                this->store(args, numBytes);
            }
            else if (func == string("diff") && args.size() >= 2) {
                int numBytes = args.size() - 1;
                this->diff(args, numBytes);
            }
            else if (func == string("replicate") && args.size() == 3) {
                this->replicate(args[0], args[1], args[2]);
            }
            else if (func == string("duplicate") && args.size() == 3) {
                this->duplicate(args[0], args[1], args[2]);
            }
            else if (func == string("print") && args.size() == 2) {
                this->print(args[0], args[1]);
            }
            // if the line is empty or its a comment its valid. Else error.
            else if(!curLine.empty() && !(&curLine[0] == "#")) {
                throw invalid_argument("Invalid command: " + curLine);
            }
        }
    }
}

void Process::memsize(uint16_t size) {
    memBank.resize(size);
}
   
void Process::diff(std::vector<uint8_t> args, int numBytes) {
    uint8_t curVal, expVal;
    uint8_t address = args[numBytes+1];
    for (int i = 0; i < numBytes; i++) {
        curVal = args[i];
        expVal = memBank.at(address + i);
        if (curVal != expVal) {
            // TODO: Fix error message
            throw runtime_error("diff throws error");
        }
    }
}
    
void Process::store(std::vector<uint8_t> args, int numBytes) {
    int address = (int)(args[numBytes+1]);
    for (int i = 0; i < numBytes; i++) {
        memBank.at(address + i) = args[i];
    }
}
    
void Process::replicate(uint8_t value, uint8_t count, uint8_t address) {
    for (int i = 0; i < count; i++) {
        memBank.at(address + 1) = value;
    }
}
    
void Process::duplicate(uint8_t count, uint8_t sourceAddr, uint8_t destAddr) {
    for (int i = 0; i < count; i++) {
        memBank.at(destAddr + i) = memBank.at(sourceAddr + i);
    }
}
    
void Process::print(uint8_t count, uint8_t address) {
    int bytesThisLine = 0;
    string printMe = "";
    string curByte;
    for (int i = 0; i < count; i++) {
        if (bytesThisLine == 16){
            printMe.append("\n");
            bytesThisLine = 0;
        } else {
            curByte = std::to_string(memBank.at(address+i));
            printMe.append(curByte);
            printMe.append(" ");
            bytesThisLine++;
        }
    }
}

