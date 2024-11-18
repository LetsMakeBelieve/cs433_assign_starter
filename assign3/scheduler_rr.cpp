#include "scheduler_rr.h"
#include <algorithm>  //for std::min
#include <iostream>
#include <map>  //forstd::map

//constructor
SchedulerRR::SchedulerRR(int time_quantum) : time_quantum(time_quantum), current_time(0), ttime(0), wtime(0) {
}

//destructor
SchedulerRR::~SchedulerRR() {
}

//initialize scheduler with  list of processes
void SchedulerRR::init(std::vector<PCB>& process_list) {
    //add each process to  ready queue
    for (auto& process : process_list) {
        ready_queue.push(process);
    }
    std::cout << "RR Scheduler initialized with " << ready_queue.size() << " processes." << std::endl;
}

//simulate  Round Robin process scheduling
void SchedulerRR::simulate() {
    std::cout << "Starting RR-Scheduling Simulation..." << std::endl;

    float total_waiting_time = 0; 
    float total_turnaround_time = 0; 
    int num_processes = ready_queue.size();

    //maps to track remaining burst times and last execution times for each process
    std::map<int, int> remaining_burst_time;
    std::map<int, int> last_execution_time;

    //initialize
    std::queue<PCB> temp_queue = ready_queue;//copy ready_queue to process each element
    while (!temp_queue.empty()) {
        PCB current_process = temp_queue.front();
        temp_queue.pop();
        
        remaining_burst_time[current_process.id] = current_process.burst_time;
        last_execution_time[current_process.id] = 0;  //initially, all processes have 0 last execution time
    }

    //start RR simulation
    while (!ready_queue.empty()) {
        PCB current_process = ready_queue.front();  //get next process in queue
        ready_queue.pop();  //remove from queue

        //it took me about an hour of debugging to realize i needed to type cast "unsigned int" to "int" to use std::min()
        int time_slice = std::min(time_quantum, static_cast<int>(remaining_burst_time[current_process.id]));

        //simulate running process for time slice
        current_time += time_slice;
        remaining_burst_time[current_process.id] -= time_slice;

        //calculate waiting time based 
        int waiting_time = current_time - last_execution_time[current_process.id] - time_slice;
        total_waiting_time += waiting_time;

        //update last execution time to current time
        last_execution_time[current_process.id] = current_time;

        //if process is finished...
        if (remaining_burst_time[current_process.id] == 0) {
            //calculate turnaround time
            int turnaround_time = current_time - current_process.arrival_time;
            total_turnaround_time += turnaround_time;

            //mark process as finished
            finished_processes.push_back(current_process);
            std::cout << "Process " << current_process.id << " (" << current_process.name << ") finished at time " << current_time << std::endl;
        } else {
            //else, re-add it to  end of  queue
            ready_queue.push(current_process);
        }
    }

    //calculate average waiting and turnaround times
    ttime = total_turnaround_time / num_processes;
    wtime = total_waiting_time / num_processes;

    std::cout << "RR-Scheduling Simulation Completed." << std::endl;
    std::cout << "Average Turnaround Time: " << ttime << std::endl;
    std::cout << "Average Waiting Time: " << wtime << std::endl;
}

//print results after simulation
void SchedulerRR::print_results() {
    std::cout << "RR Scheduling Results:" << std::endl;
    std::cout << "Process ID\tName\tArrival Time\tBurst Time\tTurnaround Time\tWaiting Time" << std::endl;

    int current_time = 0;

    for (const auto& process : finished_processes) {
        int turnaround_time = current_time - process.arrival_time;
        int waiting_time = turnaround_time - process.burst_time;

        std::cout << process.id << "\t\t"
                  << process.name << "\t"
                  << process.arrival_time << "\t\t"
                  << process.burst_time << "\t\t"
                  << turnaround_time << "\t\t"
                  << waiting_time << std::endl;
    }

    //
    std::cout << "Average Turnaround Time: " << ttime << std::endl;
    std::cout << "Average Waiting Time: " << wtime << std::endl;
}