#include "units.h"

Instruction FetchUnit::fetch(Hardware &hw, std::vector<Instruction> program) {
    //hw.pc++;
    if (hw.pc < program.size()) {
        current_instruction = program[hw.pc];
    }
    else {
        current_instruction = PlaceholderInstruction();
    }

    hw.pc++;

    return current_instruction;
}

int DecodeUnit::decode(Instruction instr) {

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

    //current_instruction = instr;

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
                //hw.pc--;
            }
            break;
        
        case BEQ:
            if (hw.reg_file[instr.rs1] == hw.reg_file[instr.rs2]) {
                hw.pc = hw.labels[instr.label];
                //hw.pc--;
            }
            break;

        case HALT:
            hw.finished = true;
            break;
    }

    return result;
}

int MemoryUnit::memory_stage() {
    
}

void Pipeline::clock_cycle(Hardware &hw, std::vector<Instruction> program) {
    decode_unit.next_instruction = fetch_unit.fetch(hw, program);
    //execute_unit.next_instruction = fetch_unit.fetch(hw, program);
    execute_unit.next_instruction = decode_unit.current_instruction;

    if (execute_unit.current_instruction.opcode == BEQ || execute_unit.current_instruction.opcode == BLT) {
        flush_pipeline(hw);
        execute_unit.execute(execute_unit.current_instruction, hw);
    }
    else if (execute_unit.current_instruction.opcode == HALT) {
        flush_pipeline(hw);
        execute_unit.execute(execute_unit.current_instruction, hw);
    }
    else if (execute_unit.current_instruction.opcode != COUNT) {
        execute_unit.execute(execute_unit.current_instruction, hw);
    }
    
}

void Pipeline::advance_pipeline() {
    execute_unit.current_instruction = execute_unit.next_instruction;
    decode_unit.current_instruction = decode_unit.next_instruction;
}

void Pipeline::flush_pipeline(Hardware &hw) {
    execute_unit.next_instruction = PlaceholderInstruction();
    decode_unit.current_instruction = PlaceholderInstruction();
    decode_unit.next_instruction = PlaceholderInstruction();
    hw.pc -= 2;
}
