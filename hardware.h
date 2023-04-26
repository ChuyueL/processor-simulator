#ifndef HARDWARE_H
#define HARDWARE_H

#include "tag.h"
#include <unordered_map>
#include <vector>

class Hardware {
    public:

    int32_t memory[128] = {0};
    int32_t reg_file[32] = {0};
    int32_t pc;
    bool finished = false;
    bool reg_updating[32] = {false};
    //Tag register_alias_table[32] = {PlaceholderTag()};
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