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

void alu(byte opcode){
  if (opcode == 0){   
    computer.cpu.acc = computer.bus + computer.cpu.bus1;
  }
}

void execute_step1(){
  computer.cpu.bus1 = 1; // enable: put bus1 to 1 and put IAR on the bus
  computer.bus = computer.cpu.iar;
  alu(0); // set ALU compute new IAR (Bus input + bus 1) and put it into ACC and put bus 
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
      currentStep = 0;
    }

    currentStep+=1;
}

// from ram to register, first operand contains the address
void load(operand1,operand2){
  //step 4
  computer.bus = computer.cpu.regs[operand1]; //enable
  computer.memory.mar = computer.bus; //set
  //step 5 
  computer.bus = computer.memory.ram[computer.memory.mar]; //enable
  computer.cpu.regs[operand2] = computer.bus; //set
}

// from register to ram, first operand contains the address
void store(operand1,operand2){
  //step 4 
  computer.bus = computer.cpu.regs[operand1]; //enable
  computer.memory.mar = computer.bus; //set
  //step 5 
  computer.bus = computer.cpu.regs[operand2]; //enable
  computer.cpu.regs[operand2] = computer.bus; //set
}


void execute_instruction(){
  byte operation = computer.cpu.ir >> 4; 
  byte operand1 = (computer.cpu.ir & 12) >> 2;
  byte operand2 = (computer.cpu.ir & 3);
  
  if (operation == 0){
    load(operand1,operand2);
  }else if (operation = 1){
    store();
  }
  
}

void cycle(){
  for (int i = 0; i < 10; i++){
    // each step is one cycle 
    do_step();
    printf("iar %i\n", computer.cpu.iar);
  }
}

int main(){
  enum day{mon,tue,wed}today;
  today = mon;          
  printf("%i /n",today);
  
  
  


//cycle();
}


















