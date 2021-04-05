#include<array>
#include "gates.h"

namespace memory { 

  struct Bit {
    bool value = false;
  };
  
  struct Byte {
    Bit bits [8]; 
    bool bools [8]; // dup of bits but for sake of code readibility
  };

  struct Registor {
    Byte byte;
  };

  void setBit(Bit *bit, bool i, bool s);
  void setByte(Byte *byte, bool inputs[8], bool set);

}
