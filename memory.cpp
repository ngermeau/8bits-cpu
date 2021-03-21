#include "lib/catch_amalgamated.hpp"
#include <iostream>
#include "memory.h"
#include "gates.h"

using namespace gates;

namespace memory {

  Bit updateBit(Bit bit,bool value, bool set){
    bool a = nandGate(value,set);
    std::cout << 'a' << a << '\n';
    bool b = nandGate(a,set);
    std::cout << 'b' << b << '\n';
    bool c = nandGate(b,bit.value);
    std::cout << 'c' << c << '\n';
    bool output = nandGate(c,a);
    std::cout << 'o' << output << '\n';
    Bit new_bit = {output};
    return new_bit;
  }

}

using namespace memory;
TEST_CASE( "Life, the universe and everything", "[42][theAnswer]" ) {
    Bit falsyBit;
    REQUIRE(updateBit(falsyBit,true,false).value == false);
    //REQUIRE(updateBit(falsyBit,true,true).value == true);
    //REQUIRE(updateBit(falsyBit,false,false).value == false);
    //REQUIRE(updateBit(falsyBit,false,true).value == false);

    //Bit truthyBit = { true };
    //REQUIRE(updateBit(falsyBit,true,false).value == true);
    //REQUIRE(updateBit(falsyBit,false,false).value == true);
    //REQUIRE(updateBit(falsyBit,true,true).value == true);
    //REQUIRE(updateBit(falsyBit,false,true).value == false);
}
