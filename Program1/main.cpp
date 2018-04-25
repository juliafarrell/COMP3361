/* 
 * File:   main.cpp
 * Author: julia & sam
 *
 * Created on April 17, 2018, 7:49 PM
 */

#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include "Scheduler.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    // read in block duration and prediction weight from command line args
    // check validity
    string block_duration_s = argv[2];
    float block_duration = stof(block_duration_s);
    if (block_duration <=0 ) 
        throw runtime_error("block duration must be positive, nonzero");
    string prediction_weight_s = argv[3];
    float prediction_weight = stof(prediction_weight_s);
    if (prediction_weight <= 0 || prediction_weight >= 1) 
        throw runtime_error("prediction weight must be between zero and one");
    
    // if block duration and prediction weight are ok
    // attempt to read the file
    string file_name = argv[1];
    fstream file;
    file.open(file_name);
    if (!file.is_open()) {
        cerr << "ERROR: failed to open trace file: " << file_name << "\n";
        exit(2);
    }
    
    // if file opens successfully
    // 1. make a scheduler
    Scheduler* s = new Scheduler(block_duration, prediction_weight);
    
    // 2. store processes in structs line by line and send to the scheduler 
    struct process p;
    string line;  // text line read
    string p_name; 
    float time;
    while (!file.eof()) {
        getline(file, line);
        if (line.empty()) break;
        // make string stream for the line to read in the process
        istringstream line_stream(line);
        // process name from line
        line_stream >> p_name;
        line_stream >> time;
        queue<float> actual_exec_times; // execution times (actual) from line
        float exec_time;
        while (line_stream >> exec_time) { // returns false if end of line or fails to convert to float
            actual_exec_times.push(exec_time);
        }
        process p = {p_name, time, 0, -1, true, actual_exec_times};
        s->add_process(p);
    }
    s->run();
    return 0;
}

