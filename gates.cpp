#include "gates.h"

namespace gates { 
  
  bool andGate(bool a, bool b){
    return  a && b;
  }

  bool nandGate(bool a, bool b){
    return !(a && b);
  }

  bool notGate(bool a) {
    return !a;
  }

  bool tripleAndGate(bool a, bool b, bool c, bool d) {
    return andGate(andGate(andGate(a,b),c),d);
  }
}



