#include "tomasulo.h"
#include "tag.h"
#include "util.h"
#include "reorder_buffer.h"


void ALU::find_instruction_to_execute(std::vector<ReservationStation> reservation_stations) {
    //std::cout << "INSTR AT RS 0 " << opcode_to_string(reservation_stations[0].instr.opcode) << std::endl;
    bool halt_instr_exists = false;
    ReservationStation halt_instr_RS = PlaceholderRS(); //force HALT instruction to be executed only if no other instruction can be executed

    for (ReservationStation& res_stn : reservation_stations) {
        if (res_stn.busy && !res_stn.executing) {
            if (res_stn.tag1 == -1 && res_stn.tag2 == -1) {
                if (res_stn.instr.opcode == HALT) {
                    halt_instr_exists = true;
                    halt_instr_RS = res_stn;
                    continue;
                }
                //res_stn.executing = true;
                instr_res_stn = res_stn;
                //current_instruction = res_stn.instr;
                std::cout << "GOT INSTR FROM res stn " << instr_res_stn.number << std::endl;
                std::cout << "OPCODE " << opcode_to_string(instr_res_stn.instr.opcode) << std::endl;
                return;
            }

        }
        
    }
    //current_instruction = PlaceholderInstruction();
    if (!halt_instr_exists) {
        instr_res_stn = PlaceholderRS();
        std::cout << "GOT placeholder instr\n";
    }
    else {

        //check if there are any other instructions in RSes
        for (ReservationStation& res_stn : reservation_stations) {
            if (res_stn.busy && !res_stn.executing) {
                if (res_stn.instr.opcode != HALT) {
                    instr_res_stn = PlaceholderRS();
                    std::cout << "GOT placeholder instr\n";

                    return;
                }
            }
        }

        instr_res_stn = halt_instr_RS;
        std::cout << "GOT INSTR FROM res stn " << instr_res_stn.number << std::endl;
        std::cout << "OPCODE " << opcode_to_string(instr_res_stn.instr.opcode) << std::endl;
    }

    
}

void update_RAT(Hardware &hw, int reg, int rob_index) {
    if (reg == 0) {
        return;
    }
    hw.valid[reg] = false;
    hw.register_alias_table[reg] = rob_index;
}

int allocate_rob_entry(ReorderBuffer &ROB) {
    ROBEntry newEntry = PlaceholderROBEntry();
    int index = ROB.push(newEntry);
    return index;
}

bool add_to_ldst_queue(ReservationStation rs, std::deque<LDSTQueue_Entry> &queue) {
    bool is_store = (rs.instr.opcode == SW);
    LDSTQueue_Entry newEntry = LDSTQueue_Entry(rs.address, rs.get_tag(), is_store);
    queue.emplace_back(newEntry);

    return is_store;
}

void IssueUnit::issue_instruction(Hardware &hw, std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations, std::deque<LDSTQueue_Entry> &ldst_queue, ReorderBuffer &ROB) {
    std::cout << "current instr at ISSUE " << std::endl;
    print_instruction(current_instruction);

    Instruction instr = current_instruction;
    FUType required_FU = opcode_required_FU(instr.opcode);

    if (instr.opcode == COUNT) {
        return;
    }

    if (ROB.full()) {
        return;
    }


    for (auto& kvp : all_reservation_stations) {
        if (kvp.first == required_FU) {

            for (ReservationStation& rs : kvp.second) {
                if (!rs.busy && !rs.executing) {
                    //add to RS
                    int rob_index = allocate_rob_entry(ROB);
                    std::cout << "ROB_INDEX=" << rob_index << std::endl; 
                    ROB.buffer[rob_index].opcode = instr.opcode;

                    ///NEED TO CHANGE FOR DIFF INSTR TYPES
                    ROB.buffer[rob_index].destination = instr.rd;
                    ROB.buffer[rob_index].rs_tag = rs.get_tag();

                    rs.ROB_entry = rob_index;

                    rs.add_instruction(hw, instr);

                    if (required_FU == LOADSTORE) {
                        bool is_store = add_to_ldst_queue(rs, ldst_queue);
                        if (is_store) {
                            ROB.buffer[rob_index].is_store_instr = true;
                        }
                    }
                    rs.busy = true;
                    std::cout << "ADDED INSTR TO RS " << rs.number << std::endl;
                    std::cout << "with RS TYPE " << rs.FU_type << std::endl;

                    //update RAT
                    if (instr.type == R || instr.type == I) {
                        update_RAT(hw, instr.rd, rs.ROB_entry);

                    }
                    break;
                }
            }
            
        }
    }
}

