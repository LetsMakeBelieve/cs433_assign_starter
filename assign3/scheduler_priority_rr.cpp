/**
* Assignment 3: CPU Scheduler
 * @file scheduler_priority.cpp
 * @author Mitchell Karan
 * @brief This Scheduler class implements Priority scheduling algorithm.
 * @version 0.1
 */


#include "scheduler_priority_rr.h"
#include <iostream>
#include <map>

//constructor
SchedulerPriorityRR::SchedulerPriorityRR(int time_quantum) : time_quantum(time_quantum), current_time(0), ttime(0), wtime(0) {
    priority_queues.resize(51);  //priorities 0 to 50
}

//destructor
SchedulerPriorityRR::~SchedulerPriorityRR() {
}

//initialization
void SchedulerPriorityRR::init(std::vector<PCB>& process_list) {
    //place processes into their respective priority queue
    for (auto& process : process_list) {
        priority_queues[process.priority].push(process);
    }
    std::cout << "Time Quantum = " << time_quantum << std::endl; //tq = 10 by default
    std::cout << "Priority RR Scheduler initialized with " << process_list.size() << " processes." << std::endl;

}

//simulation
void SchedulerPriorityRR::simulate() {
    std::cout << "Priority RR Simulation..." << std::endl;

    float total_waiting_time = 0;
    float total_turnaround_time = 0;
    int num_processes = 0;

    //store last execution time for each process to calculate waiting time
    std::map<int, int> last_execution_time;

    bool processes_left = true;
    while (processes_left) { //while there are processes...
        processes_left = false;

        //loop through each priority level (highest to lowest)
        for (int priority = 50; priority >= 0; --priority) {
            if (!priority_queues[priority].empty()) { //if there are still processes...
                processes_left = true;

                PCB current_process = priority_queues[priority].front();  //get process at front
                priority_queues[priority].pop();  //remove it from queue

                //if first time process is running, initialize its last execution time
                if (last_execution_time.find(current_process.id) == last_execution_time.end()) {
                    last_execution_time[current_process.id] = current_time;
                }

                //how long process will run
                int time_slice = std::min(time_quantum, (int)current_process.burst_time);

                //waiting time
                int waiting_time = current_time - last_execution_time[current_process.id];
                total_waiting_time += waiting_time;

                //run process
                current_process.burst_time -= time_slice;
                current_time += time_slice;

                //update last execution time
                last_execution_time[current_process.id] = current_time;

                //if process is finished, record its turnaround time
                if (current_process.burst_time == 0) {
                    int turnaround_time = current_time - current_process.arrival_time;
                    total_turnaround_time += turnaround_time;
                    num_processes++;  //increment number of processes finished

                    finished_processes.push_back(current_process);
                    std::cout << "Process " << current_process.id << " (" << current_process.name << ") finished at time " << current_time << std::endl;
                } else {
                    //else add process back to queue
                    priority_queues[priority].push(current_process);
                }
            }
        }
    }

    //calculate averages
    ttime = total_turnaround_time / num_processes;
    wtime = total_waiting_time / num_processes;

    std::cout << "Priority Round Robin Simulation Completed." << std::endl;
    std::cout << "Average Turnaround Time: " << ttime << std::endl;
    std::cout << "Average Waiting Time: " << wtime << std::endl;
}


//print
void SchedulerPriorityRR::print_results() {
    std::cout << "Priority Round Robin Simulation Results:" << std::endl;
    std::cout << "Process ID\tName\tPriority\tArrival Time\tBurst Time\tT-around Time\tWaiting Time" << std::endl;
    int current_time = 0;

    for (const auto& process : finished_processes) {
        //recalculate turnaround time and waiting time for each process
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

    //print overall average results
    std::cout << "Average Turnaround Time: " << ttime << std::endl;
    std::cout << "Average Waiting Time: " << wtime << std::endl;
}

