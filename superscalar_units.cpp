#include "superscalar_units.h"
#include "util.h"

void SuperscalarFetchUnit::fetch_instructions(Hardware &hw, std::vector<Instruction> program, PipelineBuffers &buffers) {
    for (int i = 0; i < WIDTH; i++) {
        if (hw.pc < program.size()) {
            Instruction current_instruction = program[hw.pc];
            std::cout << "FETCHed instr \n";
            print_instruction(current_instruction);
            
            std::cout << "pc=" << hw.pc << std::endl;

            if (current_instruction.type == B) {
                if (branch_predictor.predict_branch(hw, current_instruction)) {
                    current_instruction.predicted_taken = true;
                    current_instruction.prev_pc = hw.pc + 1;
                    hw.pc = hw.labels[current_instruction.label] - 1;
                }
                else {
                    current_instruction.predicted_taken = false;
                    current_instruction.prev_pc = hw.pc+1;
                }
            }

            buffers.instr_queue.emplace(current_instruction);


            hw.pc++;


        }

        

    }
}

void allocate_to_rs(Hardware &hw, Instruction instr, PipelineBuffers &buffers) {
    FUType required_FU = opcode_required_FU(instr.opcode);

    if (instr.opcode == COUNT) {
        return;
    }

    if (buffers.ROB.full()) {
        return;
    }


    for (auto& kvp : buffers.all_reservation_stations) {
        if (kvp.first == required_FU) {

            for (ReservationStation& rs : kvp.second) {
                if (!rs.busy && !rs.executing) {
                    //add to RS
                    int rob_index = allocate_rob_entry(buffers.ROB);
                    std::cout << "ROB_INDEX=" << rob_index << std::endl; 
                    buffers.ROB.buffer[rob_index].opcode = instr.opcode;

                    ///NEED TO CHANGE FOR DIFF INSTR TYPES
                    buffers.ROB.buffer[rob_index].destination = instr.rd;

                    buffers.ROB.buffer[rob_index].instr_type = instr.type;

                    if (instr.opcode == HALT) {
                        buffers.ROB.buffer[rob_index].ready = true;
                        return;
                    }

                    buffers.ROB.buffer[rob_index].rs_tag = rs.get_tag();

                    rs.ROB_entry = rob_index;


                    rs.add_instruction(hw, instr);

                    //update RAT
                    if (instr.type == R || instr.type == I || instr.type == LIDX) {
                        update_RAT(hw, instr.rd, rs.ROB_entry);

                    }

                    if (required_FU == LOADSTORE) {
                        bool is_store = add_to_ldst_queue(rs, buffers.ldst_queue);
                        if (is_store) {
                            buffers.ROB.buffer[rob_index].is_store_instr = true;
                        }
                    }
                    rs.busy = true;
                    std::cout << "ADDED INSTR TO RS " << rs.number << std::endl;
                    std::cout << "with RS TYPE " << rs.FU_type << std::endl;

                    if (instr.type == B) {
                        buffers.ROB.buffer[rob_index].is_branch = true;
                    }

                    buffers.instr_queue.pop();
                    return;
                }
            }
            
        }
    }
}

