#include <stdbool.h> 
#include <stdio.h>
#include <unistd.h>

typedef __uint8_t byte;

struct Memory  
{
    byte mar;
    byte ram[256];
};

struct CPU {
  byte regs[4];
  byte iar;
  byte ir;
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

//remove bus1 as state and pass it along  
void alu(byte opcode, byte bus1){
  byte op1 = computer.bus;
  byte op2 = bus1 ? bus1 : computer.cpu.tmp;

  if (opcode == 0){   
    computer.cpu.acc = op1 + op2;
    // carry flag and such ? 
  }
}

void execute_step1(){
  computer.bus = computer.cpu.iar;
  alu(0,1); // set ALU compute new IAR (Bus input + bus 1) and put it into ACC and put bus 
  computer.memory.mar = computer.bus;
}

void execute_step2(){
  computer.bus = computer.memory.ram[computer.bus]; //enable: put ram content at addr in bus on bus
  computer.cpu.ir = computer.bus; //set: put bus into instruction register
}

void execute_step3(){
  computer.bus = computer.cpu.acc; //enable:: put acc in bus
  computer.cpu.iar = computer.bus; //set: put bus into iar
}

// from ram to register, first operand contains the address
void load(byte operand1,byte operand2){
  //step 4
  computer.bus = computer.cpu.regs[operand1]; //enable
  computer.memory.mar = computer.bus; //set
  //step 5 
  computer.bus = computer.memory.ram[computer.memory.mar]; //enable
  computer.cpu.regs[operand2] = computer.bus; //set
}

// from register to ram, first operand contains the address
void store(byte operand1,byte operand2){
  //step 4 
  printf("operand %i",computer.cpu.regs[operand1]);
  computer.bus = computer.cpu.regs[operand1]; //enable
  computer.memory.mar = computer.bus; //set
  //step 5 
  computer.bus = computer.cpu.regs[operand2]; //enable
  computer.memory.ram[computer.memory.mar] = computer.bus; //enable
}

void data(byte operand1){
  //step 4
  computer.bus = computer.cpu.iar; //enable
  alu(0,1); //increment iar
  computer.memory.mar = computer.bus; //set

  //step5
  computer.bus = computer.memory.ram[computer.memory.mar]; //enable
  computer.cpu.regs[operand1] = computer.bus; //set
  
  //step6
  computer.bus = computer.cpu.acc; //enable:: put acc in bus
  computer.cpu.iar = computer.bus; //set: put bus into iar
}

void execute_instruction(){
  byte operation = computer.cpu.ir >> 4; 
  byte operand1 = (computer.cpu.ir & 12) >> 2;
  byte operand2 = (computer.cpu.ir & 3);

  printf("\n\n");
  if (operation == 0){
    load(operand1,operand2);
  }else if (operation == 1){
    store(operand1,operand2);
  }else if (operation == 2){
    data(operand2);
  }
  printf("instruction %i\n", computer.cpu.iar - 1 );
  printf("r0 %i\n",  computer.cpu.regs[0]);
  printf("r1 %i\n",  computer.cpu.regs[1]);
  printf("execution op %i\n", operation); 
  printf("next iar %i\n", computer.cpu.iar);
  
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
    }
    if (currentStep == 4){
      execute_instruction();
    }
    if (currentStep == 5){
      currentStep = 0;
    }
    
    currentStep+=1;
}



void cycle(){
  for (int i = 0; i < 15; i++){
    // each step is one cycle 
    do_step();
  }
}



// from ram to register, first operand contains the address
// load R0,R0 = 0
// load R0,R1 = 1 
// load R0,R2 = 2 
// load R0,R3 = 3 
//
// from register to ram, first operand contains the address
// store R0,R0 = 16 
// store R0,R1 = 17 
// store R0,R2 = 18 
// store R0,R3 = 19 
//
// data R0,    = 32
// data R0,    = 33
// data R0,    = 34
// data R0,    = 35


int main(){
  computer.memory.ram[0] = 32;
  computer.memory.ram[1] = 254; //data 254 to R0
  computer.memory.ram[2] = 1; //load data at R0 address into R1;
  computer.memory.ram[3] = 20;  //store R1, R0 
  //data
  computer.memory.ram[254] = 32; 
  cycle();
  printf("ram at 32 %i\n", computer.memory.ram[32]);
}


















