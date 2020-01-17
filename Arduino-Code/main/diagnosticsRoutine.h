//Header for global functions for 
#ifndef DIAG_CLASS_H
#define DIAG_CLASS_H

#include "arduino.h"
//#include <MemoryUsage.h>
#include "realTimer.h"
#include "loggingFunctions.h"

class diagnoticsRoutine {
  private:
  //Private Variables
  String routineName = "Diag";
    double cycleStartTime;
    double cycleEndTime;
    double cycleTime;
    double downTime;
    int targetCycleTime;
    int debugPrioritySetting;
    realTimer memStats;

    float batteryVoltage;
    int cycleCount = 0;

    byte logSize = 100;
    
    double cycleTimeSum;
    long downTimeSum;
    float batteryVoltageSum;

    //Private Functions
    void printMemStats();
    void otherStats();
    
  public:
    //Public variables

    //Public functions
    void init(int debugPrioritySetting, int targetCycleTime);
    void run(float batteryVoltage, double rpmA, double rpmB);
};

#endif // DIAG_CLASS_H
