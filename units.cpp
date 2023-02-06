#include "units.h"

int FetchUnit::fetch(Instruction instructions[], int pc) {

}

int ExecuteUnit::execute(Instruction instr, int32_t arg1, int32_t arg2, int32_t imm) {
    Opcode opcode = instr.opcode;
    int32_t result = 0;
    switch (opcode) {
        case ADD:
            result = arg1 + arg2;
            break;

        case SUB:
            //
            break;
    }
    return 0;
}