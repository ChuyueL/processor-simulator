#ifndef RESERVATION_STATION_H
#define RESERVATION_STATION_H

#include "instruction.h"
#include "units.h"
#include "tag.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>

class ReservationStation {
    public:

    int number; 
    Instruction instr = PlaceholderInstruction();
    FUType FU_type;
    Tag tag1 = PlaceholderTag();
    Tag tag2 = PlaceholderTag();
    int value1;
    int value2;
    int imm;
    int address;
    bool busy = false;

    ReservationStation(FUType _FU_type, int _number) {
        FU_type = _FU_type;
        number = _number;
    };

    Tag get_tag();

    void add_instruction(Hardware &hw, Instruction instr);
};

class PlaceholderRS : public ReservationStation {
    public:

    PlaceholderRS() : ReservationStation(NONE, 0) {

    }
};

#endif