#include <stdbool.h> 
#include <stdio.h>
#include <unistd.h>


typedef __uint8_t byte;

struct Memory  
{
    bool memoryAddressRegister[8];
    bool ram[16][16][8];
};

struct CPU {
  byte instructionAddressRegister;
  byte instructionRegister;
  byte bus1[8];
  byte tmp[8];
  byte acc[8];
};

struct Computer {
  struct Memory memory;
  struct CPU cpu;
  bool bus[8];
};


int currentStep = 1; 

struct Computer computer;


void executeAlu(opcode){
  __uint16_t test; 
  uint8_t bla;
  test =3;
  /* if (opcode == 0){   //add */
  /*   computer.bus */ 
  /* } */
}


void execute_step1(){
  
  printf("step 1, put IAR into MAR and increment IAR of one (result in ACC) \n");
  // enable    //put bus1 to 1 and put IAR on the bus
  computer.cpu.bus1[7] = true; 
  memcpy(computer.bus,computer.cpu.instructionAddressRegister,sizeof(computer.bus));
  
  // ask ALU to compute (Bus input + bus 1)
  // set 
  // put MAR from bus (IAR value) 
  // put value of ALU to ACC
  executeAlu(opcode);
  memcpy(computer.memory.memoryAddressRegister,computer.bus,sizeof(computer.bus));
  memcpy(computer.cpu.acc, alu.value ,sizeof(computer.bus));
}



void execute_step2(){
  printf("step 2, put instruction from memory into IR \n");     
  computer.bus.value = fetchInstruction(computer.memory.memoryAddressRegister)
  computer.cpu.instructionRegister = computer.bus.value;
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







