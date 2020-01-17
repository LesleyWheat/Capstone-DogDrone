//Header for global functions for outputs
#ifndef OUT_CLASS_H
#define OUT_CLASS_H

#include "arduino.h"

class outputsRoutine {
  private:
    //Declare private variables
    String routineName = "outputs";
    int debugPrioritySetting;
    
  public:
    void init(int debugPrioritySetting);
    void run();
};

#endif // OUT_CLASS_H
