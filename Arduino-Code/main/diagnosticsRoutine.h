//Header for global functions for 
#ifndef DIAG_CLASS_H
#define DIAG_CLASS_H

#include "arduino.h"
#include "realTimer.h"
#include "miscFunctions.h"

#define cycleOverhead 200
#define cyclePrintOut_Period 10000
#define batteryPrintOut_Period 5000
#define batteryPoll_Period 1000
#define cycleTimeWarning 10000
#define targetCycleTime 5000

class diagnoticsRoutine {
  private:
    //Private Variables
  
    //Standard variables
    String routineName = "DIA";
    int debugPrioritySetting;
    byte printOutFlag = 0;

    //input pins
    byte batteryCompPin;
    byte batteryMotorPin;

    //Cycle time variables
    unsigned long cycleStartTime = 0;
    unsigned long cycleEndTime = 0;
    unsigned long cycleTime = 0;
    int avgCycleTime =0;
    float percentUse = 0;
    double downtime = 0;
    int cycleCount = 0;
    double cycleTimeSum;
    long downTimeSum;
    byte logSizeCycle = 100;
    int delayTime=0;

    //Battery monitor variables
    double batteryVoltageComp_Avg=0;
    double batteryVoltageMotor_Avg=0;
    
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
    void init(int debugPrioritySetting, byte batteryCompPin, byte batteryMotorPin);
    void run();
};

#endif // DIAG_CLASS_H
