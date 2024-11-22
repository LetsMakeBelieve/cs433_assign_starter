/**
* Assignment 5: Page replacement algorithms
 * @file lru_replacement.cpp
 * @author Mitchell Karan
 * @brief A class implementing the LRU page replacement algorithms
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "lru_replacement.h"
#include <stdexcept>

LRUReplacement::LRUReplacement(int num_pages, int num_frames)
    : Replacement(num_pages, num_frames) {}

LRUReplacement::~LRUReplacement() {}

void LRUReplacement::touch_page(int page_num) {
    if (page_map.find(page_num) != page_map.end()) {
        page_list.erase(page_map[page_num]); // Remove from current position
    }
    page_list.push_front(page_num);          // Add to front
    page_map[page_num] = page_list.begin(); // Update map
}

void LRUReplacement::load_page(int page_num) {
    page_list.push_front(page_num);
    page_map[page_num] = page_list.begin();

    // Update the page table
    page_table[page_num].frame_num = num_frames - free_frames;
    page_table[page_num].valid = true;
}

int LRUReplacement::replace_page(int page_num) {
    if (page_list.empty()) {
        throw std::runtime_error("No pages to replace: page list is empty.");
    }
    
    int victim_page = page_list.back();  // Least recently used page
    page_list.pop_back();
    page_map.erase(victim_page);
    page_table[victim_page].valid = false; // Invalidate the victim page
    load_page(page_num);
    return victim_page;
}

