#include "tomasulo.h"
#include "tag.h"
#include "util.h"


void FunctionalUnit::find_instruction_to_execute(std::vector<ReservationStation> reservation_stations) {
    //std::cout << "INSTR AT RS 0 " << opcode_to_string(reservation_stations[0].instr.opcode) << std::endl;
    bool halt_instr_exists = false;
    ReservationStation halt_instr_RS = PlaceholderRS(); //force HALT instruction to be executed only if no other instruction can be executed

    for (ReservationStation& res_stn : reservation_stations) {
        if (res_stn.busy && !res_stn.executing) {
            if (res_stn.tag1.FU_type == NONE && res_stn.tag2.FU_type == NONE) {
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
            // else {
            //     if (res_stn.instr.opcode != HALT) {
            //         halt_instr_exists = false;
            //     }
            // }
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

void update_RAT(Hardware &hw, int reg, Tag tag) {
    if (reg == 0) {
        return;
    }
    hw.valid[reg] = false;
    hw.register_alias_table[reg] = tag;
}

void IssueUnit::issue_instruction(Hardware &hw, std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations) {
    std::cout << "current instr at ISSUE " << std::endl;
    print_instruction(current_instruction);

    Instruction instr = current_instruction;
    FUType required_FU = opcode_required_FU(instr.opcode);

    if (instr.opcode == COUNT) {
        return;
    }


    for (auto& kvp : all_reservation_stations) {
        if (kvp.first == required_FU) {

            for (ReservationStation& rs : kvp.second) {
                if (!rs.busy && !rs.executing) {
                    //add to RS
                    rs.add_instruction(hw, instr);
                    rs.busy = true;
                    std::cout << "ADDED INSTR TO RS " << rs.number << std::endl;
                    std::cout << "with RS TYPE " << rs.FU_type << std::endl;

                    //update RAT
                    update_RAT(hw, instr.rd, rs.get_tag());
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
    std::cout << "RESULT = " << result << std::endl;

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

    std::cout << "FROM RS " << instr_res_stn.number << std::endl;


    print_instruction(instr_res_stn.instr);


    if (instr_res_stn.FU_type == NONE) {
        return;
    }

    update_RAT(hw, instr_res_stn.instr.rd, instr_res_stn.get_tag());

    perform_ALU_operation(hw, instr_res_stn.instr.opcode, instr_res_stn.value1, instr_res_stn.value2, instr_res_stn.imm);

    
    //print_cpu_info(instr_res_stn.instr, hw);
}

void WriteUnit::write_result(Hardware &hw, std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations) {
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


    int reg_number = 0;
    for (int& reg : hw.reg_file) {
        if (!hw.valid[reg_number]) {
            if (hw.register_alias_table[reg_number].equals(res_stn.get_tag())) {
                std::cout << "UPDATING REG " << std::endl;
                hw.register_alias_table[reg_number] = PlaceholderTag();
                hw.valid[reg_number] = true;
                hw.reg_file[reg_number] = res_stn.instr.result;
            }
        }
        reg_number++;
    }
    (all_reservation_stations[res_stn.FU_type])[res_stn.number].busy = false;

    (all_reservation_stations[res_stn.FU_type])[res_stn.number].executing = false;

    //update instr res stn to no longer be busy


    for (auto& kvp : all_reservation_stations) {
        
        for (ReservationStation& res_station : kvp.second) {
            if (res_station.tag1.equals(res_stn.get_tag())) {
                res_station.value1 = res_stn.instr.result; //where does result come from?
                res_station.tag1 = PlaceholderTag();
            }
            if (res_station.tag2.equals(res_stn.get_tag())) {
                res_station.value2 = res_stn.instr.result;
                res_station.tag2 = PlaceholderTag();
            }
        }
    }

    completed_instr_res_stns.pop();


    // for (ReservationStation res_stn : completed_instr_res_stns) {

    //     std::cout << "current instr at WRITE " << std::endl;
    //     print_instruction(res_stn.instr);
    //     if (res_stn.instr.opcode == COUNT) {
    //         continue;
    //     }


    //     int reg_number = 0;
    //     for (int& reg : hw.reg_file) {
    //         if (!hw.valid[reg_number]) {
    //             if (hw.register_alias_table[reg_number].equals(res_stn.get_tag())) {
    //                 std::cout << "UPDATING REG " << std::endl;
    //                 hw.register_alias_table[reg_number] = PlaceholderTag();
    //                 hw.valid[reg_number] = true;
    //                 hw.reg_file[reg_number] = res_stn.instr.result;
    //                 std::cout << "with RESULT " << res_stn.instr.result;
    //             }
    //         }
    //         reg_number++;
    //     }
    //     (all_reservation_stations[res_stn.FU_type])[res_stn.number].busy = false;

    //     //update instr res stn to no longer be busy


    //     for (auto& kvp : all_reservation_stations) {
            
    //         for (ReservationStation& res_station : kvp.second) {
    //             if (res_station.tag1.equals(res_stn.get_tag())) {
    //                 res_station.tag1 = PlaceholderTag();
    //                 res_station.value1 = res_stn.instr.result; //where does result come from?
    //             }
    //             else if (res_station.tag2.equals(res_stn.get_tag())) {
    //                 res_station.tag2 = PlaceholderTag();
    //                 res_station.value2 = res_stn.instr.result;
    //             }
    //         }
    //     }
    // }


    //write result to ARF
    //broadcast to all res stn's

    //remove completed rs from todolist for write

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
    for (Tag reg_alias : hw.register_alias_table) {
        std::cout << register_to_string(static_cast<Register>(counter)) << "=";
        counter++;
        std::cout << reg_alias.number << " ";
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

void OoOPipeline::clock_cycle(Hardware &hw, std::vector<Instruction> program) {
    fetch_unit.fetch(hw, program);

    issue_unit.issue_instruction(hw, all_reservation_stations);

    //std::cout << "INSTR AT RS 0 " << opcode_to_string((all_reservation_stations[ARITH])[0].instr.opcode) << std::endl;


    for (ALU& alu : ALUs) {
        alu.execute(hw);

        if (alu.instr_res_stn.FU_type != NONE) {
            //(all_reservation_stations[alu.instr_res_stn.FU_type])[alu.instr_res_stn.number].busy = false;

            //(all_reservation_stations[alu.instr_res_stn.FU_type])[alu.instr_res_stn.number].executing = false;

        }

    }

    // for (LDSTUnit mem_unit : ldst_units) {
    //     mem_unit.execute(all_reservation_stations[LOADSTORE]);
    // }

    // for (BranchUnit branch_unit : branch_units) {
    //     branch_unit.execute(all_reservation_stations[BRANCH]);
    // }

    write_unit.write_result(hw, all_reservation_stations);

    for (ReservationStation rs : all_reservation_stations[ARITH]) {
        std::cout << "RS " << rs.number << std::endl;
        std::cout << "OPCODE " << opcode_to_string(rs.instr.opcode) << std::endl;
        std::cout << "TAG1 " << rs.tag1.number << std::endl;
        std::cout << "TAG2 " << rs.tag2.number << std::endl;
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