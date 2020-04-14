/*
  DogDrone Arduino Mega Pins
*/

//------------------------------------------------------------------------------
//External libraries
#include "arduino.h"
#include <SoftwareSerial.h>

//Modules
#include "positionRoutine.h"
#include "controlRoutine.h"
#include "diagnosticsRoutine.h"
#include "commRoutine.h"

//Functions
#include "realTimer.h"
#include "miscFunctions.h"

//---------------------------------------------------------------------------------
//PIN MAP
//INPUT
#define batteryMotorPin A1
#define batteryCompPin A3

#define sonarEchoPin 7
#define rssiInPin A0

//Interupts
#define motorFrontA_Encoder_Pin 3
#define motorFrontB_Encoder_Pin 2
#define motorRearA_Encoder_Pin 19
#define motorRearB_Encoder_Pin 18


//OUTPUT
#define motorOp1 12
#define motorOp2 13
#define sonarTrig 6

//PWM
#define motorFrontA_PWM_Pin 9
#define motorFrontB_PWM_Pin 8
#define motorRearA_PWM_Pin 11
#define motorRearB_PWM_Pin 10


//-------------------------------------------------------------------------------
//OBJECTS
//Object Routines
positionRoutine pos;
controlRoutine control;
commRoutine comm;
diagnoticsRoutine diagnotics;

//Loop objects
realTimer blinkTimer;
realTimer echoTimer;

//-----------------------------------------------------------------------------
//Define system variables
#define DEBUG 1
#define WIRED 1
#define blinkPeriod 1000
#define echoPeriod 5000
// 5 is all msgs, higher filters out lower priority msgs
#define debugPrioritySetting 5

//-----------------------------------------------------------------------------
//VARIABLES
//global variables for main
byte blinkState = 0;
byte echoFlag = 0;
int echoDuration = 0;

//Interupts
volatile byte motorFrontA_EncoderState = LOW;
volatile byte motorFrontB_EncoderState = LOW;
volatile byte motorRearA_EncoderState = LOW;
volatile byte motorRearB_EncoderState = LOW;

//----------------------------------------------------------------------------
//SETUP
// the setup function runs once when you press reset or power the board
void setup() {
  #ifdef DEBUG
  #endif

  //Serial.println(F("Adafruit AHRS Fusion Example"));
  //debugPrint(debugPrioritySetting, "setup", 5, "Starting...");
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  //Input pins
  pinMode(batteryMotorPin, INPUT);
  pinMode(batteryCompPin, INPUT);
  
  pinMode(rssiInPin, INPUT);
  pinMode(motorFrontA_Encoder_Pin, INPUT);
  pinMode(motorFrontB_Encoder_Pin, INPUT);
  pinMode(motorRearA_Encoder_Pin, INPUT);
  pinMode(motorRearB_Encoder_Pin, INPUT);
  pinMode(sonarEchoPin, INPUT);

  //Outputs
  pinMode(motorOp1, OUTPUT);
  pinMode(motorOp2, OUTPUT);
  pinMode(sonarTrig, OUTPUT);
  
  pinMode(motorFrontA_PWM_Pin, OUTPUT);
  pinMode(motorFrontB_PWM_Pin, OUTPUT);
  pinMode(motorRearA_PWM_Pin, OUTPUT);
  pinMode(motorRearB_PWM_Pin, OUTPUT);

  //Motor setup
  digitalWrite(motorOp1, HIGH);
  digitalWrite(motorOp2, LOW);

  //Declare objects for loop routine
  blinkTimer.init(blinkPeriod);
  echoTimer.init(echoPeriod);

  //Initalize main routines
  comm.init(debugPrioritySetting, rssiInPin);
  pos.init(debugPrioritySetting);
  control.init(debugPrioritySetting, motorOp1, motorOp2, motorFrontA_PWM_Pin, motorFrontB_PWM_Pin, motorRearA_PWM_Pin, motorRearB_PWM_Pin);
  diagnotics.init(debugPrioritySetting, batteryCompPin, batteryMotorPin);

  //Setup interupts
  attachInterrupt(digitalPinToInterrupt(motorFrontA_Encoder_Pin), encoderInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(motorFrontB_Encoder_Pin), encoderInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(motorRearA_Encoder_Pin), encoderInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(motorRearB_Encoder_Pin), encoderInterrupt, RISING);
  
  //Finish setup
  debugPrint(debugPrioritySetting, F("SET"), 5, F("Startup complete"));
}

//----------------------------------------------------------------------------------------
//LOOP
void loop() {
  
  //str_msg.data = hello;
  //chatter.publish( &str_msg );
  //nh.spinOnce();
  
  //local variables
  String routineName = "main";

  //Cycle startup

  //Main sequence

  if(blinkTimer.check(true)){
      //Next state
      blinkState = ((blinkState + 1)%2);    
  };

  //Run echo on conditions
  if(echoTimer.check(true)){
    echo();
  }

  
  //Run main routines
  pos.run();
  control.run(pos.rpmFrontA, pos.rpmFrontB, pos.rpmRearA, pos.rpmRearB, comm.rpm_set, comm.angle_set);
  comm.run();
  diagnotics.run();
  //Diagnostics includes delay to target cycle time
}

//-----------------------------------------------------------------------------------------

//Encoder interrupt routines
void encoderInterrupt(void){
  if(digitalRead(motorFrontA_Encoder_Pin) != motorFrontA_EncoderState){
    motorFrontA_EncoderState = digitalRead(motorFrontA_Encoder_Pin);
    //debugPrint(debugPrioritySetting, F("MAI"), 5, String(F("Interupt A triggered")));
    pos.motorEncoderFrontA_count++;
  }
  
  if(digitalRead(motorFrontB_Encoder_Pin) != motorFrontB_EncoderState){
    motorFrontB_EncoderState = digitalRead(motorFrontB_Encoder_Pin);
    //debugPrint(debugPrioritySetting, F("MAI"), 5, String(F("Interupt B triggered")));
    pos.motorEncoderFrontB_count++;
  }

  if(digitalRead(motorRearA_Encoder_Pin) != motorRearA_EncoderState){
    motorRearA_EncoderState = digitalRead(motorRearA_Encoder_Pin);
    //debugPrint(debugPrioritySetting, F("MAI"), 5, String(F("Interupt A triggered")));
    pos.motorEncoderRearA_count++;
  }
  
  if(digitalRead(motorRearB_Encoder_Pin) != motorRearB_EncoderState){
    motorRearB_EncoderState = digitalRead(motorRearB_Encoder_Pin);
    //debugPrint(debugPrioritySetting, F("MAI"), 5, String(F("Interupt B triggered")));
    pos.motorEncoderRearB_count++;
  }
  
}


//Additional I/O
void echo(void){
  if (echoFlag == 0){
      digitalWrite(sonarTrig, HIGH);
      echoFlag = 1;
    }else{
      echoFlag = 0;
      digitalWrite(sonarTrig, LOW);
      echoDuration = pulseIn(sonarEchoPin, HIGH, 8000);
      debugPrint(debugPrioritySetting, F("MAI"), 5, String(F("Echo duration: ")) + String(echoDuration));
    }
}
