addi t0 zero .a #ld addr of a into t0
addi t1 zero .b #ld
addi t2 zero .gcd
lw t3 t0 0 #ld value of a
lw t4 t1 0 #ld value of b

loop:
blt t3 t4 snd
beq zero zero fst

check:
beq t3 t4 end
beq zero zero loop

fst:
sub t3 t3 t4 #a = a - b
beq zero zero check

snd:
sub t4 t4 t3 #b = b - a
beq zero zero check

end:
sw t2 t3 0 #store value value of a in gcd
halt


.data:

.a
0xF

.b
0x7D

.gcd
0x0