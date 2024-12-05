/**
* Assignment 5: Page replacement algorithms
 * @file fifo_replacement.cpp
 * @author Mitchell Karan
 * @brief A class implementing FIFO page replacement algorithms
 * @version 0.1
 */

#include "fifo_replacement.h"
#include <stdexcept>

//constructor
FIFOReplacement::FIFOReplacement(int num_pages, int num_frames) : Replacement(num_pages, num_frames) {
}
//destructor
FIFOReplacement::~FIFOReplacement() = default;

void FIFOReplacement::load_page(int page_num) {
    PageEntry new_page; //make new page
    new_page.valid = true; //page is valid
    new_page.frame_num = next_frame_num; //set frame number as current

    page_queue.push(page_num); //add page number queu
    free_frames--;//decrement free frames

    page_table[page_num] = new_page; //update page table

    next_frame_num++; //go to next frame
}

//access an invalid page and no free frames are available
int FIFOReplacement::replace_page(int page_num) {
    PageEntry new_page; //make new page
    new_page.valid = true; //page is valid
    new_page.frame_num = next_frame_num; //set frame number as current

    //get front and remove
    int victim_page_number = page_queue.front();
    page_queue.pop();

    //add to the FIFO queue
    page_queue.push(page_num);

    //update the page table and set replaced page as invalid
    page_table[page_num] = new_page;
    page_table[victim_page_number].valid = false;

    //return the page number of the page to be replaced
    return victim_page_number;
}