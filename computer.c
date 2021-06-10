#include <stdbool.h> 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 

#define MAX_BYTE_VALUE 255

typedef __uint8_t byte;

enum flags {c,a,e,z};

struct Memory  {
    byte mar;
    byte ram[256];
};

struct ALU {
  bool flags[4]; 
};

struct CPU {
  byte regs[4];
  byte iar;
  byte ir;
  byte tmp;
  byte acc;
};

struct Computer {
  struct Memory memory;
  struct CPU cpu;
  struct ALU alu;
  byte bus;
};

struct Computer computer;

int currentStep = 1; 

void alu(byte opcode, byte bus1){
  byte op1 = computer.bus;
  byte op2 = bus1 ? bus1 : computer.cpu.tmp;
  byte cf = computer.alu.flags[c] ? 1 : 0;

  switch (opcode){   
    case 0:  //add
      computer.cpu.acc = op1 + op2 + cf;
      computer.alu.flags[c] = ((op1 + op2 + cf) > MAX_BYTE_VALUE) ? true : false; 
    break;
    case 1:  //shl
      computer.cpu.acc = (op1 << 1) | cf;
      computer.alu.flags[c] = (op1 & 128) ? true : false; 
    break;
    case 2:  //shr
      computer.cpu.acc = (op1 >> 1) | (cf ? 128 : 0); 
      computer.alu.flags[c] = (op1 & 1) ? true : false; 
    break;
    case 3:  //not
      computer.cpu.acc = ~op1;
    break;
    case 4:  //and
      computer.cpu.acc = op1 & op2;
    break;
    case 5:  //or
      computer.cpu.acc = op1 | op2;
    break;
    case 6:  //xor
      computer.cpu.acc = op1 ^ op2;
    break;
    case 7:  //cmp
      computer.alu.flags[a] = (op1 > op2);
      computer.alu.flags[e] = (op1 == op2);
    break;
  }
    computer.alu.flags[z] = (computer.cpu.acc == 0);
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

void load(byte operand1,byte operand2){
  //step 4
  computer.bus = computer.cpu.regs[operand1]; //enable
  computer.memory.mar = computer.bus; //set
  //step 5 
  computer.bus = computer.memory.ram[computer.memory.mar]; //enable
  computer.cpu.regs[operand2] = computer.bus; //set
}

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
  //step4
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

void jumpr(byte operand1){
  //step4 
  computer.bus = computer.cpu.regs[operand1]; //enable 
  computer.cpu.iar = computer.bus;     //set
}

void jump(){
  //step4
  computer.bus = computer.cpu.iar; //enable
  computer.memory.mar = computer.bus; //set

  //step5
  computer.bus = computer.memory.ram[computer.memory.mar];
  computer.cpu.iar = computer.bus;
}

void jumpc(byte operands){
  //step4
  computer.bus = computer.cpu.iar;  //enable 
  computer.memory.mar = computer.bus; //set
  alu(0,1); //increment iar 

  //step5 
  computer.bus = computer.cpu.acc; //enable 
  computer.cpu.iar = computer.bus; //set

  //step6 
  byte res = 0;
  res |=   computer.alu.flags[z] | 
          (computer.alu.flags[e] ? 2 : 0) | 
          (computer.alu.flags[a] ? 4 : 0) | 
          (computer.alu.flags[c] ? 8 : 0);
  
  if (res & operands){ 
    computer.bus = computer.memory.ram[computer.memory.mar];
    computer.cpu.iar = computer.bus;
  }
}

void clear(){
  //step 4
  for (int i = 0; i < sizeof(computer.alu.flags); i++){
    computer.alu.flags[i] = false;
  }
}
 

void execute_instruction(){
  byte operation = computer.cpu.ir >> 4; 
  byte operands = (computer.cpu.ir & 15);
  byte operand1 = (operands & 12) >> 2;
  byte operand2 = (operands & 3);

  if (operation == 0){
    load(operand1,operand2);
  }else if (operation == 1){
    store(operand1,operand2);
  }else if (operation == 2){
    data(operand2);
  }else if (operation == 3){
    jumpr(operand1);
  }else if (operation == 4){
    jump(operand1);
  }else if (operation == 5){
    jumpc(operands);
  }else if (operation == 6){
    clear();
  }else if (operation > 7){
    computer.bus = computer.cpu.regs[operand2];
    computer.cpu.tmp = computer.bus;
    computer.bus = computer.cpu.regs[operand1];
    byte opcode = operation & 7; 
    alu(opcode,0);
    if (opcode != 7){
      //execute move of the acc if not a cmp (not very clean)
      computer.bus = computer.cpu.acc;
      computer.cpu.regs[operand2] = computer.bus; //set: put result into operand2
    }
  }

  printf("instruction %i\n", computer.cpu.ir);
  printf("r0 %i\n",  computer.cpu.regs[0]);
  printf("r1 %i\n",  computer.cpu.regs[1]);
  printf("r2 %i\n",  computer.cpu.regs[2]);
  printf("r3 %i\n",  computer.cpu.regs[3]);
  printf("operation %i\n", operation); 
  printf("opcode %i\n", operation & 7); 
  printf("next iar %i\n", computer.cpu.iar);
  printf("acc %i\n", computer.cpu.acc );
  
}

// the 6 steps taken as a whole is called a instruction cycle
// 3 for fetch and 3 for execution
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

void start(){
  // A 1000 cycle
  for (int i = 0; i < 1000; i++){
    do_step(); // each step is one cycle 
  }
}

void load_file(char * filename){
  FILE *fileptr;
  char *buffer;
  long filelen;

  fileptr = fopen(filename, "rb");  
  fseek(fileptr, 0, SEEK_END);       
  filelen = ftell(fileptr);         
  rewind(fileptr);                 

  fread(computer.memory.ram, filelen, 1, fileptr);
  fclose(fileptr); 
}

void print_ram(){
  for (int i = 0; i< 11; i++){
    printf("computer ram at %i = %i\n",i,computer.memory.ram[i]);  
  }
}

int main(int argc, char *argv[]){
  load_file(argv[1]); 
  print_ram();
  start();
  
}
