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
#define encoderPPR 120
#define speedCheckPeriod 10
#define speedPrintOutPeriod 4000

class positionRoutine {
  private:
    //Private variables

    //Standard variables
    String routineName = "position";
    int debugPrioritySetting;

    //IMU objects
    Adafruit_FXAS21002C gyro;
    Adafruit_FXOS8700 accelmag;
    sensors_event_t gyro_event;
    sensors_event_t accel_event;
    sensors_event_t mag_event;

    //pins
    byte motorEncoderA_Pin;
    byte motorEncoderB_Pin;

    //Encoder counts
    unsigned long enA_lastCount = 0;
    unsigned long enA_tempCount = 0;
    unsigned long enB_lastCount = 0;
    unsigned long enB_tempCount = 0;
    unsigned long lastCountCheckTime = 0;

    //timers
    realTimer speedUpdateTimer;
    realTimer speedPrintOut;
    

    //Private functions
    
    
  public:
    //Public variables

    //IMU
    float accelX;
    float accelY;
    float accelZ;
    float accelX_lowpass;
    float accelY_lowpass;
    float accelZ_lowpass;
    
    //encoder
    unsigned long motorEncoderA_count = 0;
    unsigned long motorEncoderB_count = 0;
    float rpmA = 0;
    float rpmB = 0;

    //Public functions
    void init(int debugPrioritySetting);
    void run();
    void updatedof(void);
    void updateAccel(float ax, float ay, float az);
    void updateSpeed();

};

#endif // POSITION_CLASS_H
