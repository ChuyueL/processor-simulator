#include "units.h"

int FetchUnit::fetch(int pc) {
    return pc+1;
}

int ExecuteUnit::execute(Instruction instr, Hardware &hw) {
    Opcode opcode = instr.opcode;
    int32_t result = 0;
    switch (opcode) {
        case ADD:
            hw.reg_file[instr.rd] = hw.reg_file[instr.rs1] + hw.reg_file[instr.rs2];
            break;

        case SUB:
            hw.reg_file[instr.rd] = hw.reg_file[instr.rs1] - hw.reg_file[instr.rs2];
            break;
        
        case HALT:
            hw.finished = true;
            break;
    }
    return result;
}