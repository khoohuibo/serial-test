#include <Servo.h>

Servo myservo;


char IncomingData[20];

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  if(Serial.available()){
    char y;
    int counter = 0;
    while((y = Serial.read()) != ':'){
      if (y != -1){
        IncomingData[counter] = y;
        ++ counter;       
      }
    }
    Serial.print(IncomingData);
    Serial.write(IncomingData);
    delay(200);
    
  }
  // put your main code here, to run repeatedly:

}
