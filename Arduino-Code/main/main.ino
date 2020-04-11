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
#define motorFrontA_Encoder 3
#define motorFrontB_Encoder 2
#define motorRearA_Encoder 19
#define motorRearB_Encoder 18


//OUTPUT
#define motorOp1 12
#define motorOp2 13
#define sonarTrig 6

//PWM
#define motorFrontA_PWM 9
#define motorFrontB_PWM 8
#define motorRearA_PWM 11
#define motorRearB_PWM 10


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
  pinMode(motorFrontA_Encoder, INPUT);
  pinMode(motorFrontB_Encoder, INPUT);
  pinMode(sonarEchoPin, INPUT);

  //Outputs
  pinMode(motorOp1, OUTPUT);
  pinMode(motorOp2, OUTPUT);
  pinMode(sonarTrig, OUTPUT);
  
  pinMode(motorFrontA_PWM, OUTPUT);
  pinMode(motorFrontB_PWM, OUTPUT);

  //Motor setup
  digitalWrite(motorOp1, HIGH);
  //digitalWrite(motorOp1, LOW);
  digitalWrite(motorOp2, LOW);

  //Declare objects for loop routine
  blinkTimer.init(blinkPeriod);
  echoTimer.init(echoPeriod);

  //Initalize main routines
  comm.init(debugPrioritySetting, rssiInPin);
  pos.init(debugPrioritySetting);
  control.init(debugPrioritySetting, motorOp1, motorOp2, motorFrontA_PWM, motorFrontB_PWM);
  diagnotics.init(debugPrioritySetting, batteryCompPin, batteryMotorPin);

  //Setup interupts
  attachInterrupt(digitalPinToInterrupt(motorFrontA_Encoder), encoderInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(motorFrontB_Encoder), encoderInterrupt, RISING);
  
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

  //Change led state to show board is running
  if(blinkState == 0){
    //If this runs then it stops the motor
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }else{
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  }
  
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
  control.run(pos.rpmA, pos.rpmB);
  comm.run();
  diagnotics.run();
  //Diagnostics includes delay to target cycle time
}

//-----------------------------------------------------------------------------------------

//Encoder interrupt routines
void encoderInterrupt(void){
  if(digitalRead(motorFrontA_Encoder) != motorFrontA_EncoderState){
    motorFrontA_EncoderState = digitalRead(motorFrontA_Encoder);
    //debugPrint(debugPrioritySetting, F("MAI"), 5, String(F("Interupt A triggered")));
    pos.motorEncoderA_count++;
  }
  
  if(digitalRead(motorFrontB_Encoder) != motorFrontB_EncoderState){
    motorFrontB_EncoderState = digitalRead(motorFrontB_Encoder);
    //debugPrint(debugPrioritySetting, F("MAI"), 5, String(F("Interupt B triggered")));
    pos.motorEncoderB_count++;
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
