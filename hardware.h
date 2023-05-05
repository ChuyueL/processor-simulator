#ifndef HARDWARE_H
#define HARDWARE_H

#include "tag.h"
#include <unordered_map>
#include <vector>
#include <array>

class Hardware {
    public:

    int32_t memory[128] = {0};
    std::array<int32_t, 32> reg_file = {0};
    int32_t pc;
    bool finished = false;
    bool reg_updating[32] = {false};
    std::vector<int> register_alias_table;
    bool valid[32] = {true};

    std::unordered_map<std::string, int> labels;
    std::unordered_map<std::string, int> variable_locations;

    Hardware() {
        for (int i = 0; i < 31; i++) {
            register_alias_table.push_back(-1);
        }
    }

};

#endif