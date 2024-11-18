/**
* Assignment 4: Producer Consumer Problem
 * @file main.cpp
 * @author Mitchell Karan
 * @brief The main program for the producer consumer problem.
 * @version 0.1
 */
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include "buffer.h"

using namespace std;

//global buffer (default size = 5)
Buffer buffer;

//producer thread
void *producer(void *param) {
    int id = *((int *) param); //extracts that integer value so the thread can use it (its hard to look at)
    buffer_item item = id;

    while (true) {
        usleep(rand() % 1000000);

        if (buffer.insert_item(item)) {
            cout << "Producer " << id << ": Inserted item " << item << endl;
            buffer.print_buffer();
        } else {
            cout << "Producer error condition" << endl;
        }
    }
}

//consumer thread
void *consumer(void *param) {
    buffer_item item;

    while (true) {
        usleep(rand() % 1000000);

        if (buffer.remove_item(&item)) {
            cout << "Consumer Removed item " << item << endl;
            buffer.print_buffer();
        } else {
            cout << "Consumer error condition" << endl;
        }
    }
}

int main(int argc, char *argv[]) {
    //make sure there are fewer than 4 args else spit out error
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <sleep time> <num producers> <num consumers>" << endl;
        return -1;
    }

    //get command line args
    int sleep_time = atoi(argv[1]); 
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);

    //initialize threads with command line args
    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];
    int producer_ids[num_producers];

    //produce
    for (int i = 0; i < num_producers; ++i) {
        producer_ids[i] = i + 1;
        pthread_create(&producers[i], nullptr, producer, &producer_ids[i]);
    }

    //consume
    for (int i = 0; i < num_consumers; ++i) {
        pthread_create(&consumers[i], nullptr, consumer, nullptr);
    }

    //sleep
    sleep(sleep_time);

    cout << "Main thread exiting..." << endl;
    exit(0);
}
