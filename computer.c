#include <stdbool.h> 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 

#define MAX_BYTE_VALUE 255

typedef __uint8_t byte;

struct Memory  
{
    byte mar;
    byte ram[256];
};

struct ALU {
  bool c;
  bool a;
  bool e;
  bool z;
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


//remove bus1 as state and pass it along  
void alu(byte opcode, byte bus1){
  byte op1 = computer.bus;
  byte op2 = bus1 ? bus1 : computer.cpu.tmp;
  switch (opcode){   
    case 0:  //add
      printf("op1 : %i\n",op1);
      printf("op2 : %i\n",op2);
      computer.cpu.acc = op1 + op2;
      computer.alu.c = ((op1 + op2) > MAX_BYTE_VALUE) ? true : false; 
      computer.alu.z = (computer.cpu.acc == 0);
    break;
    case 1:  //shl
      computer.cpu.acc = (op1 << 1);
      computer.alu.c = (op1 & 128) ? true : false; 
      computer.alu.z = (computer.cpu.acc == 0);
    break;
    case 2:  //shr
      computer.cpu.acc = (op1 >> 1);
      computer.alu.c = (op1 & 1) ? true : false; 
      computer.alu.z = (computer.cpu.acc == 0);
    break;
    case 3:  //not
      computer.cpu.acc = ~op1;
      computer.alu.z = (computer.cpu.acc == 0);
    break;
    case 4:  //and
      computer.cpu.acc = op1 & op2;
      computer.alu.z = (computer.cpu.acc == 0);
    break;
    case 5:  //or
      computer.cpu.acc = op1 | op2;
      computer.alu.z = (computer.cpu.acc == 0);
    break;
    case 6:  //xor
      computer.cpu.acc = op1 ^ op2;
      computer.alu.z = (computer.cpu.acc == 0);
    break;
    case 7:  //cmp
      computer.alu.a = (op1 > op2);
      computer.alu.e = (op1 == op2);
    break;
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
  if (computer.alu.z){ res+=1; }
  if (computer.alu.e){ res+=2; }
  if (computer.alu.a){ res+=4; }
  if (computer.alu.c){ res+=8; }

  if (res & operands){
    computer.bus = computer.memory.ram[computer.memory.mar];
    computer.cpu.iar = computer.bus;
  }
}

void clear(){
  //step 4
  computer.alu.c = false;
  computer.alu.a = false;
  computer.alu.e = false;
  computer.alu.z = false;
}

void execute_instruction(){
  byte operation = computer.cpu.ir >> 4; 
  byte operands = (computer.cpu.ir & 15);
  byte operand1 = (operands & 12) >> 2;
  byte operand2 = (operands & 3);

  printf("\n\n");
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

  printf("address %i\n", computer.cpu.iar - 1 );
  printf("instruction %i\n", computer.cpu.ir);
  printf("r0 %i\n",  computer.cpu.regs[0]);
  printf("r1 %i\n",  computer.cpu.regs[1]);
  printf("r2 %i\n",  computer.cpu.regs[2]);
  printf("operation %i\n", operation); 
  printf("opcode %i\n", operation & 7); 
  printf("next iar %i\n", computer.cpu.iar);
  printf("acc %i\n", computer.cpu.acc );
  
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
  for (int i = 0; i < 1000; i++){
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
// data R1,    = 33
// data R2,    = 34
// data R3,    = 35

// add R0,R1   = 129
// shr R0,R3   = 163 

long int fileSize(FILE * fp)
{
    fseek(fp, 0L, SEEK_END);
    long int res = ftell(fp);
    return res;
}

void load_file(char * filename){
  FILE *fileptr;
  char *buffer;
  long filelen;

  fileptr = fopen(filename, "rb");  
  printf("file: %p", fileptr); 
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

int main(){
  /* computer.memory.ram[0] = 32; */
  /* computer.memory.ram[1] = 32; //value 32 to R0 */
  /* computer.memory.ram[2] = 33; */
  /* computer.memory.ram[3] = 10; //value 10 to R1 */
  /* computer.memory.ram[4] = 163; */
  
  print_ram();
  load_file("counter.bin");
  printf("after\n");
  print_ram();
  cycle();
  printf("value of r0\n");
  computer.cpu.regs[0];
}



















/* computer ram at 0 = 32 */ 
/* computer ram at 1 = 1   r0 doit contenir 1 ok */
/* computer ram at 2 = 33 */
/* computer ram at 3 = 1   r1 doit contenir 1 ok */   
/* computer ram at 4 = 34 */
/* computer ram at 5 = 100 r2 doit contenir 100 ok */
/* computer ram at 6 = 129 add */ 
/* computer ram at 7 = 96 clf */
/* computer ram at 8 = 249 cmp */
/* computer ram at 9 = 84 ja */
/* computer ram at 10 = 0 */
