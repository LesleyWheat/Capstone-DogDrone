//Header for global functions for comm
#ifndef COMM_CLASS_H
#define COMM_CLASS_H

//External libraries
#include "arduino.h"
#include <ros.h>
#include <std_msgs/String.h>

//Internal libraries
#include "realTimer.h"
#include "loggingFunctions.h"
#include "miscFunctions.h"


class commRoutine {
  private:
    int debugPrioritySetting;
    realTimer contactTimeout;
    realTimer test;
    String routineName = "Comm";
    void establishContact();
    
  public:
    void init(int debugPrioritySetting);
    void run();
};

#endif // COMM_CLASS_H
