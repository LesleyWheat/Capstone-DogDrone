//Header for global functions for 9d0f
#ifndef DOF_CLASS_H
#define DOF_CLASS_H

#include <Madgwick.h>
#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>

class dof{
  private:
    Madgwick filter;
    float mag_offsets[3];
    float mag_softiron_matrix[3][3];
    float mag_field_strength;
    float gyro_zero_offsets[3];
    Adafruit_FXAS21002C gyro;
    Adafruit_FXOS8700 accelmag;
    
  public:
    double heading;
    double pitch;
    double roll;
    void init(void);
    void update(void);
    void printOutPlot(void);
};

#endif // DOF_CLASS_H
