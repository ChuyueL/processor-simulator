#include "instruction.h"
#include "units.h"
#include "parser.h"
#include "tomasulo.h"
#include "superscalar.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <string>

int main(int argc, char* argv[]) {

    Hardware hw = Hardware();
    hw.pc = 0;
    hw.reg_file[0] = 0;

    std::string filename = argv[1];

    std::vector<Instruction> program = parse_file(filename, hw);

    std::cout << program.size() << std::endl;


    int num_cycles = 0;

    SuperscalarOoOPipeline pipeline = SuperscalarOoOPipeline();

    while (!hw.finished) {
        pipeline.clock_cycle(hw, program);
        pipeline.advance_pipeline(hw);
        num_cycles++;
    }


    for (Instruction instr : program) {
        std::cout << "opcode=" << opcode_to_string(instr.opcode);
        std::cout << " rd=" << std::to_string(instr.rd);
        std::cout << " rs1=" << std::to_string(instr.rs1);
        std::cout << " rs2=" << std::to_string(instr.rs2);
        std::cout << " imm=" << std::to_string(instr.imm);
        std::cout << " label=" << instr.label;
        std::cout << std::endl;
    }

    std::cout << "REGISTERS" << std::endl;
    int counter = 0;
    for (int reg : hw.reg_file) {
        std::cout << register_to_string(static_cast<Register>(counter)) << "=";
        counter++;
        std::cout << reg << " ";
    }
    std::cout << std::endl;

    std::cout << "MEMORY " << std::endl;
    for (int entry : hw.memory) {
        std::cout << entry << " ";
    }
    std::cout << std::endl;

    std::cout << "total cycles = " << num_cycles << std::endl;
    
    std::cout << "total instrs executed = " << pipeline.instructions_executed << std::endl;

    std::cout << std::setprecision(2) << std::fixed << "IPC=" << (float)pipeline.instructions_executed / (float)num_cycles << std::endl;

    std::cout << "total branches encountered = " << pipeline.commit_unit.total_branches << std::endl;

    std::cout << "correctly predicted branches = " << pipeline.commit_unit.correctly_predicted_branches << std::endl;

    std::cout << std::setprecision(2) << std::fixed << "proportion of correctly predicted branches = " << (float)pipeline.commit_unit.correctly_predicted_branches / (float)pipeline.commit_unit.total_branches << std::endl;

    return 0;
}