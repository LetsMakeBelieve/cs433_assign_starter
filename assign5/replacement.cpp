/**
* Assignment 5: Page replacement algorithms
 * @file replacement.cpp
 * @author Mitchell Karan
 * @brief A base class for different page replacement algorithms.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code
#include <iostream>
#include "replacement.h"

// Constructor
Replacement::Replacement(int num_pages, int num_frames)
    : page_table(num_pages), num_frames(num_frames), free_frames(num_frames) {}

// Destructor
Replacement::~Replacement() {}

// Simulate a single page access
bool Replacement::access_page(int page_num, bool is_write) {
    num_references++;  // Increment the count of references

    // If the page is valid, it calls the touch_page function.
    if (page_table[page_num].valid) {
        touch_page(page_num);
        return false;  // no page fault
    }
    // If the page is not valid but free frames are available, it calls the load_page function.
    if (!page_table[page_num].valid && free_frames > 0) {
        load_page(page_num);
        num_page_faults++;  // Increment the count of page faults
        return true;  // page fault
    }
    // If the page is not valid and there is no free frame, it calls the replace_page function.
    if (!page_table[page_num].valid && free_frames == 0) {
        replace_page(page_num);
        num_page_replacements++;  // Increment the count of page replacements
        num_page_faults++;  // Increment the count of page faults
        return true;  // page fault
    }
    return false;  // no page fault
}

// Load a page into a free frame
void Replacement::load_page(int page_num) {
    PageEntry& entry = page_table[page_num];
    entry.frame_num = num_frames - free_frames; // Assign a frame
    entry.valid = true;                        // Mark as valid
}

// Print out simulation statistics
void Replacement::print_statistics() const {
    std::cout << "Number of references: \t\t" << num_references << std::endl;
    std::cout << "Number of page faults: \t\t" << num_page_faults << std::endl;
    std::cout << "Number of page replacements: \t" << num_page_replacements << std::endl;
}
