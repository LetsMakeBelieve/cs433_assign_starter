/**
 * Assignment 1: priority queue of processes
 * @file readyqueue.h
 * @author Mitchell Karan
 * @brief ReadyQueue is a queue of PCB's that are in the READY state to be scheduled to run.
 * It should be a priority queue such that the process with the highest priority can be selected next.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient comments to your code
#pragma once

#include "pcb.h"

/**
 * @brief A queue of PCB's that are in the READY state to be scheduled to run.
 * It should be a priority queue such that the process with the highest priority can be selected next.
 */
class ReadyQueue {
private:
    // Internal data structure to hold the PCBs
    PCB** heap;       //array of PCB pointers
    int capacity;     //capacity of the heap
    int heapSize;     //number of elements in the heap

    // Helper functions for heap operations
    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    /**
     * @brief Construct a new ReadyQueue object
     *
     */
    ReadyQueue();
    ReadyQueue(const ReadyQueue& other);

    /**
     * @brief Construct a new ReadyQueue object with specified capacity
     *
     * @param capacity: the initial capacity of the queue
     */
    ReadyQueue(int capacity);

    /**
     * @brief Overwrite a readyqueue object with another
     *
     * @param other: the ReadyQueue object that is overwriting this one
     */
    ReadyQueue& operator=(const ReadyQueue& other);

    /**
     * @brief Destructor
     */
    ~ReadyQueue();

    /**
     * @brief Add a PCB representing a process into the ready queue.
     *
     * @param pcbPtr: the pointer to the PCB to be added
     */
    void addPCB(PCB* pcbPtr);

    /**
     * @brief Remove and return the PCB with the highest priority from the queue
     *
     * @return PCB*: the pointer to the PCB with the highest priority
     */
    PCB* removePCB();

    /**
     * @brief Returns the number of elements in the queue.
     *
     * @return int: the number of PCBs in the queue
     */
    int size();

    /**
     * @brief Display the PCBs in the queue.
     */
    void displayAll();
};