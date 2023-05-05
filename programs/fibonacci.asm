addi t0 zero 0x2
addi t8 zero 0xF

for:
lw t1 t0 -1
lw t2 t0 -2
add t3 t1 t2

sw t0 t3 0

addi t0 t0 1

blt t0 t8 for

halt



data:

.a
0x0 0x1