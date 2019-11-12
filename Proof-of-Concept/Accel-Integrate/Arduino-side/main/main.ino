/*
  
*/

//Include functions
#include <ros.h>
#include <std_msgs/String.h>
#include "realTimer.h"
#include "input_9dof.h"
#include "arduino.h"
#include "loggingFunctions.h"

//ros::NodeHandle nh;

//std_msgs::String str_msg;
//ros::Publisher chatter("chatter", &str_msg);

//char hello[13] = "hello world!";

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

  
// the setup function runs once when you press reset or power the board
//Runs only once at setup
void setup() {
  //nh.initNode();
  //nh.advertise(chatter);
  
  Serial.begin(115200);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
  //Serial.println(F("Adafruit AHRS Fusion Example"));
  //debugPrint(debugPrioritySetting, "setup", 5, "Starting...");
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  blinkTimer.init(100);
  
  //Initalize main routines
  outputSetup();
  inputDOF.init();

  //Finish setup
  //debugPrint(debugPrioritySetting, "setup", 5, "Startup complete");
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
      //debugPrint(5, routineName, 5, String("BlinkState: ") + String(blinkState));
      //corrected_ax = lowpassX.output() + 9.81*sin(inputDOF.pitch); 
      //corrected_ay = lowpassY.output() + 9.81*sin(inputDOF.roll); 
      //corrected_az = lowpassZ.output() + 9.81*cos(inputDOF.roll)*cos(inputDOF.pitch) ;
      corrected_ax = 0.9*corrected_ax + 0.1*(inputDOF.accelX- 0.42);
      corrected_ay = 0.9*corrected_ay + 0.1*(inputDOF.accelY + 0.46);
      corrected_az = 0.9*corrected_az + 0.1*(inputDOF.accelZ - 0.46);

       
  
      sumX = sumX + (100.0/1000)*abs(corrected_ax)*corrected_ax/2.0;
      sumY = sumY + (100.0/1000)*abs(corrected_ay)*corrected_ay/2.0;
      sumZ = sumZ + (100.0/1000)*abs(corrected_az)*corrected_az/2.0;
  
      printOutPlot();
      
  };

  // Delay so CPU doesn't run at 100% all the time
  delay(5);

  
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
  Serial.print("Heading: ");
  Serial.print(inputDOF.heading);
  //Serial.print(" Heading1 ");
  //Serial.print(inputDOF.headingAdj);
  Serial.print(" Pitch: ");
  Serial.print(inputDOF.pitch);
  Serial.print(" Roll: ");
  Serial.print(inputDOF.roll);
  Serial.print(" AccelX: ");
  Serial.print(inputDOF.accelX*10 );
  Serial.print(" AccelY: ");
  Serial.print(inputDOF.accelY*10 );
  Serial.print(" AccelZ: ");
  Serial.println(inputDOF.accelZ*10 );
  //Serial.print(" Total Force ");
  //Serial.println(pow(pow(corrected_ax ,2) +pow(corrected_ay,2) +pow(corrected_az,2) , 0.5));
  
}
