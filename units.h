#ifndef UNITS_H
#define UNITS_H

#include "instruction.h"

class FetchUnit {
    int fetch();
};

class ExecuteUnit {
    int execute(Instruction instr);
};

class WritebackUnit {
    int fetch();
};

#endif