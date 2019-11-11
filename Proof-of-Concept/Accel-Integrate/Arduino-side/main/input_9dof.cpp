#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Madgwick.h>
#include "arduino.h"
#include "input_9dof.h"

void dof::init(){
  
  Serial.begin(115200);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
  Serial.println(F("Adafruit AHRS Fusion Example"));

  gyro = Adafruit_FXAS21002C(0x0021002C);
  accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);
  
  if(!gyro.begin()){
    /* There was a problem detecting the gyro ... check your connections */
    Serial.println("Ooops, no gyro detected ... Check your wiring!");
    while(1);
  }
  
  if(!accelmag.begin(ACCEL_RANGE_4G)){
    Serial.println("Ooops, no FXOS8700 detected ... Check your wiring!");
    while(1);
  }

  filter.begin(10);
  
}

void dof::updatedof(){
  // Mag calibration values are calculated via ahrs_calibration.
  // These values must be determined for each baord/environment.
  // See the image in this sketch folder for the values used
  // below.
  
// Offsets applied to raw x/y/z mag values
float mag_offsets[3]            = { 8.16F, -21.02F, -249.24F };

// Soft iron error compensation matrix
float mag_softiron_matrix[3][3] = { {  1.014,  -0.028,  0.002 },
                                    {  -0.029,  0.974, 0.026 },
                                    {  0.002, 0.026,  1.015 } };

float mag_field_strength        = 61.66F;

// Offsets applied to compensate for gyro zero-drift error for x/y/z
float gyro_zero_offsets[3]      = { 0.0F, 0.0F, 0.0F };

  // Get new data samples
  gyro.getEvent(&gyro_event);
  accelmag.getEvent(&accel_event, &mag_event);

  
  // Apply mag offset compensation (base values in uTesla)
  float x = mag_event.magnetic.x - mag_offsets[0];
  float y = mag_event.magnetic.y - mag_offsets[1];
  float z = mag_event.magnetic.z - mag_offsets[2];

  // Apply mag soft iron error compensation
  float mx = x * mag_softiron_matrix[0][0] + y * mag_softiron_matrix[0][1] + z * mag_softiron_matrix[0][2];
  float my = x * mag_softiron_matrix[1][0] + y * mag_softiron_matrix[1][1] + z * mag_softiron_matrix[1][2];
  float mz = x * mag_softiron_matrix[2][0] + y * mag_softiron_matrix[2][1] + z * mag_softiron_matrix[2][2];

  // Apply gyro zero-rate error compensation
  float gx = gyro_event.gyro.x + gyro_zero_offsets[0];
  float gy = gyro_event.gyro.y + gyro_zero_offsets[1];
  float gz = gyro_event.gyro.z + gyro_zero_offsets[2];

  // The filter library expects gyro data in degrees/s, but adafruit sensor
  // uses rad/s so we need to convert them first (or adapt the filter lib
  // where they are being converted)
  gx *= 57.2958F;
  gy *= 57.2958F;
  gz *= 57.2958F;

  //acceleration
  accelX = accel_event.acceleration.x;
  accelY = accel_event.acceleration.y;
  accelZ = accel_event.acceleration.z;

  // Update the filter
  filter.update(gx, gy, gz,
                accelX, accelY, accelZ,
                mx, my, mz);

  roll = filter.getRoll();
  pitch = filter.getPitch();
  heading = filter.getYaw();
  
  headingAdj = heading - 72;
  if (headingAdj < 0) {headingAdj = headingAdj + 360;};
  //headingAdj = 360-headingAdj;
   
                
}

void dof::updateAccel(float ax, float ay, float az){
  accelX_lowpass = ax;
  accelY_lowpass = ay;
  accelZ_lowpass = az;
}
