#define pressure_pin 3

//int Pressure_pin = 3;

int Vp;
void setup() {
  pinMode(pressure_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
//  Vp = analogRead(Pressure_pin);
  Serial.print("New value");
  analogWrite(pressure_pin, 1000);
//  Serial.print("Pressure Voltage: ");
//  Serial.print(Vp);
//  Serial.print("\n");
  delay(5000);
}
