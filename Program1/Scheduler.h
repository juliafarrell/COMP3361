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
    float time_blocked;
    bool new_process;
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
    Scheduler();                                 // constructor
    Scheduler(float block, float weight);
    Scheduler(const Scheduler& orig);            // copy constructor
    virtual ~Scheduler();                        // destructor
    Scheduler& operator=(const Scheduler& orig); // copy assignment
    Scheduler& operator=(Scheduler&& orig);      // move assignment
    
    void add_process(process p);
    void run();
    
private:
    float block_duration;
    float prediction_weight;
    float simulated_timer;
    // stores all processes
    std::vector<process> process_list;
    
    // blocked processes, stores the struct
    std::vector<process> blocked_queue;
    
    // because '<' operator in struct, ordered in reverse on prediction time
    // ready processes, stores the struct
    std::priority_queue<process> ready_queue;
    
    // average turnaround times stored here when they complete
    std::vector<float> avg_turnaround;
    
    // returns the actual execution time of the last executed process
    float update_prediction_value(process &p);
    
    // checks to see if blocked and ready queue are empty (false if both empty)
    bool still_running();
    
    // checks if any processes have arrived
    void arrive_proccesses();
    
    // gets the shortest process from the ready queue, assuming non-empty ready
    process get_next_process();
    
    // determines if the process blocks when trying to finish its block
    bool process_blocks(process p);
    
    // update the global timer based on execution time
    void update_time(float time_elapsed);
    
    // checks to see if a process completes: true if args.size is zero
    bool process_completed(process p);
    
    // updates the blocked queue based on (preseumably new) time elapsed value
    void update_blocked_queue();
    
    // print functions for idle and process completion
    void print_process(float start_time, process p, float exec_time, char status);
    void print_idle(float idle_time);
    void print_done(float avg_turnarounds);
    
    // insertion sort for blocked vector on blocked_time
    // (likely that it's already sorted, so O(n) will by typical)
    void sort_blocked(std::vector<process> &p);
    
    // returns time the CPU is idle due to blocks
    // assumes blocked list is sorted & nonempty
    float get_idle_time();
    float calculate_avg_turnaround();
};

#endif /* SCHEDULER_H */

