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
    // copy ready_queue
//    for (int i = 0; i < orig.ready_queue.size(); i++){
//        ready_queue.push(orig.ready_queue.top());
//        orig.ready_queue.pop();
//    }
//    // copy process_list
//    for (int i = 0; i < orig.process_list.size(); i++){
//        process_list.push_back(orig.process_list.front());
//        orig.process_list.pop_back();
//    }
//    // copy blocked_queue
//    for (int i = 0; i < orig.blocked_queue.size(); i++){
//        blocked_queue.push_back(orig.blocked_queue.front());
//        orig.blocked_queue.pop_back();
//    }
}

Scheduler::~Scheduler() {
    while (!ready_queue.empty()) {
        ready_queue.pop();
    }
    while (!blocked_queue.empty()) {
        blocked_queue.pop_back();
    }
    process_list.clear();
    // TODO: check, is this right?
}

Scheduler& Scheduler::operator =(const Scheduler orig) {
    block_duration = orig.block_duration;
    prediction_weight = orig.prediction_weight;
    simulated_timer = orig.simulated_timer;
    process_list = orig.process_list;
    ready_queue = orig.ready_queue;
    blocked_queue = orig.blocked_queue;
}
// TODO: What is the difference between copy and move implementation?
Scheduler& Scheduler::operator =(Scheduler&& orig) {
    block_duration = orig.block_duration;
    prediction_weight = orig.prediction_weight;
    simulated_timer = orig.simulated_timer;
    process_list = orig.process_list;
    ready_queue = orig.ready_queue;
    blocked_queue = orig.blocked_queue;
}

void Scheduler::add_process(process p) {
    process_list.push_back(p);
}

void Scheduler::run() {
    // 1. pull processes into ready queue if arrived
    arrive_proccesses();
    
    // 2. while there are processes in ready and/or blocked queues
    process cur_process;
    float time_elapsed;
    while (still_running()) {
        // pull processes into ready queue if arrived
        arrive_proccesses();
        
        // if there are processes in ready
        if (!ready_queue.empty()) {
            // get the next shortest process that is ready
            // this pops it off the ready queue
            cur_process = get_next_process();
            // update its prediction value
            time_elapsed = update_prediction_value(cur_process);
            // increment the global timer by 'time elapsed' 
            update_time(time_elapsed);
            update_blocked_queue();
            // if process doesn't complete, add to blocked list
            if (!process_completed(cur_process)) {
                // update block time of current process
                cur_process.time_blocked = this->simulated_timer;
                this->blocked_queue.push_back(cur_process);
                // make sure blocked queue in ascending blocked_time order
                sort_blocked(blocked_queue);
                print_process(cur_process, time_elapsed, 'B');
            }
            // if process completes, print with code 'T'
            else {
                print_process(cur_process, time_elapsed, 'T');
                this->avg_turnaround.push_back(cur_process);
            }
            
        } else if (!this->blocked_queue.empty()) {
            // if there are no processes ready,
            // CPU is idle, calculate time
            time_elapsed = get_idle_time(); 
            // print
            print_idle(time_elapsed);
        } else {
            print_done();
        }
    }
}

bool Scheduler::still_running() {
    if (!ready_queue.empty() || !blocked_queue.empty()) return true;
    else return false;
}

float Scheduler::update_prediction_value(process p) {
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
    return last_execution_time;
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

void Scheduler::update_time(float time_elapsed) {
    this->simulated_timer += time_elapsed;
}

bool Scheduler::process_completed(process p) {
    if (p.args.size() == 0) return true;
    else return false;
}

void Scheduler::update_blocked_queue() {
    if (blocked_queue.size() == 0) return;
    else {
        // sort queue by block_time, smallest first
        sort_blocked(blocked_queue);
        
        // then if its done blocking, pop off
        for(int i = 0; i < this->blocked_queue.size(); i++) {
            if(this->blocked_queue[i].time_blocked + this->block_duration > this->simulated_timer) {
                this->ready_queue.push(this->blocked_queue[i]);
                this->blocked_queue.erase(this->blocked_queue.begin() + i);
                i--;
            } else {
                break;
            }
        }
    }
}

void Scheduler::print_process(process p, float exec_time, char status) {
    std::cout << this->simulated_timer << "\t" <<
            p.name << "\t" << 
            exec_time << "\t" <<
            status << "\t" <<
            p.prediction_value;
}

void Scheduler::print_idle(float idle_time) {
    std::cout << this->simulated_timer << "\t(IDLE)\t" << idle_time << "\tI";
}

void Scheduler::print_done(float avg_turnaround) {
    std::cout << this->simulated_timer << "\t(DONE)\t" << avg_turnaround << "\tI";
}

void Scheduler::sort_blocked(std::vector<process> &p) {
    // insertion sort, smallest first
    int j;
    process tmp;
    for (int i = 1; i < p.size(); i++) {
        j = i;
        tmp = p[i];
        while (j > 0 && tmp.time_blocked < p[j-1].time_blocked) {
            p[j] = p[j-1];
            j--;
        }
        p[j] = tmp;
    }
}

float Scheduler::get_idle_time() {
    return this->blocked_queue[0].time_blocked + this->block_duration;
}

float Scheduler::calculate_avg_turnaround(std::vector<float> t) {
    
}