#include<array>
#include "gates.h"

using std::array;
using gates::AndGate;
using gates::NandGate;

namespace memory { 

  struct Bit {
    AndGate gate1,gate2,gate3,gate4; 
    bool value = false;
  };
  
  struct Byte {
    array<Bit, 8>  value; 
  };

  struct Enabler { 
    array<AndGate, 8> value;
  };

  struct Registor {
    Byte byte;
    Enabler enabler;
  };

  Bit updateBit(bool i, bool s);
}
