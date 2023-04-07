#include "tomasulo.h"
#include "tag.h"
#include "util.h"

void FunctionalUnit::find_instruction_to_execute(std::vector<ReservationStation> reservation_stations) {
    std::cout << "INSTR AT RS 0 " << opcode_to_string(reservation_stations[0].instr.opcode) << std::endl;
    for (ReservationStation res_stn : reservation_stations) {
        if (res_stn.busy) {
            if (res_stn.tag1.FU_type == NONE && res_stn.tag2.FU_type == NONE) {
                instr_res_stn = res_stn;
                //current_instruction = res_stn.instr;
                std::cout << "res stn " << instr_res_stn.number << std::endl;
                std::cout << "OPCODE " << opcode_to_string(instr_res_stn.instr.opcode) << std::endl;
                return;
            }
        }
        
    }
    //current_instruction = PlaceholderInstruction();
    instr_res_stn = PlaceholderRS();
    std::cout << "placeholder instr\n";
}

void IssueUnit::issue_instruction(Hardware &hw, std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations) {
    std::cout << "current instr at ISSUE " << std::endl;
    print_instruction(current_instruction);

    Instruction instr = current_instruction;
    FUType required_FU = opcode_required_FU(instr.opcode);

    //std::vector<ReservationStation> reservation_stations = all_reservation_stations[required_FU];

    for (auto& kvp : all_reservation_stations) {
        if (kvp.first == required_FU) {

            for (ReservationStation& rs : kvp.second) {
                if (!rs.busy) {
                    //add to RS
                    rs.add_instruction(hw, instr);
                    rs.busy = true;
                    std::cout << "ADDED INSTR TO RS " << rs.number << std::endl;
                    break;
                }
            }
            
        }
    }

    // for (ReservationStation rs : reservation_stations) {
    //     if (!rs.busy) {
    //         //add to RS
    //         rs.add_instruction(instr);
    //         std::cout << "ADDED INSTR\n";
    //         break;
    //     }
    // }
}

void update_RAT(Hardware &hw, int reg, Tag tag) {
    hw.register_alias_table[reg] = tag;
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
            hw.finished = true;
            break;
        
        default:
            break;
    }

    if (op != COUNT) {
        //std::cout << "alu_output at EX " << result << std::endl;

    }

    instr_res_stn.instr.result = result;

}

void ALU::execute(Hardware &hw) {
    // bool can_execute_instr = false;
    // Instruction current_instr;
    // ReservationStation instr_res_stn;

    // //Find instr to execute
    // // can_execute_instr = true;

    // for (ReservationStation res_stn : reservation_stations) {
    //     if (res_stn.tag1.FU_type == NONE && res_stn.tag2.FU_type == NONE) {
    //         current_instr = res_stn.instr;
    //         instr_res_stn = res_stn;
    //         break;
    //     }
    // }
    

    // if (can_execute_instr) {
    //     //update RAT
    //     //execute instr

    //     hw.register_alias_table[current_instr.rd] = instr_res_stn.get_tag();

    //     //perform_ALU_operation(hw, current_instr.opcode, current_instr.rd,)
    // }

    std::cout << "current instr at EXECUTE " << std::endl;


    perform_ALU_operation(hw, instr_res_stn.instr.opcode, instr_res_stn.value1, instr_res_stn.value2, instr_res_stn.imm);

    print_instruction(instr_res_stn.instr);

    print_cpu_info(instr_res_stn.instr, hw);
}

void WriteUnit::write_result(Hardware &hw, std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations) {
    for (ReservationStation res_stn : completed_instr_res_stns) {
        if (res_stn.instr.opcode == COUNT) {
            continue;
        }
        int reg_number = 0;
        for (int reg : hw.reg_file) {
            if (!hw.valid[reg_number]) {
                if (hw.register_alias_table[reg_number].equals(res_stn.get_tag())) {
                    hw.register_alias_table[reg_number] = PlaceholderTag();
                    hw.valid[reg_number] = true;
                    hw.reg_file[reg_number] = res_stn.instr.result;
                }
            }
        }


        for (auto kvp : all_reservation_stations) {
            std::vector<ReservationStation> res_stns = kvp.second;
            
            for (ReservationStation res_station : res_stns) {
                if (res_station.tag1.equals(res_stn.get_tag())) {
                    res_station.value1 = res_stn.instr.result; //where does result come from?
                }
                else if (res_station.tag2.equals(res_stn.get_tag())) {
                    res_station.value2 = res_stn.instr.result;
                }
            }
        }
    }
    //write result to ARF
    //broadcast to all res stn's


    

}

void OoOPipeline::clock_cycle(Hardware &hw, std::vector<Instruction> program) {
    fetch_unit.fetch(hw, program);

    issue_unit.issue_instruction(hw, all_reservation_stations);

    //std::cout << "INSTR AT RS 0 " << opcode_to_string((all_reservation_stations[ARITH])[0].instr.opcode) << std::endl;


    for (ALU alu : ALUs) {
        alu.execute(hw);
    }

    // for (LDSTUnit mem_unit : ldst_units) {
    //     mem_unit.execute(all_reservation_stations[LOADSTORE]);
    // }

    // for (BranchUnit branch_unit : branch_units) {
    //     branch_unit.execute(all_reservation_stations[BRANCH]);
    // }

    write_unit.write_result(hw, all_reservation_stations);

}

void OoOPipeline::advance_pipeline(Hardware &hw) {

    for (ALU& alu : ALUs) {
        write_unit.completed_instr_res_stns.push_back(alu.instr_res_stn);

        alu.find_instruction_to_execute(all_reservation_stations[ARITH]);
    }

    // for (LDSTUnit ldst_unit : ldst_units) {
    //     write_unit.completed_instr_res_stns.push_back(ldst_unit.instr_res_stn);

    //     ldst_unit.find_instruction_to_execute(all_reservation_stations[LOADSTORE]);
    // }

    // for (BranchUnit branch_unit : branch_units) {
    //     write_unit.completed_instr_res_stns.push_back(branch_unit.instr_res_stn);

    //     branch_unit.find_instruction_to_execute(all_reservation_stations[BRANCH]);
    // }

    issue_unit.current_instruction = fetch_unit.current_instruction;


}