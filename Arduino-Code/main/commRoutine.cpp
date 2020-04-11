//Communications module
#include "commRoutine.h"

void commRoutine::init(int debugPrioritySetting, byte rssiInPin){
  //set variables
  this->debugPrioritySetting=debugPrioritySetting;
  this->rssiInPin=rssiInPin;

  //Starting variables

  
  //create objects
  rssiUpdate.init(rssiUpdatePeriod);
  rssiRead.init(rssiReadPeriod);

  // serial for arduino connection
  //serial serial1 for xbee
  // both at serialrate
  
  // start serial port at preset rate:
  Serial.begin(serialRate);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //Start serial port
  Serial1.begin(serialRate);
  
  //establishContact();  // send a byte to establish contact until receiver responds
    
};

//Make sure serial is active during setup
void commRoutine::establishContact() {
  
  contactTimeout.init(contactTimeoutPeriod);
  
  while ((Serial.available() <= 0) & (contactTimeout.check(true) == false)) {
    Serial.print('A');   // send a capital A
    delay(500);
  }
  Serial.println(' ');
}

//main run loop
void commRoutine::run(){
  //Read from pi to xbee
  while (Serial.available() > 0){
    received = Serial.read();
    Serial1.write(received);
    Serial.write(received);

    if(rssiRead.check(true)){
      rssi_raw = ((analogRead(rssiInPin)*1000.0)/1024.0)*5.0;
      debugPrint(5, routineName, 5, String(F("RSSI Raw * 1000: ")) + String(rssi_raw));
    }
  }

  //Read from xbee to pi
  while (Serial1.available() > 0){
    received = Serial1.read();
    Serial.write(received);
  }
  
};
