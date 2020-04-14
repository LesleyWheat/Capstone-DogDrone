//Header here
#include "positionRoutine.h"

void positionRoutine::init(int debugPrioritySetting){
  //Set local variables
  this->debugPrioritySetting=debugPrioritySetting;

  //Set starting variables
  
  //Create objects
  speedUpdateTimer.init(speedCheckPeriod);
  speedPrintOut.init(speedPrintOutPeriod);
  IMUUpdate.init(IMUUpdatePeriod);

  gyro = Adafruit_FXAS21002C(0x0021002C);
  accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);
  
  if(!gyro.begin()){
    /* There was a problem detecting the gyro ... check your connections */
    Serial.println(F("Ooops, no gyro detected ... Check your wiring!"));
    //while(1);
  }
  
  if(!accelmag.begin(ACCEL_RANGE_4G)){
    Serial.println(F("Ooops, no FXOS8700 detected ... Check your wiring!"));
    //while(1);
  }
  
}

//-------------------------------------------------------------------------------------------------------
void positionRoutine::updatedof(void){
  // Mag calibration values are calculated via ahrs_calibration.
  // These values must be determined for each baord/environment.
  // See the image in this sketch folder for the values used
  // below.
  
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
   
                
}

//-------------------------------------------------------------------------------------------------
void positionRoutine::updateAccel(void){
  accelX_lowpass = accelX/2 + accelX_lowpass/2;
  accelY_lowpass = accelY/2 + accelY_lowpass/2;
  accelZ_lowpass = accelZ/2 + accelZ_lowpass/2;
}

//-------------------------------------------------------------------------------------------------
void positionRoutine::updateSpeed(void){
  enFrontA_tempCount = (unsigned long)(motorEncoderFrontA_count - enFrontA_lastCount);
  enFrontB_tempCount = (unsigned long)(motorEncoderFrontB_count - enFrontB_lastCount);
  enRearA_tempCount = (unsigned long)(motorEncoderRearA_count - enRearA_lastCount);
  enRearB_tempCount = (unsigned long)(motorEncoderRearB_count - enRearB_lastCount);

  timePeriod = (unsigned long)(micros() - lastCountCheckTime)/(1000000.0);
  

  rpmFrontA = (enFrontA_tempCount*60.0)/(timePeriod * encoderPPR)*(9.0/10) + rpmFrontA*(1.0/10) ;
  rpmFrontB = (enFrontB_tempCount*60.0)/(timePeriod * encoderPPR)*(9.0/10) + rpmFrontB*(1.0/10) ;
  rpmRearA = (enRearA_tempCount*60.0)/(timePeriod * encoderPPR)*(9.0/10) + rpmRearA*(1.0/10) ;
  rpmRearB = (enRearB_tempCount*60.0)/(timePeriod * encoderPPR)*(9.0/10) + rpmRearB*(1.0/10) ;

  //debugPrint(5, routineName, 5, String(F("rpmA: ")) + String(rpmA));

  //if(rpmA < rpmLowBound) rpmA = 0;
  //if(rpmB < rpmLowBound) rpmB = 0;

  lastCountCheckTime = micros();
  enFrontA_lastCount = motorEncoderFrontA_count;
  enFrontB_lastCount = motorEncoderFrontB_count;
  enRearA_lastCount = motorEncoderRearA_count;
  enRearB_lastCount = motorEncoderRearB_count;
}

//----------------------------------------------------------------------------------------------
//runs in main loop
void positionRoutine::run(){
  if(IMUUpdate.check(true)){
    updatedof();
    updateAccel();
  }
  
  if(speedUpdateTimer.check(true)){
    updateSpeed();
  }
  
  if(speedPrintOut.check(true)){
    if(posPrintFlag==0) {debugPrint(5, routineName, 5, String(F("rpmA: ")) + String(rpmFrontA) + String(F(" rpmB: ")) + String(rpmFrontB));};
    //if(posPrintFlag==0) {debugPrint(5, routineName, 5, String(F("en A count: ")) + String(motorEncoderA_count) + String(F(" en B count: ")) + String(motorEncoderB_count));};

    if(accelmag.begin(ACCEL_RANGE_4G)){
    if(posPrintFlag==1) {debugPrint(5, routineName, 5, String(F("ax: ")) + String(accelX_lowpass));};
    
    if(posPrintFlag==2) {debugPrint(5, routineName, 5, String(F("ay: ")) + String(accelY_lowpass));};
    
    if(posPrintFlag==3) {debugPrint(5, routineName, 5, String(F("az: ")) + String(accelZ_lowpass));};
    
    posPrintFlag = (posPrintFlag +1)%4;
    }
  }
}
