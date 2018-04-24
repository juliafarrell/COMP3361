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
#include <queue>
#include <list>
#include <iostream>

struct process {
    std::string name; 
    float arrival_time; 
    float prediction_value; 
    std::queue<float> args; // first in first out
    
    bool operator<(const process& other_process) const {
        if(prediction_value == other_process.prediction_value) {
            return arrival_time < other_process.arrival_time;
        }
        
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
    
    void add_process(process p);
    void run();
    
private:
    float block_duration;
    float prediction_weight;
    float simulated_timer;
    // stores all processes
    std::vector<process> process_list;
    // blocked processes, stores the struct
    // because '<' operator in struct, ordered in reverse
    std::priority_queue<process> blocked_queue;
    // ready processes, stores the struct
    std::priority_queue<process> ready_queue;
    void update_prediction_value(process p);
    // checks to see if blocked and ready queue are empty (false if both empty)
    bool still_running();
    // checks if any processes have arrived
    void arrive_proccesses();
    // gets the shortest process from the ready queue, assuming non-empty ready
    process get_next_process();
    // determines if the process blocks when trying to finish its block
    bool process_blocks(process p);
};

#endif /* SCHEDULER_H */

