#include<iostream>
#include "gates.h"
#include "memory.h"

int main(){
  memory::Bit bit = memory::hola(false,true);
  std::cout << bit.value;
  return 0;
}
