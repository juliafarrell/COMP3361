/* 
 * File:   Scheduler.cpp
 * Author: julia & sam
 * 
 * Created on April 20, 2018, 9:27 AM
 */

#include "Scheduler.h"
#include <stdexcept>

Scheduler::Scheduler() {
    throw std::runtime_error("constructor must specify block_duration and prediction_weight");
}

Scheduler::Scheduler(float block, float weight) {
    block_duration = block;
    prediction_weight = weight;
    simulated_timer = 0;
}   

Scheduler::Scheduler(const Scheduler& orig) {
    block_duration = orig.block_duration;
    prediction_weight = orig.prediction_weight;
    simulated_timer = orig.simulated_timer;
    // TODO copy ready_queue
    // TODO copy process_list
    // TODO copy blocked_queue
    // ^^^ I think those have to be done element by element
}

Scheduler::~Scheduler() {
    while (!ready_queue.empty()) {
        ready_queue.pop();
    }
    while (!blocked_queue.empty()) {
        blocked_queue.pop();
    }
    process_list.clear();
    // TODO: check, is this right?
}

Scheduler& Scheduler::operator =(const Scheduler orig) {
    // TODO: implement me
}

Scheduler& Scheduler::operator =(Scheduler&& orig) {
    // TODO: implement me
}

void Scheduler::add_process(process p) {
    process_list.push_back(p);
}

void Scheduler::run() {
    // 1. pull processes into ready queue
    arrive_proccesses();
    // 2. while there are processes in ready and/or blocked queues
    process cur_process;
    while (still_running()) {
        arrive_proccesses();
        // if there are processes in ready
        if (!ready_queue.empty()) {
            // get the next shortest process that is ready
            cur_process = get_next_process();
            // update its prediction value
            update_prediction_value(cur_process);
            // increment the global timer by 'time elapsed' 
            // (block time if full, exec time if under)
            
            // check to see if process is completes
            
        } else {
        // if there are no processes ready,
            // CPU is idle
            
        }
    }
}

bool Scheduler::still_running() {
    if (!ready_queue.empty() || !blocked_queue.empty()) return true;
    else return false;
}

void Scheduler::update_prediction_value(process p) {
    // get last execution time (head of args vector)
    if (p.args.size() == 0) {
        std::cerr << "process completed but still trying to run";
    }
    float last_execution_time = p.args.front();
    // remove the head of args vector
    p.args.pop();
    // update prediction value for the process
    p.prediction_value = this->prediction_weight * p.prediction_value 
            + (1 - this->prediction_weight) * last_execution_time;
}

void Scheduler::arrive_proccesses() {
    for (int i = 0; i < process_list.size(); i++) {
        if(this->simulated_timer >= process_list[i].arrival_time) {
            ready_queue.push(process_list[i]);
            process_list.erase(process_list.begin() + i);
            i--;
        }
    }
}

process Scheduler::get_next_process() {
    process shortest = ready_queue.top();
    ready_queue.pop();
    return shortest;
}
