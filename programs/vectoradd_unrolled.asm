addi t0 zero .a #load addr of a into t0
addi t1 zero .b # load addr of b into t1
addi t2 zero 0 # load index into t2
addi t7 zero .c # store addr of c into t7
addi t8 zero 0x14 #number of loop iterations

for:
addi s0 t2 0
addi s1 t2 1
addi s2 t2 2
addi s3 t2 3
addi s4 t2 4

ldidx t3 t0 s0 #load a[i]
ldidx t4 t1 s0 #load b[i]
add t5 t3 t4 #result for addition of a[i] + b[i]
stidx t5 t7 s0 #store result into c[i]

ldidx t3 t0 s1 #load a[i]
ldidx t4 t1 s1 #load b[i]
add t5 t3 t4 #result for addition of a[i] + b[i]
stidx t5 t7 s1 #store result into c[i]

ldidx t3 t0 s2 #load a[i]
ldidx t4 t1 s2 #load b[i]
add t5 t3 t4 #result for addition of a[i] + b[i]
stidx t5 t7 s2 #store result into c[i]

ldidx t3 t0 s3 #load a[i]
ldidx t4 t1 s3 #load b[i]
add t5 t3 t4 #result for addition of a[i] + b[i]
stidx t5 t7 s3 #store result into c[i]

ldidx t3 t0 s4 #load a[i]
ldidx t4 t1 s4 #load b[i]
add t5 t3 t4 #result for addition of a[i] + b[i]
stidx t5 t7 s4 #store result into c[i]

addi t2 t2 5

blt t2 t8 for # if (i < 10) go round loop again
halt



.data:

.a
0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1

.b
0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2 0x2
    

.c
0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 