/**
* Assignment 3: CPU Scheduler
 * @file scheduler_fcfs.cpp
 * @author Mitchell Karan
 * @brief This Scheduler class implements the FCSF scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "scheduler_fcfs.h"
#include <iostream>

//constructor
SchedulerFCFS::SchedulerFCFS() : current_time(0), ttime(0), wtime(0) {
}

//destructor
SchedulerFCFS::~SchedulerFCFS() {
}

//initializatoin
void SchedulerFCFS::init(std::vector<PCB>& process_list) {
    //populate the ready_queue with the processes in their arrival order
    for (auto& process : process_list) {
        ready_queue.push(process);  //push each process into the ready_queue
    }
    std::cout << "FCFS Scheduler initialized with " << ready_queue.size() << " processes." << std::endl;
}

//simulation
void SchedulerFCFS::simulate() {
    std::cout << "FCFS Simulation..." << std::endl;

    float total_turnaround_time = 0;  // Track total turnaround time
    float total_waiting_time = 0;     // Track total waiting time
    int num_processes = ready_queue.size();  // Total number of processes

    int current_time = 0;  // Simulation time

    while (!ready_queue.empty()) {
        PCB current_process = ready_queue.front();  // Get the process at the front of the queue
        ready_queue.pop();  // Remove the process from the queue

        // Calculate completion, turnaround, and waiting times
        int completion_time = current_time + current_process.burst_time;
        int turnaround_time = completion_time - current_process.arrival_time;
        int waiting_time = turnaround_time - current_process.burst_time;

        // Update total turnaround and waiting times
        total_turnaround_time += turnaround_time;
        total_waiting_time += waiting_time;

        // Move the simulation time forward by the burst time of the current process
        current_time = completion_time;

        // Store results in the finished_processes with just the burst times
        finished_processes.push_back(current_process);

        std::cout << "Process " << current_process.id << " (" << current_process.name << ") finished at time " << completion_time << std::endl;
    }

    // Calculate the averages
    ttime = total_turnaround_time / num_processes;
    wtime = total_waiting_time / num_processes;

    std::cout << "FCFS Simulation Completed." << std::endl;
}

//print
void SchedulerFCFS::print_results() {
    std::cout << "FCFS Simulation Results:" << std::endl;
    std::cout << "Process ID\tName\tArrival Time\tBurst Time\tT-around Time\tWaiting Time" << std::endl;
    int current_time = 0;

    for (const auto& process : finished_processes) {
        current_time += process.burst_time;
        int turnaround_time = current_time - process.arrival_time;
        int waiting_time = turnaround_time - process.burst_time;

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
