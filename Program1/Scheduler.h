/* 
 * File:   Scheduler.h
 * Author: julia & sam
 *
 * Created on April 20, 2018, 9:27 AM
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <string>
#include <vector>

struct process {
    std::string name; 
    float arrival_time; 
    float prediction_value; 
    std::vector<float> args;
    
    bool operator<(const process& other_process) const {
        return prediction_value < other_process.prediction_value;
    }
};

class Scheduler {
public:
    // Rule of 5
    Scheduler();                                // constructor
    Scheduler(float block, float weight);
    Scheduler(const Scheduler& orig);           // copy constructor
    virtual ~Scheduler();                       // destructor
    Scheduler& operator=(const Scheduler orig); // copy assignment
    Scheduler& operator=(Scheduler&& orig);     // move assignment
    
    
private:
    float block_duration;
    float prediction_weight;
    // blocked processes, stores the struct
    std::priority_queue<process> blocked;
    // ready processes, stores the struct
    std::priority_queue<process> ready;
};

#endif /* SCHEDULER_H */

