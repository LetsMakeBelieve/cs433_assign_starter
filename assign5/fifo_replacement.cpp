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

FIFOReplacement::FIFOReplacement(int num_pages, int num_frames)
    : Replacement(num_pages, num_frames) {

}

FIFOReplacement::~FIFOReplacement() {
}

void FIFOReplacement::load_page(int page_num) {
    // Add the page to the FIFO queue and update the page table.
    page_queue.push(page_num);
    page_table[page_num] = page_queue.size() - 1; // Map page to frame index.
}

int FIFOReplacement::replace_page(int page_num) {
    // Replace the oldest page in the queue.
    int victim_page = page_queue.front(); // Get the oldest page.
    page_queue.pop(); // Remove it from the queue.

    // Remove the victim from the page table.
    page_table.erase(victim_page);

    // Add the new page to the page table and queue.
    page_queue.push(page_num);
    page_table[page_num] = page_queue.size() - 1;

    return victim_page; // Return the page that was replaced.
}