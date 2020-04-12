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
#define OUTPUT_MIN 40
#define OUTPUT_MAX 200
#define KP 0.2
#define KI 0.2
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
   
    double rpmA = 0;
    double rpmB = 0;


    //Set variables for motor
    realTimer timerRampUp;
    byte left_setPWM;
    byte right_setPWM;
    int left_setRPM;
    int right_setRPM;
    byte left_currentPWM;
    byte right_currentPWM;

    double motorA_setRPM = 200;
    double motorB_setRPM = 200;
    double motorA_outPWM =0;
    double motorB_outPWM =0;
    byte testState = 0;
    realTimer timerTest;
    realTimer pidTimer;

    //Set objects for pid controllers
    PID *pidA;
    PID *pidB;
    PID *pidServo;

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
    void init(int debugPrioritySetting, byte motorInPin1, byte motorInPin2, byte motorPWMA_Pin, byte motorPWMB_Pin);
    void run(double rpmA, double rpmB);
    void set(float angle, float targetSpeed);
};

#endif // CONTROL_CLASS_H
