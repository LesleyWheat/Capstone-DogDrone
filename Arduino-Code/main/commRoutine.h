//Header for global functions for comm
#ifndef COMM_CLASS_H
#define COMM_CLASS_H

//External libraries
#include "arduino.h"
#include <ros.h>
#include <std_msgs/String.h>
#include <SoftwareSerial.h>

//Internal libraries
#include "realTimer.h"
#include "miscFunctions.h"
#include "miscFunctions.h"

#define rssiUpdatePeriod 1000


class commRoutine {
  private:
    //Private variables
    int debugPrioritySetting;
    String routineName = "Comm";

    //Pins
    byte rssiInPin;

    //Objects
    SoftwareSerial* mySerial;
    realTimer contactTimeout;
    realTimer test;
    realTimer rssiUpdate;

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
