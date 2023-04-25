#ifndef LDST_QUEUE_H
#define LDST_QUEUE_H

#include "instruction.h"
#include "units.h"
#include "reservation_station.h"
#include "reorder_buffer.h"
#include "tag.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>

class LDSTQueue_Entry {

    public:
    int address = 0;
    Tag RS_Tag = PlaceholderTag();
    bool is_store = false;

    LDSTQueue_Entry(int _addr, Tag _tag, bool _store) {
        address = _addr;
        RS_Tag = _tag;
        is_store = _store;

    }


};

#endif