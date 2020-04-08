//Header for global functions for comm
#ifndef COMM_CLASS_H
#define COMM_CLASS_H

//-------------------------------------------------------

//External libraries
#include "arduino.h"
#include <ros.h>
#include <std_msgs/String.h>
#include <SoftwareSerial.h>

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

    //Pins
    byte rssiInPin;
    
    //Objects
    SoftwareSerial* mySerial;
    realTimer contactTimeout;
    realTimer rssiUpdate;
    realTimer rssiRead;

    //Private functions
    void establishContact();
    
  public:
    //Public variables
    float rssi_raw = 0;
    
    //Public functions
    void init(int debugPrioritySetting, byte rssiInPin, SoftwareSerial* ss);
    void run();
};

#endif // COMM_CLASS_H
