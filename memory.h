namespace memory { 

  struct Bit {
    bool value = false;
  };
  
  struct Byte {
   Bit value[8]; 
  };

  Bit updateBit(bool i, bool s);
}
