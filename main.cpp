#include "instruction.h"

int32_t memory[2048];
int32_t registers[32];
int32_t pc;

Instruction program[];

int main(int argc, char* argv[]) {
    string filename;
    if (argc > 1) {
        filename = argv[1];
    }
    return 0;
}

void parse_program() {

}

int fetch() {
    return 0;
}

// int execute(Instruction instr) {
//     Opcode opcode = instr.opcode;
//     switch (opcode) {
//         case (ADD):
//             instr.rd = instr.rs1 + instr.rs2
//             break;
//     }
//     return 0;
// }

int writeback() {
    return 0;
}
