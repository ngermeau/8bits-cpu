#include <stdlib.h>
#include <stdio.h>

#define LINE_LENGTH 64

struct file {
  int nbr_lines;
  char** lines;
};

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
  rewind(fileptr);    //effet de bord beurk, save ptr debut et la remettre à la fin 
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
  display_file(file);
}
