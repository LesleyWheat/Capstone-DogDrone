// Alarm/warning detection and monitoring

#include "diagnosticsRoutine.h"


//output other stats and information
void diagnoticsRoutine::otherStats(){

};
  
    
void diagnoticsRoutine::init(int debugPrioritySetting, int targetCycleTime, byte batteryCompPin, byte batteryMotorPin){
  //Set local variables
  this->debugPrioritySetting=debugPrioritySetting;
  this->targetCycleTime=targetCycleTime;
  this->batteryCompPin=batteryCompPin;
  this->batteryMotorPin=batteryMotorPin;

  //Set starting variables
  
  //Create objects
  cyclePrintOut.init(10000);
  batteryPrintOut.init(600000);
  batteryPoll.init(20000);
};

void diagnoticsRoutine::batteryMonitor(){
  batteryLogCount++;
  
  batteryVoltageComp_Avg = batteryVoltageComp_Avg * (batteryLogCount-1)/batteryLogCount + analogRead(batteryCompPin)/batteryLogCount;
  batteryVoltageMotor_Avg = batteryVoltageMotor_Avg * (batteryLogCount-1)/batteryLogCount + analogRead(batteryMotorPin)*12.0/batteryLogCount;

  if(batteryLogCount > logSizeBattery){
    batteryLogCount=1;
  }
  
  if(batteryPrintOut.check(true)){
    debugPrint(5, routineName, 5, String("Average computer battery voltage: ") + String(batteryVoltageComp_Avg) + String(" Average motor battery voltage: ") + String(batteryVoltageMotor_Avg)  );
  };
}

void diagnoticsRoutine::cycleStats(){
  //add delay to meet target cycle time
  //add 200us for overhead
  if (((unsigned long)(micros()- cycleStartTime)) < (targetCycleTime - 300)){
    delayTime = targetCycleTime- (unsigned long)(micros()- cycleStartTime)- 200;
    delayMicroseconds(delayTime);
  }
  
  //Cycle finish
  cycleEndTime = micros();
  cycleTime = cycleEndTime - cycleStartTime;

  
  //Read into memory
  if(cycleCount == (logSizeCycle+1)){
    //Check how much extra operating time there is to meet cycle time
    avgCycleTime = cycleTimeSum/logSizeCycle;
    percentUse = (cycleTimeSum-downTimeSum)*(100.0/targetCycleTime)/logSizeCycle;
    
    //Reset loop
    cycleCount =0;
    cycleTimeSum =0;
    downTimeSum =0;
  }else{
    cycleCount += 1;
  }

  if(cyclePrintOut.check(true)){
    debugPrint(5, routineName, 5, String("Average cycle time: ") + String(avgCycleTime));
    debugPrint(5, routineName, 5, String("Percent Use: ") + String(percentUse));
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
