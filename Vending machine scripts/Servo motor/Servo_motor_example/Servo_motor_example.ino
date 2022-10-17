#include <Servo.h>          //Servo motor library. This is standard library

#define servo_pin 12

Servo servo_motor; //our servo name

void setup() {
  servo_motor.attach(servo_pin); //our servo pin
}

void loop() {
  servo_motor.write(0);

  delay(2000);

  servo_motor.write(270);

  delay(2000);
}
