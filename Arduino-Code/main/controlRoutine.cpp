//Control module
#include "controlRoutine.h"

//Set target motor values
void controlRoutine::setMotor(byte spd, byte dir, int accel){
  //debugPrint(5, routineName, 5, String("Motor set to: ") + String(spd));
  //Accel sets timer change rate
  //Accel not used at moment, integrate with PID k values later
  
  //Speed sets motor PWM based on direction and speed, Direction sets difference in wheel speed
  motorA_setRPM = (spd + (50-dir)*spd/50);
  motorB_setRPM = (spd + (50-(100-dir))*spd/50);
};

//Set up tests to cycle through modes
void controlRoutine::testStateMachine(){
  if(timerTest.check(true)){
    //Next state
    testState = ((testState + 1)%5);

    //Set motor settings
    switch(testState){
      case 0:
        setMotor(SPEED_FAST, STRAIGHT, ACCEL_NORMAL);
        break;
      case 1:
        setMotor(SPEED_BRAKE, STRAIGHT, ACCEL_NORMAL);
        break;
      case 2:
        setMotor(SPEED_FAST, STRAIGHT, ACCEL_NORMAL);
        break;
      case 3:
        setMotor(SPEED_SLOW, STRAIGHT, ACCEL_NORMAL);
        break;
      case 4:
        setMotor(SPEED_SLOW, STRAIGHT, ACCEL_MIN);
        break;
      default:
        setMotor(SPEED_BRAKE, STRAIGHT, ACCEL_MIN);
    };

    //output for reference
    debugPrint(5, routineName, 5, String("Test state: ") + String(testState));
    debugPrint(5, routineName, 5, String("Left speed running at: ") + String(motorOptionPin1_PWM/255.0*100));
    debugPrint(5, routineName, 5, String("Right speed running at: ") + String(motorOptionPin2_PWM/255.0*100));
    debugPrint(5, routineName, 5, String("Left set at running at: ") + String(left_setPWM));
    debugPrint(5, routineName, 5, String("Right set at running at: ") + String(right_setPWM));
  };
};

void controlRoutine::init(int debugPrioritySetting){
  //Set local variables
  this->debugPrioritySetting=debugPrioritySetting;

  //Set starting variables

  //Create PIDs for motors
  pidA = new PID(&rpmA, &motorA_outPWM, &motorA_setRPM, KP, KI, KD, DIRECT);
  pidB = new PID(&rpmB, &motorB_outPWM, &motorB_setRPM, KP, KI, KD, DIRECT);

  //Configure PIDs
  pidA->SetOutputLimits(OUTPUT_MIN, OUTPUT_MAX);
  pidB->SetOutputLimits(OUTPUT_MIN, OUTPUT_MAX);
  pidA->SetMode(AUTOMATIC);
  pidB->SetMode(AUTOMATIC);

  //set test timer to rotate states
  timerTest.init(10000);
  //set pid update timer, remember rpm only updates every cycle
  pidTimer.init(250);
  
};

//Runs in main loop
void controlRoutine::run(float batteryVoltage, double rpmA, double rpmB){
  //Read inputs
  this->batteryVoltage=batteryVoltage;
  this->rpmA=rpmA;
  this->rpmB=rpmB;

  //Update state
  //runMotor();
  //testStateMachine();

  setMotor(SPEED_NORMAL, LEFT_SLIGHT, ACCEL_NORMAL);

  if(motorA_setRPM > 30){pidA->SetOutputLimits(40, OUTPUT_MAX);}
  else{pidA->SetOutputLimits(OUTPUT_MIN, 100);}

  if(motorB_setRPM > 30){pidB->SetOutputLimits(40, OUTPUT_MAX);}
  else{pidB->SetOutputLimits(OUTPUT_MIN, 100);}
  
  //Update PID controllers
  if(pidTimer.check(true)){
    pidA->Compute();
    pidB->Compute();
  }
  
  //debugPrint(5, routineName, 5, String("rpmA: ") + String(rpmA) + String(" rpmA_out: ") + String(motorA_outPWM)+ String(" rpmA_set: ") + String(motorA_setRPM));
  //debugPrint(5, routineName, 5, String("rpmB: ") + String(rpmB) + String(" rpmB_out: ") + String(motorB_outPWM)+ String(" rpmB_set: ") + String(motorB_setRPM));

  motorOptionPin1_PWM = (byte) motorA_outPWM;
  motorOptionPin2_PWM = (byte) motorB_outPWM;
};
