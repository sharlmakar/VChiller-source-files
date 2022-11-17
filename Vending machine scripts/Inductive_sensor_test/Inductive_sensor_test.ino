#define Sensor A0

void setup() {
  pinMode(Sensor, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  if(digitalRead(Sensor)== true){
    Serial.println("No object detected");
  }

  else{
    Serial.println("Object detected");
  }
//  Serial.print(String(analogRead(Sensor)) + " and ");
//  delay(10);
//  Serial.println(analogRead(Sensor));
  delay(1000);
}
