/**
* Assignment 5: Page replacement algorithms
 * @file pagetable.cpp
 * @author Mitchell Karan
 * @brief This class represents a traditional pagetable data structure.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "pagetable.h"

// Constructor
PageTable::PageTable(int num_pages) {
    // Resize the vector to hold `num_pages` entries
    pages.resize(num_pages);

    // Initialize all pages with default values
    for (int i = 0; i < num_pages; ++i) {
        pages[i].frame_num = -1; // Indicates that no frame is assigned
        pages[i].valid = false;  // Page is not in physical memory
    }
}

// Destructor
PageTable::~PageTable() {
    // No dynamic memory to clean up since `std::vector` handles its own memory
}
