#ifndef SUPERSCALAR_UNITS_H
#define SUPERSCALAR_UNITS_H

#include "instruction.h"
#include "superscalar_units.h"
#include "tomasulo.h"
#include "units.h"
#include "branch_predictor.h"
#include <array>
#include <vector>
#include <unordered_map>
#include <queue>

#define WIDTH 2

class PipelineBuffers {
    public: 
    std::queue<Instruction> instr_queue;
    std::unordered_map<FUType, std::vector<ReservationStation>> all_reservation_stations;
    std::deque<LDSTQueue_Entry> ldst_queue;

    std::queue<ReservationStation> completed_instr_res_stns;

    ReorderBuffer ROB;
    
    PipelineBuffers() {
        for (int i = 0; i < NUM_RS; i++) {
            all_reservation_stations[ARITH].push_back(ReservationStation(ARITH, i));
            all_reservation_stations[LOADSTORE].push_back(ReservationStation(LOADSTORE, i));
            all_reservation_stations[BRANCH].push_back(ReservationStation(BRANCH, i));
        }
    }

};

class SuperscalarFetchUnit {

    public:

    BranchPredictor branch_predictor;

    void fetch_instructions(Hardware &hw, std::vector<Instruction> program, PipelineBuffers &buffers);

};


class SuperscalarIssueUnit {
    public:

    void issue_instructions(Hardware &hw, PipelineBuffers &buffers);

};

class SuperscalarWriteUnit {
    public:

    void write_results(Hardware &hw, PipelineBuffers &buffers);
};

class SuperscalarCommitUnit {
    public:
        bool flush = false;
        std::array<CommitUnit, WIDTH> units;

        int committed_instrs = 0;

        void commit_results(Hardware &hw, PipelineBuffers &buffers);

        SuperscalarCommitUnit () {
            for (int i = 0; i < WIDTH; i++) {
                units[i] = CommitUnit();
            }
        }
};

#endif