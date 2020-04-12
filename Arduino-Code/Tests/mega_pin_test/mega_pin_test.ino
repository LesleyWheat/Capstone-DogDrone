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
int received;

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

  //Motor Forwards
  digitalWrite(motorOp1, HIGH);
  digitalWrite(motorOp2, LOW);

  analogWrite(motorFrontA_PWM, 155);
  analogWrite(motorFrontB_PWM, 155);
  analogWrite(motorRearA_PWM, 155);
  analogWrite(motorRearB_PWM, 155);

  digitalWrite(sonarTrig, LOW);

  
  // put your setup code here, to run once:
  Serial.begin(38400);
  Serial3.begin(9600);
  Serial2.begin(38400);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  while ((Serial.available() <= 0)) {
    Serial.print('A');   // send a capital A
    delay(500);
  }
  Serial.println(' ');
  
  Serial3.println(' ');

}

void loop() {

  while (Serial2.available() > 0) {
    received = Serial2.read();
    Serial.write(received);
  }
  
  while (Serial.available() > 0) {
    received = Serial.read();
    Serial2.write(received);
    Serial.write(received);
  }

}
