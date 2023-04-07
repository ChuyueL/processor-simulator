all:
	clang++ -g -Wall -fsanitize=address main.cpp instruction.cpp units.cpp parser.cpp -o sim 

run:
	clang++ -g -Wall -fsanitize=address main.cpp tag.cpp hardware.cpp instruction.cpp units.cpp parser.cpp tomasulo.cpp reservation_station.cpp -o sim 
	./sim