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
}   

Scheduler::Scheduler(const Scheduler& orig) {
}

Scheduler::~Scheduler() {
}

Scheduler& Scheduler::operator =(const Scheduler orig) {
}

Scheduler& Scheduler::operator =(Scheduler&& orig) {
}

float Scheduler::calculate_process_prediction_value(float last_execution_time, process p) {
    // W = w i
    // w = w i-1
    // W = a*w + (1 - a) * T
    return this->prediction_weight * p.prediction_value 
            + (1 - this->prediction_weight) * last_execution_time;
}