#include <array>
#include "gates.h"

namespace memory
{

  struct Bit
  {
    bool value = false;
  };

  struct Byte
  {
    Bit bits[8];
    bool value[8] = {false, false, false, false, false, false, false, false}; // dup of bits but for sake of readibility
  };

  struct Reg
  {
    Byte byte;
  };

  struct Ram
  {
    Reg regs[16][16]; //todo; would probably be better if ram contains the MAR and a pointer to the bus +e +s
  };

  struct Bus
  {
    bool value[8];
  };

  void setBit(Bit *bit, bool i, bool s);
  void setByte(Byte *byte, bool inputs[8], bool set);
  bool *enabl(bool inputs[8], bool enable);
  void setReg(Reg *reg, bool inputs[8], bool set);
  bool *enablReg(Reg *reg, bool enable);

}
