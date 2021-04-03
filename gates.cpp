#include "gates.h"

namespace gates { 
  
  void setAndGate(AndGate &andGate, bool a, bool b){
    andGate.value = a && b;
  }

  void setNandGate(NandGate &nandGate, bool a, bool b){
    nandGate.value = !(a && b);
  }

  void setNotGate(NotGate &notGate, bool a) {
    notGate.value = !a;
  }

  void setTripleAndGate(TripleAndGate &tripleAndGate, bool a, bool b, bool c, bool d) {
    setAndGate(tripleAndGate.andGates[0],a,b);
    setAndGate(tripleAndGate.andGates[1],tripleAndGate.andGates[0].value,c);
    setAndGate(tripleAndGate.andGates[2],tripleAndGate.andGates[1].value,d);
    tripleAndGate.value = tripleAndGate.andGates[2].value;
  }
}



