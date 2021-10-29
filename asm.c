#define _GNU_SOURCE
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define LINE_LENGTH 64
typedef __uint8_t byte;

struct file
{
  int nbr_lines;
  char **lines;
};

struct instruction_ref
{
  char *operation;
  int length;
  int value;
  char *operand_type;
};

//enum operation {load,store,data,jumpr,jump,jumpc,clf,nop,add,shl,shr,not,and,or,xor,cmp};
struct instruction_ref instructions_ref_table[] = {
    {"ld",    1, 0x00, "RA_RB"},    
    {"st",    1, 0x10, "RA_RB"},    
    {"data",  2, 0x20, "RB_VAL"},   
    {"jmpr",  2, 0x30, "RB"},       
    {"jmp",   2, 0x40, "ADDR"},     
    {"jc",    2, 0x58, "ADDR"},  
    {"ja",    2, 0x54, "ADDR"}, 
    {"je",    2, 0x52, "ADDR"},
    {"jz",    2, 0x51, "ADDR"},
    {"jca",   2, 0x5c, "ADDR"},  
    {"jce",   2, 0x5a, "ADDR"}, 
    {"jcz",   2, 0x59, "ADDR"},
    {"jae",   2, 0x56, "ADDR"},  
    {"jaz",   2, 0x55, "ADDR"},  
    {"jez",   2, 0x53, "ADDR"},  
    {"jcae",  2, 0x5e, "ADDR"}, 
    {"jcaz",  2, 0x5d, "ADDR"},  
    {"jcez",  2, 0x5b, "ADDR"},  
    {"jaez",  2, 0x57, "ADDR"},  
    {"jcaez", 2, 0x5f, "ADDR"},  
    {"clf",   2, 0x60, "NO"},
    {"nop",   2, 0x70, "NO"},
    {"add",   2, 0x80, "RA_RB"},
    {"shl",   2, 0x90, "RA_RB"},
    {"shr",   2, 0xa0, "RA_RB"},
    {"not",   2, 0xb0, "RA_RB"},
    {"and",   2, 0xc0, "RA_RB"},
    {"or",    2, 0xd0, "RA_RB"},
    {"xor",   2, 0xe0, "RA_RB"},
    {"cmp",   2, 0xf0, "RA_RB"}};

struct register_ref
{
  char *name;
  int value;
};

struct register_ref registers_ref_table[4] = {
    {"r0", 0x00},
    {"r1", 0x01},
    {"r2", 0x02},
    {"r3", 0x03},
};

struct instruction
{
  char *operation;
  char *operand1;
  char *operand2;
};

struct symbol
{
  char *name;
  int address;
};

struct symbol *symbols_table;
int symbol_table_size = 0;

void write_to_file()
{
  FILE *fp = fopen("compiled.out", "w");
  for (int i = 0; i < 10; i++)
  {
    fputc(i, fp);
  }
  fclose(fp);
}

bool is_symbol(char *line)
{
  if (strchr(line, ':') != NULL)
  {
    return true;
  }
  return false;
}

struct instruction_ref find_instruction_ref(char *operation)
{
  for (int i = 0; i < 3; i++)
  {
    if (!strcmp(instructions_ref_table[i].operation, operation))
    {
      struct instruction_ref instruction_ref = instructions_ref_table[i];
      return instruction_ref;
    }
  } // bad because if you don't return anything
}

void print_symbols_table()
{
  printf("symbol table\n");
  for (int i = 0; i < symbol_table_size; i++)
  {
    printf("%s: %d\n", ((struct symbol)symbols_table[i]).name, ((struct symbol)symbols_table[i]).address);
  }
}

void add_to_symbols_table(char *line, int current_address)
{
  struct symbol *symbol = (struct symbol *)malloc(sizeof(struct symbol));
  symbol->name = line; //remove :
  symbol->address = current_address;
  symbols_table = (struct symbol *)realloc(symbols_table, (symbol_table_size + 1) * sizeof(struct symbol));
  symbols_table[symbol_table_size] = *symbol;
  symbol_table_size++;
}

