#define _GNU_SOURCE
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define LINE_LENGTH 64
typedef __uint8_t byte;

struct parsed_file
{
  int nbr_lines;
  char **lines;
};

struct compiled_file
{
  int size;
  byte *bytes;
};

struct instruction_ref
{
  char *operation;
  int length;
  byte code;
  char *operand_type;
};

struct instruction_ref instructions_ref_table[] = {
    {"ld", 1, 0x00, "RA_RB"},
    {"st", 1, 0x10, "RA_RB"},
    {"data", 2, 0x20, "RB_VAL"},
    {"jmpr", 2, 0x30, "RB"},
    {"jmp", 2, 0x40, "ADDR"},
    {"jc", 2, 0x58, "ADDR"},
    {"ja", 2, 0x54, "ADDR"},
    {"je", 2, 0x52, "ADDR"},
    {"jz", 2, 0x51, "ADDR"},
    {"jca", 2, 0x5c, "ADDR"},
    {"jce", 2, 0x5a, "ADDR"},
    {"jcz", 2, 0x59, "ADDR"},
    {"jae", 2, 0x56, "ADDR"},
    {"jaz", 2, 0x55, "ADDR"},
    {"jez", 2, 0x53, "ADDR"},
    {"jcae", 2, 0x5e, "ADDR"},
    {"jcaz", 2, 0x5d, "ADDR"},
    {"jcez", 2, 0x5b, "ADDR"},
    {"jaez", 2, 0x57, "ADDR"},
    {"jcaez", 2, 0x5f, "ADDR"},
    {"clf", 2, 0x60, "NO"},
    {"nop", 2, 0x70, "NO"},
    {"add", 2, 0x80, "RA_RB"},
    {"shl", 2, 0x90, "RA_RB"},
    {"shr", 2, 0xa0, "RA_RB"},
    {"not", 2, 0xb0, "RA_RB"},
    {"and", 2, 0xc0, "RA_RB"},
    {"or", 2, 0xd0, "RA_RB"},
    {"xor", 2, 0xe0, "RA_RB"},
    {"cmp", 2, 0xf0, "RA_RB"}};

struct register_ref
{
  char *name;
  byte code;
};

struct register_ref registers_ref_table[] = {
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

struct symbols_table
{
  int size;
  struct symbol *symbols;
};

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

struct parsed_file *parse_file(char *filename)
{
  FILE *fileptr = fopen(filename, "rb");
  int filelen = size_of_file(fileptr);

  struct parsed_file *p_file = malloc(sizeof(struct parsed_file));
  p_file->lines = (char **)calloc(filelen, sizeof(char *));
  p_file->nbr_lines = 0;

