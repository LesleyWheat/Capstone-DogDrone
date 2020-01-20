/*
  
*/

//External libraries
#include "arduino.h"
#include <SoftwareSerial.h>

//Modules
#include "inputsRoutine.h"
#include "positionRoutine.h"
#include "controlRoutine.h"
#include "diagnosticsRoutine.h"
#include "commRoutine.h"
#include "outputsRoutine.h"

//Include functions
#include "realTimer.h"
#include "loggingFunctions.h"

//Declare Objects
inputsRoutine inputs;
controlRoutine control;
commRoutine comm;
diagnoticsRoutine diagnotics;
outputsRoutine outputs;

//Set input pins
#define batteryMotorPin A1
#define batteryCompPin A2
#define rssiInPin A0
#define encoderAPin 8
#define encoderBPin 9
#define serialRX 3
#define serialTX 2
#define sonerTrig 13

//Set output pins
#define motorIn1 4
#define motorIn2 7
#define motorPWMA 5
#define motorPWMB 6
#define servoPWM 11
#define sonarEcho 12

//Define global objects
SoftwareSerial mySerial(serialRX, serialTX); // RX, TX
realTimer blinkTimer;

//Define system variables
#define targetCycleTime 50

//global variables for main
byte blinkState = 0;
      
//gloabl settings variables
int const debugPrioritySetting = 5;
float filterFrequency = 5;

  
// the setup function runs once when you press reset or power the board
//Runs only once at setup
void setup() {

  //Serial.println(F("Adafruit AHRS Fusion Example"));
  //debugPrint(debugPrioritySetting, "setup", 5, "Starting...");
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  blinkTimer.init(100);
  
  //Initalize main routines
  inputs.init(debugPrioritySetting, batteryCompPin, encoderAPin, encoderBPin);
  control.init(debugPrioritySetting);
  comm.init(debugPrioritySetting);
  diagnotics.init(debugPrioritySetting, targetCycleTime);
  outputs.init(debugPrioritySetting);

  //Finish setup
  debugPrint(debugPrioritySetting, "setup", 5, "Startup complete");
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
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }else{
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  }
  
  if(blinkTimer.check(true)){
      //Next state
      blinkState = ((blinkState + 1)%2);    
  };

  // Delay so CPU doesn't run at 100% all the time
  delay(5);

  
  //Run main routines
  inputs.run();
  control.run(inputs.batteryVoltage, inputs.rpmA, inputs.rpmB);
  comm.run();
  diagnotics.run(inputs.batteryVoltage, inputs.rpmA, inputs.rpmB);
  outputs.run();
}

void inputsRead(){  
  //Read inputs and translate into readable format
  
}

void outputSetup(){
  //Setup output pins

}


void outputWrite(){
  
  //Set outputs
  //Motor state
  
  //Motor PWM

  
}

void printOutPlot(){
   
  //Serial.print(millis());
  Serial.print("Heading: ");
  //Serial.print(inputDOF.heading);
  //Serial.print(" Heading1 ");
  //Serial.print(inputDOF.headingAdj);
  Serial.print(" Pitch: ");
  //Serial.print(inputDOF.pitch);
  Serial.print(" Roll: ");
  //Serial.print(inputDOF.roll);
  Serial.print(" AccelX: ");
  //Serial.print(inputDOF.accelX*10 );
  Serial.print(" AccelY: ");
  //Serial.print(inputDOF.accelY*10 );
  Serial.print(" AccelZ: ");
  //Serial.println(inputDOF.accelZ*10 );
  //Serial.print(" Total Force ");
  //Serial.println(pow(pow(corrected_ax ,2) +pow(corrected_ay,2) +pow(corrected_az,2) , 0.5));
  
}
