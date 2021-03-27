#include "lib/catch_amalgamated.hpp"
#include <iostream>
#include "memory.h"
#include "gates.h"

using namespace gates;

namespace memory {

  void setBit(Bit &bit,bool value, bool set){
    setNandGate(bit.gates[0],value,set);
    setNandGate(bit.gates[1],bit.gates[0].value,set);
    setNandGate(bit.gates[3],bit.gates[1].value,bit.value);
    setNandGate(bit.gates[2],bit.gates[3].value,bit.gates[0].value);
    bit.value = {bit.gates[2].value};
  }
 
  void setByte(Byte &byte,array<bool,8> inputs, bool set){
    for (int i=0; i < 8; i++){
      setBit(byte.value[i],inputs[i],set);
    }
  }
  
  array<bool,8> byteToBool(Byte byte){
    array<bool,8> values;
    for (int i=0; i < 8; i++){
      values[i] = byte.value[i].value; 
    }
    return values;
  }
  
  array<bool,8> enabler(array<bool,8> inputs, bool enable){
    array<AndGate, 8> enabler;

    for (int i=0; i < 8; i++){
      setAndGate(enabler[i],inputs[i], enable);
    }

    array<bool,8> values;
    for (int i=0; i < 8; i++){
      values[i] = enabler[i].value;
    }
    return values;
  }
  
  void setRegistor(Registor &registor,array<bool,8> inputs, bool set){
    setByte(registor.byte,inputs,set);
  }

  array<bool,8> enableRegistor(Registor registor, bool enable){
    return enabler(byteToBool(registor.byte),enable);
  }
}

using namespace memory;
TEST_CASE( "Bit", "[memory]" ) {
    Bit falsyBit;
    setBit(falsyBit,true,false); 
    REQUIRE(falsyBit.value == false);

    setBit(falsyBit,false,false);
    REQUIRE(falsyBit.value == false);

    setBit(falsyBit,false,true);
    REQUIRE(falsyBit.value == false);

    setBit(falsyBit,true,true);
    REQUIRE(falsyBit.value == true);

    Bit truthyBit;
    truthyBit.value = true;
    setBit(truthyBit,true,false); 
    REQUIRE(truthyBit.value == true);

    setBit(truthyBit,false,false);
    REQUIRE(truthyBit.value == true);

    setBit(truthyBit,false,true);
    REQUIRE(truthyBit.value == false);

    setBit(truthyBit,true,true);
    REQUIRE(truthyBit.value == true);
}

TEST_CASE( "Byte", "[memory]" ) {
    Byte byte;
    array<bool,8> inputs = {true,false,true,true,false,true,false,false};

    setByte(byte,inputs,true);
    for (int i = 0; i< 8; ++i){
      REQUIRE(byte.value[i].value == inputs[i]); 
    }
    
    Byte byte2;
    setByte(byte2,inputs,false);
    for (int i = 0; i< 8; ++i){
      REQUIRE(byte2.value[i].value == false); 
    }
}

TEST_CASE( "Registor", "[memory]") {
    Registor registor;
    array<bool,8> inputs = {true,false,true,true,false,true,false,false};

    setRegistor(registor, inputs, true);
    for (int i = 0; i< 8; ++i){
      REQUIRE(registor.byte.value[i].value == inputs[i]); 
    }

    Registor registor2;
    setRegistor(registor2, inputs, false);
    for (int i = 0; i< 8; ++i){
      REQUIRE(registor2.byte.value[i].value == false); 
    }

    array<bool,8> disabledRegistor = {false,false,false,false,false,false,false,false};
    array<bool,8> registorValue = enableRegistor(registor, false);
    REQUIRE(registorValue == disabledRegistor); 

    registorValue = enableRegistor(registor, true);
    REQUIRE(registorValue == inputs); 
}

