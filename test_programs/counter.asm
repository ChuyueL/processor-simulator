addi t0 zero .a #load addr of a into t0
addi t1 zero 0
addi t8 zero 0x14 #number of loop iterations

for:
addi t2 zero 1 #result for addition of a[i] + b[i]
sw t2 zero 1 #store result into c[i]

sw t2 zero 2 #store result into c[i]

sw t2 zero 3 #store result into c[i]

sw t2 zero 4 #store result into c[i]

sw t2 zero 5 #store result into c[i]

addi t1 t1 5

blt t1 t8 for # if (i < 10) go round loop again
halt



.data:

.a
0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0
