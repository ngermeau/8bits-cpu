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

struct instruction_ref {
  char* operation;
  int length;
  int value;
  char* operand_type;
};

//enum operation {load,store,data,jumpr,jump,jumpc,clf,nop,add,shl,shr,not,and,or,xor,cmp};
struct instruction_ref instructions_ref_table[3] = {
  {"ld",      1, 0x00, "REGS" },
  {"st",      2, 0x01, "REGS" }, 
  {"data",    2, 0x02, "REG_VAL" }
};

struct instruction {
  char* operation;
  char* operand1;
  char* operand2;
};

struct register_ref {
  char* name;
  int value;
};

struct register_ref registers_ref_table[4] = {
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

struct instruction_ref find_instruction_ref(char* operation){
   //printf("test: \n");
   //printf("instructions size: % \n", sizeof(instructions));
   //int instruction_size = sizeof(instructions) / sizeof(struct instruction);
   //:w
      printf("op: %s\n ",operation);
   for (int i = 0; i< 3 ; i++){
      if (!strcmp(instructions_ref_table[i].operation,operation)){
        struct instruction_ref instruction_ref = instructions_ref_table[i];
        printf("instruction found\n");
        printf("inst name: %s\n",instruction_ref.operation);
        printf("inst length: %d\n",instruction_ref.length);
        return instruction_ref;
      }
   }
   // bad because if you don't return anything 
}


void print_symbols_table(){
 for (int i = 0; i< 5; i++){
  printf("%s: %i\n",symbols[i].name, symbols[i].address);
 }
}

struct symbol* from_line_to_symbol(char* line){
  struct symbol* symbol = (struct symbol*) malloc(sizeof(struct symbol));
  symbol->name = line; //remove :
  return symbol;
}

struct instruction* from_line_to_instruction(char* line){
    struct instruction* instruction= (struct instruction*) malloc(sizeof(struct instruction));
    instruction->operation = strtok(line," ");
    char* operand = strtok(NULL," "); 
    instruction->operand1 = strtok(operand," "); 
    instruction->operand2 = strtok(NULL," "); 
    return instruction; 
}

bool is_empty_line(char* line){
 if (*line == '\0'){
  return true;
 }
 return false;
}

void create_symbol_table(struct file* file){
  int current_address= 0;
  for (int i = 0; i<file->nbr_lines ;i++){
    char* current_line = file->lines[i];
    printf("parsing line %s\n",file->lines[i]);

    if (is_empty_line(current_line)){
      printf("empty line found\n");
    } else if (is_symbol(file->lines[i])){
      printf("symbol found\n");
      struct symbol* symbol = from_line_to_symbol(current_line);
      symbol->address = current_address;
      symbols[symbol_count] = *symbol; 
      symbol_count++; 
      printf("symbol added\n");
    }else {
      printf("instruction line \n");
      struct instruction* instruction = from_line_to_instruction(current_line);
      struct instruction_ref instruction_ref = find_instruction_ref(instruction->operation);
      current_address+=instruction_ref.length;
    }
  }
  print_symbols_table();
}



//struct reg find_reg(char* reg){
 //  int reg_size = sizeof(regs) / sizeof(struct reg);
  // for (int i = 0; i< reg_size ; i++){
   //     printf("try found reg %s: \n",reg);   
    //  if (!strcmp(regs[i].name,reg)){
     //   printf("found reg %s: \n",regs[i].name);   
      //  printf("reg value %i: \n",regs[i].value);   
       // return regs[i];
      //}
   //}
   // bad because if you don't return anything 
//}

//void compile(struct file *file){
  //for (int i = 0; i<file->nbr_lines ;i++){
    //if (!is_symbol(file->lines[i])){
      //char* operation = get_instruction_from_line(file->lines[i]);
      //struct instruction instr = find_instruction(operation);
      //if (!strcmp(instr.operand_type,"REGS")){
        ////char * op1= strtok(operand,",");
        //struct reg rega = find_reg(op1);
        //char * op2= strtok(NULL,",");
        //printf("op2 %s: \n",op2);   
        //struct reg regb = find_reg(op2);
        //byte out = instr.value << 4;
        //printf("instr found %i: \n",out);   
        //out = out | (rega.value << 2);
        //printf("rega found %i: \n",rega.value << 2);   
        //out = out | (regb.value);
        //printf("regb found %i: \n",regb.value);   
        //printf("%s: ",file->lines[i]);   
        //printf(" -> %i\n",out);   
      //}
    //}
  //}
//}


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
  // if (argv[1] == NULL){
  //   printf("please enter a assembly file\n");
  //   exit(1);
  // }
  struct file* file = load_file("test_asm.asm"); 
  //display_file(file);
  create_symbol_table(file);
  //compile(file);
  //display_symbol_table();
  //write_to_file();
}
