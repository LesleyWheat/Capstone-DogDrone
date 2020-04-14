//Control module
#include "controlRoutine.h"

void controlRoutine::init(int debugPrioritySetting, byte motorInPin1, byte motorInPin2, byte motorFrontAPWM_Pin, byte motorFrontBPWM_Pin, byte motorRearAPWM_Pin, byte motorRearBPWM_Pin){
  //Set local variables
  this->debugPrioritySetting=debugPrioritySetting;
  this->motorInPin1=motorInPin1;
  this->motorInPin2=motorInPin2;
  this->motorFrontAPWM_Pin=motorFrontAPWM_Pin;
  this->motorFrontBPWM_Pin=motorFrontBPWM_Pin;
  this->motorRearAPWM_Pin=motorRearAPWM_Pin;
  this->motorRearBPWM_Pin=motorRearBPWM_Pin;
  
  //Set starting variables
  

  //Create PIDs for motors
  pidFrontA = new PID(&rpmFrontA, &frontA_outPWM, &frontA_setRPM, KP, KI, KD, DIRECT);
  pidFrontB = new PID(&rpmFrontB, &frontB_outPWM, &frontB_setRPM, KP, KI, KD, DIRECT);
  pidRearA = new PID(&rpmRearA, &rearA_outPWM, &rearA_setRPM, KP, KI, KD, DIRECT);
  pidRearB = new PID(&rpmRearB, &rearB_outPWM, &rearB_setRPM, KP, KI, KD, DIRECT);

  //Configure PIDs
  pidFrontA->SetOutputLimits(OUTPUT_MIN, OUTPUT_MAX);
  pidFrontB->SetOutputLimits(OUTPUT_MIN, OUTPUT_MAX);
  pidRearA->SetOutputLimits(OUTPUT_MIN, OUTPUT_MAX);
  pidRearB->SetOutputLimits(OUTPUT_MIN, OUTPUT_MAX);
  pidFrontA->SetMode(AUTOMATIC);
  pidFrontB->SetMode(AUTOMATIC);
  pidRearA->SetMode(AUTOMATIC);
  pidRearB->SetMode(AUTOMATIC);

  //set test timer to rotate states
  timerTest.init(5000);
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
  //motorA_setRPM = (spd + (50-dir)*spd/50);
  //motorB_setRPM = (spd + (50-(100-dir))*spd/50);
};

//-----------------------------------------------------------------------------------------------
//Set up tests to cycle through modes
void controlRoutine::testStateMachine(){
  if(timerTest.check(true)){
    //Next state
    testState = ((testState + 1)%5);

    //Set motor settings
    switch(testState){
      case 0:
        analogWrite(motorFrontAPWM_Pin, 0);
        analogWrite(motorFrontBPWM_Pin, 0);
        analogWrite(motorRearAPWM_Pin, 0);
        analogWrite(motorRearBPWM_Pin, 0);
        break;
      case 1:
        analogWrite(motorFrontAPWM_Pin, 100);
        analogWrite(motorFrontBPWM_Pin, 100);
        analogWrite(motorRearAPWM_Pin, 100);
        analogWrite(motorRearBPWM_Pin, 100);
        break;
      case 2:
        analogWrite(motorFrontAPWM_Pin, 180);
        analogWrite(motorFrontBPWM_Pin, 180);
        analogWrite(motorRearAPWM_Pin, 180);
        analogWrite(motorRearBPWM_Pin, 180);
        break;
      case 3:
        analogWrite(motorFrontAPWM_Pin, 200);
        analogWrite(motorFrontBPWM_Pin, 200);
        analogWrite(motorRearAPWM_Pin, 200);
        analogWrite(motorRearBPWM_Pin, 200);
        break;
      case 4:
        analogWrite(motorFrontAPWM_Pin, 220);
        analogWrite(motorFrontBPWM_Pin, 220);
        analogWrite(motorRearAPWM_Pin, 220);
        analogWrite(motorRearBPWM_Pin, 220);
        break;
      default:
        analogWrite(motorFrontAPWM_Pin, 0);
        analogWrite(motorFrontBPWM_Pin, 0);
        analogWrite(motorRearAPWM_Pin, 0);
        analogWrite(motorRearBPWM_Pin, 0);
    };

    //output for reference
    debugPrint(5, routineName, 5, String(F("Test state: ")) + String(testState));
    debugPrint(5, routineName, 5, String("Left front speed running at: ") + String(rpmFrontA));
    debugPrint(5, routineName, 5, String("Right front speed running at: ") + String(rpmFrontB));
    debugPrint(5, routineName, 5, String("Left rear speed running at: ") + String(rpmRearA));
    debugPrint(5, routineName, 5, String("Right rear speed running at: ") + String(rpmRearB));
    debugPrint(5, routineName, 5, String("Left front set at running at: ") + String(frontA_outPWM));
    debugPrint(5, routineName, 5, String("Right front set at running at: ") + String(frontB_outPWM));
    debugPrint(5, routineName, 5, String("Left back set at running at: ") + String(rearA_outPWM));
    debugPrint(5, routineName, 5, String("Right back set at running at: ") + String(rearB_outPWM));
  };
};

