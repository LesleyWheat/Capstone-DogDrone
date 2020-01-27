/*
  
*/

//External libraries
#include "arduino.h"

//Modules
#include "positionRoutine.h"
#include "controlRoutine.h"
#include "diagnosticsRoutine.h"
#include "commRoutine.h"

//Include functions
#include "realTimer.h"
#include "miscFunctions.h"

//Declare Objects
positionRoutine pos;
controlRoutine control;
commRoutine comm;
diagnoticsRoutine diagnotics;

//Set input pins
#define servoFeedback A2
#define batteryMotorPin A1
#define batteryCompPin A0
#define encoderAPin 2
#define encoderBPin 3
#define sonarTrig 7

//Set output pins
#define motorInPin1 12
#define motorInPin2 13
#define motorPWMA_Pin 9
#define motorPWMB_Pin 10
#define servoPWM_Pin 11
#define sonarEchoPin 4

//Define global objects
realTimer blinkTimer;
realTimer echoTimer;

//Define system variables
#define DEBUG 1
#define WIRED 1
#define blinkPeriod 1000
#define echoPeriod 5000
// 5 is all msgs, higher filters out lower priority msgs
#define debugPrioritySetting 5

//global variables for main
byte blinkState = 0;
byte echoFlag = 0;
int echoDuration = 0;

//Variables for interupt
volatile byte encoderAPinState = LOW;
volatile byte encoderBPinState = LOW;
  
// the setup function runs once when you press reset or power the board
//Runs only once at setup
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
  pinMode(servoFeedback, INPUT);
  pinMode(encoderAPin, INPUT);
  pinMode(encoderBPin, INPUT);
  pinMode(sonarEchoPin, INPUT);

  //Outputs
  pinMode(motorInPin1, OUTPUT);
  pinMode(motorInPin2, OUTPUT);
  pinMode(motorPWMA_Pin, OUTPUT);
  pinMode(motorPWMB_Pin, OUTPUT);
  pinMode(servoPWM_Pin, OUTPUT);
  pinMode(sonarTrig, OUTPUT);

  //Motor setup
  digitalWrite(motorInPin1, HIGH);
  //digitalWrite(motorInPin1, LOW);
  digitalWrite(motorInPin2, LOW);

  //Declare objects
  blinkTimer.init(blinkPeriod);
  echoTimer.init(echoPeriod);

  //Initalize main routines
  comm.init(debugPrioritySetting);
  pos.init(debugPrioritySetting);
  control.init(debugPrioritySetting, motorInPin1, motorInPin2, motorPWMA_Pin, motorPWMB_Pin, servoPWM_Pin);
  diagnotics.init(debugPrioritySetting, batteryCompPin, batteryMotorPin);

  //Setup interupts
  attachInterrupt(digitalPinToInterrupt(encoderAPin), encoderInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderBPin), encoderInterrupt, RISING);
  
  //Finish setup
  debugPrint(debugPrioritySetting, F("SET"), 5, F("Startup complete"));
}

// the loop function runs over and over again forever
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
      //blinkState = ((blinkState + 1)%2);    
  };

  //Run echo on conditions
  if(echoTimer.check(true) || echoFlag == 1){
    if (echoFlag == 0){
      digitalWrite(sonarTrig, HIGH);
      echoFlag = 1;
    }else{
      echoFlag = 0;
      digitalWrite(sonarTrig, LOW);
      echoDuration = pulseIn(sonarEchoPin, HIGH, 10000);
      debugPrint(debugPrioritySetting, F("MAI"), 5, String(F("Echo duration: ")) + String(echoDuration));
    }
  }

  
  //Run main routines
  pos.run();
  control.run(pos.rpmA, pos.rpmB);
  comm.run();
  diagnotics.run();
  //Diagnostics includes delay to target cycle time
}


//Encoder interrupt routines
void encoderInterrupt(void){
  if(digitalRead(encoderAPin) != encoderAPinState){
    encoderAPinState = digitalRead(encoderAPin);
    //debugPrint(debugPrioritySetting, F("MAI"), 5, String(F("Interupt A triggered")));
    pos.motorEncoderA_count++;
  }
  
  if(digitalRead(encoderBPin) != encoderBPinState){
    encoderBPinState = digitalRead(encoderBPin);
    //debugPrint(debugPrioritySetting, F("MAI"), 5, String(F("Interupt B triggered")));
    pos.motorEncoderB_count++;
  }
  
}


//Additional I/O
