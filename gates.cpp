#include "gates.h"

namespace gates { 
  
  void updateAndGate(AndGate &andGate, bool a, bool b){
    andGate.value = a && b;
  }

  void updateNandGate(AndGate &andGate, bool a, bool b){
    andGate.value = !(a && b);
  }

}