  int line_counter = 0;
  for (int i = 0; i < filelen; i++)
  {
    char *line = (char *)calloc(LINE_LENGTH, sizeof(char));
    fgets(line, LINE_LENGTH, fileptr);
    if (!(*line == '\n'))
    {
      p_file->lines[line_counter] = line;
      int len = strlen(p_file->lines[line_counter]);
      if (p_file->lines[line_counter][len - 1] == '\n')
      {
        p_file->lines[line_counter][len - 1] = 0;
      }
      line_counter++;
    }
  }
  p_file->nbr_lines = line_counter;
  fclose(fileptr);
  return p_file;
}

void write_file(const char *o_filename, struct compiled_file *c_file)
{
  FILE *fp = fopen(o_filename, "w");
  for (int i = 0; i < c_file->size; i++)
  {
    fputc(c_file->bytes[i], fp);
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
  int inst_table_size = sizeof(instructions_ref_table) / sizeof(struct instruction_ref);
  for (int i = 0; i < inst_table_size; i++)
  {
    if (!strcmp(instructions_ref_table[i].operation, operation))
    {
      struct instruction_ref instruction_ref = instructions_ref_table[i];
      return instruction_ref;
    }
  } 
}

struct register_ref find_register_ref(char *name)
{
  int reg_table_size = sizeof(registers_ref_table) / sizeof(struct register_ref);
  for (int i = 0; i < reg_table_size; i++)
  {
    if (!strcmp(registers_ref_table[i].name, name))
    {
      struct register_ref register_ref = registers_ref_table[i];
      return register_ref;
    }
  } 
}

struct instruction *from_line_to_instruction(char *line)
{
  char *tempstr = calloc(strlen(line) + 1, sizeof(char));
  strcpy(tempstr, line);
  struct instruction *instruction = malloc(sizeof(struct instruction));
  instruction->operation = strtok(tempstr, " ");
  char *operand = strtok(NULL, " ");
  instruction->operand1 = strtok(operand, ",");
  instruction->operand2 = strtok(NULL, " ");
  return instruction;
}

struct symbols_table *create_symbol_table(struct parsed_file *p_file)
{
  struct symbols_table *sym_table = malloc(sizeof(struct symbols_table));
  sym_table->symbols = malloc(0);
  sym_table->size = 0;
  int current_address = 0;

  for (int i = 0; i < p_file->nbr_lines; i++)
  {
    char *cur_line = p_file->lines[i];
    if (is_symbol(cur_line))
    {
      char *sym_name = malloc(strlen(cur_line));
      sym_name = strcpy(sym_name, cur_line);
      sym_name[strlen(sym_name) - 1] = '\0';

      struct symbol *symbol = malloc(sizeof(struct symbol));
      symbol->name = sym_name;
      symbol->address = current_address;

      struct symbol *symbols = (struct symbol *)realloc(sym_table->symbols, (sym_table->size + 1) * sizeof(struct symbol));
      sym_table->symbols[sym_table->size] = *symbol;
      sym_table->size++;
    }
    else
    {
      struct instruction *instruction = from_line_to_instruction(cur_line);
      struct instruction_ref instruction_ref = find_instruction_ref(instruction->operation);
      current_address += instruction_ref.length;
    }
  }
  return sym_table;
}

void add_to_compiled_file(byte data, struct compiled_file *c_file)
{
  c_file->bytes = (byte *)realloc(c_file->bytes, (c_file->size + 1) * sizeof(byte));
  c_file->bytes[c_file->size] = data;
  c_file->size++;
}

struct compiled_file *compile(struct parsed_file *p_file, struct symbols_table *symbols_table)
{
  struct compiled_file *c_file = malloc(0);
  c_file->bytes = malloc(0);
  c_file->size = 0;

  for (int i = 0; i < p_file->nbr_lines; i++)
  {
    char *current_line = p_file->lines[i];
    if (!is_symbol(current_line))
    {
      struct instruction *instruction = from_line_to_instruction(current_line);
      struct instruction_ref instruction_ref = find_instruction_ref(instruction->operation);
      bool a_data = false;
      byte ins_code, data, ra, rb;

      if (!strcmp(instruction_ref.operand_type, "RA_RB"))
      {
        ra = find_register_ref(instruction->operand1).code;
        rb = find_register_ref(instruction->operand2).code;
        ins_code = instruction_ref.code | (ra << 2) | rb;
      }

      if (!strcmp(instruction_ref.operand_type, "RB"))
      {
        rb = find_register_ref(instruction->operand1).code;
        ins_code = instruction_ref.code | rb;
      }

      if (!strcmp(instruction_ref.operand_type, "RB_VAL"))
      {
        a_data = true;
        rb = find_register_ref(instruction->operand1).code;
        ins_code = instruction_ref.code | rb;
        data = (byte)strtol(instruction->operand2, NULL, 10);
      }

      if (!strcmp(instruction_ref.operand_type, "ADDR"))
      {
        a_data = true;
        ins_code = instruction_ref.code;
        for (int i = 0; i < symbols_table->size; i++)
        {
          if (!strcmp(symbols_table->symbols[i].name, instruction->operand1))
          {
            data = symbols_table->symbols[i].address;
          }
        }
      }

      if (!strcmp(instruction_ref.operand_type, "NO"))
      {
        ins_code = instruction_ref.code;
      }

      add_to_compiled_file(ins_code, c_file);
      if (a_data == true)
      {
        add_to_compiled_file(data, c_file);
      }
    }
  }
  return c_file;
}

char *get_output_file_name(const char *i_filename)
{
  char *o_filename;
  o_filename = malloc(strlen(i_filename) + 5); // +5 in case no .asm
  strcpy(o_filename, i_filename);
  return strcat(strtok(o_filename, "."), ".bin");
}

int main(int argc, char *argv[])
{
  if (argv[1] == NULL)
  {
    printf("please enter a assembly file\n");
    exit(1);
  }

  struct parsed_file *p_file = parse_file(argv[1]);
  struct symbols_table *syms_table = create_symbol_table(p_file);
  struct compiled_file *c_file = compile(p_file, syms_table);
  char *o_filename = get_output_file_name(argv[1]);
  write_file(o_filename, c_file);
  printf("successfully assembled in %s\n", o_filename);
}
