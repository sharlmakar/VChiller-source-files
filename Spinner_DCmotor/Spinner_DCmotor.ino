#define enA 5
#define in1 6
#define in2 8

void setup() {
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 255); // Send PWM signal to L298N Enable pin
  delay(4000);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 255); // Send PWM signal to L298N Enable pin
  delay(4000);

}
