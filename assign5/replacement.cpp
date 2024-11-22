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
    ++num_references; // Increment total references

    PageEntry& entry = page_table[page_num];
    if (entry.valid) {
        // Page is already in memory
        touch_page(page_num);
        if (is_write) {
            entry.dirty = true; // Mark as dirty if this is a write access
        }
        return false; // No page fault
    }

    // Page fault
    ++num_page_faults;
    if (free_frames > 0) {
        // Free frames are available; load the page
        load_page(page_num);
        --free_frames;
    } else {
        // No free frames; replace a page
        int victim_page = replace_page(page_num);
        PageEntry& victim_entry = page_table[victim_page];

        if (victim_entry.dirty) {
            // Handle writing back the dirty page if needed
            // (In a real system, this would involve writing to disk)
        }

        // Replace the victim with the new page
        ++num_page_replacements;
        load_page(page_num);
    }

    if (is_write) {
        entry.dirty = true; // Mark as dirty if this is a write access
    }

    return true; // Page fault occurred
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
