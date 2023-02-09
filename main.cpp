#include "instruction.h"
#include "units.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>

int main(int argc, char* argv[]) {

    // int32_t memory[2048];
    // int32_t reg_file[32];
    // int32_t pc;

    std::vector<Instruction> program;

    // bool finished = false;

    // std::unordered_map<std::string, int> labels;

    Hardware hw;
    hw.pc = -1;
    hw.reg_file[1] = 10;
    hw.reg_file[2] = 2;

    Instruction add = {};
    add.opcode = SUB;
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
  
    while (!hw.finished) {
        hw.pc = fetch_unit.fetch(hw.pc);
        execute_unit.execute(program[hw.pc], hw);
    }

    std::cout << hw.reg_file[3] << std::endl;

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