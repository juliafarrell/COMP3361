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

}

Scheduler& Scheduler::operator =(const Scheduler& orig) {
    std::cerr << "not implemented";
}

Scheduler& Scheduler::operator =(Scheduler&& orig) {
    std::cerr << "not implemented";
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
    float start_time;
    int debugging = 0;
    
    while (still_running()) {
//    while (debugging < 50) {
        start_time = this->simulated_timer;
        
        // pull processes into ready queue if arrived
        arrive_proccesses();
        
        // if there are processes in ready
        if (ready_queue.size() != 0) {
            // get the next shortest process that is ready
            // this pops it off the ready queue
            cur_process = get_next_process();
            
            // update its prediction value
            // time_elapsed is actual execution time of the process before blocking
            time_elapsed = update_prediction_value(cur_process);
            
            // increment the global timer & see if things unblock
            update_time(time_elapsed);
            update_blocked_queue();
            
            // if process doesn't complete, add to blocked list
            if (!process_completed(cur_process)) {
                // update block time of current process
                cur_process.time_blocked = this->simulated_timer;
                this->blocked_queue.push_back(cur_process);
                print_process(start_time, cur_process, time_elapsed, 'B');
            }
            
            // if process completes, print with code 'T'
            else {
                print_process(start_time, cur_process, time_elapsed, 'T');
                this->avg_turnaround.push_back(simulated_timer);
            }
            
        } else {
            if (this->blocked_queue.size() != 0) {
                // there are blocked processes but no processes ready,
                // CPU is idle, calculate time
                time_elapsed = get_idle_time(); 

                // print
                print_idle(time_elapsed);

                // update global timer
                this->simulated_timer = this->simulated_timer + time_elapsed;
                
                // update blocked queue
                update_blocked_queue();
            } 
        }
        debugging++;
    }
    float avg = calculate_avg_turnaround();
    print_done(avg);
}

bool Scheduler::still_running() {
    if ((ready_queue.size() != 0 || blocked_queue.size() != 0) && this->simulated_timer < 600) return true;
    else return false;
}

float Scheduler::update_prediction_value(process &p) {
    if (p.args.size() == 0) {
        std::cerr << p.name << "completed but still trying to run";
    }
    
    float ti = p.args.front();
    p.args.pop();
    
    if(p.prediction_value == 0) {
        p.prediction_value = ti;
        return ti;
    }
    
    p.prediction_value = (this->prediction_weight * p.prediction_value) +
        (1 - this->prediction_weight) * ti;
    return ti;
}

void Scheduler::arrive_proccesses() {
    for (int i = process_list.size() - 1; i >= 0; i--) {
        if (process_list[i].arrival_time > this->simulated_timer) {
            std::cout << "block process " << process_list[i].name << std::endl;

            this->blocked_queue.push_back(process_list[i]);
        } else {
            std::cout << "ready process " << process_list[i].name << std::endl;
            process_list[i].new_process = false;
            this->ready_queue.push(process_list[i]);
        }
    }
    process_list.clear();
    
    for(int i = blocked_queue.size()-1; i >= 0; i--) {
        if(blocked_queue[i].arrival_time < this->simulated_timer && blocked_queue[i].new_process) {
            std::cout << "arrive process " << blocked_queue[i].name << " " << blocked_queue[i].arrival_time << " " <<
                    this->simulated_timer << std::endl;
            blocked_queue[i].new_process = false;
            ready_queue.push(blocked_queue[i]);
            blocked_queue.erase(blocked_queue.begin() + i);
            i++;
        }
    }
}

process Scheduler::get_next_process() {
//    std::cout << "\nGNP\n";
    process shortest = ready_queue.top();
//    std::cout << "\tshortest name: " << shortest.name << "\n";
    this->ready_queue.pop();
//    std::cout << "\tnext shortest: " << ready_queue.top().name << "\n\n";
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
        
        // check if each is blocked. if it isn't move it to the ready_queue, if not, break;
        for(int i = 0; i < blocked_queue.size(); i++) {
            if(!blocked_queue[i].new_process && blocked_queue[i].time_blocked + this->block_duration < this->simulated_timer) {
                ready_queue.push(blocked_queue[i]);
                blocked_queue.erase(blocked_queue.begin() + i);
                i--;
            }
        }
    }
}

void Scheduler::print_process(float start_time, process p, float exec_time, char status) {
    std::cout << start_time << "\t" <<
            p.name << "\t" << 
            exec_time << "\t" <<
            status << "\t";
            if(status != 'T')
                std::cout << p.prediction_value << "\n";
            else
                std::cout << std::endl;
}

void Scheduler::print_idle(float idle_time) {
    std::cout << this->simulated_timer << "\t(IDLE)\t" << idle_time << "\tI\n";
}

void Scheduler::print_done(float avg_turnaround) {
    std::cout << this->simulated_timer << "\t(DONE)\t" << avg_turnaround << "\n";
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
    for(int i = 0; i < blocked_queue.size(); i++) {
        if(!blocked_queue[i].new_process) {
            // std::cout << "idle calc " << blocked_queue[i].time_blocked << " " << this->block_duration << std::endl;
            return this->block_duration;
        }
    }
    return 0;
}

float Scheduler::calculate_avg_turnaround() {
    int sum = 0, size = this->avg_turnaround.size();
    while (!this->avg_turnaround.empty()) {
        sum += this->avg_turnaround.front();
        this->avg_turnaround.pop_back();
    }
    return sum / size;
}