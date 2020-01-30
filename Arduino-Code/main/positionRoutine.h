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
#define speedCheckPeriod 250
#define speedPrintOutPeriod 1000
#define rpmLowBound 25
#define IMUUpdatePeriod 50

class positionRoutine {
  private:
    //Private variables

    //Standard variables
    String routineName = "POS";
    int debugPrioritySetting;
    byte posPrintFlag = 0;

    //IMU objects
    Adafruit_FXAS21002C gyro;
    Adafruit_FXOS8700 accelmag;
    sensors_event_t gyro_event;
    sensors_event_t accel_event;
    sensors_event_t mag_event;

    //Calibration settings for IMU
    // Offsets applied to raw x/y/z mag values
    float mag_offsets[3]            = { 9.48F, -20.76F, -251.61F };
    // Soft iron error compensation matrix
    float mag_softiron_matrix[3][3] = { {  0.982,  -0.012,  0.019 },
                                        {  -0.012,  0.973, 0.013 },
                                        {  0.019, 0.013,  1.048 } };
    
    float mag_field_strength        = 39.48F;
    // Offsets applied to compensate for gyro zero-drift error for x/y/z
    float gyro_zero_offsets[3]      = { 0.0F, 0.0F, 0.0F };


    //pins
    byte motorEncoderA_Pin;
    byte motorEncoderB_Pin;

    //Encoder counts
    unsigned long enA_lastCount = 0;
    unsigned long enA_tempCount = 0;
    unsigned long enB_lastCount = 0;
    unsigned long enB_tempCount = 0;
    float timePeriod = 0;
    unsigned long lastCountCheckTime = 0;

    //timers
    realTimer speedUpdateTimer;
    realTimer speedPrintOut;
    realTimer IMUUpdate;

    //Private functions
    void updatedof(void);
    void updateSpeed(void);
    void updateAccel(void);
    
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
    volatile unsigned long motorEncoderA_count = 0;
    volatile unsigned long motorEncoderB_count = 0;
    double rpmA = 0;
    double rpmB = 0;

    //Public functions
    void init(int debugPrioritySetting);
    void run();

};

#endif // POSITION_CLASS_H
