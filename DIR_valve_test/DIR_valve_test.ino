#define DIR 3

void setup() {
  pinMode(DIR, OUTPUT);

}

void loop() {
  digitalWrite(DIR, LOW);
  delay(10000);
  digitalWrite(DIR, HIGH);
  delay(10000);
}
