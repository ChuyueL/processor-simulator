addi t0 zero .a #load addr of a into t0
addi t8 zero 0x14 #number of loop iterations

for:
addi t2 zero 1 #result for addition of a[i] + b[i]

addi t3 zero 1

addi t4 zero 1

addi t5 zero 1

addi t6 zero 1

addi t7 zero 1 #result for addition of a[i] + b[i]

addi t2 zero 1

addi t3 zero 1

addi t4 zero 1

addi t5 zero 1

addi t1 t1 5

blt t1 t8 for # if (i < 10) go round loop again
halt



.data:

.a
0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0