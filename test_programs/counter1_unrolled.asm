addi t0 zero 0
addi t1 zero 0x32

for:
addi s1 t0 1
addi s2 t0 2
addi s3 t0 3
addi s4 t0 4

sw t0 t0 0
sw s1 s1 0
sw s2 s2 0
sw s3 s3 0
sw s4 s4 0

addi t0 t0 5

blt t0 t1 for

halt