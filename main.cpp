#include "instruction.h"
#include "units.h"
#include <vector>
#include <iostream>


int32_t memory[2048];
int32_t reg_file[32];
int32_t pc;

std::vector<Instruction> program;

bool finished = false;

int main(int argc, char* argv[]) {
    pc = -1;
    reg_file[1] = 1;
    reg_file[2] = 2;

    Instruction add = {};
    add.opcode = ADD;
    add.rs1 = 1;
    add.rs2 = 2;
    add.rd = 3;

    Instruction halt = {};
    halt.opcode = HALT;

    program.push_back(add);
    program.push_back(halt);


    FetchUnit fetch_unit;
    ExecuteUnit execute_unit;
    //string filename;
    // if (argc > 1) {
    //     filename = argv[1];
    // }

    while (!finished) {
        pc = fetch_unit.fetch(pc);
        execute_unit.execute(program[pc], reg_file, finished);
    }

    std::cout << reg_file[3] << std::endl;

    std::cin.ignore();

    return 0;
}

void parse_program() {

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