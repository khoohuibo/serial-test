#include <Servo.h>

Servo myservo;
char incomingByte[20];

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  if(Serial.available() > 0){
    char y = Serial.read();
    int counter = 0;
    if (y != -1 and y != 0){
        incomingByte[counter] = y;
        ++ counter;   
        }
    Serial.write(incomingByte);
    memset(&incomingByte[0], 0, sizeof(incomingByte));
    delay(200);
    Serial.flush();  
  }
  
}
