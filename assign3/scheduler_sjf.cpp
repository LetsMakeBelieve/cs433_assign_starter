/**
* Assignment 3: CPU Scheduler
 * @file scheduler_sjf.cpp
 * @author Mitchell Karan
 * @brief This Scheduler class implements SJF scheduling algorithm.
 * @version 0.1
 */

#include "scheduler_sjf.h"
#include <algorithm>
#include <iostream>

//constructor
SchedulerSJF::SchedulerSJF() : current_time(0), ttime(0), wtime(0) {
}

//destructor
SchedulerSJF::~SchedulerSJF() {
}

//initialization
void SchedulerSJF::init(std::vector<PCB>& process_list) {
    //copy processes into rq
    ready_queue = process_list;

    //sort queue based on burst time (ascending order, shortest job first)
    std::sort(ready_queue.begin(), ready_queue.end(), [](const PCB& a, const PCB& b) {
        return a.burst_time < b.burst_time;  //sort by burst time, shorter first
    });

    std::cout << "SJF Scheduler initialized with " << ready_queue.size() << " processes." << std::endl;
}

//simulate
void SchedulerSJF::simulate() {
    std::cout << "Starting Shortest Job First Scheduling Simulation..." << std::endl;

    float total_waiting_time = 0;
    float total_turnaround_time = 0;
    int num_processes = ready_queue.size();
    int current_time = 0; //initialize clock

    //process jobs in ready_queue
    for (auto& process : ready_queue) {
        //calculate waiting time and turnaround time for each process
        int waiting_time = current_time - process.arrival_time;  //time process has waited in queue
        int turnaround_time = waiting_time + process.burst_time; //total time from arrival to completion

        //increment current simulation time by burst time of current process
        current_time += process.burst_time;

        //accumulate total waiting and turnaround times
        total_waiting_time += waiting_time;
        total_turnaround_time += turnaround_time;

        finished_processes.push_back(process);  //save process to finished_processes

        std::cout << "Process " << process.id << " (" << process.name << ") finished at time " << current_time << std::endl;
    }

    //calculate average waiting and turnaround times
    ttime = total_turnaround_time / num_processes;
    wtime = total_waiting_time / num_processes;

    std::cout << "SJF Scheduling Simulation Completed." << std::endl;
    std::cout << "Average Turnaround Time: " << ttime << std::endl;
    std::cout << "Average Waiting Time: " << wtime << std::endl;
}


//print
void SchedulerSJF::print_results() {
    std::cout << "SJF Scheduling Results:" << std::endl;
    std::cout << "Process ID\tName\tArrival Time\tBurst Time\tTurnaround Time\tWaiting Time" << std::endl;

    int current_time = 0;  //reset simulation clock

    for (const auto& process : finished_processes) {
        int waiting_time = current_time - process.arrival_time;
        int turnaround_time = waiting_time + process.burst_time;

        current_time += process.burst_time;

        std::cout << process.id << "\t\t"
                  << process.name << "\t"
                  << process.arrival_time << "\t\t"
                  << process.burst_time << "\t\t"
                  << turnaround_time << "\t\t"
                  << waiting_time << std::endl;
    }
    std::cout << "Average Turnaround Time: " << ttime << std::endl;
    std::cout << "Average Waiting Time: " << wtime << std::endl;
}
