/**
 * Assignment 1: priority queue of processes
 * @file pcbtable.h
 * @author Mitchell Karan
 * @brief This is the implementation file for the PCBTable class.
 * //You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
 * // Remember to add sufficient comments to your code
 */

#include "pcbtable.h"


/**
 * @brief Construct a new PCBTable object of the given size (number of PCBs)
 *
 * @param size: the capacity of the PCBTable
 */
PCBTable::PCBTable(int size) {
   blocks.resize(size, nullptr); //initialize blocks & set all elements to nullptr
}

/**
 * @brief Destroy the PCBTable object. Make sure to delete all the PCBs in the table.
 *
 */
PCBTable::~PCBTable() {
   for (PCB* pcb : blocks) {
        if (pcb != nullptr) {
            delete pcb;
        }
    }
    blocks.clear();
}

/**
 * @brief Get the PCB at index "idx" of the PCBTable.
 *
 * @param idx: the index of the PCB to get
 * @return PCB*: pointer to the PCB at index "idx"
 */
PCB* PCBTable::getPCB(unsigned int idx) {
    if (idx >= blocks.size()) {
        return nullptr; // If the index is out of bounds, return nullptr.
    }
    return blocks[idx]; // Return the PCB at the specified index.
}

/**
 * @brief Add a PCB pointer to the PCBTable at index idx.
 *
 * @param pcb: the PCB to add
 */
void PCBTable::addPCB(PCB *pcb, unsigned int idx) {
    if (idx < blocks.size()) {
        // If there's already a PCB at that index, delete it to avoid memory leaks
        if (blocks[idx] != nullptr) {
            delete blocks[idx];
        }
        // Add the new PCB to the specified index
        blocks[idx] = pcb;
    }
}
