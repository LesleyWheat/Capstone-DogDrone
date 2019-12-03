#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); // RX, TX

int led = 13;
int received = 0;
int i;
void setup() {
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  Serial.println("Goodnight moon!\n");
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Hello, world?\n");
  
  pinMode(led, OUTPUT);
}
 
void loop() {
  if (mySerial.available() > 0) {
    received = mySerial.read();
    
    Serial.write(received);
  }

  if (Serial.available() > 0){
      received = Serial.read();
      
      mySerial.write(received);
      
      
   }
    
    if (received == 'a'){
      digitalWrite(led, HIGH);
    } else if (received == 'b'){
      digitalWrite(led, LOW);
    } 
}
