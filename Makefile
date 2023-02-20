all:
	clang++ -g -Wall -fsanitize=address main.cpp instruction.cpp units.cpp parser.cpp -o sim 