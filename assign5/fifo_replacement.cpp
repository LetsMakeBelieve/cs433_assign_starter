/**
* Assignment 5: Page replacement algorithms
 * @file fifo_replacement.cpp
 * @author Mitchell Karan
 * @brief A class implementing the FIFO page replacement algorithms
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "fifo_replacement.h"
#include <stdexcept>

FIFOReplacement::FIFOReplacement(int num_pages, int num_frames)
    : Replacement(num_pages, num_frames) {}

FIFOReplacement::~FIFOReplacement() {}

void FIFOReplacement::load_page(int page_num) {
    // Add the page to the FIFO queue
    page_queue.push(page_num);

    // Update the page table entry
    page_table[page_num].frame_num = num_frames - free_frames; // Example frame assignment
    page_table[page_num].valid = true;                        // Mark as valid
}

int FIFOReplacement::replace_page(int page_num) {
    if (page_queue.empty()) {
        throw std::runtime_error("No pages to replace: page queue is empty.");
    }

    int victim_page = page_queue.front();
    page_queue.pop();
    page_table[victim_page].valid = false; // Invalidate the victim page
    load_page(page_num); // Load the new page
    return victim_page;
}
