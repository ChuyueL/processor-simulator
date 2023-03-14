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

void update_scoreboard(Instruction instr, Hardware &hw) {
    InstrType instr_type = instr.type;

    switch (instr_type) {
        case R:
            hw.reg_updating[instr.rd] = true;
            break;
        case I:
            hw.reg_updating[instr.rd] = true;
            break;
        case S:
            
            break;
        case B:
            
            break;
        case J:
            
            break;

    }
}

bool check_for_dependency(Instruction instr, Hardware &hw) {
    InstrType instr_type = instr.type;
    switch (instr_type) {
        case R:
            if (hw.reg_updating[instr.rs1] || hw.reg_updating[instr.rs2]) {
                return true;
            }
            break;
        case I:
            if (hw.reg_updating[instr.rs1]) {
                return true;
            }
            break;
        case S:
            if (hw.reg_updating[instr.rs1] || hw.reg_updating[instr.rs2]) {
                return true;
            }
            break;
        case B:
            if (hw.reg_updating[instr.rs1] || hw.reg_updating[instr.rs2]) {
                return true;
            }
            break;
        case J:
            if (hw.reg_updating[instr.rs1]) {
                return true;
            }
            break;

    }
}

bool DecodeUnit::decode(Instruction instr, Hardware &hw) {
    bool has_dependency = check_for_dependency(instr, hw);

    if (has_dependency) {
        return false;
    }
    else {
        if (instr.opcode != COUNT && instr.opcode != HALT) {
            update_scoreboard(instr, hw);
        }    
    }
    return true;
}

int ExecuteUnit::execute(Instruction instr, Hardware &hw) {
    
    std::cout << "opcode " << opcode_to_string(instr.opcode);
    std::cout << "rd " << std::to_string((Register)instr.rd);
    std::cout << "rs1 " << register_to_string((Register)instr.rs1);
    std::cout << "rs2 " << register_to_string((Register)instr.rs2);
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
    switch (opcode) {
        case ADD:
            result = hw.reg_file[instr.rs1] + hw.reg_file[instr.rs2];            
            break;

        case SUB:
            result = hw.reg_file[instr.rs1] - hw.reg_file[instr.rs2];
            break;

        case SLT:
            if (hw.reg_file[instr.rs1] < hw.reg_file[instr.rs2]) {
                //hw.reg_file[instr.rd] = 1;
                result = 1;
            }
            else {
                //hw.reg_file[instr.rd] = 0;
                result = 0;
            }
            break;
        
        case ADDI:
            result = hw.reg_file[instr.rs1] + instr.imm;
            break;
        
        case SW:
            //hw.memory[hw.reg_file[instr.rs1] + instr.imm] = hw.reg_file[instr.rs2];
            result = hw.reg_file[instr.rs1] + instr.imm;
            break;
                
        case LW:
            //hw.reg_file[instr.rd] = hw.memory[hw.reg_file[instr.rs1] + instr.imm];
            result = hw.reg_file[instr.rs1] + instr.imm;
            break;

        case BLT:
            // if (hw.reg_file[instr.rs1] < hw.reg_file[instr.rs2]) {
            //     hw.pc = hw.labels[instr.label];
            //     //hw.pc--;
            // }
            break;
        
        case BEQ:
            // if (hw.reg_file[instr.rs1] == hw.reg_file[instr.rs2]) {
            //     hw.pc = hw.labels[instr.label];
            //     //hw.pc--;
            // }
            break;

        case HALT:
            //hw.finished = true;
            break;
    }

    return 0;
}

int MemoryUnit::memory_stage(Instruction instr, Hardware &hw) {
    std::cout << "alu_output " << alu_output << std::endl;
    Opcode opcode = instr.opcode;

    switch (opcode) {
        case ADD:
            //do nothing
            break;

        case SUB:
            //do nothing
            break;

        case SLT:
            //do nothing
            break;
        
        case ADDI:
            //do nothing
            break;
        
        case SW:
            hw.memory[alu_output] = hw.reg_file[instr.rs2];
            break;
                
        case LW:
            result = hw.memory[alu_output];
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
            //hw.finished = true;
            break;

        default:
            break;
    }

    return 0;

}

