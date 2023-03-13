#include "instruction.h"
#include "units.h"
#include "parser.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>

int main(int argc, char* argv[]) {

    Hardware hw;
    hw.pc = 0;
    hw.reg_file[0] = 0;

    std::string filename = "programs/gcd.asm";

    std::vector<Instruction> program = parse_file(filename, hw);

    std::cout << program.size() << std::endl;

    // FetchUnit fetch_unit;
    // ExecuteUnit execute_unit;
  
    // while (!hw.finished) {
    //     Instruction instr = fetch_unit.fetch(hw, program);
    //     execute_unit.execute(instr, hw);
    // }

    Pipeline pipeline = Pipeline();

    while (!hw.finished) {
        pipeline.clock_cycle(hw, program);
        pipeline.advance_pipeline();
    }


    for (Instruction instr : program) {
        std::cout << "opcode=" << std::to_string(instr.opcode);
        std::cout << " rd=" << std::to_string(instr.rd);
        std::cout << " rs1=" << std::to_string(instr.rs1);
        std::cout << " rs2=" << std::to_string(instr.rs2);
        std::cout << " imm=" << std::to_string(instr.imm);
        std::cout << " label=" << instr.label;
        std::cout << std::endl;
    }

    std::cin.ignore();

    return 0;
}