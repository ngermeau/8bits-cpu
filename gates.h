#pragma once

namespace gates {
  
  struct AndGate{
    bool value = false;
  };

  struct NandGate{
    bool value = false;
  };

  void updateAndGate(AndGate &AndGate, bool a, bool b); 
  void updateNandGate(AndGate &AndGate, bool a, bool b); 

}

