#include "units.h"

Instruction FetchUnit::fetch(Hardware hw, std::vector<Instruction> program) {
    return program[hw.pc];
}

int ExecuteUnit::execute(Instruction instr, Hardware &hw) {
    
    std::cout << "opcode " << std::to_string(instr.opcode);
    std::cout << "rd " << std::to_string(instr.rd);
    std::cout << "rs1 " << std::to_string(instr.rs1);
    std::cout << "rs2 " << std::to_string(instr.rs2);
    std::cout << "imm " << std::to_string(instr.imm);
    std::cout << "label " << instr.label;
    std::cout << std::endl;

    std::cout << "REGISTERS" << std::endl;
    int counter = 0;
    for (int reg : hw.reg_file) {
        std::cout << static_cast<Register>(counter) << " ";
        counter++;
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

        case SLT:
            if (hw.reg_file[instr.rs1] < hw.reg_file[instr.rs2]) {
                hw.reg_file[instr.rd] = 1;
                std::cout << "less than" << std::endl;
            }
            else {
                hw.reg_file[instr.rd] = 0;
            }
            break;
        
        case ADDI:
            hw.reg_file[instr.rd] = hw.reg_file[instr.rs1] + instr.imm;
            break;
        
        case SW:
            hw.memory[hw.reg_file[instr.rs1] + instr.imm] = hw.reg_file[instr.rs2];
            break;
                
        case LW:
            hw.reg_file[instr.rd] = hw.memory[hw.reg_file[instr.rs1] + instr.imm];
            break;

        case BLT:
            if (hw.reg_file[instr.rs1] < hw.reg_file[instr.rs2]) {
                hw.pc = hw.labels[instr.label];
                hw.pc--;
            }
            break;
        
        case BEQ:
            if (hw.reg_file[instr.rs1] == hw.reg_file[instr.rs2]) {
                hw.pc = hw.labels[instr.label];
                hw.pc--;
            }
            break;

        case HALT:
            hw.finished = true;
            break;
    }

    hw.pc++;
    return result;
}