int WritebackUnit::writeback(Instruction instr, Hardware &hw) {
    Opcode opcode = instr.opcode;

    switch (opcode) {
        case ADD:
            hw.reg_file[instr.rd] = alu_output;
            hw.reg_updating[instr.rd] = false;
            break;

        case SUB:
            hw.reg_file[instr.rd] = alu_output;
            hw.reg_updating[instr.rd] = false;

            break;

        case SLT:
            hw.reg_file[instr.rd] = alu_output;
            hw.reg_updating[instr.rd] = false;

            break;
        
        case ADDI:
            hw.reg_file[instr.rd] = alu_output;
            hw.reg_updating[instr.rd] = false;

            break;
        
        case SW:
            //hw.memory[alu_output] = hw.reg_file[instr.rs2];
            break;
                
        case LW:
            hw.reg_file[instr.rd] = load_mem_data;
            hw.reg_updating[instr.rd] = false;

            break;

        case BLT:
            // if (hw.reg_file[instr.rs1] < hw.reg_file[instr.rs2]) {
            //     hw.pc = hw.labels[instr.label];
            //     //hw.pc--;
            // }
            break;
        
        case BEQ:
            // if (hw.reg_file[instr.rs1] == hw.reg_file[instr.rs2]) {
            //     hw.pc = hw.labels[instr.label];
            //     //hw.pc--;
            // }
            break;

        case HALT:
            hw.finished = true;
            break;
    }
    
    return 0;
}

void Pipeline::clock_cycle(Hardware &hw, std::vector<Instruction> program) {
    if (!stalled) {
        decode_unit.next_instruction = fetch_unit.fetch(hw, program);
    }
    else {
        decode_unit.next_instruction = PlaceholderInstruction();
        std::cout << "stalled" << std::endl;
    }
    //execute_unit.next_instruction = fetch_unit.fetch(hw, program);
    execute_unit.next_instruction = decode_unit.current_instruction;

    if (execute_unit.current_instruction.opcode != COUNT) {
        execute_unit.execute(execute_unit.current_instruction, hw);
    }

    memory_unit.next_instruction = execute_unit.current_instruction;
    memory_unit.alu_output = execute_unit.result;

    if (memory_unit.current_instruction.opcode == BEQ || memory_unit.current_instruction.opcode == BLT) { ///move to MEM
        flush_pipeline(hw, 3);
        memory_unit.memory_stage(memory_unit.current_instruction, hw);
    }
    else if (memory_unit.current_instruction.opcode != COUNT) {
        memory_unit.memory_stage(memory_unit.current_instruction, hw);
    }

    writeback_unit.next_instruction = memory_unit.current_instruction;
    writeback_unit.alu_output = memory_unit.alu_output;
    writeback_unit.load_mem_data = memory_unit.result;

    if (writeback_unit.current_instruction.opcode == HALT) {
        flush_pipeline(hw, 4);
        writeback_unit.writeback(writeback_unit.current_instruction, hw);
    }
    else if (writeback_unit.current_instruction.opcode != COUNT) {
        writeback_unit.writeback(writeback_unit.current_instruction, hw);
    }


    if (stalled) {
        //check if can unstall
        if (!check_for_dependency(decode_unit.current_instruction, hw)) {
            continue_pipeline();
        }
    }
    
}

void Pipeline::advance_pipeline() {
    writeback_unit.current_instruction = writeback_unit.next_instruction;
    memory_unit.current_instruction = memory_unit.next_instruction;
    execute_unit.current_instruction = execute_unit.next_instruction;
    decode_unit.current_instruction = decode_unit.next_instruction;
}

void Pipeline::flush_pipeline(Hardware &hw, int stages) {
    writeback_unit.next_instruction = PlaceholderInstruction();
    execute_unit.next_instruction = PlaceholderInstruction();
    execute_unit.current_instruction = PlaceholderInstruction();
    decode_unit.current_instruction = PlaceholderInstruction();
    decode_unit.next_instruction = PlaceholderInstruction();

    ////////////CHANGE//////////
    hw.pc -= stages;
}

void Pipeline::stall_pipeline() {
    fetch_unit.stalled = true;
    decode_unit.stalled = true;
    stalled = true;
}

void Pipeline::continue_pipeline() {
    fetch_unit.stalled = false;
    decode_unit.stalled = false;
    execute_unit.stalled = false;
    memory_unit.stalled = false;
    stalled = false;
}