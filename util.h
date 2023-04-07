#ifndef UTIL_H
#define UTIL_H

#include "instruction.h"
#include "hardware.h"
#include <iostream>
#include <unordered_map>

void print_cpu_info(Instruction instr, Hardware hw);

void print_instruction(Instruction instr);

#endif