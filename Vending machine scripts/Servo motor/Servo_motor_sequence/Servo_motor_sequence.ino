#include <Servo.h>          //Servo motor library. This is standard library

#define servo_pin 7
//#define LIMIT_SWITCH_PIN 7

Servo servo_motor; //our servo name

void setup() {
  servo_motor.attach(servo_pin); //our servo pin
//  pinMode(LIMIT_SWITCH_PIN, INPUT);
}

void loop() {
  servo_motor.write(180);
  
  delay(10000);
  
  servo_motor.write(0);
  
  delay(10000);
}
