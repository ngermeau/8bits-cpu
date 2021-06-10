# Minimalistic And Naive 8 Bit CPU Simulator 


# Todo

[ ] exception for not moving acc to register in case of CMP not very clean     
[ ] computer (cycles) stop after program finish executing   
[ ] display graphical rendering of memory, regs... (use ncurse lib) 
[ ] write a correct readme (supported instructions etc...)    
[ ] write your own assembly compiler   
[ ] take care of peripherals     
[x] let a program be passed in parameter   
[x] take carry flag as input for add, shr, shl    
[x] shr, shl are not correct they use two operands 
[x] check which operations affect which flags on dia c,a,e,z 
[x] using enum for flag instead of boolean variables    
[x] do I really need byte cf = computer.alu.flags[c] ? 1 : 0;


# Demo programs

- add: add two value 
- counter: increment r1 until value of r2 and then stop
