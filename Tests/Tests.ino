#define Motor 8

void setup() {
  Serial.begin(9600);
//  pinMode(Motor, OUTPUT);
//  digitalWrite(Motor, HIGH);

}

void loop() {
  char var = Serial.read();
  Serial.println(var);
//  digitalWrite(Motor, LOW);
//  delay(10000);
//  digitalWrite(Motor, HIGH);
//  delay(10000);
//  digitalWrite(Motor, LOW);
//  delay(2000);
//  digitalWrite(Motor, HIGH);
//  delay(15000);
//  digitalWrite(Motor, LOW);
//  delay(500);
//  digitalWrite(Motor, HIGH);
//  delay(15000);
}
