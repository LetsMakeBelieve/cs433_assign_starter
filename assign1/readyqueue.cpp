#include <iostream>
#include "readyqueue.h"

using namespace std;

//constructor
ReadyQueue::ReadyQueue() : capacity(500), heapSize(0) {
    heap = new PCB*[capacity];
    for (int i = 0; i < capacity; ++i){ //create empty heap
       heap[i] = nullptr;
    }
}

//copy constructor
ReadyQueue::ReadyQueue(const ReadyQueue& other) : capacity(other.capacity), heapSize(other.heapSize) {
    heap = new PCB*[capacity];
    for (int i = 0; i < heapSize; ++i) {
        heap[i] = new PCB(*other.heap[i]);  // Deep copy each PCB
    }
}

//destructor
ReadyQueue::~ReadyQueue() {
    delete[] heap; //thank you professor
}

//asignment operator
ReadyQueue& ReadyQueue::operator=(const ReadyQueue& other) {
    if (this == &other) {
        return *this; //self-assignment check
    }

    //clear data
    for (int i = 0; i < heapSize; ++i) {
        delete heap[i];
    }
    delete[] heap;

    //copy data from other
    capacity = other.capacity;
    heapSize = other.heapSize;
    heap = new PCB*[capacity];
    for (int i = 0; i < heapSize; ++i) {
        heap[i] = new PCB(*other.heap[i]);
    }

    return *this;
}

//add a PCB representing a process into the ready queue
void ReadyQueue::addPCB(PCB* pcbPtr) {
    if (heapSize >= capacity) {
        // Optional: Handle the case where the heap is full (e.g., resize or throw an exception)
        cout << "ReadyQueue is full. Cannot add more PCBs." << endl;
        return;
    }

    pcbPtr->setState(ProcState::READY);  // Ensure PCB state is set to READY
    heap[heapSize] = pcbPtr;
    heapifyUp(heapSize);
    ++heapSize;
}

//remove (and returns) the PCB with the highest priority from the queue
PCB* ReadyQueue::removePCB() {
    if (heapSize == 0) {
        cout << "ready queue is empty" << endl;
        return nullptr;
    }

    PCB* root = heap[0];
    root->setState(ProcState::RUNNING);  //change state to RUNNING
    heap[0] = heap[--heapSize];
    heapifyDown(0);
    return root;
}

//number of elements in the queue
int ReadyQueue::size() {
    return heapSize;
}

//show PCBs in the queue
void ReadyQueue::displayAll() {
    for (int i = 0; i < heapSize; ++i) {
        heap[i]->display();
    }
}

//maintain heap property while moving up
void ReadyQueue::heapifyUp(int index) {
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        if (heap[index]->getPriority() <= heap[parentIndex]->getPriority()) {
            break;
        }
        swap(heap[index], heap[parentIndex]);
        index = parentIndex;
    }
}

//maintain heap property while moving down
void ReadyQueue::heapifyDown(int index) {
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    int largest = index;

    if (leftChild < heapSize && heap[leftChild]->getPriority() > heap[largest]->getPriority()) {
        largest = leftChild;
    }

    if (rightChild < heapSize && heap[rightChild]->getPriority() > heap[largest]->getPriority()) {
        largest = rightChild;
    }

    if (largest != index) {
        swap(heap[index], heap[largest]);
        heapifyDown(largest);
    }
}
