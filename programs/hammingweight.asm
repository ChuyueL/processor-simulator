addi t0 zero .a
addi t7 zero 0x14
addi t8 zero 0

for:

lw t1 t0 0

xor t2 t1 zero 

add t8 t8 t2

addi t0 t0 1

blt t0 t7 for

halt


data:

.a
0x1 0x0 0x0 0x1 0x1 0x0 0x1 0x1 0x1 0x1 0x0 0x1 0x1 0x1 0x1 0x1 0x0 0x1 0x1 0x1 
