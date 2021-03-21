#include "lib/catch_amalgamated.hpp"
#include <iostream>
#include "memory.h"
#include "gates.h"

using namespace gates;

namespace memory {

  Bit updateBit(Bit bit,bool value, bool set){
    bool gate1 = nandGate(value,set);
    bool gate2 = nandGate(gate1,set);
    bool gate4 = nandGate(gate2,bit.value);
    bool gate3 = nandGate(gate4,gate1);
    Bit new_bit = {gate3};
    return new_bit;
  }

}

using namespace memory;
TEST_CASE( "Life, the universe and everything", "[42][theAnswer]" ) {
    Bit falsyBit;
    REQUIRE(updateBit(falsyBit,true,false).value == false);
    REQUIRE(updateBit(falsyBit,false,false).value == false);
    REQUIRE(updateBit(falsyBit,false,true).value == false);
    REQUIRE(updateBit(falsyBit,true,true).value == true);

    Bit truthyBit = { true };
    REQUIRE(updateBit(truthyBit,true,false).value == true);
    REQUIRE(updateBit(truthyBit,false,false).value == true);
    REQUIRE(updateBit(truthyBit,false,true).value == false);
    REQUIRE(updateBit(truthyBit,true,true).value == true);
}
