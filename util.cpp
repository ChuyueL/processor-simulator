#include "util.h"

void print_cpu_info(Instruction instr, Hardware hw) {
    if (instr.opcode == COUNT) {
        return;
    }
    std::cout << "REGISTERS" << std::endl;
    int counter = 0;
    for (int reg : hw.reg_file) {
        std::cout << register_to_string(static_cast<Register>(counter)) << "=";
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
}

void print_instruction(Instruction instr) {
    if (instr.opcode == COUNT) {
        std::cout << "NOP\n";
        return;
    }
    std::cout << "opcode=" << opcode_to_string(instr.opcode);
    std::cout << " rd=" << register_to_string((Register)instr.rd);
    std::cout << " rs1=" << register_to_string((Register)instr.rs1);
    std::cout << " rs2=" << register_to_string((Register)instr.rs2);
    std::cout << " imm=" << std::to_string(instr.imm);
    std::cout << " label=" << instr.label;
    std::cout << std::endl;
}