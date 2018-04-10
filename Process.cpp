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
    ifs.close(fileName);
}

void Process::Run() {
    while (ifs.is_open()) {
        string curLine;
        int lineNo = 1;
        while(!ifs.eof()) {
            getline(ifs, curLine);
            
            istringstream line(curLine);
            
            std::vector<std::string> args;
            while (line >> arg) {
                args.push_back(arg);
            }
            
            if (args[0] == "memsize" && args.size() == 2) {
                // memsize size
                int memSize = (int)strtol(args[1], NULL, 16);
                this->memsize(memSize);
            } 
            else if (args[0] == "store" && args.size() >= 3) {
                // store values addr
                std::string values = "";
                int valuesSize = args.size()-1;
                for(int i = 1; i < valuesSize; i++) {
                    values << " " << args[i];
                }
                int addr = (int)strtol(args[args.size()-1], NULL, 16);
                
                this->store(values, addr);
            }
            else if (args[0] == "diff" && args.size() >= 3) {
                // diff expected_values addr
                std::string expected_values = "";
                int expectedSize = args.size()-1;
                for(int i = 1; i < expectedSize; i++) {
                    expected_values << " " << args[i];
                }
                int addr = (int)strtol(args[args.size()-1], NULL, 16);
                
                this->diff(expected_values, addr, expectedSize);
            }
            else if (args[0] == "replicate" && args.size() == 4) {
                // replicate value count addr
                std::string value = args[1];
                int count = (int)strtol(args[2], NULL, 16);
                int addr = (int)strtol(args[3], NULL, 16);
                
                this->replicate(value, count, addr);
            }
            else if (args[0] == "duplicate" && args.size() == 4) {
                // duplicate count src_addr dest_addr
                int count = (int)strtol(args[1], NULL, 16);
                int src_addr = (int)strtol(args[2], NULL, 16);
                int dest_addr = (int)strtol(args[3], NULL, 16);
                
                this->duplicate(count, src_addr, dest_addr);
            }
            else if (args[0] == "print" && args.size() == 3) {
                // print count addr
                int count = (int)strtol(args[1], NULL, 16);
                int addr = (int)strtol(args[2], NULL, 16);
                
                this->print(count, addr);
            }
            // if the line is empty or its a comment its valid. Else error.
            else if(!(curLine.empty() || (!curLine.empty() && curLine[0] !== "#"))) {
                throw invalid_argument("Invalid command");
            }
            cout << lineNo << ':' << curLine;
            lineNo++;
        }
    }
}

void Process::memsize(int size) {
    memBank.resize(size);
}
   
void Process::diff(string expectedValues, int address, int expectedSize) {
    string curVal;
    istringstream exp(expectedValues);
    for (int i = 0; i < expectedSize; i++) {
        exp >> curVal;
        if (memBank[address+i].compareTo(curVal) != 0) {
            string errMsg = "address: " 
                    + (address + i) 
                    + "; expected: "
                    + curVal
                    + "; actual: "
                    + memBank[address+i];
            throw std::runtime_error(errMsg);
        }
    }
}
    
void Process::store(string values, int address) {
    memBank.at(address) = values;
}
    
void Process::replicate(string value, int count, int address) {
    value = value.append("  ");
    for (int i = 0; i < count; i++) {
        value = value.append(value);
    }
    store(value, address);
}
    
void Process::duplicate(int count, int sourceAddr, int destAddr) {
    for (int i = 0; i < count; i++) {
        memBank.at(destAddr+i) = memBank.at(sourceAddr+i);
    }
}
    
void Process::print(int count, int address) {
    int bytesThisLine = 0;
    string printMe = "";
    for (int i = 0; i < count; i++) {
        if (bytesThisLine == 16){
            printMe.append("\n");
            bytesThisLine = 0;
        } else {
            printMe.append(memBank.at(address+i));
            printMe.append(" ");
            bytesThisLine++;
        }
    }
}

