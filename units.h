#ifndef UNITS_H
#define UNITS_H

#include "instruction.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>

struct Hardware {
    int32_t memory[2048];
    int32_t reg_file[32];
    int32_t pc;
    bool finished;

    std::unordered_map<std::string, int> labels;
    std::unordered_map<std::string, int> variable_locations;

};

//fetch next instruction from instr list/array
class FetchUnit {

    public:

    Instruction fetch(Hardware hw, std::vector<Instruction> program);

    FetchUnit() {};

    
};

//Execute the instruction, given the arguments
class ExecuteUnit {

    public:

    int execute(Instruction instr, Hardware &hw);

    ExecuteUnit() {}


};

//Make changes to ARF
class WritebackUnit {

    public: 

    int writeback();

    WritebackUnit() {

    }
};

#endif