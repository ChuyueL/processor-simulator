#ifndef UNITS_H
#define UNITS_H

#include "instruction.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>

struct Hardware {
    int32_t memory[2048] = {0};
    int32_t reg_file[32] = {0};
    int32_t pc;
    bool finished = false;
    bool reg_updating[32] = {false};

    std::unordered_map<std::string, int> labels;
    std::unordered_map<std::string, int> variable_locations;

};

class Unit {
    public:
        Instruction current_instruction;
        Instruction next_instruction;
        bool stalled = false;
};

//fetch next instruction from instr list/array
class FetchUnit : Unit {

    public:

    Instruction fetch(Hardware &hw, std::vector<Instruction> program);

    FetchUnit() {};

    
};

class DecodeUnit : Unit {
    public:
        Instruction current_instruction;
        int decode(Instruction instr);

        DecodeUnit() {}
};

//Execute the instruction, given the arguments
class ExecuteUnit : Unit {

    public:

    Instruction current_instruction;

    int execute(Instruction instr, Hardware &hw);

    ExecuteUnit() {}


};

//Make changes to ARF
class WritebackUnit : Unit {

    public: 

    int writeback();

    WritebackUnit() {

    }
};

class Pipeline {
    public:
        FetchUnit fetch_unit;
        DecodeUnit decode_unit;
        ExecuteUnit execute_unit;
};

#endif