#ifndef SUPERSCALAR_H
#define SUPERSCALAR_H

#include "instruction.h"
#include "superscalar_units.h"
#include "tomasulo.h"
#include <queue>
#include <vector>
#include <unordered_map>

#define ALU_COUNT 2
#define LDST_COUNT 2

class SuperscalarOoOPipeline {

    public:

    int instructions_executed = 0;

    SuperscalarFetchUnit fetch_unit;

    SuperscalarIssueUnit issue_unit;

    std::vector<ALU> ALUs;
        
    std::vector<LDSTUnit> ldst_units;

    std::vector<BranchUnit> branch_units;

    PipelineBuffers pipeline_buffers;

    SuperscalarWriteUnit write_unit;

    SuperscalarCommitUnit commit_unit;

    SuperscalarOoOPipeline() {
        fetch_unit = SuperscalarFetchUnit();
        issue_unit = SuperscalarIssueUnit();
        for (int i = 0; i < ALU_COUNT; i++) {
            ALUs.push_back(ALU());
        }
        for (int i = 0; i < LDST_COUNT; i++) {
            ldst_units.push_back(LDSTUnit());
        }
        branch_units.push_back(BranchUnit());

        pipeline_buffers = PipelineBuffers();

        write_unit = SuperscalarWriteUnit();

        commit_unit = SuperscalarCommitUnit();
    }

    void clock_cycle(Hardware &hw, std::vector<Instruction> program);
    void advance_pipeline(Hardware &hw);

    void reset_regs(Hardware &hw);

    void reset_all_res_stns();

    void flush_pipeline();
    




};

#endif