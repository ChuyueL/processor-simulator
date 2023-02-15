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
    else if (str == "sw") opcode = SW;
    else if (str == "blt") opcode = BLT;
    else if (str == "beq") opcode = BEQ;
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

    return reg;

}

Instruction tokens_to_R_instr(std::vector<std::string> tokens) {
    Opcode opcode = string_to_opcode(tokens[0]);

    int rd = register_name_to_int(tokens[1]);
    int rs1 = register_name_to_int(tokens[2]);
    int rs2 = register_name_to_int(tokens[3]);

    Instruction instr;
    instr.MakeRTypeInstruction(opcode, rd, rs1, rs2);

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

    Instruction instr;
    instr.MakeITypeInstruction(opcode, rd, rs1, imm);

    return instr;
}

Instruction tokens_to_S_instr(std::vector<std::string> tokens) {
    Opcode opcode = string_to_opcode(tokens[0]);

    int rs1 = register_name_to_int(tokens[1]);
    int rs2 = register_name_to_int(tokens[2]);
    int imm = std::stoi(tokens[3], 0, 16);

    Instruction instr;
    instr.MakeSTypeInstruction(opcode, rs1, rs2, imm);

    return instr;
}

Instruction tokens_to_B_instr(std::vector<std::string> tokens) {
    Opcode opcode = string_to_opcode(tokens[0]);

    int rs1 = register_name_to_int(tokens[1]);
    int rs2 = register_name_to_int(tokens[2]);
    std::string label = tokens[3];
    
    Instruction instr;
    instr.MakeBTypeInstruction(opcode, rs1, rs2, label);

    return instr;
}

std::vector<std::string> read_program(std::string filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(file, line)) {
        lines.push_back(line);

        //std::cout << "line " << line << std::endl;
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
            //std::vector<std::string> data_tokens = std::vector<std::string>(tokens.begin() + 1, tokens.end());
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

    // int next_free_memory = 0;
    // bool processing_data = false;

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

        // if (processing_data) {
        //     for (std::string token : tokens) {
        //         int data = std::stoi(token, 0, 16);
        //         hw.memory[next_free_memory] = data;
        //         next_free_memory++;
        //     }
        // }

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

        if (tokens[0] == "add" || tokens[0] == "sub" || tokens[0] == "slt") {
            Instruction new_instr = tokens_to_R_instr(tokens);
            std::cout << "ADDDDDDDD" << std::endl;
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
        else if (tokens[0] == "halt") {
            Instruction new_instr;
            new_instr.MakeRTypeInstruction(HALT, 0, 0, 0);
            program.push_back(new_instr);
            counter++;
        }
        else if (tokens[0].find(":") != std::string::npos) { //label
            std::string label = line.substr(0, line.size()-1);
            hw.labels.insert({label, counter});
        }
        // else if (tokens[0][0] == '.') {
        //     if (tokens[0] != ".data") {
        //         processing_data = true;
        //         hw.variable_locations.insert({tokens[0], next_free_memory});
        //     }
        // }


        // if (line.find("add") != std::string::npos) {
        //     std::vector<std::string> tokens = tokenise_string(line, ' ');
        //     int rd = std::stoi(tokens[1]);
        //     int rs1 = std::stoi(tokens[2]);
        //     int rs2 = std::stoi(tokens[3]);

        //     Instruction instr;
        //     instr.MakeRTypeInstruction(ADD, rd, rs1, rs2);

        //     counter++;


        // }
        // else if (line.find("sub") != std::string::npos) {
        //     std::vector<std::string> tokens = tokenise_string(line, ' ');
        //     int rd = std::stoi(tokens[1]);
        //     int rs1 = std::stoi(tokens[2]);
        //     int rs2 = std::stoi(tokens[3]);

        //     Instruction instr;
        //     instr.MakeRTypeInstruction(SUB, rd, rs1, rs2);
        // }
        // else if (line.find("lw") != std::string::npos) {
        //     std::vector<std::string> tokens = tokenise_string(line, ' ');

        //     int rd = std::stoi(tokens[1]);
        //     int rs1 = std::stoi(tokens[2]);
        //     int imm = std::stoi(tokens[3], 0, 16);

        //     Instruction instr;
        //     instr.MakeITypeInstruction(LW, rd, rs1, imm);
        // }
        // else if (line.find("halt") != std::string::npos) {
        //     Instruction instr;
        //     instr.MakeRTypeInstruction(HALT, 0, 0, 0);
        // }
        // else if (line.find(":") != std::string::npos) { //label
        //     std::string label = line.substr(0, line.size()-1);
        //     hw.labels.insert({label, counter + 1});
        //     counter--;
        // } 

        // counter++;
    }

    return program;
}

std::vector<Instruction> parse_file(std::string filename, Hardware &hw) {
    std::vector<std::string> lines = read_program(filename);

    return parse_program(lines, hw);
}
