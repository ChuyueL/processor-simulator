#ifndef BRANCH_PREDICTOR_H
#define BRANCH_PREDICTOR_H

#include "instruction.h"
#include "hardware.h"

class BranchPredictor {
    public:
    bool predict_branch(Hardware &hw, Instruction instr);
};

#endif