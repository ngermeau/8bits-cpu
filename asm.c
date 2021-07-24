#define _GNU_SOURCE
#include <stdlib.h>
#include <stdbool.h> 
#include <stdio.h>
#include <string.h> 

#define LINE_LENGTH 64
typedef __uint8_t byte;

struct file {
  int nbr_lines;
  char** lines;
};

struct instruction {
  char* name;
  int length;
  int value;
  char* operand_type;
};

//enum operation {load,store,data,jumpr,jump,jumpc,clf,nop,add,shl,shr,not,and,or,xor,cmp};
struct instruction instructions[3] = {
  {"ld",      1, 0x00, "REGS" },
  /* {"st",      2, 0x01, "REGS" }, */
  /* {"data",    2, 0x02, "REG_VAL" } */
};

struct reg {
  char* name;
  int value;
};

struct reg regs[4] = {
  {"r0", 0x00 },
  {"r1", 0x01 },
  {"r2", 0x02 },
  {"r3", 0x03 },
};

struct symbol {
  char* name;
  int address;
};

int symbol_count = 0;
struct symbol symbols[20];

void write_to_file(){
  FILE* fp= fopen("compiled.out", "w"); 
  for (int i =0; i < 10; i++){
    fputc(i, fp );  
  }
  fclose(fp);
}

void display_symbol_table(){
  for (int i = 0; i<20;i++){
    printf("%i value: %s\n",symbols[i].address, symbols[i].name);
  }
}

bool is_symbol(char *line){
  if(strchr(line, ':') != NULL){
    return true;
  }
  return false;
} 

struct instruction find_instruction(char * operation){
   int instruction_size = sizeof(instructions) / sizeof(struct instruction);
   for (int i = 0; i< instruction_size; i++){
      if (!strcmp(instructions[i].name,operation)){
        return instructions[i];
      }
   }
   // bad because if you don't return anything 
}

void add_symbol_to_table(char* line,int address){
  struct symbol* symbol = (struct symbol*) malloc(sizeof(struct symbol));
  symbol->name = line;
  symbol->address = address;
  symbols[symbol_count] = *symbol; 
  symbol_count++; 
}


void create_symbol_table(struct file* file){
  int addr= 0;
  for (int i = 0; i<file->nbr_lines ;i++){
    if (is_symbol(file->lines[i])){
      add_symbol_to_table(file->lines[i],addr);
    }else {
      struct instruction instr= find_instruction(file->lines[i]);
      addr+=instr.length;
    }
  }
}

struct reg find_reg(char* reg){
   int reg_size = sizeof(regs) / sizeof(struct reg);
   for (int i = 0; i< reg_size ; i++){
        printf("try found reg %s: \n",reg);   
      if (!strcmp(regs[i].name,reg)){
        printf("found reg %s: \n",regs[i].name);   
        printf("reg value %i: \n",regs[i].value);   
        return regs[i];
      }
   }
   // bad because if you don't return anything 
}

void compile(struct file *file){
  for (int i = 0; i<file->nbr_lines ;i++){
    if (!is_symbol(file->lines[i])){
      char * operation = strtok(file->lines[i]," ");
      char * operand = strtok(NULL," "); 
      char * op1= strtok(operand,",");
      struct instruction instr = find_instruction(operation);
      if (!strcmp(instr.operand_type,"REGS")){
        char * op2= strtok(NULL,",");
        struct reg rega = find_reg(op1);
        printf("op2 %s: \n",op2);   
        struct reg regb = find_reg(op2);
        byte out = instr.value << 4;
        printf("instr found %i: \n",out);   
        out = out | (rega.value << 2);
        printf("rega found %i: \n",rega.value << 2);   
        out = out | (regb.value);
        printf("regb found %i: \n",regb.value);   
        printf("%s: ",file->lines[i]);   
        printf(" -> %i\n",out);   
      }
    }
  }
}


void display_file(struct file* file){
  for (int i = 0; i<file->nbr_lines ;i++){
    printf("%s",file->lines[i]);
  }
}


int size_of_file(FILE* fileptr){
  int filelen = 0;
 char line[64]; 
  while (fgets(line, sizeof(line), fileptr)) {
      filelen+=1;
  }
  rewind(fileptr);    //bad sideeffect, should save the ptr place before
  return filelen; 
}


struct file* load_file(char* filename){
  FILE* fileptr = fopen(filename, "rb"); 
  int filelen = size_of_file(fileptr);
  
  struct file* file= malloc(sizeof(struct file));
  file->nbr_lines = filelen;

  file->lines = (char**) calloc(filelen,sizeof(char*));
  for (int i = 0; i<filelen;i++){
    file->lines[i] = (char*) calloc(LINE_LENGTH,sizeof(char));
    fgets(file->lines[i], LINE_LENGTH, fileptr); 
    int len = strlen(file->lines[i]);
    if(file->lines[i][len-1] == '\n' )
      file->lines[i][len-1] = 0;
  }

  fclose(fileptr);
  return file;
}

int main(int argc, char *argv[]){
  if (argv[1] == NULL){
    printf("please enter a assembly file\n");
    exit(1);
  }
  struct file* file=  load_file(argv[1]); 
  //display_file(file);
  //create_symbol_table(file);
  compile(file);
  //display_symbol_table();
  //write_to_file();
}
