addi t0 zero 0
addi t1 zero 0x32

for:
sw t0 t0 0
addi t0 t0 1
blt t0 t1 for

halt