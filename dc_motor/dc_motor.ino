/*  Arduino DC Motor Control - PWM | H-Bridge | L298N  -  Example 01
    by Dejan Nedelkovski, www.HowToMechatronics.com
*/
#define enA 5
#define in1 6
#define in2 7
#define button 4
int rotDirection = 0;
int pressed = false;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(button, INPUT);
  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  Serial.begin(9600);
}
void loop() {
//  int potValue = analogRead(A0); // Read potentiometer value
//  Serial.println(potValue);
//  int pwmOutput = map(potValue, 0, 1023, 0 , 255); // Map the potentiometer value from 0 to 255
  analogWrite(enA, 255); // Send PWM signal to L298N Enable pin
  Serial.println("pwmOutput");
  // Read button - Debounce
//  if (digitalRead(button) == true) {
//    pressed = !pressed;
//  }
//  while (digitalRead(button) == true);
//  delay(150);
//  Serial.println(pressed);
  // If button is pressed - change rotation direction
//  if (pressed == true  & rotDirection == 0) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
//  rotDirection = 1;
  delay(5000);
//  }
  // If button is pressed - change rotation direction
//  if (pressed == false & rotDirection == 1) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
//  rotDirection = 0;
  delay(5000);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(10000);
//  }
 }
