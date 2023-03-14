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
    {HALT, "halt"}
};

std::unordered_map<Register, std::string> register_table = {
    {zero, "zero"},
    {t0, "t0"},
    {t1, "t1"},
    {t2, "t2"},
    {t3, "t3"},
    {t4, "t4"},
    {t5, "t5"},
    {t6, "t6"},
    {t7, "t7"},
    {t8, "t8"},
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