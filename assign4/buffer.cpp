/**
* Assignment 4: Producer Consumer Problem
 * @file buffer.cpp
 * @author Mitchell Karan
 * @brief Implementation file for the buffer class
 * @version 0.1
 */

#include "buffer.h"

Buffer::Buffer(int size) : size(size), count(0) {
    buffer.reserve(size);
    //initialize threads and mutex lock
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&not_full, nullptr);
    pthread_cond_init(&not_empty, nullptr);
}

Buffer::~Buffer() {
    //destruct thread stuff
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);
}

bool Buffer::insert_item(buffer_item item) {
    //lock
    pthread_mutex_lock(&mutex);

    //check if full
    while (is_full()) {
        pthread_cond_wait(&not_full, &mutex);
    }

    //add to buffer
    buffer.push_back(item);
    count++;

    //unlock
    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&mutex);

    return true;
}

bool Buffer::remove_item(buffer_item *item) {
    //lock
    pthread_mutex_lock(&mutex);

    //check empty
    while (is_empty()) {
        pthread_cond_wait(&not_empty, &mutex);
    }

    //remove item
    *item = buffer.front();
    buffer.erase(buffer.begin());
    count--;

    //unlock
    pthread_cond_signal(&not_full);
    pthread_mutex_unlock(&mutex);

    return true;
}

int Buffer::get_size() {
    return size;
}

int Buffer::get_count() {
    return count;
}

bool Buffer::is_empty() {
    return count == 0;
}

bool Buffer::is_full() {
    return count == size;
}

void Buffer::print_buffer() {
    //print stuff
    std::cout << "Buffer: [";
    for (size_t i = 0; i < buffer.size(); ++i) {
        std::cout << buffer[i];
        if (i < buffer.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}
