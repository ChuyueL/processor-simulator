#ifndef UNITS_H
#define UNITS_H

#include "instruction.h"
#include "hardware.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>


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

    FetchUnit() {
        current_instruction = PlaceholderInstruction();
        next_instruction = PlaceholderInstruction();
    };

    
};

class DecodeUnit : public Unit {
    public:
        bool decode(Instruction instr, Hardware &hw);

        DecodeUnit() {
            current_instruction = PlaceholderInstruction();
            next_instruction = PlaceholderInstruction();
        }
};

//Execute the instruction, given the arguments
class ExecuteUnit : public Unit {

    public:
    int result = 0;

    int execute(Instruction instr, Hardware &hw);

    ExecuteUnit() {
        current_instruction = PlaceholderInstruction();
        next_instruction = PlaceholderInstruction();
    }


};

class MemoryUnit : public Unit {
    public:
        int alu_output = 0;
        int result = 0;
        
        int memory_stage(Instruction instr, Hardware &hw);

        MemoryUnit() {
            current_instruction = PlaceholderInstruction();
            next_instruction = PlaceholderInstruction();
        }
};

//Make changes to ARF
class WritebackUnit : public Unit {

    public: 
    int alu_output = 0;
    int load_mem_data = 0;

    int writeback(Instruction instr, Hardware &hw);

    WritebackUnit() {
        current_instruction = PlaceholderInstruction();
        next_instruction = PlaceholderInstruction();
    }
};

class Pipeline {
    public:
        FetchUnit fetch_unit;
        DecodeUnit decode_unit;
        ExecuteUnit execute_unit;
        MemoryUnit memory_unit;
        WritebackUnit writeback_unit;

        bool stalled = false;

        int instructions_executed = 0;

        Pipeline() {
            fetch_unit = FetchUnit();
            decode_unit = DecodeUnit();
            execute_unit = ExecuteUnit();
            memory_unit = MemoryUnit();
            writeback_unit = WritebackUnit();
        }

        void clock_cycle(Hardware &hw, std::vector<Instruction> program);

        void advance_pipeline(Hardware &hw);

        void flush_pipeline(Hardware &hw, int number);

        void stall_pipeline();

        void continue_pipeline();

        void update_scoreboard(Hardware &hw);

};

#endif