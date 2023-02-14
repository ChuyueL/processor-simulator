#include "units.h"

Instruction FetchUnit::fetch(Hardware hw, std::vector<Instruction> program) {
    return program[hw.pc];
}

int ExecuteUnit::execute(Instruction instr, Hardware &hw) {
    std::cout << "REGISTERS" << std::endl;
    for (int reg : hw.reg_file) {
        std::cout << reg << " ";
    }
    std::cout << std::endl;
    std::cout << "MEMORY " << std::endl;
    for (int entry : hw.memory) {
        std::cout << entry << " ";
    }
    std::cout << std::endl;

    std::cout << "VARIABLE LOCATIONS " << std::endl;

    for (auto entry : hw.variable_locations) {
        std::cout << entry.first << " ";
        std::cout << entry.second << " ";

    }
    std::cout << std::endl;

    Opcode opcode = instr.opcode;
    int32_t result = 0;
    switch (opcode) {
        case ADD:
            hw.reg_file[instr.rd] = hw.reg_file[instr.rs1] + hw.reg_file[instr.rs2];
            break;

        case SUB:
            hw.reg_file[instr.rd] = hw.reg_file[instr.rs1] - hw.reg_file[instr.rs2];
            break;
        
        case ADDI:
            hw.reg_file[instr.rd] = hw.reg_file[instr.rs1] + instr.imm;
            break;
        
        case SW:
            hw.memory[instr.rs1 + instr.imm] = instr.rs2;
            break;
                
        case LW:
            hw.reg_file[instr.rd] = hw.memory[instr.rs1 + instr.imm];
            break;

        case BLT:
            if (hw.reg_file[instr.rs1] < hw.reg_file[instr.rs2]) {
                hw.pc = hw.labels[instr.label];
            }
            break;
        
        case HALT:
            hw.finished = true;
            break;
    }

    hw.pc++;
    return result;
}