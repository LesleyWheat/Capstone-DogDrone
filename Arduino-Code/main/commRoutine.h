//Header for global functions for comm
#ifndef COMM_CLASS_H
#define COMM_CLASS_H

#include "arduino.h"
#include "realTimer.h"
#include "loggingFunctions.h"
#include "miscFunctions.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); // RX, TX

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
