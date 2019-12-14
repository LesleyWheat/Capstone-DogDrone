#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); // RX, TX

int led = 13;
int received = 0;
int i;

int rssi_lowpass = 0;
int rssi_raw = 0;
int rssi_error = 0;
int analogPin = A0;
byte PWM_PIN = 3;
int pwm_value_high;
int pwm_value_low;

void setup() {
  pinMode(PWM_PIN, INPUT);
  
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  //Serial.println("Goodnight moon!\n");
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Hello, world?\n");
  
  pinMode(led, OUTPUT);
}
 
void loop() {
  if (mySerial.available() > 0) {
    received = mySerial.read();

    pwm_value_high = pulseIn(PWM_PIN, HIGH);
    pwm_value_low = pulseIn(PWM_PIN, LOW);
    rssi_raw = analogRead(analogPin)*5.0*1000/1024;

    Serial.println("pwm_value_high " + String(pwm_value_high) + " pwm_value_low " + String(pwm_value_low)+ "rssi_raw " + String(rssi_raw));

    //rssi_raw = analogRead(analogPin)*5.0*1000/1024;
    //rssi_lowpass = (rssi_lowpass*90.0 + rssi_raw*10.0)/100.0;
    //Serial.println("rssi_raw " + String(rssi_raw) + " rssi_lowpass " + String(rssi_lowpass));

    
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
