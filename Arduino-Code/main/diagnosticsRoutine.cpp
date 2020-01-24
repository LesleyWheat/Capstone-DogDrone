// Alarm/warning detection and monitoring

#include "diagnosticsRoutine.h"


//output other stats and information
void diagnoticsRoutine::otherStats(){

};
  
    
void diagnoticsRoutine::init(int debugPrioritySetting, byte batteryCompPin, byte batteryMotorPin){
  //Set local variables
  this->debugPrioritySetting=debugPrioritySetting;
  this->batteryCompPin=batteryCompPin;
  this->batteryMotorPin=batteryMotorPin;

  //Set starting variables
  
  //Create objects
  cyclePrintOut.init(cyclePrintOut_Period);
  batteryPrintOut.init(batteryPrintOut_Period);
  batteryPoll.init(batteryPoll_Period);

  debugPrint(5, routineName, 5, String(F("Sram free startup: ")) + String(freeMemory()));
};

void diagnoticsRoutine::batteryMonitor(){
  batteryLogCount++;
  
  batteryVoltageComp_Avg = batteryVoltageComp_Avg * (batteryLogCount-1)/batteryLogCount + analogRead(batteryCompPin)/batteryLogCount;
  batteryVoltageMotor_Avg = batteryVoltageMotor_Avg * (batteryLogCount-1)/batteryLogCount + analogRead(batteryMotorPin)*12.0/batteryLogCount;

  if(batteryLogCount > logSizeBattery){
    batteryLogCount=1;
  }
  
  if(batteryPrintOut.check(true)){
    debugPrint(5, routineName, 5, String(F("Avg compbattery V: ")) + String(batteryVoltageComp_Avg));
    //debugPrint(5, routineName, 5, String(F("Avg motorbattery V: ")) + String(batteryVoltageMotor_Avg));
  };
}

void diagnoticsRoutine::cycleStats(){
  if((unsigned long)(micros()- cycleStartTime) > cycleTimeWarning){
    debugPrint(5, routineName, 3, String(F("Long cycle detected: ")) + String((unsigned long)(micros()- cycleStartTime)));
  }
  
  //add delay to meet target cycle time
  if (((unsigned long)(micros()- cycleStartTime)) < (targetCycleTime - cycleOverhead - 100)){
    delayTime = targetCycleTime- (unsigned long)(micros()- cycleStartTime)- cycleOverhead;
    delayMicroseconds(delayTime);
  }
  
  //Cycle finish
  cycleEndTime = micros();
  cycleTime = cycleEndTime - cycleStartTime;


  
  //Read into memory
  if(cycleCount == (logSizeCycle+1)){
    //Check how much extra operating time there is to meet cycle time
    avgCycleTime = (int)(cycleTimeSum/logSizeCycle);
    percentUse = (cycleTimeSum-downTimeSum)*(100.0/targetCycleTime)/logSizeCycle;
    
    //Reset loop
    cycleCount =0;
    cycleTimeSum =0;
    downTimeSum =0;
  }else{
    cycleCount += 1;
  }


  if(cyclePrintOut.check(true)){
    if (printOutFlag == 0)debugPrint(5, routineName, 5, String(F("Sram free: ")) + String(freeMemory()));
    if (printOutFlag == 1)debugPrint(5, routineName, 5, String(F("CPU Percent Use: ")) + String(percentUse));
    if (printOutFlag == 2)debugPrint(5, routineName, 5, String(F("Average cycle time: ")) + String(avgCycleTime));
    printOutFlag = (printOutFlag+1)%3;
  };

  //Add sums
  cycleTimeSum += cycleTime;
  downTimeSum += delayTime;
  
  //End cycle
  cycleStartTime = cycleEndTime;
}

//runs in main loop
void diagnoticsRoutine::run(){
  //Read inputs

  if(batteryPoll.check(true)){
    batteryMonitor();
  };

  
  otherStats();

  //Cycle finish
  cycleStats();
};