void SuperscalarIssueUnit::issue_instructions(Hardware &hw, PipelineBuffers &buffers) {
    if (buffers.instr_queue.size() == 0) {
        return;
    }

    for (int i = 0; i < WIDTH; i++) {
        std::cout << "current instr at ISSUE " << std::endl;
        Instruction current_instruction = buffers.instr_queue.front();
        print_instruction(current_instruction);

        Instruction instr = current_instruction;
        allocate_to_rs(hw, instr, buffers);
        // FUType required_FU = opcode_required_FU(instr.opcode);

        // if (instr.opcode == COUNT) {
        //     return;
        // }

        // if (buffers.ROB.full()) {
        //     return;
        // }


        // for (auto& kvp : buffers.all_reservation_stations) {
        //     if (kvp.first == required_FU) {

        //         for (ReservationStation& rs : kvp.second) {
        //             if (!rs.busy && !rs.executing) {
        //                 //add to RS
        //                 int rob_index = allocate_rob_entry(buffers.ROB);
        //                 std::cout << "ROB_INDEX=" << rob_index << std::endl; 
        //                 buffers.ROB.buffer[rob_index].opcode = instr.opcode;

        //                 ///NEED TO CHANGE FOR DIFF INSTR TYPES
        //                 buffers.ROB.buffer[rob_index].destination = instr.rd;

        //                 buffers.ROB.buffer[rob_index].instr_type = instr.type;

        //                 if (instr.opcode == HALT) {
        //                     buffers.ROB.buffer[rob_index].ready = true;
        //                     break;
        //                 }

        //                 buffers.ROB.buffer[rob_index].rs_tag = rs.get_tag();

        //                 rs.ROB_entry = rob_index;


        //                 rs.add_instruction(hw, instr);

        //                 //update RAT
        //                 if (instr.type == R || instr.type == I || instr.type == LIDX) {
        //                     update_RAT(hw, instr.rd, rs.ROB_entry);

        //                 }

        //                 if (required_FU == LOADSTORE) {
        //                     bool is_store = add_to_ldst_queue(rs, buffers.ldst_queue);
        //                     if (is_store) {
        //                         buffers.ROB.buffer[rob_index].is_store_instr = true;
        //                     }
        //                 }
        //                 rs.busy = true;
        //                 std::cout << "ADDED INSTR TO RS " << rs.number << std::endl;
        //                 std::cout << "with RS TYPE " << rs.FU_type << std::endl;

        //                 if (instr.type == B) {
        //                     buffers.ROB.buffer[rob_index].is_branch = true;
        //                 }

        //                 buffers.instr_queue.pop();
        //                 break;
        //             }
        //         }
                
        //     }
        // }
    }
}

void SuperscalarWriteUnit::write_results(Hardware &hw, PipelineBuffers &buffers) {
    for (int i = 0; i < WIDTH; i++) {
        if (buffers.completed_instr_res_stns.size() == 0) {
            return;
        }
        ReservationStation res_stn = buffers.completed_instr_res_stns.front();

        std::cout << "current instr at WRITE " << std::endl;
        print_instruction(res_stn.instr);

        if (res_stn.instr.opcode == COUNT) {
            //continue;
            buffers.completed_instr_res_stns.pop();
            buffers.ROB.buffer[res_stn.ROB_entry].ready = true;
            return;
        }

        buffers.ROB.buffer[res_stn.ROB_entry].result = res_stn.instr.result;

        // if (res_stn.instr.opcode != SW) {

        //     //ROB.buffer[res_stn.ROB_entry].destination = dest_reg;
        //     std::cout << "SETTING ROB ENTRY READY ENTRY " << res_stn.ROB_entry << std::endl;
        //     ROB.buffer[res_stn.ROB_entry].ready = true;

        // }

        std::cout << "SETTING ROB ENTRY READY ENTRY " << res_stn.ROB_entry << std::endl;
        buffers.ROB.buffer[res_stn.ROB_entry].ready = true;


        //(all_reservation_stations[res_stn.FU_type])[res_stn.number].busy = false;


        buffers.completed_instr_res_stns.pop();
    }
}

void SuperscalarCommitUnit::commit_results(Hardware &hw, PipelineBuffers &buffers) {
    for (CommitUnit &unit : units) {
        unit.commit_result(hw, buffers.all_reservation_stations, buffers.ROB);
        if (unit.committed) {
            committed_instrs++;
            unit.committed = false;
        }
        if (unit.flush) {
            flush = true;
            unit.flush = false;
            return;
        }
        if (unit.execution_finished) {
            hw.finished = true;
            unit.execution_finished = false;
            return;
        }
    }
}