//Communications module
#include "commRoutine.h"

//Make sure serial is active during setup
void commRoutine::establishContact() {
  
  contactTimeout.init(contactTimeoutPeriod);
  
  while ((Serial.available() <= 0) & (contactTimeout.check(true) == false)) {
    Serial.print('A');   // send a capital A
    delay(500);
  }
  Serial.println(' ');
}

void commRoutine::init(int debugPrioritySetting, byte rssiInPin, SoftwareSerial* ss){
  //set variables
  this->debugPrioritySetting=debugPrioritySetting;
  this->rssiInPin=rssiInPin;
  mySerial = ss;

  //Starting variables

  
  //create objects
  rssiUpdate.init(rssiUpdatePeriod);

  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  establishContact();  // send a byte to establish contact until receiver responds

  //Start xbee serial
  mySerial->begin(9600);
  
  //ros::NodeHandle nh;

  //std_msgs::String str_msg;
  //ros::Publisher chatter("chatter", &str_msg);
  
  //char hello[13] = "hello world!";
  //nh.initNode();
  //nh.advertise(chatter);
    
};

//main run loop
void commRoutine::run(){
  //send out echo signal
  if(rssiUpdate.check(true)){
    mySerial->write("a");
  }

  //Read echo from xbee target
  if (mySerial->available() > 0) {
    int received = mySerial->read();
    
    rssi_raw = analogRead(rssiInPin)*100.0*5.0/1024.0;
    
    debugPrint(5, routineName, 5, String(F("RSSI Raw * 100: ")) + String(rssi_raw));
    
  }
  
};
