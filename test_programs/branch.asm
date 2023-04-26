addi t0 zero .a
addi t1 zero .b
addi t5 zero .c

lw t2 t0 0
lw t3 t1 0

add t4 t3 t2

sw t5 t4 0

add t6 t4 t4

sw t5 t6 1

blt t7 t5 br

halt

br:
add t7 t6 t4

sw t5 t7 2

halt


.data:

.a
0x1

.b
0x2

.c
0x0
