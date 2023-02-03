#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>

enum Opcode {
// R TYPE
    ADD,
    SUB,
    AND,
    OR,
    XOR,
    SLT,
// I TYPE
    ADDI,
    LW,
// S TYPE
    SW,
// B TYPE
    BEQ,
    BLT,
// J TYPE
    JAL

};

class Instruction {

    public:
        Opcode opcode;
        int rs1;
        int rs2;
        int rd;
        int32_t imm;

        int MakeRTypeInstruction();

};

// class RTypeInstruction : Instruction {
//     int rs1;
//     int rs2;
//     int rd;
// };

#endif