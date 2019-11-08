/*
  
*/

//Include functions
#include "loggingFunctions.h"
#include "realTimer.h"


//Declare Objects



//Set input pins


//Set output pins


//global variables for main
byte blinkState = 0;

//gloabl settings variables
int const debugPrioritySetting = 5;

//global objects for main
realTimer blinkTimer;

// the setup function runs once when you press reset or power the board
//Runs only once at setup
void setup() {
  debugPrint(debugPrioritySetting, "setup", 5, "Starting...");
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  blinkTimer.init(1000);
  
  //Initalize main routines
  outputSetup();

  //Finish setup
  debugPrint(debugPrioritySetting, "setup", 5, "Startup complete");
}

// the loop function runs over and over again forever
void loop() {
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
      //debugPrint(5, routineName, 5, String("BlinkState: ") + String(blinkState));
  };

  // Delay so CPU doesn't run at 100% all the time
  delay(50);
  
  //Run main routines
  inputsRead();
  outputWrite();
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
