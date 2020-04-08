//Communications module
#include "commRoutine.h"

void commRoutine::init(int debugPrioritySetting, byte rssiInPin, SoftwareSerial* ss){
  //set variables
  this->debugPrioritySetting=debugPrioritySetting;
  this->rssiInPin=rssiInPin;
  mySerial = ss;

  //Starting variables

  
  //create objects
  rssiUpdate.init(rssiUpdatePeriod);
  rssiRead.init(rssiReadPeriod);
  
  // start serial port at preset rate:
  Serial.begin(serialRate);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  establishContact();  // send a byte to establish contact until receiver responds

  //Start xbee serial
  mySerial->begin(serialRate);
  
  //ros::NodeHandle nh;

  //std_msgs::String str_msg;
  //ros::Publisher chatter("chatter", &str_msg);
  
  //char hello[13] = "hello world!";
  //nh.initNode();
  //nh.advertise(chatter);
    
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
  //send out echo signal
  if(rssiUpdate.check(true)){
    mySerial->write("a");
  }

  //Read echo from xbee target
  if (mySerial->available() > 0) {
    int received = mySerial->read();

    
    if(rssiRead.check(true)){
      rssi_raw = ((analogRead(rssiInPin)*1000.0)/1024.0)*5.0;
      debugPrint(5, routineName, 5, String(F("RSSI Raw * 1000: ")) + String(rssi_raw));
    }
  }
  
};
