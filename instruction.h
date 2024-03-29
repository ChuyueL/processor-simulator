#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "tag.h"
#include <stdint.h>
#include <string>

enum Register {
    zero, //0
    ra, //1
    sp, //2
    gp, //3
    tp, //4
    fp, //5
    t0, //6
    t1, //7
    t2, //8
    t3, //9
    t4, //10
    t5, //11
    t6,
    t7,
    t8,
    s0,
    s1,
    s2,
    s3,
    s4,
    s5,
    s6,
    s7,
    s8,
    a0,
    a1,
    a2,
    a3,
    a4,
    a5,
    a6,
    a7
};

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
    JAL,
    HALT,
//IDX TYPE
    LDIDX,
    STIDX,
    COUNT //not actually an instruction

};

enum InstrType {
    R,
    I,
    S,
    B,
    J,
    LIDX,
    SIDX
};

class Instruction {

    public:
        InstrType type;
        bool predicted_taken = false;
        int prev_pc = 0;
        Opcode opcode;
        int rs1;
        int rs2;
        int rd;
        int ri = 0;
        int32_t imm;
        std::string label;
        int result = 0;
};


class RTypeInstruction : public Instruction {
    public:
        RTypeInstruction(Opcode _opcode, int _rd, int _rs1, int _rs2) {
            type = R;
            opcode = _opcode;
            rd = _rd;
            rs1 = _rs1;
            rs2 = _rs2;
            ri = 0;
            imm = 0;
        }
};

class ITypeInstruction : public Instruction {
    public:
        ITypeInstruction(Opcode _opcode, int _rd, int _rs1, int32_t _imm) {
            type = I;
            opcode = _opcode;
            rd = _rd;
            rs1 = _rs1;
            rs2 = 0;
            ri = 0;
            imm = _imm;
        }
};

class STypeInstruction : public Instruction {
    public:
        STypeInstruction(Opcode _opcode, int _rs1, int _rs2, int32_t _imm) {
            type = S;
            opcode = _opcode;
            rd = 0;
            rs1 = _rs1;
            rs2 = _rs2;
            ri = 0;
            imm = _imm;
        }
};

class BTypeInstruction : public Instruction {
    public:
        BTypeInstruction(Opcode _opcode, int _rs1, int _rs2, std::string _label) {
            type = B;
            opcode = _opcode;
            rd = 0;
            rs1 = _rs1;
            rs2 = _rs2;
            ri = 0;
            imm = 0;
            label = _label;
        }
};

class JTypeInstruction : public Instruction {
    public:
        JTypeInstruction(Opcode _opcode, int _rd, int32_t _imm) {
            type = B;
            opcode = _opcode;
            rd = _rd;
            rs1 = 0;
            rs2 = 0;
            ri = 0;
            imm = _imm;
        }
};

class LIDXInstruction : public Instruction {
    public:
        LIDXInstruction(Opcode _opcode, int _rd, int _rs1, int _ri) {
            type = LIDX;
            opcode = _opcode;
            rd = _rd;
            rs1 = _rs1;
            rs2 = 0;
            ri = _ri;
            imm = 0;
        }
};

class SIDXInstruction : public Instruction {
    public:
        SIDXInstruction(Opcode _opcode, int _rs1, int _rs2, int _ri) {
            type = SIDX;
            opcode = _opcode;
            rd = 0;
            ri = _ri;
            rs1 = _rs1;
            rs2 = _rs2;
            imm = 0;
        }
};

class PlaceholderInstruction : public Instruction {
    public:
        PlaceholderInstruction() {
            type = R;
            opcode = COUNT;
            rd = 0;
            rs1 = 0;
            rs2 = 0;
            ri = 0;
            imm = 0;
        }
};

std::string opcode_to_string(Opcode opcode);

std::string register_to_string(Register reg);

FUType opcode_required_FU(Opcode opcode);

#endif