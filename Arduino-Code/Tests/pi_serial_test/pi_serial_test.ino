//External libraries
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX


void setup() {
  // put your setup code here, to run once:
    // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  //mySerial.println("Hello, world?");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());
}
