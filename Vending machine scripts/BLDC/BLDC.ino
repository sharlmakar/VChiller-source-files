#define Dir 8
#define Brake 9
#define PWM 10


void setup() {
  pinMode(Dir, OUTPUT);
  pinMode(Brake, OUTPUT);
}

void loop() {
  analogWrite(PWM, 255);
  digitalWrite(Brake, LOW);
  digitalWrite(Dir, LOW);

  delay(5000);

  digitalWrite(Dir, HIGH);
  
  delay(5000);
  
  analogWrite(PWM, 0);
  digitalWrite(Brake, HIGH);

  delay(2000);
}
