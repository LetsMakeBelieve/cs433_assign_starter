/**
* Assignment 5: Page replacement algorithms
 * @file lifo_replacement.cpp
 * @author Mitchell Karan
 * @brief A class implementing the Last in First Out (LIFO) page replacement algorithms
 * @version 0.1
 */
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
    PageEntry new_page; //make new page
    new_page.valid = true; //page is valid
    new_page.frame_num = next_frame_num; //set frame number as current

    page_stack.push(page_num); //add page number queue

    free_frames--;//decrement free frames

    page_table[page_num] = new_page; //update the page table
    next_frame_num++; //increment next open frame number
}

/**
 * @brief Replace a page when no free frames are available.
 * @param page_num The page to be loaded.
 * @return The victim page that was replaced.
 */
int LIFOReplacement::replace_page(int page_num) {//create a new page entry
    PageEntry new_page; //make new page
    new_page.valid = true; //page is valid
    new_page.frame_num = next_frame_num; //set frame number as current

    int victim_page_number = page_stack.top(); //get top of queue
    page_stack.pop(); //remove page number from queue

    //add to queue
    page_stack.push(page_num);

    //update page table
    page_table[page_num] = new_page;
    //set replaced page as invalid
    page_table[victim_page_number].valid = false;

    //return the page number of the page to be replaced
    return victim_page_number;
}