struct instruction *from_line_to_instruction(char *line)
{
  struct instruction *instruction = (struct instruction *)malloc(sizeof(struct instruction));
  instruction->operation = strtok(line, " ");
  char *operand = strtok(NULL, " ");
  instruction->operand1 = strtok(operand, " ");
  instruction->operand2 = strtok(NULL, " ");
  return instruction;
}

bool is_empty_line(char *line)
{
  if (*line == '\0')
  {
    return true;
  }
  return false;
}

void create_symbol_table(struct file *file)
{
  int current_address = 0;
  for (int i = 0; i < file->nbr_lines; i++)
  {
    char *current_line = file->lines[i];
    printf("%s", file->lines[i]);
    if (!is_empty_line(current_line))
    {
      if (is_symbol(current_line))
      {
        printf(" : symbol line\n");
        add_to_symbols_table(current_line, current_address);
      }
      else
      {
        printf(": instruction line \n");
        struct instruction *instruction = from_line_to_instruction(current_line);
        struct instruction_ref instruction_ref = find_instruction_ref(instruction->operation);
        current_address += instruction_ref.length;
      }
    }
  }
  print_symbols_table();
}

void compile(struct file *file)
{
  int current_address = 0;
  for (int i = 0; i < file->nbr_lines; i++)
  {
    char *current_line = file->lines[i];
    if (!is_empty_line(current_line) && !is_symbol(current_line))
    {
      struct instruction *instruction = from_line_to_instruction(current_line);
      struct instruction_ref instruction_ref = find_instruction_ref(instruction->operation);
      if (!strcmp(instruct_ref.operand_type, "REGS")){

      }
    }
  }
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

void compile(struct file *file)
{
  for (int i = 0; i < file->nbr_lines; i++)
  {
    if (!is_symbol(file->lines[i]))
    {
      char *operation = get_instruction_from_line(file->lines[i]);
      struct instruction instr = find_instruction(operation);
      if (!strcmp(instr.operand_type, "REGS"))
      {
        //char * op1= strtok(operand,",");
        struct reg rega = find_reg(op1);
        char *op2 = strtok(NULL, ",");
        printf("op2 %s: \n", op2);
        struct reg regb = find_reg(op2);
        byte out = instr.value << 4;
        printf("instr found %i: \n", out);
        out = out | (rega.value << 2);
        printf("rega found %i: \n", rega.value << 2);
        out = out | (regb.value);
        printf("regb found %i: \n", regb.value);
        printf("%s: ", file->lines[i]);
        printf(" -> %i\n", out);
      }
    }
  }
}

void display_file(struct file *file)
{
  for (int i = 0; i < file->nbr_lines; i++)
  {
    printf("%s", file->lines[i]);
  }
}

int size_of_file(FILE *fileptr)
{
  int filelen = 0;
  char line[64];
  while (fgets(line, sizeof(line), fileptr))
  {
    filelen += 1;
  }
  rewind(fileptr); //bad sideeffect, should save the ptr place before
  return filelen;
}

struct file *load_file(char *filename)
{
  FILE *fileptr = fopen(filename, "rb");
  int filelen = size_of_file(fileptr);

  struct file *file = malloc(sizeof(struct file));
  file->nbr_lines = filelen;

  file->lines = (char **)calloc(filelen, sizeof(char *));
  for (int i = 0; i < filelen; i++)
  {
    file->lines[i] = (char *)calloc(LINE_LENGTH, sizeof(char));
    fgets(file->lines[i], LINE_LENGTH, fileptr);
    int len = strlen(file->lines[i]);
    if (file->lines[i][len - 1] == '\n')
      file->lines[i][len - 1] = 0;
  }

  fclose(fileptr);
  return file;
}

int main(int argc, char *argv[])
{
  symbols_table = (struct symbol *)malloc(0);
  // if (argv[1] == NULL){
  //   printf("please enter a assembly file\n");
  //   exit(1);
  // }
  struct file *file = load_file("test_asm.asm");
  //display_file(file);
  create_symbol_table(file);
  //compile(file);
  //display_symbol_table();
  //write_to_file();
}
