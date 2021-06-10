#include <stdbool.h> 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 

#define MAX_BYTE_VALUE 255

typedef __uint8_t byte;

enum operation {load,store,data,jumpr,jump,jumpc,clear};
enum opcode {add,shl,shr,not,and,or,xor,cmp};
enum flags {c,a,e,z};

struct Memory  {
    byte mar;
    byte ram[256];
} mem;

struct ALU {
  bool flags[4]; 
} alu;

struct CPU {
  byte regs[4];
  byte iar;
  byte ir;
  byte tmp;
  byte acc;
} cpu;

byte bus;


int currentStep = 1; 

void call_alu(byte opcode, byte bus1){
  byte op1 = bus;
  byte op2 = bus1 ? bus1 : cpu.tmp;
  byte cf = alu.flags[c] ? 1 : 0;

  if (opcode == add) {
      cpu.acc = op1 + op2 + cf;
      alu.flags[c] = ((op1 + op2 + cf) > MAX_BYTE_VALUE) ? true : false; 
  }else if (opcode == shl){
      cpu.acc = (op1 << 1) | cf;
      alu.flags[c] = (op1 & 128) ? true : false; 
  }else if (opcode == shr){
      cpu.acc = (op1 >> 1) | (cf ? 128 : 0); 
      alu.flags[c] = (op1 & 1) ? true : false; 
  }else if (opcode == not){
      cpu.acc = ~op1;
  }else if (opcode == and){
      cpu.acc = op1 & op2;
  }else if (opcode == or){
      cpu.acc = op1 | op2;
  }else if (opcode == xor){
      cpu.acc = op1 ^ op2;
  }else if (opcode == cmp){
      alu.flags[a] = (op1 > op2);
      alu.flags[e] = (op1 == op2);
  }
  alu.flags[z] = (cpu.acc == 0);
}

void step1(){
  bus = cpu.iar;
  call_alu(0,1);
  mem.mar = bus;
}

void step2(){
  bus = mem.ram[bus]; 
  cpu.ir = bus; 
}

void step3(){
  bus = cpu.acc;
  cpu.iar = bus;
}

void step4(){
  byte op = cpu.ir >> 4; 
  byte opr = (cpu.ir & 15);
  byte opr1 = (opr & 12) >> 2;
  byte opr2 = (opr & 3);

  if (op == load || op == store || op == jumpr) {               
    bus = cpu.regs[opr1]; 
    mem.mar = bus; 
  }else if (op == data || jumpc){        
    bus = cpu.iar; 
    call_alu(0,1); 
    mem.mar = bus; 
  }else if (op == jump){      
    bus = cpu.iar; 
    mem.mar = bus; 
  }else if (op == clear){ 
    for (int i = 0; i < 4; i++){ alu.flags[i] = false; }
  }else if (op > 7){
    bus = cpu.regs[opr2];
    cpu.tmp = bus;
  }
}

void step5(){
  byte op = cpu.ir >> 4; 
  byte opr = (cpu.ir & 15);
  byte opr1 = (opr & 12) >> 2;
  byte opr2 = (opr & 3);

  if (op == load) {         
    bus = mem.ram[mem.mar]; 
    cpu.regs[opr2] = bus; 
  }else if (op == store){  
    bus = cpu.regs[opr2]; 
    mem.ram[mem.mar] = bus; 
  }else if (op == data){  
    bus = mem.ram[mem.mar]; 
    cpu.regs[opr1] = bus; 
  }else if (op == jumpr){
    bus = cpu.regs[opr1]; 
    cpu.iar = bus;     
  }else if (op == jump){
    bus = mem.ram[mem.mar];
    cpu.iar = bus;
  }else if (op == jumpc){ 
    bus = cpu.acc; 
    cpu.iar = bus; 
  }else if (op > 7){
    bus = cpu.regs[opr1];
    byte opcode = op & 7; 
    call_alu(opcode,0);
    if (opcode != 7){
      //execute move of the acc if not a cmp (not very clean)
      bus = cpu.acc;
      cpu.regs[opr2] = bus; //set: put result into operand2
    }
  }
}

void step6(){
  byte op = cpu.ir >> 4; 
  byte opr = (cpu.ir & 15);
  byte opr1 = (opr & 12) >> 2;
  byte opr2 = (opr & 3);

  if (op == data){  
    bus = cpu.acc; 
    cpu.iar = bus; 
  }else if (op == jumpc){ 
    byte res = 0;
    res |=   alu.flags[z] | 
            (alu.flags[e] ? 2 : 0) | 
            (alu.flags[a] ? 4 : 0) | 
            (alu.flags[c] ? 8 : 0);
    
    if (res & opr){ 
      bus = mem.ram[mem.mar];
      cpu.iar = bus;
    }
  }else if (op > 7){
    byte opcode = op & 7; 
    if (opcode != 7){
      //execute move of the acc if not a cmp (not very clean)
      bus = cpu.acc;
      cpu.regs[opr2] = bus; //set: put result into operand2
    }

  }
}

void cycle(){
    if (currentStep == 1) { step1(); }
    else if (currentStep == 2){ step2(); }
    else if (currentStep == 3){ step3(); }
    else if (currentStep == 4){ step4(); }
    else if (currentStep == 5){ step5(); }
    else if (currentStep == 6){ step6(); }
    currentStep = (currentStep == 6) ? 1 : currentStep + 1;
}

void start(){
  for (int i = 0; i < 100; i++){ 
    cycle(); 
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

  fread(mem.ram, filelen, 1, fileptr);
  fclose(fileptr); 
}

void print_ram(){
  for (int i = 0; i< 11; i++){
    printf(" ram at %i = %i\n",i,mem.ram[i]);  
  }
}

int main(int argc, char *argv[]){
  load_file(argv[1]); 
  start();
}

