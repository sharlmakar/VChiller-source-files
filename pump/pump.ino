#define pump 8
void setup() {
  pinMode(pump, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println("Pump on");
  digitalWrite(pump, LOW);
  delay(10000);
  Serial.println("Pump off");
  digitalWrite(pump, HIGH);
  delay(5000);
}
