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
    
}

std::string Process::memsize(int size) {
    
}
   
std::string Process::diff(int expectedValues, int address) {
    
}
    
std::string Process::store(int values, int address) {
    
}
    
std::string Process::replicate(int value, int count, int address) {
    
}
    
std::string Process::duplicate(int count, int sourceAddr, int destAddr) {
    
}
    
std::string Process::print(int count, int address) {
    
}
    
std::string Process::comment(std::string commentMe) {
    
}

