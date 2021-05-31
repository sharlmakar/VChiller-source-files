#define Solenoid 18

void setup() {
  pinMode(Solenoid, OUTPUT);
  digitalWrite(Solenoid,LOW);
  Serial.begin(9600);

}

void loop() {
  digitalWrite(Solenoid,LOW);
  delay(10000);
  digitalWrite(Solenoid,HIGH);
  delay(3000);
}