//----------------------------------------------------------------------------------------------
void controlRoutine::set(float angle, float targetSpeed){
  
}

//----------------------------------------------------------------------------------------------
//Runs in main loop
void controlRoutine::run(double rpmFrontA, double rpmFrontB, double rpmRearA, double rpmRearB, double rpmSet, double angleSet){
  //Read inputs
  this->rpmFrontA=rpmFrontA;
  this->rpmFrontB=rpmFrontB;
  this->rpmRearA=rpmRearA;
  this->rpmRearB=rpmRearB;

  
  frontA_setRPM = rpmSet;
  frontB_setRPM = rpmSet;
  
  rearA_setRPM = frontA_setRPM;
  rearB_setRPM = frontB_setRPM;

  //Update state
  //runMotor();
  testStateMachine();
  
  //Update PID controllers
  if(pidTimer.check(true)){
    pidFrontA->Compute();
    pidFrontB->Compute();
    pidRearA->Compute();
    pidRearB->Compute();
  
    //debugPrint(5, routineName, 5, String("rpmB: ") + String(rpmB) );
    //debugPrint(5, routineName, 5, String("PWM_out: ") + String(motorB_outPWM));
    //debugPrint(5, routineName, 5,  String("rpmB_set: ") + String(motorB_setRPM));
    debugPrint(5, routineName, 5, String("rpmFrontA: ") + String(rpmFrontA) + String(" rpmFrontA_out: ") + String(frontA_outPWM)+ String(" frontA_setRPM: ") + String(frontA_setRPM));
    debugPrint(5, routineName, 5, String("rpmFrontB: ") + String(rpmFrontB) + String(" rpmFrontB_out: ") + String(frontB_outPWM)+ String(" frontB_setRPM: ") + String(frontB_setRPM));
    debugPrint(5, routineName, 5, String("rpmRearA: ") + String(rpmRearA) + String(" rpmRearA_out: ") + String(rearA_outPWM)+ String(" rearA_setRPM: ") + String(rearA_setRPM));
    debugPrint(5, routineName, 5, String("rpmRearB: ") + String(rpmRearB) + String(" rpmRearB_out: ") + String(rearB_outPWM)+ String(" rearB_setRPM: ") + String(rearB_setRPM));
    
    //analogWrite(motorFrontAPWM_Pin, frontA_outPWM);
    //analogWrite(motorFrontBPWM_Pin, frontB_outPWM);
    //analogWrite(motorRearAPWM_Pin, rearA_outPWM);
    //analogWrite(motorRearBPWM_Pin, rearB_outPWM);
    
  }
  
};
