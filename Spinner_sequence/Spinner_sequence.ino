/* Example sketch to control a stepper motor with TB6600 stepper motor driver and Arduino without a library: number of revolutions, speed and direction. More info: https://www.makerguides.com */

// Define stepper motor connections and steps per revolution:
#define dirPin 2
#define stepPin 3
#define stepsPerRevolution 8800

#define enA 5
#define in1 6
#define in2 7
#define button 4
int rotDirection = 0;
double Time = millis();
void setup() {
  // Declare pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  
  Serial.begin(9600);
}

void loop() {

//  // Set the spinning direction counterclockwise:
//  digitalWrite(dirPin, LOW);
//
//  // Spin the stepper motor 1 revolution quickly:
//  for (int i = 0; i < stepsPerRevolution; i++) {
//    // These four lines result in 1 step:
//    digitalWrite(stepPin, HIGH);
//    delayMicroseconds(1000);
//    digitalWrite(stepPin, LOW);
//    delayMicroseconds(1000);
//  }
//
//  delay(1000);

  // Set the spinning direction clockwise:
//  digitalWrite(dirPin, HIGH);
//
//  // Spin the stepper motor 5 revolutions fast:
//  for (int i = 0; i < stepsPerRevolution; i++) {
//    // These four lines result in 1 step:
//    digitalWrite(stepPin, HIGH);
//    delayMicroseconds(500);
//    digitalWrite(stepPin, LOW);
//    delayMicroseconds(500);
//  }
  while(Time < 60000){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enA, 255); // Send PWM signal to L298N Enable pin
    delay(10000);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    delay(10000);
    Time = millis();
  }
  analogWrite(enA, 0); // Send PWM signal to L298N Enable pin
  
  // Set the spinning direction counterclockwise:
//  digitalWrite(dirPin, LOW);
//
//  // Spin the stepper motor 5 revolutions fast:
//  for (int i = 0; i < stepsPerRevolution; i++) {
//    // These four lines result in 1 step:
//    digitalWrite(stepPin, HIGH);
//    delayMicroseconds(500);
//    digitalWrite(stepPin, LOW);
//    delayMicroseconds(500);
//  }

//  delay(1000);
}
