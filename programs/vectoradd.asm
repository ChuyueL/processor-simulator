addi t0 zero .a #load addr of a into t0
addi t1 zero .b # load addr of b into t1
addi t2 zero 0 # load i into t2
addi t3 zero 0 # store offset in t3
addi t7 zero .c # store addr of c into t7
addi t8 zero 0xA #number of loop iterations

for:
add t4 t0 t3 #address for array access (base addr of a + offset)
lw t5 t4 0 #load a[i]
add t4 t1 t3 #adddress for array access for b
lw t6 t4 0 #load b[i]
add t4 t5 t6 #result for addition of a[i] + b[i]
add t5 t7 t3 #memory address for c[i]
sw t5 t4 0 #store result into c[i]
addi t2 t2 1 #i++
addi t3 t3 1 #increase offset by 4 because word size
halt

blt t2 t8 for # if (i < 10) go round loop again


.data:

.a
0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1

.b
0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2
    

.c
0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0
