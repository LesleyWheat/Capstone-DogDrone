//Header for global functions for 
#ifndef DIAG_CLASS_H
#define DIAG_CLASS_H

#include "arduino.h"
#include "realTimer.h"
#include "miscFunctions.h"

class diagnoticsRoutine {
  private:
    //Private Variables
  
    //Standard variables
    String routineName = "Diag";
    int debugPrioritySetting;

    //input pins
    byte batteryCompPin;
    byte batteryMotorPin;

    //Cycle time variables
    unsigned long cycleStartTime = 0;
    unsigned long cycleEndTime = 0;
    unsigned long cycleTime = 0;
    double avgCycleTime =0;
    float percentUse = 0;
    double downtime = 0;
    int targetCycleTime;
    int cycleCount = 0;
    double cycleTimeSum;
    long downTimeSum;
    byte logSizeCycle = 100;
    int delayTime=0;

    //Battery monitor variables
    int batteryLogCount =0;
    double batteryVoltageComp_Avg=0;
    double batteryVoltageMotor_Avg=0;
    byte logSizeBattery = 10;
    
    //Objects
    realTimer cyclePrintOut;
    realTimer batteryPrintOut;
    realTimer batteryPoll;

    //Private Functions
    void otherStats();
    void cycleStats();
    void batteryMonitor();
  public:
    //Public variables

    //Public functions
    void init(int debugPrioritySetting, int targetCycleTime, byte batteryCompPin, byte batteryMotorPin);
    void run();
};

#endif // DIAG_CLASS_H
