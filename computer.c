#include <stdbool.h> 
#include <stdio.h>
#include <unistd.h>

typedef __uint8_t byte;

struct Memory  
{
    byte mar;
    byte ram[16][16];
};

struct CPU {
  byte iar;
  byte bus1;
  byte tmp;
  byte acc;
};

struct Computer {
  struct Memory memory;
  struct CPU cpu;
  byte bus;
};


int currentStep = 1; 

struct Computer computer;

void alu(byte opcode){
  if (opcode == 0){   
    computer.cpu.acc = computer.bus + computer.cpu.bus1;
  }
}

void execute_step1(){
  
  // enable    
  // put bus1 to 1 and put IAR on the bus
  computer.cpu.bus1 = 1; 
  computer.bus = computer.cpu.iar;
  
  // set 
  // ALU compute new IAR (Bus input + bus 1) and put it into ACC
  // put bus (IAR value) into MAR 
  alu(0);
  computer.memory.mar = computer.bus;
}



void execute_step2(){
  printf("step 2, put instruction from memory into IR \n");     
  /* computer.bus.value = fetchInstruction(computer.memory.memoryAddressRegister) */
  /* computer.cpu.instructionRegister = computer.bus.value; */
}

void execute_step3(){
  printf("step 3, put content of ACC to IAR\n");     
}

// the 6 steps taken as a whole is called a instruction cycle 
// first 3 steps are fetch 
// 4,5,6 is decode and execute 
void do_step(){
    if (currentStep == 1){
      execute_step1();
    }
    if (currentStep == 2){
      execute_step2();
    }
    if (currentStep == 3){
      execute_step3();
      currentStep = 0;
    }

    currentStep+=1;
}

void cycle(){
  for (int i = 0; i < 10; i++){
    // each step is one cycle 
    do_step();
    printf("iar %i", computer.cpu.iar);
  }
}

int main(){
cycle();
}









//1 read bin file 
//2 create new computer 
//3 load program into memory
//5 run computer 
  //5.1 put addressRegister to beginning of memory 
  //tick cpu (one tick up and one tick down)







