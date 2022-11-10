#define spin_RIGHT 5
#define spin_LEFT 6

void setup() {

  Serial.begin(9600);

}

void loop() {
  analogWrite(spin_RIGHT, 255);
  analogWrite(spin_LEFT, 0);

  delay(1500);

  analogWrite(spin_RIGHT, 0);
  analogWrite(spin_LEFT, 255);

  delay(1500);

}
