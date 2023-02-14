#include "instruction.h"
#include "units.h"
#include <vector>

std::vector<Instruction> parse_file(std::string filename, Hardware &hw);