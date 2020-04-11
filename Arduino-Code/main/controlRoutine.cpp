//Control module
#include "controlRoutine.h"

void controlRoutine::init(int debugPrioritySetting, byte motorInPin1, byte motorInPin2, byte motorPWMA_Pin, byte motorPWMB_Pin){
  //Set local variables
  this->debugPrioritySetting=debugPrioritySetting;
  this->motorInPin1=motorInPin1;
  this->motorInPin2=motorInPin2;
  this->motorPWMA_Pin=motorPWMA_Pin;
  this->motorPWMB_Pin=motorPWMB_Pin;
  
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
  pidTimer.init(PIDupdatePeriod);
  
};

//------------------------------------------------------------------------------------------------
//Set target motor values
void controlRoutine::setMotor(byte spd, byte dir, int accel){
  //debugPrint(5, routineName, 5, String("Motor set to: ") + String(spd));
  //Accel sets timer change rate
  //Accel not used at moment, integrate with PID k values later
  
  //Speed sets motor PWM based on direction and speed, Direction sets difference in wheel speed
  motorA_setRPM = (spd + (50-dir)*spd/50);
  motorB_setRPM = (spd + (50-(100-dir))*spd/50);
};

//-----------------------------------------------------------------------------------------------
//Set up tests to cycle through modes
void controlRoutine::testStateMachine(){
  if(timerTest.check(true)){
    //Next state
    testState = ((testState + 1)%5);

    //Set motor settings
    /*switch(testState){
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
    };*/

    //output for reference
    debugPrint(5, routineName, 5, String(F("Test state: ")) + String(testState));
    debugPrint(5, routineName, 5, String("Left speed running at: ") + String(rpmA));
    debugPrint(5, routineName, 5, String("Right speed running at: ") + String(rpmB));
    debugPrint(5, routineName, 5, String("Left set at running at: ") + String(motorA_outPWM));
    debugPrint(5, routineName, 5, String("Right set at running at: ") + String(motorB_outPWM));
  };
};

//----------------------------------------------------------------------------------------------
void controlRoutine::set(float angle, float targetSpeed){
  
}

//----------------------------------------------------------------------------------------------
//Runs in main loop
void controlRoutine::run(double rpmA, double rpmB){
  //Read inputs
  this->rpmA=rpmA;
  this->rpmB=rpmB;

  //Update state
  //runMotor();
  //testStateMachine();
  
  //Update PID controllers
  if(pidTimer.check(true)){

    
    pidA->Compute();
    pidB->Compute();

    //debugPrint(5, routineName, 5, String("rpmB: ") + String(rpmB) );
    //debugPrint(5, routineName, 5, String("PWM_out: ") + String(motorB_outPWM));
    //debugPrint(5, routineName, 5,  String("rpmB_set: ") + String(motorB_setRPM));
    //debugPrint(5, routineName, 5, String("rpmB: ") + String(rpmB) + String(" rpmB_out: ") + String(motorB_outPWM)+ String(" rpmB_set: ") + String(motorB_setRPM));

    analogWrite(motorPWMA_Pin, motorA_outPWM);
    analogWrite(motorPWMB_Pin, motorB_outPWM);
  }
  
};
