#include "reorder_buffer.h"

void ROBEntry::reclaim() {
    opcode = COUNT;
    in_use = false;
    ready = false;
}

void ReorderBuffer::pop() {
    //buffer.pop_front();
    buffer[head].reclaim();

    head = (head + 1) % ROB_SIZE;
}

//returns index of new entry
int ReorderBuffer::push(ROBEntry newEntry) {
    if (!full())
    {
        //buffer.push_back(newEntry);
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