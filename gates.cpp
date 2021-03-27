#include "gates.h"

namespace gates { 
  
  void setAndGate(AndGate &andGate, bool a, bool b){
    andGate.value = a && b;
  }

  void setNandGate(NandGate &nandGate, bool a, bool b){
    nandGate.value = !(a && b);
  }

}



