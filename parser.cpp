#include "instruction.h"
#include "units.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>


std::vector<std::string> tokenise_string(std::string str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;

    std::stringstream stream(str);

    while (std::getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;

}

Opcode string_to_opcode(std::string str) {
    Opcode opcode;

    if (str == "add") opcode = ADD;
    else if (str == "sub") opcode = SUB;
    else if (str == "slt") opcode = SLT;
    else if (str == "lw") opcode = LW;
    else if (str == "addi") opcode = ADDI;
    else if (str == "xor") opcode = XOR;
    else if (str == "sw") opcode = SW;
    else if (str == "blt") opcode = BLT;
    else if (str == "beq") opcode = BEQ;
    else if (str == "ldidx") opcode = LDIDX;
    else if (str == "stidx") opcode = STIDX;
    else if (str == "halt") opcode = HALT;

    return opcode;
}

int register_name_to_int(std::string reg_name) {
    Register reg;
    if (reg_name == "zero") reg = zero;
    else if (reg_name == "t0") reg = t0;
    else if (reg_name == "t1") reg = t1;
    else if (reg_name == "t2") reg = t2;
    else if (reg_name == "t3") reg = t3;
    else if (reg_name == "t4") reg = t4;
    else if (reg_name == "t5") reg = t5;
    else if (reg_name == "t6") reg = t6;
    else if (reg_name == "t7") reg = t7;
    else if (reg_name == "t8") reg = t8;
    else if (reg_name == "s0") reg = s0;
    else if (reg_name == "s1") reg = s1;
    else if (reg_name == "s2") reg = s2;
    else if (reg_name == "s3") reg = s3;
    else if (reg_name == "s4") reg = s4;
    else if (reg_name == "s5") reg = s5;

    return reg;

}

Instruction tokens_to_R_instr(std::vector<std::string> tokens) {
    Opcode opcode = string_to_opcode(tokens[0]);

    int rd = register_name_to_int(tokens[1]);
    int rs1 = register_name_to_int(tokens[2]);
    int rs2 = register_name_to_int(tokens[3]);

    Instruction instr = RTypeInstruction(opcode, rd, rs1, rs2);

    return instr;
}

Instruction tokens_to_I_instr(std::vector<std::string> tokens, Hardware hw) {
    Opcode opcode = string_to_opcode(tokens[0]);

    int rd = register_name_to_int(tokens[1]);
    int rs1 = register_name_to_int(tokens[2]);
    std::cout << "tokens[3] " << tokens[3] << std::endl;
    int imm;

    if (tokens[3][0] != '.') {
        imm = std::stoi(tokens[3], 0, 16);
    }
    else {
        std::cout << "label " << tokens[3] << std::endl;
        imm = hw.variable_locations[tokens[3]];
    }

    Instruction instr = ITypeInstruction(opcode, rd, rs1, imm);

    return instr;
}

Instruction tokens_to_S_instr(std::vector<std::string> tokens) {
    Opcode opcode = string_to_opcode(tokens[0]);

    int rs1 = register_name_to_int(tokens[1]);
    int rs2 = register_name_to_int(tokens[2]);
    int imm = std::stoi(tokens[3], 0, 16);

    Instruction instr = STypeInstruction(opcode, rs1, rs2, imm);

    return instr;
}

Instruction tokens_to_B_instr(std::vector<std::string> tokens) {
    Opcode opcode = string_to_opcode(tokens[0]);

    int rs1 = register_name_to_int(tokens[1]);
    int rs2 = register_name_to_int(tokens[2]);
    std::string label = tokens[3];
    
    Instruction instr = BTypeInstruction(opcode, rs1, rs2, label);

    return instr;
}

Instruction tokens_to_LIDX_instr(std::vector<std::string> tokens) {
    Opcode opcode = string_to_opcode(tokens[0]);
    int rd = register_name_to_int(tokens[1]);
    int rs1 = register_name_to_int(tokens[2]);
    int ri = register_name_to_int(tokens[3]);

    Instruction instr = LIDXInstruction(opcode, rd, rs1, ri);
    
    return instr;
}

Instruction tokens_to_SIDX_instr(std::vector<std::string> tokens) {
    Opcode opcode = string_to_opcode(tokens[0]);
    int rs1 = register_name_to_int(tokens[1]);
    int rs2 = register_name_to_int(tokens[2]);
    int ri = register_name_to_int(tokens[3]);

    Instruction instr = SIDXInstruction(opcode, rs1, rs2, ri);

    return instr;
}

std::vector<std::string> read_program(std::string filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}

void parse_data(std::vector<std::string> lines, Hardware &hw) {
    std::cout << "PARSING DATA" << std::endl;
    int next_free_memory = 0;
    bool processing_data = false;

    for (std::string line : lines) {
        std::cout << line << std::endl;

        std::vector<std::string> tokens = tokenise_string(line, ' ');

        if (tokens.size() == 0) {
            continue;
        }

        if (processing_data) {
            std::cout << "size " << tokens.size() << std::endl;
            for (std::string token : tokens) {
                std::cout << token << std::endl;
                int data = std::stoi(token, 0, 16);

                std::cout << "next free memory " << next_free_memory << std::endl;
                hw.memory[next_free_memory] = data;
                next_free_memory++;
            }
            processing_data = false;
        }
        if (tokens[0][0] == '.') {
            if (tokens[0] != ".data:") {
                processing_data = true;
                std::string name = tokens[0].substr(0, tokens[0].size());
                hw.variable_locations.insert({name, next_free_memory});
            }
        }
    }

}

std::vector<Instruction> parse_program(std::vector<std::string> lines, Hardware &hw) {
    int counter = 0;
    std::vector<Instruction> program;

    parse_data(lines, hw);

    std::cout << "memory after data parsing  " << std::endl;
    for (int entry : hw.memory) {
        std::cout << entry << " ";
    }
    std::cout << std::endl;

    for (auto pair : hw.variable_locations) {
        std::cout << pair.first << " ";
        std::cout << pair.second << std::endl;
    }

    for (std::string line : lines) {
        std::cout << line << std::endl;
        std::vector<std::string> tokens = tokenise_string(line, ' ');

        std::cout << "tokens" << std::endl;

        for (std::string token : tokens) {
            std::cout << token << " ";
        }
        std::cout << std::endl;
        std::cout << "size" << tokens.size() << std::endl;


        if (tokens.size() == 0) {
            continue;
            std::cout << "skip" << std::endl;
        }

        //remove leading whitespace
        // size_t string_start = line.find_first_not_of(' ');

        // if (string_start != std::string::npos) {
        //     line = line.substr(string_start);
        // }
        


        // tokens.erase(std::remove_if(tokens.begin(),
        //                             tokens.end(),
        //                             [](std::string x) {return x == "";}));

        if (tokens[0] == "add" || tokens[0] == "sub" || tokens[0] == "slt" || tokens[0] == "xor") {
            Instruction new_instr = tokens_to_R_instr(tokens);
            program.push_back(new_instr);
            counter++;
        }
        else if (tokens[0] == "addi" || tokens[0] == "lw") {
            Instruction new_instr = tokens_to_I_instr(tokens, hw);
            program.push_back(new_instr);
            counter++;
        }
        else if (tokens[0] == "sw") {
            Instruction new_instr = tokens_to_S_instr(tokens);
            program.push_back(new_instr);
            counter++;
        }
        else if (tokens[0] == "blt" || tokens[0] == "beq") {
            Instruction new_instr = tokens_to_B_instr(tokens);
            program.push_back(new_instr);
            counter++;
        }
        else if (tokens[0] == "ldidx") {
            Instruction new_instr = tokens_to_LIDX_instr(tokens);
            program.push_back(new_instr);
            counter++;
        }
        else if (tokens[0] == "stidx") {
            Instruction new_instr = tokens_to_SIDX_instr(tokens);
            program.push_back(new_instr);
            counter++;
        }
        else if (tokens[0] == "halt") {
            Instruction new_instr = RTypeInstruction(HALT, 0, 0, 0);
            program.push_back(new_instr);
            counter++;
        }
        else if (tokens[0].find(":") != std::string::npos) { //label
            std::string label = line.substr(0, line.size()-1);
            hw.labels.insert({label, counter});
        }
    }

    return program;
}

std::vector<Instruction> parse_file(std::string filename, Hardware &hw) {
    std::vector<std::string> lines = read_program(filename);

    return parse_program(lines, hw);
}
