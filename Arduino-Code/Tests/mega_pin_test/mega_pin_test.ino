//External libraries
#include "arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>

//PIN MAP
//Battery voltage
#define batteryMotorPin A1
#define batteryCompPin A3

//Xbee breakout
//Serial3
#define rssiInPin A0
#define xbeeRX 15                                                                                                                                  
#define xbeeTX 14

//IMU
#define imuSCL 21
#define imuSDA 20

//Ultrasonic
#define sonarEchoPin 7
#define sonarTrig 6

//Rpi serial2
#define piRX 17
#define piTX 16

//Motor Driver
#define motorOp1 12
#define motorOp2 13
#define motorFrontA_PWM 9
#define motorFrontB_PWM 8
#define motorRearA_PWM 11
#define motorRearB_PWM 10

//Motor Encoders
#define motorFrontA_Encoder 3
#define motorFrontB_Encoder 2
#define motorRearA_Encoder 19
#define motorRearB_Encoder 18

//Servo pins

//VARIABLES
//global variables for main
byte blinkState = 0;
byte echoFlag = 0;
float batCV = 0;
float batMV = 0;


//IMU objects
    Adafruit_FXAS21002C gyro;
    Adafruit_FXOS8700 accelmag;
    sensors_event_t gyro_event;
    sensors_event_t accel_event;
    sensors_event_t mag_event;

void setup() {
  //Pin setup
  pinMode(motorFrontA_PWM, OUTPUT);
  pinMode(motorFrontB_PWM, OUTPUT);
  pinMode(motorRearA_PWM, OUTPUT);
  pinMode(motorRearB_PWM, OUTPUT);
  
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  while ((Serial.available() <= 0)) {
    Serial.print('A');   // send a capital A
    delay(500);
  }
  Serial.println(' ');
  
  Serial2.begin(9600);
  Serial3.begin(9600);

  digitalWrite(motorOp1, LOW);
  digitalWrite(motorOp2, HIGH);

  analogWrite(motorFrontA_PWM, 155);
  analogWrite(motorFrontB_PWM, 155);
  analogWrite(motorRearA_PWM, 155);
  analogWrite(motorRearB_PWM, 155);

  digitalWrite(sonarTrig, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!gyro.begin()){
    /* There was a problem detecting the gyro ... check your connections */
    Serial.println(F("Ooops, no gyro detected ... Check your wiring!"));
  }
  
  if(!accelmag.begin(ACCEL_RANGE_4G)){
    Serial.println(F("Ooops, no FXOS8700 detected ... Check your wiring!"));
  }


  Serial.println("Computer battery voltage: " + String(5.0*analogRead(batteryCompPin)/1024.0));
  delay(10);
  Serial.println("Motor battery voltage: " + String(5.0*analogRead(batteryMotorPin)/1024.0));
  delay(10);
  Serial.println("RSSI voltage: " + String(5.0*analogRead(rssiInPin)/1024.0));

  //Sonar
  Serial.println("sonar echo: " + String(digitalRead(sonarEchoPin)));
  digitalWrite(sonarTrig, HIGH);
  delay(100);
  digitalWrite(sonarTrig, LOW);
  Serial.println("sonar echo: " + String(digitalRead(sonarEchoPin)));
  Serial.println("Echo duration: " + String(pulseIn(sonarEchoPin, HIGH)));

  delay(1000);
}
