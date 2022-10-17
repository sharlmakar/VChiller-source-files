#include <Stepper.h>  //Used for controlling step motor driver

#define button 4
#define enA 5
#define in1 6
#define in2 7
int pressed = false;
int nowtime;
int Time;

const int stepsPerRevolution = 200;                  // Number of steps per output rotation
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);  // Create Instance of Stepper library

//int potValue;  // value from the analog pin
int spintime = 10000;
int pwmOutput;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(button, INPUT);
  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  nowtime=0;
//  potValue = 0;
  pwmOutput = 0;
  pressed = false;
  //Read button - Debounce
  if (digitalRead(button) == true) {
    delay(20);
    if(digitalRead(button) == true){
      pressed = !pressed;
    }
  }

  if (pressed == true){
      // step one revolution in one direction to bring can down:
      Serial.println("clockwise");
      myStepper.step(stepsPerRevolution*2);
      delay(2000);

      nowtime = millis();
      Time = millis() - nowtime;
      while(Time<spintime){
          //DC motor sequence
//          potValue = analogRead(A0); // Read potentiometer value
//          Serial.println(potValue);
//          pwmOutput = map(potValue, 0, 1023, 0 , 255); // Map the potentiometer value from 0 to 255
          analogWrite(enA, 140); // Send PWM signal to L298N Enable pin
          Time = millis() - nowtime;
      }
      analogWrite(enA, 0); // Send PWM signal to L298N Enable pin
      delay(2000);
      
      // step one revolution in the other direction:
      Serial.println("counterclockwise");
      myStepper.step(-stepsPerRevolution);
      pressed = false;
  }
  delay(50);
}
