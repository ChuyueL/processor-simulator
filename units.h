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
        bool has_instr = false;
};

//fetch next instruction from instr list/array
class FetchUnit : public Unit {

    public:

    Instruction fetch(Hardware &hw, std::vector<Instruction> program);

    FetchUnit() {};

    
};

class DecodeUnit : public Unit {
    public:
        Instruction current_instruction;
        int decode(Instruction instr);

        DecodeUnit() {}
};

//Execute the instruction, given the arguments
class ExecuteUnit : public Unit {

    public:

    Instruction current_instruction;

    int execute(Instruction instr, Hardware &hw);

    ExecuteUnit() {
        current_instruction = RTypeInstruction(COUNT, 0, 0, 0);
    }


};

class MemoryUnit : Unit {
    public:
        Instruction current_instruction;

        int memory_stage(Instruction instr, Hardware &hw);

        MemoryUnit() {
            current_instruction = PlaceholderInstruction();
        }
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

        bool stalled = false;

        Pipeline() {
            fetch_unit = FetchUnit();
            decode_unit = DecodeUnit();
            execute_unit = ExecuteUnit();
        }

        void clock_cycle(Hardware &hw, std::vector<Instruction> program);

        void advance_pipeline();

        void flush_pipeline(Hardware &hw);

};

#endif