/**
* Assignment 5: Page replacement algorithms
 * @file lifo_replacement.cpp
 * @author Mitchell Karan
 * @brief A class implementing the Last in First Out (LIFO) page replacement algorithms
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "lifo_replacement.h"
#include <stdexcept>

LIFOReplacement::LIFOReplacement(int num_pages, int num_frames)
    : Replacement(num_pages, num_frames) {}

LIFOReplacement::~LIFOReplacement() {}

/**
 * @brief Load a page into an available frame. Free frames are assumed to be available.
 */
void LIFOReplacement::load_page(int page_num) {
    // Push the page onto the stack (LIFO behavior)
    page_stack.push(page_num);

    // Update the page table
    page_table[page_num].frame_num = num_frames - free_frames; // Assign a frame
    page_table[page_num].valid = true;                        // Mark as valid
}

/**
 * @brief Replace a page when no free frames are available.
 * @param page_num The page to be loaded.
 * @return The victim page that was replaced.
 */
int LIFOReplacement::replace_page(int page_num) {
    if (page_stack.empty()) {
        throw std::runtime_error("No pages to replace: page stack is empty.");
    }

    // Get the most recently added page (top of the stack)
    int victim_page = page_stack.top();
    page_stack.pop(); // Remove it from the stack

    // Invalidate the victim page in the page table
    page_table[victim_page].valid = false;

    // Load the new page into the stack
    load_page(page_num);

    return victim_page; // Return the replaced page
}
