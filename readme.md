# Minimalistic And Naive 8 Bit CPU Simulator 


# Todo

[x] let a program be passed in parameter   
[x] take carry flag as input for add, shr, shl    
[x] shr, shl are not correct they use two operands 
[x] does other operations than CMP also affect the flags c,a,e,z ?  nope, check the dia
[ ] using enum for flag instead of boolean variables    
[ ] use hexadecimal instead of decimal
[ ] put declaration in h files     
[ ] exception for not moving acc to register in case of CMP not very clean   
[ ] computer (cycles) stop after program finish executing   
[ ] display graphical rendering of memory, regs... (use ncurse lib) 
[ ] write a correct readme (supported instructions etc...)    
[ ] write your own assembly compiler   
[ ] take care of peripherals     


# Demo programs

- add: add two value 
- counter: increment r1 until value of r2 and then stop
