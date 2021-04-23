#define button 4
#define enA 5
#define in1 6
#define in2 7
int pressed = false;
int nowtime;
int Time;

const int stepsPerRevolution = 1000;                  // Number of steps per output rotation
// Define stepper motor connections and steps per revolution:
#define dirPin 8
#define stepPin 9

//int potValue;  // value from the analog pin
int spintime = 10000;
int pwmOutput;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(button, INPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
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
      // Set the spinning direction counterclockwise:
      digitalWrite(dirPin, HIGH);
    
      // Spin the stepper motor 1 revolution quickly:
      for (int i = 0; i < stepsPerRevolution; i++) {
        // These four lines result in 1 step:
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(1000);
      }


      nowtime = millis();
      Time = millis() - nowtime;
      while(Time<spintime){
          //DC motor sequence
//          potValue = analogRead(A0); // Read potentiometer value
//          Serial.println(potValue);
//          pwmOutput = map(potValue, 0, 1023, 0 , 255); // Map the potentiometer value from 0 to 255
          analogWrite(enA, 200); // Send PWM signal to L298N Enable pin
          Time = millis() - nowtime;
      }
      analogWrite(enA, 0); // Send PWM signal to L298N Enable pin
      delay(2000);
      
      // step one revolution in the other direction:
      Serial.println("counterclockwise");
      // Set the spinning direction counterclockwise:
      digitalWrite(dirPin, LOW);
    
      // Spin the stepper motor 1 revolution quickly:
      for (int i = 0; i < stepsPerRevolution; i++) {
        // These four lines result in 1 step:
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(1000);
      }

      pressed = false;
  }
  delay(50);
}
