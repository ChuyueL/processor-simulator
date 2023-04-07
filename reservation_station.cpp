#include "reservation_station.h"


Tag ReservationStation::get_tag() {
    return Tag(FU_type, number);
}

void ReservationStation::add_instruction(Hardware &hw, Instruction _instr) {
    instr = _instr;
    imm = instr.imm;

    if (!hw.valid[instr.rs1]) {
        tag1 = hw.register_alias_table[instr.rs1];
    }
    else {
        value1 = hw.reg_file[instr.rs1];
    }

    if (!hw.valid[instr.rs2]) {
        tag2 = hw.register_alias_table[instr.rs2];
    }
    else {
        value2 = hw.reg_file[instr.rs2];
    }
}