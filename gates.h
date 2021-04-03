#pragma once
#include<array>

using std::array;

namespace gates {
  
  struct AndGate{
    bool value = false;
  };

  struct NandGate{
    bool value = false;
  };

  struct NotGate{
    bool value = false;
  };
  
  struct TripleAndGate{
    array<AndGate,3> andGates;
    bool value = false;
  };

  void setAndGate(AndGate &andGate, bool a, bool b); 
  void setNandGate(NandGate &nandGate, bool a, bool b); 
  void setNotGate(NotGate &notGatet, bool a);
  void setTripleAndGate(TripleAndGate &tripleAndGate, bool a, bool b, bool c, bool d);

}

