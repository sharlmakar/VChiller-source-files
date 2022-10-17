#include <Servo.h>          //Servo motor library. This is standard library

#define servo_pin 11
#define LIMIT_SWITCH_PIN 7

Servo servo_motor; //our servo name

void setup() {
  servo_motor.attach(servo_pin); //our servo pin
  pinMode(LIMIT_SWITCH_PIN, INPUT);
}

void loop() {
  if(digitalRead(LIMIT_SWITCH_PIN) == LOW){
    servo_motor.write(0);
  
    delay(2000);
  
    servo_motor.write(359);
  
    delay(2000);
  }
}
