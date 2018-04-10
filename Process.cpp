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

Process::Run() {
    while (ifs.is_open()) {
        string curLine;
        int lineNo = 1;
        while(!ifs.eof()) {
            cout << lineNo << ':';
            getline(ifs, curLine);
            istringstream line(curLine);
<<<<<<< HEAD
            line >> func;
            if (func.compare('memsize') == 0) {
                line >> func;
                memsize(func);
            } 
            else if (func.compare('store') == 0) {

            }
            else if (func.compare('diff') == 0) {
=======
            std::vector<std::string> args;
            
            while (line >> arg) {
                args.push_back(arg);
            }
            
            if (args[0].compare('memsize') == 0) {

            } 
            else if (args[0].compare('diff') == 0) {

            }
            else if (args[0].compare('store') == 0) {
>>>>>>> 0a5ff34847a4aaa3632cc44fd3579fd3b976d447
                
            }
            else if (args[0].compare('replicate') == 0) {
                
            }
            else if (args[0].compare('duplicate') == 0) {
                
            }
            else if (args[0].compare('print') == 0) {
                
            }
            else if(!curLine.empty() && curLine[0] == "#") {
                cout << curLine;
            }
<<<<<<< HEAD
            lineNo++;
=======
            else {
                throw invalid_argument("Invalid command");
            }
>>>>>>> 0a5ff34847a4aaa3632cc44fd3579fd3b976d447
        }
    }
}

void Process::memsize(int size) {
    memBank.resize(size);
}
   
void Process::diff(string expectedValues, int address) {
    
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
    
}
    
void Process::print(int count, int address) {
    
}

