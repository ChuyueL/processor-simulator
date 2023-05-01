addi t0 zero .a #load addr of a into t0
addi t1 zero .b # load addr of b into t1
addi t2 zero 0 # load index into t2
addi t7 zero .c # store addr of c into t7
addi t8 zero 0xA #number of loop iterations

for:
ldidx t3 t0 t2 #load a[i]
ldidx t4 t1 t2 #load b[i]
add t5 t3 t4 #result for addition of a[i] + b[i]
stidx t5 t7 t2 #store result into c[i]
addi t2 t2 1

ldidx t3 t0 t2 #load a[i]
ldidx t4 t1 t2 #load b[i]
add t5 t3 t4 #result for addition of a[i] + b[i]
stidx t5 t7 t2 #store result into c[i]
addi t2 t2 1

ldidx t3 t0 t2 #load a[i]
ldidx t4 t1 t2 #load b[i]
add t5 t3 t4 #result for addition of a[i] + b[i]
stidx t5 t7 t2 #store result into c[i]
addi t2 t2 1

ldidx t3 t0 t2 #load a[i]
ldidx t4 t1 t2 #load b[i]
add t5 t3 t4 #result for addition of a[i] + b[i]
stidx t5 t7 t2 #store result into c[i]
addi t2 t2 1

ldidx t3 t0 t2 #load a[i]
ldidx t4 t1 t2 #load b[i]
add t5 t3 t4 #result for addition of a[i] + b[i]
stidx t5 t7 t2 #store result into c[i]
addi t2 t2 1

blt t2 t8 for # if (i < 10) go round loop again
halt



.data:

.a
0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1

.b
0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2
    

.c
0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 