#include<array>
#include "gates.h"

using std::array;
using gates::AndGate;
using gates::NandGate;

namespace memory { 

  struct Bit {
    array<NandGate,4> gates;
    bool value = false;
  };
  
  struct Byte {
    array<Bit, 8> value; 
  };

  struct Registor {
    Byte byte;
  };

  void setBit(Bit &bit, bool i, bool s);

}
