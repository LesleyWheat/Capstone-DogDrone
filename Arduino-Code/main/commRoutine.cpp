//Communications module
#include "commRoutine.h"

//Make sure serial is active during setup
void commRoutine::establishContact() {
  contactTimeout.init(60000);
  
  while ((Serial.available() <= 0) & (contactTimeout.check(true) == false)) {
    Serial.print('A');   // send a capital A
    delay(500);
  }
  Serial.println(' ');
}

void commRoutine::init(int debugPrioritySetting){
  //set variables
  this->debugPrioritySetting=debugPrioritySetting;

  //Starting variables

  
  //create objects
  test.init(2000);

  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  establishContact();  // send a byte to establish contact until receiver responds
  
  
};

//main run loop
void commRoutine::run(){
  if(test.check(true)){
    //debugPrint(5, routineName, 5, String("Fuzztest: ") + String(fuzzyNum(1000, 100)));
    //Serial1.println(String("Fuzztest: ") + String(fuzzyNum(1000, 100)));
  }
};