void ALU::perform_ALU_operation(Hardware &hw, Opcode op, int val1, int val2, int imm) {
    int result = 0;
    switch (op) {
        case ADD:
            result = val1 + val2;            
            break;

        case SUB:
            result = val1 - val2;
            break;

        case SLT:
            if (val1 < val2) {
                //hw.reg_file[instr.rd] = 1;
                result = 1;
            }
            else {
                //hw.reg_file[instr.rd] = 0;
                result = 0;
            }
            break;
        
        case ADDI:
            //std::cout << "ADDI imm=" << instr.imm << "\n";
            result = val1 + imm;
            break;
    

        case HALT:
            //hw.finished = true;
            break;
        
        default:
            break;
    }

    if (op != COUNT) {
        //std::cout << "alu_output at EX " << result << std::endl;

    }

    instr_res_stn.instr.result = result;
    std::cout << "RESULT = " << result << std::endl;

}

void ALU::execute(Hardware &hw) {

    std::cout << "current instr at EXECUTE " << std::endl;

    std::cout << "FROM RS " << instr_res_stn.number << std::endl;


    print_instruction(instr_res_stn.instr);


    if (instr_res_stn.FU_type == NONE) {
        return;
    }

    update_RAT(hw, instr_res_stn.instr.rd, instr_res_stn.ROB_entry);

    perform_ALU_operation(hw, instr_res_stn.instr.opcode, instr_res_stn.value1, instr_res_stn.value2, instr_res_stn.imm);

    
    //print_cpu_info(instr_res_stn.instr, hw);
}

int find_load_instr(std::vector<ReservationStation> &reservation_stations, std::deque<LDSTQueue_Entry> &queue) {
    for (int i = 0; i < queue.size(); i++) {
        //can't exec ld if st in front of it 
        if (queue[i].is_store) {
            return -1;
        }
        Tag current_rs_tag = queue[i].RS_Tag;
        if (reservation_stations[current_rs_tag.number].tag1 == -1) { //ready
            return i;
        }
    }
    return -1; //can't find
}

void LDSTUnit::find_mem_instr(std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations, std::deque<LDSTQueue_Entry> &queue) {
    if (queue.size() == 0) {
        return;
    }
    LDSTQueue_Entry queue_head = queue.front();

    std::vector<ReservationStation> reservation_stations = all_reservation_stations[LOADSTORE];


    if (queue_head.is_store) {
        if (reservation_stations[queue_head.RS_Tag.number].tag1 == -1) {
            instr1_rs_tag = queue_head.RS_Tag;
            (all_reservation_stations[LOADSTORE])[queue_head.RS_Tag.number].executing = true;
            
            queue.pop_front();
        }
    }
    else {
        int index = find_load_instr(reservation_stations, queue);

        if (index != -1) {
            instr1_rs_tag = queue[index].RS_Tag;
            (all_reservation_stations[LOADSTORE])[queue[index].RS_Tag.number].executing = true;

            queue.erase(queue.begin() + index);
        }
    }
}

