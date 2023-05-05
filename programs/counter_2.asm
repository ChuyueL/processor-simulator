addi t0 zero 0
addi t8 zero 0x1E

for:
addi t1 zero 1
addi t2 zero 2
addi t3 zero 3
addi t4 zero 4

sw t0 zero 0
sw t0 t1 1
sw t0 t2 2
sw t0 t3 3
sw t0 t4 4

addi t0 t0 5

blt t0 t8 for

halt

