//Header for global functions for comm
#ifndef COMM_CLASS_H
#define COMM_CLASS_H

//External libraries
#include "arduino.h"
#include <ros.h>
#include <std_msgs/String.h>

//Internal libraries
#include "realTimer.h"
#include "miscFunctions.h"
#include "miscFunctions.h"

#define contactTimeoutPeriod 60000

class commRoutine {
  private:
    //Private variables
    int debugPrioritySetting;
    String routineName = "COM";

    //Pins

    //Objects
    realTimer contactTimeout;

    //Private functions
    void establishContact();
    
  public:
    //Public variables

    //Public functions
    void init(int debugPrioritySetting);
    void run();
};

#endif // COMM_CLASS_H
