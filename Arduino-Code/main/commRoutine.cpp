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
  Serial2.begin(serialRate);
  
  //establishContact();  // send a byte to establish contact until receiver responds

  Serial2.println("ubuntu");
  delay(3000);
  Serial2.println("pipipi4");
    
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
  
  //Read from computer to pi
  while (Serial.available() > 0){
    received = Serial.read();
    msg[msgCount] = received;

    if(rssiRead.check(true)){
      rssi_raw = ((analogRead(rssiInPin)*1000.0)/1024.0)*5.0;
      //debugPrint(5, routineName, 5, String(F("RSSI Raw * 1000: ")) + String(rssi_raw));
    }
    
    if (msg[msgCount] == 10 || msgCount > 99){
      if (msg[0] == 'x' && msg[1] == 'x'){
          angle_set = (msg[3]-48)*100+(msg[4]-48)*10+(msg[5]-48)*1;
          rpm_set = (msg[7]-48)*100+(msg[8]-48)*10+(msg[9]-48)*1;
          Serial.write(msg);
          debugPrint(5, routineName, 5, String(F("Angle Set: ")) + String(angle_set));
          debugPrint(5, routineName, 5, String(F("RPM Set: ")) + String(rpm_set));
      }else{
        Serial2.write(msg);
      }
      memset(&msg[0], 0, sizeof(msg));
      msgCount = 0;
    }else{
      msgCount++;
    }
    
  }
  

  //Read from pi to computer
  while (Serial2.available() > 0){
    received = Serial2.read();
    msgPi[msgCountPi] = received;
    
    if (msgPi[msgCountPi] == 10 || msgCountPi > 99){
      if (msgPi[0] == 'x' && msgPi[1] == 'x'){
          angle_set = (msgPi[3]-48)*100+(msgPi[4]-48)*10+(msgPi[5]-48)*1;
          rpm_set = (msgPi[7]-48)*100+(msgPi[8]-48)*10+(msgPi[9]-48)*1;
          Serial.write(msgPi);
          debugPrint(5, routineName, 5, String(F("Angle Set: ")) + String(angle_set));
          debugPrint(5, routineName, 5, String(F("RPM Set: ")) + String(rpm_set));
      }else{
        Serial.write(msgPi);
      }
      memset(&msgPi[0], 0, sizeof(msgPi));
      msgCountPi = 0;
    }else{
      msgCountPi++;
    }
  }
  
};
