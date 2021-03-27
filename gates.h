#pragma once

namespace gates {
  
  struct AndGate{
    bool value = false;
  };

  struct NandGate{
    bool value = false;
  };

  void setAndGate(AndGate &andGate, bool a, bool b); 
  void setNandGate(NandGate &nandGate, bool a, bool b); 

}

