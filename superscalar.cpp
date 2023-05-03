#include "superscalar.h"

void SuperscalarOoOPipeline::reset_regs(Hardware &hw) {
    for (int i = 0; i < hw.reg_file.size(); i++) {
        hw.valid[i] = true;
        hw.register_alias_table[i] = -1;
    }
}

void SuperscalarOoOPipeline::reset_all_res_stns() {
    for (auto &kvp : pipeline_buffers.all_reservation_stations) {
        for (ReservationStation &rs : kvp.second) {
            // rs = PlaceholderRS();
            // rs.FU_type = kvp.first;
            rs.address = 0;
            rs.busy = false;
            rs.executing = false;
            rs.imm = 0;
            rs.instr = PlaceholderInstruction();
            rs.ROB_entry = 0;
            rs.tag1 = -1;
            rs.tag2 = -1;
            rs.tag_i = -1;
            rs.value1 = 0;
            rs.value2 = 0;
            rs.value_i = 0;
        }
    }
}

void SuperscalarOoOPipeline::flush_pipeline() {
    //fetch_unit.current_instruction = PlaceholderInstruction();

    pipeline_buffers.instr_queue = std::queue<Instruction>();
    pipeline_buffers.ldst_queue = std::deque<LDSTQueue_Entry>();

    //issue_unit.current_instruction = PlaceholderInstruction();

    for (ALU& alu : ALUs) {
        alu.current_instruction = PlaceholderInstruction();
        alu.instr_res_stn = PlaceholderRS();

    }

    for (LDSTUnit& mem_unit : ldst_units) {
        mem_unit.current_instruction = PlaceholderInstruction();
        mem_unit.instr1_rs_tag = PlaceholderTag();
        mem_unit.instr2_rs_tag = PlaceholderTag();
    }

    for (BranchUnit& branch_unit : branch_units) {
        branch_unit.current_instruction = PlaceholderInstruction();
        branch_unit.instr_rs_tag = PlaceholderTag();
    }
    // std::queue<ReservationStation> empty;
    // std::swap(write_unit.completed_instr_res_stns, empty);

    pipeline_buffers.completed_instr_res_stns = std::queue<ReservationStation>();
}

