//Header for global functions for control
#ifndef CONTROL_CLASS_H
#define CONTROL_CLASS_H

//-----------------------------------------------

//Libraries
#include "arduino.h"
#include <PID_v1.h>

//local functions
#include "realTimer.h"
#include "miscFunctions.h"

//-----------------------------------------------

//Minimum motor voltage is 9.75 or it will stall, pwm of approx 70/255

//PID variables
#define OUTPUT_MIN 0
#define OUTPUT_MAX 200
#define KP 0.05
#define KI 0.01
#define KD 0
#define PIDupdatePeriod 500

//-----------------------------------------------

class controlRoutine {
  private:
    //Standard routine settings
    int debugPrioritySetting;
    String routineName = "CON";

    //Input pins
    byte motorInPin1;
    byte motorInPin2;
    byte motorFrontAPWM_Pin;
    byte motorFrontBPWM_Pin;
    byte motorRearAPWM_Pin;
    byte motorRearBPWM_Pin;
    byte servoPWM_Pin;
   
    double rpmFrontA = 0;
    double rpmFrontB = 0;
    double rpmRearA = 0;
    double rpmRearB = 0;

    //Set variables for motor
    realTimer timerRampUp;
    byte frontA_setPWM;
    byte frontB_setPWM;
    byte rearA_setPWM;
    byte rearB_setPWM;
    
    double frontA_currentPWM = 0;
    double frontB_currentPWM = 0;
    double rearA_currentPWM = 0;
    double rearB_currentPWM = 0;

    double frontA_setRPM = 0;
    double frontB_setRPM = 0;
    double rearA_setRPM = 0;
    double rearB_setRPM = 0;
    
    double frontA_outPWM =0;
    double frontB_outPWM =0;
    double rearA_outPWM =0;
    double rearB_outPWM =0;
    
    byte testState = 0;
    realTimer timerTest;
    realTimer pidTimer;

    //Set objects for pid controllers
    PID *pidFrontA;
    PID *pidFrontB;
    PID *pidRearA;
    PID *pidRearB;

    //Test state machine
    enum state{
      STANDBY = 0,
      TEST = 1
    }; 

    //Private functions
    void setMotor(byte spd, byte dir, int accel);
    void testStateMachine();
    
  public:
    //Public variables
    

    //Public functions
    void init(int debugPrioritySetting, byte motorInPin1, byte motorInPin2, byte motorFrontAPWM_Pin, byte motorFrontBPWM_Pin, byte motorRearAPWM_Pin, byte motorRearBPWM_Pin);
    void run(double rpmFrontA, double rpmFrontB, double rpmRearA, double rpmRearB, double rpmSet, double angleSet);
    void set(float angle, float targetSpeed);
};

#endif // CONTROL_CLASS_H
