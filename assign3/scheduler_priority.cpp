/**
* Assignment 3: CPU Scheduler
 * @file scheduler_priority.cpp
 * @author Mitchell Karan
 * @brief This Scheduler class implements Priority scheduling algorithm.
 * @version 0.1
 */

#include "scheduler_priority.h"
#include <algorithm>
#include <iostream>

//constructor
SchedulerPriority::SchedulerPriority() : current_time(0) {
}

//destructor
SchedulerPriority::~SchedulerPriority() {
}

//initialization
void SchedulerPriority::init(std::vector<PCB>& process_list) {
    //copy processes and sort by priority (higher number = higher priority)
    ready_queue = process_list;

    //sort ready queue (higher priority first)
    std::sort(ready_queue.begin(), ready_queue.end(), [](const PCB& a, const PCB& b) {
        return a.priority > b.priority;  //sort descending by priority
    });

    std::cout << "Priority Scheduler initialized with " << ready_queue.size() << " processes." << std::endl;
}

//simulate
void SchedulerPriority::simulate() {
    std::cout << "Priority Scheduling Simulation..." << std::endl;

    float total_waiting_time = 0;
    float total_turnaround_time = 0;
    int num_processes = ready_queue.size();

    for (auto& process : ready_queue) {
        //waiting time and turnaround time for current process
        int waiting_time = current_time - process.arrival_time;
        int turnaround_time = waiting_time + process.burst_time;

        //simulate running process to completion
        current_time += process.burst_time;

        //accumulate total waiting and turnaround times
        total_waiting_time += waiting_time;
        total_turnaround_time += turnaround_time;

        finished_processes.push_back(process);  //save process to finished_processes

        std::cout << "Process " << process.id << " (" << process.name << ") finished at time " << current_time << std::endl;
    }

    //calculate averages
    ttime = total_turnaround_time / num_processes;
    wtime = total_waiting_time / num_processes;

    std::cout << "Priority Scheduling Simulation Completed." << std::endl;
    std::cout << "Average Turnaround Time: " << ttime << std::endl;
    std::cout << "Average Waiting Time: " << wtime << std::endl;
}


//print results
void SchedulerPriority::print_results() {
    std::cout << "Priority Scheduling Results:" << std::endl;
    std::cout << "Process ID\tName\tPriority\tArrival Time\tBurst Time\tT-around Time\tWaiting Time" << std::endl;
    int current_time = 0;

    for (const auto& process : finished_processes) {
        current_time += process.burst_time;
        int turnaround_time = current_time - process.arrival_time;
        int waiting_time = turnaround_time - process.burst_time;

        std::cout << process.id << "\t\t"
                  << process.name << "\t"
                  << process.priority << "\t\t"
                  << process.arrival_time << "\t\t"
                  << process.burst_time << "\t\t"
                  << turnaround_time << "\t\t"
                  << waiting_time << std::endl;
    }

    std::cout << "Average Turnaround Time: " << ttime << std::endl;
    std::cout << "Average Waiting Time: " << wtime << std::endl;
}
