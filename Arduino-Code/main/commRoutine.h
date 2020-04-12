//Header for global functions for comm
#ifndef COMM_CLASS_H
#define COMM_CLASS_H

//-------------------------------------------------------

//External libraries
#include "arduino.h"
#include <ros.h>
#include <std_msgs/String.h>

//Internal libraries
#include "realTimer.h"
#include "miscFunctions.h"

//-----------------------------------------------------

#define rssiUpdatePeriod 1000
#define rssiReadPeriod 100
#define contactTimeoutPeriod 60000
#define serialRate 38400

class commRoutine {
  private:
    //Private variables
    int debugPrioritySetting;
    String routineName = "COM";
    int received;
    char msg[100] = "";
    byte msgCount = 0;
    char msgPi[100] = "";
    byte msgCountPi = 0;

    //Pins
    byte rssiInPin;
    
    //Objects
    realTimer contactTimeout;
    realTimer rssiUpdate;
    realTimer rssiRead;

    //Private functions
    void establishContact();
    
  public:
    //Public variables
    float rssi_raw = 0;
    int rpm_set = 0;
    int angle_set = 0;
    
    //Public functions
    void init(int debugPrioritySetting, byte rssiInPin);
    void run();
};

#endif // COMM_CLASS_H
