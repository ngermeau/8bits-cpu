data r0,0
data r1,1
data r2,100
data r3,55

loop:
st r2,r1
add r0,r1
ld r2,r0
cmp r3,r1
ja loop         
