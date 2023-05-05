#include "units.h"

//fn def
void print_instruction(Instruction instr);
void print_cpu_info(Instruction instr, Hardware hw);

Instruction FetchUnit::fetch(Hardware &hw, std::vector<Instruction> program) {
    
    //hw.pc++;
    if (hw.pc < program.size()) {
        current_instruction = program[hw.pc];
        std::cout << "FETCHed instr \n";
        print_instruction(current_instruction);
        
        std::cout << "pc=" << hw.pc << std::endl;

    }
    else {
        //current_instruction = PlaceholderInstruction();
    }

    hw.pc++;


    return current_instruction;
}

void update_scoreboard_for_instr(Instruction instr, Hardware &hw) {
    InstrType instr_type = instr.type;

    if (instr.opcode == COUNT || instr.opcode == HALT) {
        return;
    }

    switch (instr_type) {
        case R:
            // if (instr.rd != instr.rs1 && instr.rd != instr.rs2) {
            //     hw.reg_updating[instr.rd] = true;
            // }

            hw.reg_updating[instr.rd] = true;
            break;
        case I:
            // if (instr.rd != instr.rs1) {
            //     hw.reg_updating[instr.rd] = true;
            // }

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
            // if (hw.reg_updating[instr.rs1] || hw.reg_updating[instr.rs2]) {
            //     return true;
            // }
            if (hw.reg_updating[instr.rs1]) {
                std::cout << "DATA HAZARD at " << register_to_string((Register)instr.rs1) << std::endl;
                return true;
            }
            if (hw.reg_updating[instr.rs2]) {
                std::cout << "DATA HAZARD at " << register_to_string((Register)instr.rs2) << std::endl;
                return true;
            }
            break;
        case I:
            // if (hw.reg_updating[instr.rs1]) {
            //     return true;
            // }
            if (hw.reg_updating[instr.rs1]) {
                std::cout << "DATA HAZARD at " << register_to_string((Register)instr.rs1) << std::endl;
                return true;
            }
            break;
        case S:
            // if (hw.reg_updating[instr.rs1] || hw.reg_updating[instr.rs2]) {
            //     return true;
            // }
            if (hw.reg_updating[instr.rs1]) {
                std::cout << "DATA HAZARD at " << register_to_string((Register)instr.rs1) << std::endl;
                return true;
            }
            if (hw.reg_updating[instr.rs2]) {
                std::cout << "DATA HAZARD at " << register_to_string((Register)instr.rs2) << std::endl;
                return true;
            }
            break;
        case B:
            // if (hw.reg_updating[instr.rs1] || hw.reg_updating[instr.rs2]) {
            //     return true;
            // }
            if (hw.reg_updating[instr.rs1]) {
                std::cout << "DATA HAZARD at " << register_to_string((Register)instr.rs1) << std::endl;
                return true;
            }
            if (hw.reg_updating[instr.rs2]) {
                std::cout << "DATA HAZARD at " << register_to_string((Register)instr.rs2) << std::endl;
                return true;
            }
            break;
        case J:
            // if (hw.reg_updating[instr.rs1]) {
            //     return true;
            // }
            if (hw.reg_updating[instr.rs1]) {
                std::cout << "DATA HAZARD at " << register_to_string((Register)instr.rs1) << std::endl;
                return true;
            }
            break;

    }
    return false;
}

bool DecodeUnit::decode(Instruction instr, Hardware &hw) {
    // if (instr.opcode != COUNT) {
    //     std::cout << "decoding instr \n";
    //     print_instruction(current_instruction);
    // }

    std::cout << "DECODing instr \n";
    print_instruction(current_instruction);
    

    bool has_dependency = false;
    if (instr.opcode != COUNT && instr.opcode != HALT) {
        has_dependency = check_for_dependency(instr, hw);
    }

    if (has_dependency) {
        //std::cout << "DATA HAZARD\n";
        //current_instruction = PlaceholderInstruction();
        return false;
    }
    else {
        //current_instruction = next_instruction;
        // if (instr.opcode != COUNT && instr.opcode != HALT) {
        //     update_scoreboard(instr, hw);
        // }    
    }
    return true;
}

int ExecuteUnit::execute(Instruction instr, Hardware &hw) {
    // if (instr.opcode != COUNT) {
    //     std::cout << "instruction at EX ";
    //     print_instruction(instr);
    // }

    // if (instr.opcode != COUNT && instr.opcode != HALT) {
    //     update_scoreboard(instr, hw);
    // }  
    
    std::cout << "instruction at EX ";
    print_instruction(instr);

    //current_instruction = instr;

    Opcode opcode = instr.opcode;

    result = 0;
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
            std::cout << "ADDI imm=" << instr.imm << "\n";
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

    if (instr.opcode != COUNT) {
        std::cout << "alu_output at EX " << result << std::endl;

    }


    return 0;
}

