# Minimalistic And Naive 8 Bit CPU Simulator And Assembler


# Todo

- [ ] change asm program to include symbol
- [ ] refactor for more clarity of code 
- [ ] asm output filename = entry output filename 
- [ ] remove global var  
- [x] remove empty lines check everywhere and put it during loading of file
- [ ] seriously loading file sucks, better way of doing this 
- [ ] use typedef for struct 
- [x] write your own assembly compiler   
- [x] let a program be passed in parameter   
- [x] add quick expanation for next cycle or exit 
- [x] take carry flag as input for add, shr, shl    
- [x] shr, shl are not correct they use two operands     
- [x] check which operations affect which flags on dia c,a,e,z    
- [x] using enum for flag instead of boolean variables    
- [x] do I really need byte cf = computer.alu.flags[c] ? 1 : 0;    
- [x] be able to execute cycle per cycle with info     
- [x] display graphical rendering of memory, regs... (use ncurse lib)    
- [x] exception for not moving acc to register in case of CMP not very clean       
- [x] verify not_and_or_xor    
- [x] create make file 
- [x] write fibonnaci   
- [ ] write a correct readme (supported instructions etc...)    


# Demo programs

