/* 
 * File:   Scheduler.cpp
 * Author: julia & sam
 * 
 * Created on April 20, 2018, 9:27 AM
 */

#include "Scheduler.h"

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

Scheduler::operator =(const Scheduler orig) {
}

Scheduler::operator =(Scheduler&& orig) {
}