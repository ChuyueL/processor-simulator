#ifndef REORDER_BUFFER_H
#define REORDER_BUFFER_H

#include "instruction.h"
#include "units.h"
#include "reservation_station.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>
#include <deque>
#include <array>

#define ROB_SIZE 50

class ROBEntry {
    public:

    bool in_use = false;
    bool is_store_instr = false;
    bool is_branch = false;
    Opcode opcode = COUNT;
    Tag rs_tag = PlaceholderTag();
    InstrType instr_type = J;
    int destination = 0;
    int result = 0;
    bool ready = false;

    void reclaim();
};

class PlaceholderROBEntry : public ROBEntry {

};

class ReorderBuffer {
    // ROBEntry head;
    // ROBEntry tail;

    // void pop();

    // void push();

    public:
        // std::deque<ROBEntry> buffer; 

        std::array<ROBEntry, ROB_SIZE> buffer;

        int head = 0;
        int tail = 0;

        void pop();
        int push(ROBEntry newEntry);
        bool full();
        bool empty();
        void flush();

        ROBEntry get_front();



};

#endif