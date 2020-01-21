//Header for global functions for 9d0f 
#ifndef POSITION_CLASS_H
#define POSITION_CLASS_H

//External libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>
#include "arduino.h"

//Internal libraries
#include "realTimer.h"
#include "miscFunctions.h"


#define encoderSamples 4


class positionRoutine {
  private:
    Adafruit_FXAS21002C gyro;
    Adafruit_FXOS8700 accelmag;
    sensors_event_t gyro_event;
    sensors_event_t accel_event;
    sensors_event_t mag_event;

    String routineName = "position";
    int debugPrioritySetting;

    //pins
    byte BatterySensorPin;
    byte motorEncoderA_Pin;
    byte motorEncoderB_Pin;

    //Encoder counts
    unsigned int motorEncoderA_count = 0;
    unsigned int motorEncoderB_count = 0;
    
    float encoderA_RPM = 0;
    float encoderB_RPM = 0;
    
    
  public:
    float accelX;
    float accelY;
    float accelZ;
    float accelX_lowpass;
    float accelY_lowpass;
    float accelZ_lowpass;
    void init(void);
    void updatedof(void);
    void updateAccel(float ax, float ay, float az);

    float batteryVoltage = 0;
    double rpmA = 0;
    double rpmB = 0;

    void init(int debugPrioritySetting, byte motorEncoderA_Pin, byte motorEncoderB_Pin);
    void run();
    void encoderA();
    void encoderB();
};

#endif // POSITION_CLASS_H
