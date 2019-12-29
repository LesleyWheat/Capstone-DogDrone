int received = 0;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  //Serial.begin(9600);
  //while (!Serial) {
    //; // wait for serial port to connect. Needed for Native USB only
  //}
 while ((Serial1.available() <= 0) ) {
        Serial1.print('A');   // send a capital A
        delay(500);
      }
      Serial1.println(' ');
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available() > 0){
      received = Serial1.read();
      
      Serial1.write(received);
   }
   //if (Serial.available() > 0){
      //received = Serial.read();
      
      //Serial1.write(received);
   //}
}
