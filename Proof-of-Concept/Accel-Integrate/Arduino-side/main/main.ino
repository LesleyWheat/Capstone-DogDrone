/*
  
*/

//Include functions
#include "loggingFunctions.h"
#include "realTimer.h"
#include "input_9dof.h"
#include "Filters.h"
#include "FilterOnePole.h"
#include "arduino.h"

//Declare Objects
dof inputDOF;


//Set input pins


//Set output pins


//global variables for main
byte blinkState = 0;
float sumX = 0;
float sumY = 0;
float sumZ = 0;
float corrected_ax; 
float corrected_ay; 
float corrected_az; 
      
//gloabl settings variables
int const debugPrioritySetting = 5;
float filterFrequency = 5;

//global objects for main
realTimer blinkTimer;
FilterOnePole lowpassX(LOWPASS, filterFrequency);
FilterOnePole lowpassY(LOWPASS, filterFrequency);
FilterOnePole lowpassZ(LOWPASS, filterFrequency);

  
// the setup function runs once when you press reset or power the board
//Runs only once at setup
void setup() {
  //Serial.begin(115200);
  debugPrint(debugPrioritySetting, "setup", 5, "Starting...");
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  blinkTimer.init(100);
  
  //Initalize main routines
  outputSetup();
  inputDOF.init();

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
      //corrected_ax = lowpassX.output() + 9.81*sin(inputDOF.pitch); 
      //corrected_ay = lowpassY.output() + 9.81*sin(inputDOF.roll); 
      //corrected_az = lowpassZ.output() + 9.81*cos(inputDOF.roll)*cos(inputDOF.pitch) ;
      corrected_ax = 0.9*corrected_ax + 0.1*(inputDOF.accelX- 0.42);
      corrected_ay = 0.9*corrected_ay + 0.1*(inputDOF.accelY + 0.46);
      corrected_az = 0.9*corrected_az + 0.1*(inputDOF.accelZ -0.46 );

       
  
      sumX = sumX + (100.0/1000)*abs(corrected_ax)*corrected_ax/2.0;
      sumY = sumY + (100.0/1000)*abs(corrected_ay)*corrected_ay/2.0;
      sumZ = sumZ + (100.0/1000)*abs(corrected_az)*corrected_az/2.0;
  
      printOutPlot();
      
  };

  // Delay so CPU doesn't run at 100% all the time
  delay(5);

  lowpassX.input( inputDOF.accelX );
  lowpassY.input( inputDOF.accelY );
  lowpassZ.input( inputDOF.accelZ );

  
  //Run main routines
  inputsRead();
  inputDOF.updatedof();
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

void printOutPlot(){
   
  //Serial.print(millis());
  Serial.print("Orientation:Heading ");
  Serial.print(inputDOF.heading);
  Serial.print(" Heading1 ");
  Serial.print(inputDOF.headingAdj);
  Serial.print(" Pitch ");
  Serial.print(inputDOF.pitch);
  Serial.print(" Roll ");
  Serial.print(inputDOF.roll);
  Serial.print(" AccelX ");
  Serial.print(corrected_ax );
  Serial.print(" AccelY ");
  Serial.print(corrected_ay );
  Serial.print(" AccelZ ");
  Serial.print(corrected_az );
  Serial.print(" Total Force ");
  Serial.println(pow(pow(corrected_ax ,2) +pow(corrected_ay,2) +pow(corrected_az,2) , 0.5));
  
}
