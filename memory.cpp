#include <iostream>
#include "memory.h"
#include "gates.h"

namespace memory {

  Bit hola(bool i, bool s){
    Bit new_bit = {
      gates::nandGate(i,s),
    };
    return new_bit;
  }
}