void LDSTUnit::address_calculation(std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations, ReorderBuffer &ROB) {
    Opcode op = (all_reservation_stations[LOADSTORE])[instr1_rs_tag.number].instr.opcode; 

    if (instr2_rs_tag.FU_type == NONE) {
        return;
    }

    switch (op) {
        case LW:
            std::cout << "BASE REG VALUE " << (all_reservation_stations[LOADSTORE])[instr1_rs_tag.number].value1 << std::endl;
            std::cout << "IMM VALUE " << (all_reservation_stations[LOADSTORE])[instr1_rs_tag.number].imm << std::endl;

            (all_reservation_stations[LOADSTORE])[instr1_rs_tag.number].address = (all_reservation_stations[LOADSTORE])[instr1_rs_tag.number].value1 + (all_reservation_stations[LOADSTORE])[instr1_rs_tag.number].imm;
            std::cout << "CALCULATED ADDRESS FOR LOAD " << (all_reservation_stations[LOADSTORE])[instr1_rs_tag.number].address << std::endl;
            break;

        case SW:
            int rob_index = (all_reservation_stations[LOADSTORE])[instr1_rs_tag.number].ROB_entry;
            ROB.buffer[rob_index].destination = (all_reservation_stations[LOADSTORE])[instr1_rs_tag.number].value1 + (all_reservation_stations[LOADSTORE])[instr1_rs_tag.number].imm;
            //ROB.buffer[rob_index].result = (all_reservation_stations[LOADSTORE])
            break;

    }
}

void LDSTUnit::memory_access(Hardware &hw, std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations, ReorderBuffer &ROB) {
    std::vector<ReservationStation> reservation_stations = all_reservation_stations[LOADSTORE];
    Opcode op = reservation_stations[instr2_rs_tag.number].instr.opcode; 

    if (instr2_rs_tag.FU_type == NONE) {
        return;
    }

    switch (op) {
        case LW:
            {
            int index = reservation_stations[instr2_rs_tag.number].address;
            int mem_result = hw.memory[index];

            (all_reservation_stations[LOADSTORE])[instr2_rs_tag.number].instr.result = mem_result;

            std::cout << "ACCESSING MEM " << (all_reservation_stations[LOADSTORE])[instr2_rs_tag.number].address << std::endl;

            }
            break;


        case SW:
            break; //do nothing
    }
}

void LDSTUnit::execute_ldst_cycle(Hardware &hw, std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations, ReorderBuffer &ROB) {
    
    address_calculation(all_reservation_stations, ROB);
    memory_access(hw, all_reservation_stations, ROB);
}

void LDSTUnit::advance_ldst_pipeline() {
    instr2_rs_tag = instr1_rs_tag;
}

void WriteUnit::write_result(Hardware &hw, std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations, ReorderBuffer &ROB) {
    if (completed_instr_res_stns.size() == 0) {
        return;
    }
    ReservationStation res_stn = completed_instr_res_stns.front();

    std::cout << "current instr at WRITE " << std::endl;
    print_instruction(res_stn.instr);

    if (res_stn.instr.opcode == COUNT) {
        //continue;

        return;
    }

    ROB.buffer[res_stn.ROB_entry].result = res_stn.instr.result;

    // if (res_stn.instr.opcode != SW) {

    //     //ROB.buffer[res_stn.ROB_entry].destination = dest_reg;
    //     std::cout << "SETTING ROB ENTRY READY ENTRY " << res_stn.ROB_entry << std::endl;
    //     ROB.buffer[res_stn.ROB_entry].ready = true;

    // }

    //PRETENd evefrytyhing's fine
    std::cout << "SETTING ROB ENTRY READY ENTRY " << res_stn.ROB_entry << std::endl;
    ROB.buffer[res_stn.ROB_entry].ready = true;


    (all_reservation_stations[res_stn.FU_type])[res_stn.number].busy = false;


    completed_instr_res_stns.pop();

}

void commit_store_instr(Hardware &hw, int destination, int result) {
    std::cout << "STORING TO MEM WITH RESULT " << result << std::endl;
    hw.memory[destination] = result;
}

void commit_result_to_ARF(Hardware &hw, int destination, int result) {
    std::cout << "COMMITTING TO REG " << destination << std::endl;
    hw.register_alias_table[destination] = -1;
    hw.valid[destination] = true;
    hw.reg_file[destination] = result;
}

