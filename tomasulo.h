#ifndef TOMASULO_H
#define TOMASULO_H

#include "instruction.h"
#include "units.h"
#include "reservation_station.h"
#include "reorder_buffer.h"
#include "ldst_queue.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>

#define NUM_RS 10

class IssueUnit : public Unit {
    public:

    bool stalled = false;

    void issue_instruction(Hardware &hw, std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations, std::deque<LDSTQueue_Entry> &ldst_queue, ReorderBuffer &ROB);

    IssueUnit() {
        current_instruction = PlaceholderInstruction();
    }
};

class FunctionalUnit : public Unit {
    public: 

    FUType type;
    ReservationStation instr_res_stn = PlaceholderRS();

};

class ALU : public FunctionalUnit {
    public: 

    void find_instruction_to_execute(std::vector<ReservationStation> reservation_stations);

    void perform_ALU_operation(Hardware &hw, Opcode op, int val1, int val2, int imm);
    void execute(Hardware &hw);

    ALU() {
        type = ARITH;
    }

};

class LDSTUnit : public FunctionalUnit {
    public: 

    //void perform_memory_operation(Hardware &hw, Opcode op, int val1, int val2, int imm);

    void find_mem_instr(std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations, std::deque<LDSTQueue_Entry> &queue);

    //calculate address for memory access
    void address_calculation(std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations, ReorderBuffer &ROB);

    //perform memory access (for loads only)
    void memory_access(Hardware &hw, std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations, ReorderBuffer &ROB);

    void execute_ldst_cycle(Hardware &hw, std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations, ReorderBuffer &ROB);

    void advance_ldst_pipeline();

    Tag instr1_rs_tag = PlaceholderTag();

    Tag instr2_rs_tag = PlaceholderTag();

    int result1 = 0;
    int result2 = 0;

    bool stage1_stalled = false;
    bool stage2_stalled = false;

    LDSTUnit() {
        type = LOADSTORE;
    }

};

class BranchUnit : public FunctionalUnit {
    public: 

    Tag instr_rs_tag = PlaceholderTag();

    void find_branch_instr(std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations);

    void get_branch_address(Hardware &hw, std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations, ReorderBuffer &ROB);

    BranchUnit() {
        type = BRANCH;
    }

};

class WriteUnit : public Unit {
    public:

    std::queue<ReservationStation> completed_instr_res_stns;
    void write_result(Hardware &hw, std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations, ReorderBuffer &ROB);
};

class CommitUnit : public Unit {
    public:
        bool flush = false;

        void commit_result(Hardware &hw, std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations, ReorderBuffer &ROB);
        bool check_if_branch_correctly_predicted(std::unordered_map<FUType, std::vector<ReservationStation>> &all_reservation_stations, ROBEntry ROB_entry);

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
        
        std::deque<LDSTQueue_Entry> ldst_queue;

        ReorderBuffer ROB;


        WriteUnit write_unit;

        CommitUnit commit_unit;

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

            commit_unit = CommitUnit();

        }

        void clock_cycle(Hardware &hw, std::vector<Instruction> program);

        void advance_pipeline(Hardware &hw);

        void reset_all_res_stns();

        void flush_pipeline();

        void stall_pipeline();

        void continue_pipeline();




};

#endif