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

    std::string filename = "programs/vectoradd_unrolled.asm";

    std::vector<Instruction> program = parse_file(filename, hw);

    std::cout << program.size() << std::endl;

    // FetchUnit fetch_unit;
    // DecodeUnit decode_unit;
    // ExecuteUnit execute_unit;
    // MemoryUnit memory_unit;
    // WritebackUnit writeback_unit;
  
    // while (!hw.finished) {
    //     Instruction instr = fetch_unit.fetch(hw, program);
    //     std::cout << "completed fetch" << std::endl;
    //     execute_unit.execute(instr, hw);
    //     std::cout << "completed exec" << std::endl;

    //     memory_unit.alu_output = execute_unit.result;
    //     std::cout << "memory_unit.alu_output = execute_unit.result;" << std::endl;

    //     memory_unit.memory_stage(instr, hw);
    //     if (instr.opcode == BEQ || instr.opcode == BLT) {
    //         continue;
    //     }
    //     std::cout << "completed mem" << std::endl;

    //     writeback_unit.alu_output = memory_unit.alu_output;
    //     writeback_unit.load_mem_data = memory_unit.result;

    //     writeback_unit.writeback(instr, hw);
    //     std::cout << "completed wb" << std::endl;

    // }

    // int num_cycles = 0;

    // Pipeline pipeline = Pipeline();

    // while (!hw.finished) {
    //     pipeline.clock_cycle(hw, program);
    //     pipeline.advance_pipeline(hw);
    //     num_cycles++;
    // }


    int num_cycles = 0;

    //OoOPipeline pipeline = OoOPipeline();
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

    std::cout << "total cycles=" << num_cycles << std::endl;
    
    //std::cout << "total instrs executed=" << pipeline.instructions_executed << std::endl;

    //std::cout << std::setprecision(2) << std::fixed << "IPC=" << (float)pipeline.instructions_executed / (float)num_cycles << std::endl;

    //std::cin.ignore();

    return 0;
}