/*
  
*/

//External libraries
#include <ros.h>
#include <std_msgs/String.h>
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

//ros::NodeHandle nh;

//std_msgs::String str_msg;
//ros::Publisher chatter("chatter", &str_msg);

//char hello[13] = "hello world!";

//Declare Objects
inputsRoutine inputs;
controlRoutine control;
commRoutine comm;
diagnoticsRoutine diagnotics;
outputsRoutine outputs;

SoftwareSerial mySerial(3, 2); // RX, TX

//Set input pins
#define batteryMotorPin A1
#define batteryCompPin A2
#define rssiInPin A0
#define encoderAPin 8
#define encoderBPin 9

//Set output pins
//#define 

//Define system variables
#define targetCycleTime 50

//global variables for main
byte blinkState = 0;
      
//gloabl settings variables
int const debugPrioritySetting = 5;
float filterFrequency = 5;

//global objects for main
realTimer blinkTimer;

  
// the setup function runs once when you press reset or power the board
//Runs only once at setup
void setup() {
  //nh.initNode();
  //nh.advertise(chatter);

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
