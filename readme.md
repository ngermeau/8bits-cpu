overview
#

1) load the program into memory 
2) run the computer (clock start to create cycle)


clock    
generate cycle:    
  1 cycle = down and up   
  for every cycle call next step   (one step per cycle)

stepper    
  move from step to step    
  step 1     
  step 2    
  step 3     
      we didn't execute anything, just loaded the next instruction from ram into the registry and increment by 1 the index
  step 4    
  step 5    
  step 6    
  step 7    

cpu:
  registers 
  acc
  tmp 
  IR 
  index 

memory: 
  mar 
  ram 
   
bus: 
  value  
