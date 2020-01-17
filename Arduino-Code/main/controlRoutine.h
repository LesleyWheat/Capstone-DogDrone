//Header for global functions for control
#ifndef CONTROL_CLASS_H
#define CONTROL_CLASS_H

//Libraries
#include "arduino.h"
#include <PID_v1.h>

//local functions
#include "realTimer.h"
#include "loggingFunctions.h"

//PID variables
#define OUTPUT_MIN 30
#define OUTPUT_MAX 250
#define KP 1.5
#define KI 0.7
#define KD 0.1

class controlRoutine {
  private:
    //Standard routine settings
    int debugPrioritySetting;
    String routineName = "cont";

    
    float batteryVoltage;
    double rpmA;
    double rpmB;

    //Set variables for motor
    realTimer timerRampUp;
    byte left_setPWM;
    byte right_setPWM;
    int left_setRPM;
    int right_setRPM;
    byte left_currentPWM;
    byte right_currentPWM;

    double motorA_setRPM =0;
    double motorB_setRPM =0;
    double motorA_outPWM =0;
    double motorB_outPWM =0;
    byte testState = 0;
    realTimer timerTest;
    realTimer pidTimer;

    //Set objects for pid controllers
    PID *pidA;
    PID *pidB;

    //Test state machine
    enum state{
      STANDBY = 0,
      TEST = 1
    }; 

    //Plain text settings for speed - needs more calibration
    enum speed{
      SPEED_BRAKE = 0,
      SPEED_VERY_SLOW = 20,
      SPEED_SLOW = 25,
      SPEED_NORMAL = 30,
      SPEED_FAST = 40,
      SPEED_MAX = 50
    };

    //Plain text settings for direction - needs calibration
    enum direction{
      LEFT_MAX = 0,
      LEFT_HARD = 10,
      LEFT_SLIGHT = 40,
      STRAIGHT = 50,
      RIGHT_SLIGHT = 60,
      RIGHT_HARD = 90,
      RIGHT_MAX = 100
    };

    //Plain text settings for acceleration - needs calibration
    enum acceleration{
      ACCEL_MIN = 60,
      ACCEL_SLOW = 30,
      ACCEL_NORMAL = 10,
      ACCEL_FAST = 5,
      ACCEL_MAX = 2
    };

    //Private functions
    void setMotor(byte spd, byte dir, int accel);
    void testStateMachine();
    
  public:
    byte motorOptionPin1_PWM = 0;
    byte motorOptionPin2_PWM = 0;

    //Public functions
    void init(int debugPrioritySetting);
    void run(float batteryVoltage, double rpmA, double rpmB);
};

#endif // CONTROL_CLASS_H
