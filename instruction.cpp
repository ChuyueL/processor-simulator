#include "instruction.h"
#include <unordered_map>
#include <string>

std::unordered_map<Opcode, std::string> opcode_table = {
    {ADD, "add"},
    {SUB, "sub"},
    {SLT, "slt"},
    {ADDI, "addi"},
    {LW, "lw"},
    {SW, "sw"},
    {BEQ, "beq"},
    {BLT, "blt"},
    {HALT, "halt"},
    {COUNT, "nop"}
};

std::unordered_map<Register, std::string> register_table = {
    {zero, "zero"},
    {ra, "ra"},
    {sp, "sp"},
    {gp, "gp"},
    {tp, "tp"},
    {fp, "fp"},
    {t0, "t0"},
    {t1, "t1"},
    {t2, "t2"},
    {t3, "t3"},
    {t4, "t4"},
    {t5, "t5"},
    {t6, "t6"},
    {t7, "t7"},
    {t8, "t8"},
    {s0, "s0"},
    {s1, "s1"},
    {s2, "s2"},
    {s3, "s3"},
    {s4, "s4"},
    {s5, "s5"},
    {s6, "s6"},
    {s7, "s6"},
    {s8, "s8"},
    {a0, "a0"},
    {a1, "a1"},
    {a2, "a2"},
    {a3, "a3"},
    {a4, "a4"},
    {a5, "a5"},
    {a6, "a6"},
    {a7, "a7"}
};

std::string opcode_to_string(Opcode opcode) {
    return opcode_table[opcode];
}

// Opcode string_to_opcode(std::string str) {
//     for (auto item : opcode_table) {
//         if (item.second == str) return item.first;
//     }
//     return COUNT;
// }

std::string register_to_string(Register reg) {
    return register_table[reg];
}