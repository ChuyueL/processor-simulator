all:
	clang++ -g -Wall -fsanitize=address main.cpp tag.cpp hardware.cpp instruction.cpp units.cpp parser.cpp tomasulo.cpp reservation_station.cpp reorder_buffer.cpp -o sim 

run:
	clang++ -g -Wall -fsanitize=address main.cpp tag.cpp hardware.cpp instruction.cpp units.cpp parser.cpp tomasulo.cpp reservation_station.cpp reorder_buffer.cpp -o sim 
	./sim