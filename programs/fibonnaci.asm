data r0,0
data r1,1
data r2,100
data r3,55
st r2,r1
add r0,r1
ld r2,r0
cmp r3,r1
ja 8          ;count in r0 until we reach r3
