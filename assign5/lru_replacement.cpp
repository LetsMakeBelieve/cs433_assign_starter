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
    : Replacement(num_pages, num_frames) {
    page_map.reserve(num_pages); //reserve space to avoid rehashing
}

LRUReplacement::~LRUReplacement() {}

void LRUReplacement::touch_page(int page_num) {
    auto it = page_map.find(page_num);
    if (it != page_map.end() && it->second != page_list.begin()) {
        //remove from current position and move to front
        page_list.erase(it->second);
        page_list.push_front(page_num);
        page_map[page_num] = page_list.begin();
    }
}

void LRUReplacement::load_page(int page_num) {
    page_list.push_front(page_num);        //add page to front
    page_map[page_num] = page_list.begin(); //update map

    //assign a free frame to page
    int frame_num = num_frames - free_frames;
    page_table[page_num].frame_num = frame_num;
    page_table[page_num].valid = true;

    if (free_frames > 0) {
        --free_frames;
    }
}

int LRUReplacement::replace_page(int page_num) {
    if (page_list.empty()) {
        throw std::runtime_error("Page list is empty during replace_page.");
    }

    int victim_page = page_list.back(); //least recently used page
    page_list.pop_back(); //remove victim
    page_map.erase(victim_page); //remove from map
    page_table[victim_page].valid = false; //invalidate victim

    //assign victim's frame to new page
    page_table[page_num].frame_num = page_table[victim_page].frame_num;
    page_table[page_num].valid = true;

    //add new page to the front of the list
    page_list.push_front(page_num);
    page_map[page_num] = page_list.begin();
    return victim_page;
}

