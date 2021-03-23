#include "lib/catch_amalgamated.hpp"
#include <iostream>
#include "memory.h"
#include "gates.h"

using namespace gates;

namespace memory {

  void updateBit(Bit &bit,bool value, bool set){
    updateNandGate(bit.gate1,value,set);
    updateNandGate(bit.gate2,bit.gate1.value,set);
    updateNandGate(bit.gate4,bit.gate2.value,bit.value);
    updateNandGate(bit.gate3,bit.gate4.value,bit.gate1.value);
    bit.value = {bit.gate3.value};
  }
 
  void updateByte(Byte &byte,bool inputs[8], bool set){
    for (int i=0; i < 8; i++){
      updateBit(byte.value[i],inputs[i],set);
    }
  }
  
  void updateRegistor(Registor &registor,bool inputs[8], bool set){
    updateByte(registor.byte,inputs[8],set);
  }
 
  bool*  enableRegistor(Registor &registor, bool enable){
    for (int i = 0; i< 8; i++){
      updateNandGate(registor.enabler.value[i],registor.byte.value[i].value, enable);
    }
   // print the output of the enabler nandgates
  } 
       
  //}
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
    Byte byte1; 
    bool inputs[8] = {true,false,true,true,false,true,false,false};

    updateByte(byte1,inputs,true);
    for (int i = 0; i< 8; ++i){
      REQUIRE(byte1.value[i].value == inputs[i]); 
    }
    
    Byte byte2;
    updateByte(byte2,inputs,false);
    for (int i = 0; i< 8; ++i){
      REQUIRE(byte2.value[i].value == false); 
    }
}