void broadcast_result(std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations, int rob_index, int result) {
    for (auto& kvp : all_reservation_stations) {
        
        for (ReservationStation& res_station : kvp.second) {
            if (res_station.tag1==(rob_index)) {
                res_station.value1 = result; //where does result come from?
                res_station.tag1 = -1;
            }
            if (res_station.tag2==(rob_index)) {
                res_station.value2 = result;
                res_station.tag2 = -1;
            }
        }
    }
}

void CommitUnit::commit_result(Hardware &hw, std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations, ReorderBuffer &ROB) {
    if (ROB.empty()) {
        return;
    }

    std::cout << "COMMITTING ROB " << ROB.head << std::endl;
    ROBEntry rob_head = ROB.get_front();
    if (rob_head.ready) {
        std::cout << "ROB HEAD READY" << std::endl;
        if (rob_head.opcode == HALT) {
            hw.finished = true;
            return;
        }
        if (!rob_head.is_store_instr) {
            commit_result_to_ARF(hw, rob_head.destination, rob_head.result);
            broadcast_result(all_reservation_stations, ROB.head, rob_head.result);
            std::cout << "POP ROB" << std::endl;
            ROB.pop();

            (all_reservation_stations[rob_head.rs_tag.FU_type])[rob_head.rs_tag.number].executing = false;

        }
        else {
            if ((all_reservation_stations[LOADSTORE])[rob_head.rs_tag.number].tag2 == -1) {
                int result = (all_reservation_stations[LOADSTORE])[rob_head.rs_tag.number].value2;
                std::cout << "RESULT=" << result << std::endl;
                commit_store_instr(hw, rob_head.destination, result);
                ROB.pop();
                (all_reservation_stations[rob_head.rs_tag.FU_type])[rob_head.rs_tag.number].executing = false;

            }
            else {
                std::cout << "COULDN'T COMMIT SW" << std::endl;

            }
            
        }
    }
}

