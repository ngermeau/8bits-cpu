#include "gates.h"

namespace gates { 

  bool andGate(bool a, bool b){
    return a && b;
  }

  bool nandGate(bool a, bool b){
    return !andGate(a,b);
  }
}