int MemoryUnit::memory_stage(Instruction instr, Hardware &hw) {
    // if (instr.opcode != COUNT) {
    //     std::cout << "instruction at MEM ";
    //     print_instruction(instr);
    //     std::cout << "alu_output at MEM " << alu_output << std::endl;
    // }
    // if (instr.opcode != COUNT && instr.opcode != HALT) {
    //     update_scoreboard(instr, hw);
    // }  

    std::cout << "instruction at MEM ";
    print_instruction(instr);
    std::cout << "alu_output at MEM " << alu_output << std::endl;
    
    Opcode opcode = instr.opcode;

    result = 0;

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
            std::cout << "SW at MEM \n";
            hw.memory[alu_output] = hw.reg_file[instr.rs2];
            break;
                
        case LW:
            result = hw.memory[alu_output];
            break;

        case BLT:
            // std::cout << "BLT" << std::endl;
            // if (hw.reg_file[instr.rs1] < hw.reg_file[instr.rs2]) {
            //     hw.pc = hw.labels[instr.label];
            //     //hw.pc--;
            // }
            //hw.finished = true;
            break;
        
        case BEQ:
            // if (hw.reg_file[instr.rs1] == hw.reg_file[instr.rs2]) {
            //     hw.pc = hw.labels[instr.label];
            //     //hw.pc--;
            // }
            //hw.finished = true;
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
    // if (instr.opcode != COUNT) {
    //     std::cout << "instruction at WB ";
    //     print_instruction(instr);
    //     std::cout << "alu_output at WB " << alu_output << std::endl;
    // }

    // if (instr.opcode != COUNT && instr.opcode != HALT) {
    //     update_scoreboard(instr, hw);
    // }  
    std::cout << "instruction at WB ";
    print_instruction(instr);
    std::cout << "alu_output at WB " << alu_output << "\n" << std::endl;
    
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
            std::cout << "ADDI \n";
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

        case COUNT:
            break;
    }
    print_cpu_info(instr, hw);
    
    return 0;
}

void Pipeline::clock_cycle(Hardware &hw, std::vector<Instruction> program) {

    if (writeback_unit.current_instruction.opcode != COUNT) {
        instructions_executed++;
    }

    if (writeback_unit.current_instruction.opcode == HALT) {
        flush_pipeline(hw, 4);

        writeback_unit.writeback(writeback_unit.current_instruction, hw);

    }
    else {
        writeback_unit.writeback(writeback_unit.current_instruction, hw);
    }

    memory_unit.memory_stage(memory_unit.current_instruction, hw);

    if (execute_unit.current_instruction.opcode == BEQ || execute_unit.current_instruction.opcode == BLT) {
        flush_pipeline(hw, 2);
        execute_unit.execute(execute_unit.current_instruction, hw);      

    }
    else {
        execute_unit.execute(execute_unit.current_instruction, hw);      
    }

    if (stalled) {
        //check if can unstall
        if (!check_for_dependency(decode_unit.current_instruction, hw)) {
            continue_pipeline();
        }
    }

    if (!stalled) {
        fetch_unit.fetch(hw, program);
        bool cont_pipeline = true;

        cont_pipeline = decode_unit.decode(decode_unit.current_instruction, hw);

        if (!cont_pipeline) {
            stall_pipeline();
        }
    }
    else {
        //decode_unit.next_instruction = PlaceholderInstruction();
        //execute_unit.next_instruction = PlaceholderInstruction();
        std::cout << "STALLED" << std::endl;
    }
    
}

void Pipeline::advance_pipeline(Hardware &hw) {

    writeback_unit.alu_output = memory_unit.alu_output;
    writeback_unit.load_mem_data = memory_unit.result;
    memory_unit.alu_output = execute_unit.result;

    
    writeback_unit.current_instruction = memory_unit.current_instruction;
    memory_unit.current_instruction = execute_unit.current_instruction;

    if (!stalled) {
        execute_unit.current_instruction = decode_unit.current_instruction;

        decode_unit.current_instruction = fetch_unit.current_instruction;
    }
    else {
        std::cout << "STALLED \n";
        execute_unit.current_instruction = PlaceholderInstruction();

        //decode_unit.current_instruction = PlaceholderInstruction();
    }


    update_scoreboard(hw);
  

}

void Pipeline::flush_pipeline(Hardware &hw, int stages) {
    std::cout << "FLUSH \n";
    //memory_unit.next_instruction = PlaceholderInstruction();
    //memory_unit.alu_output = 0;
    //execute_unit.next_instruction = PlaceholderInstruction();
    if (stages > 2) {
        execute_unit.result = 0;
        execute_unit.current_instruction = PlaceholderInstruction();
    }
    
    decode_unit.current_instruction = PlaceholderInstruction();
    //decode_unit.next_instruction = PlaceholderInstruction();
    //fetch_unit.next_instruction = PlaceholderInstruction();
    fetch_unit.current_instruction = PlaceholderInstruction();

    ////////////CHANGE//////////
    hw.pc -= stages;
    //hw.pc++;
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

    // execute_unit.next_instruction = decode_unit.current_instruction;
    // decode_unit.next_instruction = fetch_unit.current_instruction;
}

void Pipeline::update_scoreboard(Hardware &hw) {
    update_scoreboard_for_instr(execute_unit.current_instruction, hw);
    update_scoreboard_for_instr(memory_unit.current_instruction, hw);
    update_scoreboard_for_instr(writeback_unit.current_instruction, hw);
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