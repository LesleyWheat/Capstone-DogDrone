

#define serialRate 38400

int led = 13;
int received = 0;
int i;

int rssi_lowpass = 0;
float rssi_raw = 0;
float rssi_raw_first =0;
float rssi_raw_second =0;
float rssi_raw_third =0;
int rssi_error = 0;
int analogPin = A1;
byte PWM_PIN = 3;
int pwm_value_high;
int pwm_value_low;



void setup() {
  pinMode(PWM_PIN, INPUT);
  
  Serial.begin(serialRate); 
  Serial1.begin(serialRate); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  //while ((Serial.available() <= 0) ) {
        //Serial.print('B');   // send a capital A
        //delay(500);
      //}
      //Serial.println(' ');


  Serial.println("Hello, world\n");
  
  pinMode(led, OUTPUT);
}
 
void loop() {
  while (Serial1.available() > 0) {
    received = Serial1.read();
    Serial.write(received);
  }
  while (Serial.available() > 0) {
    received = Serial.read();
    Serial1.write(received);
  }


}
