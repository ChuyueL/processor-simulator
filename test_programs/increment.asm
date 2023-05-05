addi t0 zero .a #load addr of a into t0
addi t1 zero .b # load addr of b into t1
addi t2 zero 0 # load index into t2
addi t8 zero 0x14 #number of loop iterations

for:
ldidx t3 t0 t2 #load a[i]
addi t3 t3 1 #result for addition of a[i] + b[i]
stidx t3 t1 t2 #store result into c[i]
addi t2 t2 1

ldidx t3 t0 t2 #load a[i]
addi t3 t3 1 #result for addition of a[i] + b[i]
stidx t3 t1 t2 #store result into c[i]
addi t2 t2 1

ldidx t3 t0 t2 #load a[i]
addi t3 t3 1 #result for addition of a[i] + b[i]
stidx t3 t1 t2 #store result into c[i]
addi t2 t2 1

ldidx t3 t0 t2 #load a[i]
addi t3 t3 1 #result for addition of a[i] + b[i]
stidx t3 t1 t2 #store result into c[i]
addi t2 t2 1
blt t2 t8 for # if (i < 10) go round loop again
halt



.data:

.a
0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1 0x1

.b
0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0
