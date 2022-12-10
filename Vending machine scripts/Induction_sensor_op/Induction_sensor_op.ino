#define pin 2

void setup() {

  pinMode(pin, INPUT);
  Serial.begin(9600);

//  Serial.println("2");

}

void loop() {
  if(digitalRead(pin) == HIGH) Serial.println("No metal detected");
  else  Serial.println("Metal detected");

//  delay(1000);

}
