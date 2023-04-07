#ifndef TOMASULO_H
#define TOMASULO_H

#include "instruction.h"
#include "units.h"
#include "reservation_station.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>

#define NUM_RS 10

class IssueUnit : public Unit {
    public:

    void issue_instruction(Hardware &hw, std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations);
};

class FunctionalUnit : public Unit {
    public: 

    FUType type;
    ReservationStation instr_res_stn = PlaceholderRS();

    void find_instruction_to_execute(std::vector<ReservationStation> reservation_stations);


};

class ALU : public FunctionalUnit {
    public: 

    void perform_ALU_operation(Hardware &hw, Opcode op, int val1, int val2, int imm);
    void execute(Hardware &hw);

    ALU() {
        type = ARITH;
    }

};

class LDSTUnit : public FunctionalUnit {
    public: 

    void execute(Opcode op, int rd, int val1, int val2, int imm);

    LDSTUnit() {
        type = LOADSTORE;
    }

};

class BranchUnit : public FunctionalUnit {
    public: 

    void execute(Opcode op, int rd, int val1, int val2, int imm);

    BranchUnit() {
        type = BRANCH;
    }

};

class WriteUnit : public Unit {
    public:

    std::vector<ReservationStation> completed_instr_res_stns;
    void write_result(Hardware &hw, std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations);
};

//out of order pipeline
class OoOPipeline { 
    public:

        FetchUnit fetch_unit;
        
        IssueUnit issue_unit;

        std::vector<ALU> ALUs;
        
        std::vector<LDSTUnit> ldst_units;

        std::vector<BranchUnit> branch_units;

        std::unordered_map<FUType, std::vector<ReservationStation>> all_reservation_stations;

        WriteUnit write_unit;

        OoOPipeline() {
            fetch_unit = FetchUnit();
            issue_unit = IssueUnit();
            ALUs.push_back(ALU());
            ldst_units.push_back(LDSTUnit());
            branch_units.push_back(BranchUnit());

            for (int i = 0; i < NUM_RS; i++) {
                all_reservation_stations[ARITH].push_back(ReservationStation(ARITH, i));
                all_reservation_stations[LOADSTORE].push_back(ReservationStation(LOADSTORE, i));
                all_reservation_stations[BRANCH].push_back(ReservationStation(BRANCH, i));
            }
    

            write_unit = WriteUnit();

        }

        void clock_cycle(Hardware &hw, std::vector<Instruction> program);

        void advance_pipeline(Hardware &hw);

        void flush_pipeline(Hardware &hw, int number);

        void stall_pipeline();

        void continue_pipeline();

};

#endif