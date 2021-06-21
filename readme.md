# Minimalistic And Naive 8 Bit CPU Simulator 


# Todo

[ ] exception for not moving acc to register in case of CMP not very clean       
[ ] write a correct readme (supported instructions etc...)    
[ ] create make file 
[ ] add quick expanation for next cycle or exit 
[ ] split in multiple file
[ ] verify not_and_or_xor    
[ ] write your own assembly compiler   
[x] let a program be passed in parameter   
[x] take carry flag as input for add, shr, shl    
[x] shr, shl are not correct they use two operands     
[x] check which operations affect which flags on dia c,a,e,z    
[x] using enum for flag instead of boolean variables    
[x] do I really need byte cf = computer.alu.flags[c] ? 1 : 0;    
[x] be able to execute cycle per cycle with info     
[x] display graphical rendering of memory, regs... (use ncurse lib)    


# Demo programs

- add: add two value 
- counter: increment r1 until value of r2 and then stop
