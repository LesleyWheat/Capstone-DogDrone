//Header for global functions for 9d0f 
#ifndef POSITION_CLASS_H
#define POSITION_CLASS_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>



class positionRoutine {
  private:
    Adafruit_FXAS21002C gyro;
    Adafruit_FXOS8700 accelmag;
    sensors_event_t gyro_event;
    sensors_event_t accel_event;
    sensors_event_t mag_event;
    
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
};

#endif // POSITION_CLASS_H
