all:
	clang++ -g -fsanitize=address main.cpp tag.cpp hardware.cpp instruction.cpp units.cpp parser.cpp tomasulo.cpp reservation_station.cpp reorder_buffer.cpp superscalar.cpp superscalar_units.cpp branch_predictor.cpp -o sim 
run:
	clang++ -g -Wall -fsanitize=address main.cpp tag.cpp hardware.cpp instruction.cpp units.cpp parser.cpp tomasulo.cpp reservation_station.cpp reorder_buffer.cpp superscalar.cpp superscalar_units.cpp branch_predictor.cpp -o sim 
	./sim programs/vectoradd.asm