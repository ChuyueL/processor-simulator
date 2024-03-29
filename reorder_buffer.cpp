#include "reorder_buffer.h"

void ROBEntry::reclaim() {
    in_use = false;
    is_store_instr = false;
    is_branch = false;
    opcode = COUNT;
    rs_tag = PlaceholderTag();
    instr_type = J;
    destination = 0;
    result = 0;
    ready = false;
}

void ReorderBuffer::pop() {
    buffer[head].reclaim();

    head = (head + 1) % ROB_SIZE;
}

//returns index of new entry
int ReorderBuffer::push(ROBEntry newEntry) {
    if (!full())
    {
        buffer[tail] = newEntry;
        int index = tail;

        tail = (tail + 1) % ROB_SIZE;

        return index;
    }
    return -1;
    
}

bool ReorderBuffer::full() {
    if (tail == head-1) return true;
    return false;
}

bool ReorderBuffer::empty() {
    if (tail == head) return true;
    return false;
}

ROBEntry ReorderBuffer::get_front() {
    return buffer[head];
}

void ReorderBuffer::flush() {
    for (ROBEntry& entry : buffer) {
        entry.reclaim();
    }

    head = 0;
    tail = 0;
}