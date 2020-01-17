//Header for global functions for 
#ifndef IN_CLASS_H
#define IN_CLASS_H

#include "arduino.h"
#include "realTimer.h"
#include "loggingFunctions.h"
#include "miscFunctions.h"

#define encoderSamples 4

class  inputsRoutine{
  private:
    //Declare private variables
    String routineName = "inputs";
    int debugPrioritySetting;

    //pins
    byte BatterySensorPin;
    byte motorEncoderA_Pin;
    byte motorEncoderB_Pin;
    
    int motorEncoderA_SamplePeriodArray [encoderSamples];
    int motorEncoderB_SamplePeriodArray [encoderSamples];
    
    int motorEncoderA_SampleArrayindex = 0;
    int motorEncoderB_SampleArrayindex = 0;
    
    int encoderA_PrevTime = 0;
    int encoderB_PrevTime = 0;

    int encoderA_period = 0;
    int encoderB_period = 0;
    
    float encoderA_RPM = 0;
    float encoderB_RPM = 0;
    
    int counter =0;
    
    //Objects
    

    //Private functions
    
  public:
    //public variables
    float batteryVoltage = 0;
    double rpmA = 0;
    double rpmB = 0;

    void init(int debugPrioritySetting, byte BatterySensorPin, byte motorEncoderA_Pin, byte motorEncoderB_Pin);
    void run();
    void encoderA();
    void encoderB();
  
};

#endif // IN_CLASS_H