void SuperscalarOoOPipeline::clock_cycle(Hardware &hw, std::vector<Instruction> program) {
    commit_unit.commit_results(hw, pipeline_buffers);

    instructions_executed += commit_unit.committed_instrs;
    commit_unit.committed_instrs = 0;

    if (commit_unit.flush) {
        std::cout << "FLUSHING PIPELINE \n";
        pipeline_buffers.ROB.flush();
        reset_regs(hw);
        reset_all_res_stns();
        flush_pipeline();
        commit_unit.flush = false;
        return;
    }

    // if (commit_unit.committed == true) {
    //     commit_unit.committed = false;
    //     instructions_executed++;
    // }
    std::cout << "number of completed instrs=" << pipeline_buffers.completed_instr_res_stns.size() << std::endl;

    write_unit.write_results(hw, pipeline_buffers);

    for (ALU& alu : ALUs) {

        // alu.find_instruction_to_execute(pipeline_buffers.all_reservation_stations[ARITH]);
        
        // if (alu.instr_res_stn.FU_type != NONE) {
        //     (pipeline_buffers.all_reservation_stations[alu.instr_res_stn.FU_type])[alu.instr_res_stn.number].executing = true;

        // }
        alu.execute(hw);

    }

    for (LDSTUnit& mem_unit : ldst_units) {
        // mem_unit.advance_ldst_pipeline();
        
        // mem_unit.find_mem_instr(pipeline_buffers.all_reservation_stations, pipeline_buffers.ldst_queue);
        mem_unit.execute_ldst_cycle(hw, pipeline_buffers.all_reservation_stations, pipeline_buffers.ROB);
    }

    for (BranchUnit& branch_unit : branch_units) {
        //branch_unit.find_branch_instr(pipeline_buffers.all_reservation_stations);
        branch_unit.get_branch_address(hw, pipeline_buffers.all_reservation_stations, pipeline_buffers.ROB);

        // if (branch_unit.instr_rs_tag.FU_type != NONE) {
        //     (pipeline_buffers.all_reservation_stations[branch_unit.instr_rs_tag.FU_type])[branch_unit.instr_rs_tag.number].executing = true;

        // }
    }

    std::cout << "size of instruction queue =" << pipeline_buffers.instr_queue.size() << std::endl;


    issue_unit.issue_instructions(hw, pipeline_buffers);
    
    fetch_unit.fetch_instructions(hw, program, pipeline_buffers);

    

    //std::cout << "INSTR AT RS 0 " << opcode_to_string((all_reservation_stations[ARITH])[0].instr.opcode) << std::endl;


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

    for (ReservationStation rs : pipeline_buffers.all_reservation_stations[ARITH]) {
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

    for (ReservationStation rs : pipeline_buffers.all_reservation_stations[LOADSTORE]) {
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

    for (LDSTQueue_Entry entry : pipeline_buffers.ldst_queue) {
        std::cout << "ADDRESS " << entry.address << std::endl;
        std::cout << "TAG " << entry.RS_Tag.number << std::endl << std::endl;
    }

    int index = 0;
    std::cout << "ROB HEAD " << pipeline_buffers.ROB.head << std::endl;
    std::cout << "ROB TAIL " << pipeline_buffers.ROB.tail << std::endl;

    for (ROBEntry rob_entry : pipeline_buffers.ROB.buffer) {
        std::cout << "ROB " << index << std::endl;
        std::cout << "IN USE " << std::to_string(rob_entry.in_use) << std::endl;
        std::cout << "READY " << std::to_string(rob_entry.ready) << std::endl;
        std::cout << "DEST " << rob_entry.destination << std::endl;
        if (!rob_entry.is_store_instr) {
            std::cout << "as reg : " << register_to_string(static_cast<Register>(rob_entry.destination)) << std::endl;
        }
        
        std::cout << "RESULT " << rob_entry.result << std::endl;
        std::cout << "OPCODE" << opcode_to_string(rob_entry.opcode) << std::endl;
        std::cout << "IS STORE INSTR " << std::to_string(rob_entry.is_store_instr) << std::endl;
        std::cout << "RS TAG " << rob_entry.rs_tag.number << std::endl;
        std::cout << std::endl;
        index++;
    }

    getchar();
}

void SuperscalarOoOPipeline::advance_pipeline(Hardware &hw) {
    for (BranchUnit& branch_unit : branch_units) {
        if (branch_unit.instr_rs_tag.FU_type != NONE) {
            if ((pipeline_buffers.all_reservation_stations[BRANCH])[branch_unit.instr_rs_tag.number].instr.opcode != COUNT) {
                ReservationStation completed_rs = (pipeline_buffers.all_reservation_stations[BRANCH])[branch_unit.instr_rs_tag.number];
                pipeline_buffers.completed_instr_res_stns.emplace(completed_rs);
                branch_unit.instr_rs_tag = PlaceholderTag();
            }
            
        }
        

        branch_unit.find_branch_instr(pipeline_buffers.all_reservation_stations);
    }
    for (ALU& alu : ALUs) {
        if (alu.instr_res_stn.FU_type != NONE) {
            if (alu.instr_res_stn.instr.opcode != COUNT) {
                pipeline_buffers.completed_instr_res_stns.emplace(alu.instr_res_stn);
                alu.instr_res_stn = PlaceholderRS();
            }
            

        }

        alu.find_instruction_to_execute(pipeline_buffers.all_reservation_stations[ARITH]);
        
        if (alu.instr_res_stn.FU_type != NONE) {
            (pipeline_buffers.all_reservation_stations[alu.instr_res_stn.FU_type])[alu.instr_res_stn.number].executing = true;

        }
    }

    for (LDSTUnit& ldst_unit : ldst_units) {
        if (ldst_unit.instr2_rs_tag.FU_type != NONE) {
            if ((pipeline_buffers.all_reservation_stations[LOADSTORE])[ldst_unit.instr2_rs_tag.number].instr.opcode != COUNT) {
                ReservationStation completed_rs = (pipeline_buffers.all_reservation_stations[LOADSTORE])[ldst_unit.instr2_rs_tag.number];
                pipeline_buffers.completed_instr_res_stns.emplace(completed_rs);
                ldst_unit.instr2_rs_tag = PlaceholderTag();
            }
            
        }
        

        //ldst_unit.find_instruction_to_execute(all_reservation_stations[LOADSTORE]);

        ldst_unit.advance_ldst_pipeline();
        
        ldst_unit.find_mem_instr(pipeline_buffers.all_reservation_stations, pipeline_buffers.ldst_queue);

    }


}