void OoOPipeline::clock_cycle(Hardware &hw, std::vector<Instruction> program) {
    commit_unit.commit_result(hw, all_reservation_stations, ROB);
    write_unit.write_result(hw, all_reservation_stations, ROB);

    
    fetch_unit.fetch(hw, program);

    issue_unit.issue_instruction(hw, all_reservation_stations, ldst_queue, ROB);

    //std::cout << "INSTR AT RS 0 " << opcode_to_string((all_reservation_stations[ARITH])[0].instr.opcode) << std::endl;


    for (ALU& alu : ALUs) {
        alu.execute(hw);

        if (alu.instr_res_stn.FU_type != NONE) {
            //(all_reservation_stations[alu.instr_res_stn.FU_type])[alu.instr_res_stn.number].busy = false;

            //(all_reservation_stations[alu.instr_res_stn.FU_type])[alu.instr_res_stn.number].executing = false;

        }

    }

    for (LDSTUnit& mem_unit : ldst_units) {
        mem_unit.execute_ldst_cycle(hw, all_reservation_stations, ROB);
    }

    // for (BranchUnit branch_unit : branch_units) {
    //     branch_unit.execute(all_reservation_stations[BRANCH]);
    // }

    std::cout << "REGISTERS" << std::endl;
    int counter = 0;
    for (int reg : hw.reg_file) {
        std::cout << register_to_string(static_cast<Register>(counter)) << "=";
        counter++;
        std::cout << reg << " ";
    }
    std::cout << std::endl;


    std::cout << "RAT" << std::endl;
    counter = 0;
    for (int rob_index : hw.register_alias_table) {
        std::cout << register_to_string(static_cast<Register>(counter)) << "=";
        counter++;
        std::cout << rob_index << " ";
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

    getchar();

    std::cout << "ALU RSES" << std::endl;

    for (ReservationStation rs : all_reservation_stations[ARITH]) {
        std::cout << "RS " << rs.number << std::endl;
        std::cout << "OPCODE " << opcode_to_string(rs.instr.opcode) << std::endl;
        std::cout << "TAG1 " << rs.tag1 << std::endl;
        std::cout << "TAG2 " << rs.tag2 << std::endl;
        std::cout << "VALUE1 " << rs.value1 << std::endl;
        std::cout << "VALUE2 " << rs.value2 << std::endl;
        std::cout << "IMM " << rs.imm << std::endl;
        std::cout << "ADDRESS " << rs.address << std::endl;
        std::cout << std::endl;
    }

    std::cout << "LDST RSES" << std::endl;

    for (ReservationStation rs : all_reservation_stations[LOADSTORE]) {
        std::cout << "RS " << rs.number << std::endl;
        std::cout << "OPCODE " << opcode_to_string(rs.instr.opcode) << std::endl;
        std::cout << "TAG1 " << rs.tag1 << std::endl;
        std::cout << "TAG2 " << rs.tag2 << std::endl;
        std::cout << "VALUE1 " << rs.value1 << std::endl;
        std::cout << "VALUE2 " << rs.value2 << std::endl;
        std::cout << "IMM " << rs.imm << std::endl;
        std::cout << "ADDRESS " << rs.address << std::endl;
        std::cout << std::endl;
    }

    std::cout << "LDST QUEUE" << std::endl;

    for (LDSTQueue_Entry entry : ldst_queue) {
        std::cout << "ADDRESS " << entry.address << std::endl;
        std::cout << "TAG " << entry.RS_Tag.number << std::endl << std::endl;
    }

    int index = 0;
    std::cout << "ROB HEAD " << ROB.head << std::endl;
    std::cout << "ROB TAIL " << ROB.tail << std::endl;

    for (ROBEntry rob_entry : ROB.buffer) {
        std::cout << "ROB " << index << std::endl;
        std::cout << "IN USE " << std::to_string(rob_entry.in_use) << std::endl;
        std::cout << "READY " << std::to_string(rob_entry.ready) << std::endl;
        std::cout << "DEST " << rob_entry.destination << std::endl;
        std::cout << "RESULT " << rob_entry.result << std::endl;
        std::cout << "OPCODE" << opcode_to_string(rob_entry.opcode) << std::endl;
        std::cout << "IS STORE INSTR " << std::to_string(rob_entry.is_store_instr) << std::endl;
        std::cout << "RS TAG " << rob_entry.rs_tag.number << std::endl;
        std::cout << std::endl;
        index++;
    }

    getchar();

}

void OoOPipeline::advance_pipeline(Hardware &hw) {

    for (ALU& alu : ALUs) {
        if (alu.instr_res_stn.FU_type != NONE) {
            write_unit.completed_instr_res_stns.emplace(alu.instr_res_stn);

        }

        alu.find_instruction_to_execute(all_reservation_stations[ARITH]);
        
        if (alu.instr_res_stn.FU_type != NONE) {
            (all_reservation_stations[alu.instr_res_stn.FU_type])[alu.instr_res_stn.number].executing = true;

        }
    }

    for (LDSTUnit& ldst_unit : ldst_units) {
        if (ldst_unit.instr2_rs_tag.FU_type != NONE) {
            ReservationStation completed_rs = (all_reservation_stations[LOADSTORE])[ldst_unit.instr2_rs_tag.number];
            write_unit.completed_instr_res_stns.emplace(completed_rs);
        }
        

        //ldst_unit.find_instruction_to_execute(all_reservation_stations[LOADSTORE]);

        ldst_unit.advance_ldst_pipeline();
        
        ldst_unit.find_mem_instr(all_reservation_stations, ldst_queue);

    }

    // for (BranchUnit branch_unit : branch_units) {
    //     write_unit.completed_instr_res_stns.push_back(branch_unit.instr_res_stn);

    //     branch_unit.find_instruction_to_execute(all_reservation_stations[BRANCH]);
    // }

    issue_unit.current_instruction = fetch_unit.current_instruction;


}