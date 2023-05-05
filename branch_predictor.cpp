#include "branch_predictor.h"


//true = take, false = don't take
bool BranchPredictor::predict_branch(Hardware &hw, Instruction instr) {
    if (hw.labels[instr.label] > hw.pc) {
        return false;
    }
    else {
        return true;
    }

    //return false;
}

