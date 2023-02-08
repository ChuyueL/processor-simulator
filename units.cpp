#include "units.h"

int FetchUnit::fetch(int pc) {
    return pc+1;
}

int ExecuteUnit::execute(Instruction instr, int (&rf)[32], bool &finished) {
    Opcode opcode = instr.opcode;
    int32_t result = 0;
    switch (opcode) {
        case ADD:
            rf[instr.rd] = rf[instr.rs1] + rf[instr.rs2];
            break;

        case SUB:
            //
            break;
        
        case HALT:
            finished = true;
            break;
    }
    return result;
}