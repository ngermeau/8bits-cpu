#include "lib/catch_amalgamated.hpp"
#include <iostream>
#include "memory.h"
#include "gates.h"

using namespace gates;

namespace memory {

  void updateBit(Bit &bit,bool value, bool set){
    bool gate1 = nandGate(value,set);
    bool gate2 = nandGate(gate1,set);
    bool gate4 = nandGate(gate2,bit.value);
    bool gate3 = nandGate(gate4,gate1);
    bit.value = {gate3};
  }
 
  void updateByte(Byte &byte,bool inputs[8], bool set){
    for (int i=0; i < 8; i++){
      updateBit(byte.value[i],inputs[i],set);
    }
  }
  
  
}

using namespace memory;
TEST_CASE( "Bit", "[memory]" ) {
    Bit falsyBit;
    updateBit(falsyBit,true,false); 
    REQUIRE(falsyBit.value == false);

    updateBit(falsyBit,false,false);
    REQUIRE(falsyBit.value == false);

    updateBit(falsyBit,false,true);
    REQUIRE(falsyBit.value == false);

    updateBit(falsyBit,true,true);
    REQUIRE(falsyBit.value == true);

    Bit truthyBit = { true };
    updateBit(truthyBit,true,false); 
    REQUIRE(truthyBit.value == true);

    updateBit(truthyBit,false,false);
    REQUIRE(truthyBit.value == true);

    updateBit(truthyBit,false,true);
    REQUIRE(truthyBit.value == false);

    updateBit(truthyBit,true,true);
    REQUIRE(truthyBit.value == true);
}

TEST_CASE( "Byte", "[memory]" ) {
    Byte byte; 
    bool inputs[8] = {true,false,true,true,false,true,false,false};
    updateByte(byte,inputs,false);
    REQUIRE(byte.value == false); //comparing array problem (use std::arrays ?)

}
