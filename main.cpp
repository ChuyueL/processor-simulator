#include "instruction.h"
#include "units.h"
#include "parser.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>

int main(int argc, char* argv[]) {

    // int32_t memory[2048];
    // int32_t reg_file[32];
    // int32_t pc;


    // bool finished = false;

    // std::unordered_map<std::string, int> labels;

    Hardware hw;
    hw.pc = 0;
    hw.reg_file[0] = 0;

    std::string filename = "programs/vectoradd.asm";

    std::vector<Instruction> program = parse_file(filename, hw);

    std::cout << program.size() << std::endl;


    // hw.reg_file[1] = 10;
    // hw.reg_file[2] = 2;

    // Instruction add = {};
    // add.opcode = SUB;
    // add.rs1 = 1;
    // add.rs2 = 2;
    // add.rd = 3;

    // Instruction halt = {};
    // halt.opcode = HALT;

    // program.push_back(add);
    // program.push_back(halt);


    FetchUnit fetch_unit;
    ExecuteUnit execute_unit;
    //string filename;
    // if (argc > 1) {
    //     filename = argv[1];
    // }
  
    while (!hw.finished) {
        Instruction instr = fetch_unit.fetch(hw, program);
        execute_unit.execute(instr, hw);
    }

    // std::cout << hw.reg_file[3] << std::endl;

    for (Instruction instr : program) {
        std::cout << "opcode " << std::to_string(instr.opcode);
        std::cout << "rd " << std::to_string(instr.rd);
        std::cout << "rs1 " << std::to_string(instr.rs1);
        std::cout << "rs2 " << std::to_string(instr.rs2);
        std::cout << "imm " << std::to_string(instr.imm);
        std::cout << "label " << instr.label;
        std::cout << std::endl;
    }

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