#ifndef UNITS_H
#define UNITS_H

#include "instruction.h"

//fetch next instruction from instr list/array
class FetchUnit {
    int fetch();
};

//Execute the instruction, given the arguments
class ExecuteUnit {
    int execute(Instruction instr);
};

//Make changes to ARF
class WritebackUnit {
    int fetch();
};

#endif