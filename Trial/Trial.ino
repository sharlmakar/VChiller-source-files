#define Fan1 6
#define Fan2 19

void setup() {
  pinMode(Fan1, OUTPUT);
  pinMode(Fan2, OUTPUT);

  digitalWrite(Fan1, HIGH);
  digitalWrite(Fan2, HIGH);
}

void loop() {
  digitalWrite(Fan2, LOW);

}
