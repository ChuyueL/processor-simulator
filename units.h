#ifndef UNITS_H
#define UNITS_H

#include "instruction.h"

//fetch next instruction from instr list/array
class FetchUnit {

    public:

    int fetch(int pc);

    FetchUnit() {};

    
};

//Execute the instruction, given the arguments
class ExecuteUnit {

    public:

    int execute(Instruction instr, int (&rf)[32], bool &finished);

    ExecuteUnit() {}


};

//Make changes to ARF
class WritebackUnit {

    public: 

    int writeback();

    WritebackUnit() {

    }
};

#endif