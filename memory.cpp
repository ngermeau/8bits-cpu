#include "lib/catch_amalgamated.hpp"
#include <iostream>
#include "memory.h"
#include "gates.h"

using namespace gates;

namespace memory {

  void setBit(Bit *bit,bool value, bool set){
    bool gate0 = nandGate(value,set);
    bool gate1 = nandGate(gate0,set);
    bool gate3 = nandGate(gate1,(*bit).value);
    bool gate2 = nandGate(gate3,gate0);
    (*bit).value = gate2;
  }
 
  void setByte(Byte *byte, bool inputs[8],bool set){
    for (int i=0; i < 8; i++){
      setBit(&(*byte).bits[i],inputs[i],set);
      (*byte).value[i] = inputs[i];
    }
  }
  
  void setReg(Reg *reg, bool inputs[8],bool set){
    setByte(&(*reg).byte,inputs,set);
  }

  bool* enabl(bool inputs[8],bool enable){
    bool* outputs = (bool*) malloc(8);
    for (int i=0; i < 8; i++){
      outputs[i] = andGate(inputs[i], enable);
    }
    return outputs;
  }
  
  bool*  enablReg(Reg reg, bool enable){
    return enabl(reg.byte.value,enable);
  }

  bool* decoder2x4 (bool a, bool b){
    bool notGate1 = notGate(a);
    bool notGate2 = notGate(b);

    bool* outputs = (bool*) malloc(4);
    outputs[0] = andGate(notGate1, notGate2);
    outputs[1] = andGate(notGate1,b);
    outputs[2] = andGate(a,notGate2);
    outputs[3] = andGate(a,b);
    return outputs;
  }
  

      //https://www.elprocus.com/designing-4-to-16-decoder-using-3-to-8-decoder/
}

using namespace memory;
TEST_CASE( "Bit", "[memory]" ) {

    // Update bit when initial state is false 
    Bit falsyBit;
    setBit(&falsyBit,true,false); 
    REQUIRE(falsyBit.value == false);

    setBit(&falsyBit,false,false);
    REQUIRE(falsyBit.value == false);

    setBit(&falsyBit,false,true);
    REQUIRE(falsyBit.value == false);

    setBit(&falsyBit,true,true);
    REQUIRE(falsyBit.value == true);

    // Update bit when initial state is true
    Bit truthyBit;
    truthyBit.value = true;
    setBit(&truthyBit,true,false); 
    REQUIRE(truthyBit.value == true);

    setBit(&truthyBit,false,false);
    REQUIRE(truthyBit.value == true);

    setBit(&truthyBit,false,true);
    REQUIRE(truthyBit.value == false);

    setBit(&truthyBit,true,true);
    REQUIRE(truthyBit.value == true);
}

TEST_CASE( "Byte", "[memory]" ) {
    Byte byte;
    bool inputs[8] = {true,false,true,true,false,true,false,false};
    
    // Set a byte if enable flag to true 
    setByte(&byte,inputs,true);
    for (int i = 0; i< 8; ++i){
      REQUIRE(byte.value[i] == inputs[i]); 
    }
    
    // Won't set a byte if enable flag is false
    setByte(&byte,inputs,false);
    for (int i = 0; i< 8; ++i){
      REQUIRE(byte.value[i] == inputs[i]); 
    }
}

TEST_CASE( "Registor", "[memory]") {
    Reg reg;
    bool inputs[8] = {true,false,true,true,false,true,false,false};
    
    // Set a reg if enable flag is true
    setReg(&reg, inputs, true);
    for (int i = 0; i< 8; ++i){
      REQUIRE(reg.byte.value[i] == inputs[i]); 
    }

    // Won't set a reg if enable flag is false
    setReg(&reg, inputs, false);
    for (int i = 0; i< 8; ++i){
      REQUIRE(reg.byte.value[i] == inputs[i]); 
    }
    
    // Enable a reg if enable flag is true
    bool* outputs = enablReg(reg,true);
    for (int i = 0; i< 8; ++i){
      REQUIRE(outputs[i] == inputs[i]); 
    }
    free(outputs);

    // Won't enable a reg is enable flag is false
    bool disabledReg[8] = {false,false,false,false,false,false,false,false};
    outputs = enablReg(reg,false);
    for (int i = 0; i< 8; ++i){
      REQUIRE(outputs[i] == disabledReg[i]); 
    }
    free(outputs);
}

