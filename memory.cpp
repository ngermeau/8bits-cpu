#include "lib/catch_amalgamated.hpp"
#include <iostream>
#include "memory.h"
#include "gates.h"

using namespace gates;

namespace memory {

  void displayBools(bool inputs[8]){
    for (int z=0; z<8; z++){
       if (inputs[z]){
            printf("1");
          }else{
            printf("0");
          }
      }
    printf(" | ");
  }
  
  void displayRam(Ram *ram){
    for (int i = 0; i< 16; i++){
      for (int j = 0; j<16; j++){
        displayBools((*ram).regs[i][j].byte.value);
      }
      printf("\n");
    }
  }

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
  
  bool* enablReg(Reg reg, bool enable){
    return enabl(reg.byte.value,enable);
  }
  
  bool* decoder2x4(bool a, bool b){
    bool notGatea = notGate(a);
    bool notGateb = notGate(b);

    bool* outputs = (bool*) malloc(4);
    outputs[0] = andGate(notGatea, notGateb);
    outputs[1] = andGate(notGatea,b);
    outputs[2] = andGate(a,notGateb);
    outputs[3] = andGate(a,b);
    return outputs;
  }
  
  //https://line.17qq.com/articles/igibijgpbz.html
  int decoder4x16(bool e, bool a, bool b, bool c){
    bool notGatee = notGate(e);
    bool notGatea = notGate(a);
    bool notGateb = notGate(b);
    bool notGatec = notGate(c);
    bool outputs[16];
    outputs[0] = tripleAndGate(notGatee,notGatea,notGateb,notGatec);
    outputs[1] = tripleAndGate(notGatee,notGatea,notGateb,c);
    outputs[2] = tripleAndGate(notGatee,notGatea,b,notGatec);
    outputs[3] = tripleAndGate(notGatee,notGatea,b,c);
    outputs[4] = tripleAndGate(notGatee,a,notGateb,notGatec);
    outputs[5] = tripleAndGate(notGatee,a,notGateb,c);
    outputs[6] = tripleAndGate(notGatee,a,b,notGatec);
    outputs[7] = tripleAndGate(notGatee,a,b,c);
    outputs[8] = tripleAndGate(e,notGatea,notGateb,notGatec);
    outputs[9] = tripleAndGate(e,notGatea,notGateb,c);
    outputs[10] = tripleAndGate(e,notGatea,b,notGatec);
    outputs[11] = tripleAndGate(e,notGatea,b,c);
    outputs[12] = tripleAndGate(e,a,notGateb,notGatec);
    outputs[13] = tripleAndGate(e,a,notGateb,c);
    outputs[14] = tripleAndGate(e,a,b,notGatec);
    outputs[15] = tripleAndGate(e,a,b,c);
    
    int i;
    for (i=0; i< 16; i++){
      if(outputs[i]){ break; }
    }
    return i; //todo, this is bad because if we can't find anything then what ? 
  }


  void memoryCell(Ram* ram, Reg address, Bus* bus, bool enable, bool set){ 
    bool* addressValue = address.byte.value;
    int v = decoder4x16(addressValue[0],addressValue[1],addressValue[2],addressValue[3]);
    int h = decoder4x16(addressValue[4],addressValue[5],addressValue[6],addressValue[7]);
    printf("vert %d: ", v);
    printf("hori %d: ", h);
    printf("\n");
    if (andGate(andGate(true,true), set)){
        setReg(&(*ram).regs[h][v],(*bus).value, set); 
    }else {
        memcpy((*bus).value,(*ram).regs[h][v].byte.value,sizeof((*bus).value));
    }
  }
  

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

TEST_CASE( "Reg", "[memory]") {
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



TEST_CASE( "memory", "[memory]") {
  Ram ram;
  Reg address;
  Bus bus; 
  bool addressValue[8] = {true,false,false,false,true,false,true,false};
  memcpy(address.byte.value, addressValue, 8);
  bool busValue[8] = {false,false,true,true,false,true,false,false};
  memcpy(bus.value, busValue, 8);
  memoryCell(&ram,address,&bus,false, true);
  displayRam(&ram);
  
  memcpy(address.byte.value, addressValue, 8);
  memoryCell(&ram,address,&bus,false, true);
  // need to test enable

}
