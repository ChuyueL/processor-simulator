addi t0 zero .a #load addr of a into t0
addi t1 zero .b # load addr of b into t1
xor t2 t2 t2 # load i into t2
xor t3 t3 t3 # store offset in t3
addi s0 zero .c # store addr of c into s0

for:
    #######sw zero 200 t3
    add t4 t0 t3 #address for array access (base addr of a + offset)
    lw t5 t4 0 #load a[i]
    add t4 t1 t3 #adddress for array access for b
    lw t6 t4 0 #load b[i]
    add t4 t5 t6 #result for addition of a[i] + b[i]
    add t5 s0 t3 #memory address for c[i]
    sw t5 t4 0 #store result into c[i]
    add t2 t2 1 #i++
    add t3 t3 4 #increase offset by 4 because word size
    blt t2 10 for # if (i < 10) go round loop again

.data:

.a:
    .word 0x

.b:
    

.c:
    .word 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0
