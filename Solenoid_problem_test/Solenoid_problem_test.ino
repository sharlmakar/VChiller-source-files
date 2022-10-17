#define Solenoid 12
unsigned long Time, Stime;

void setup() {
  // put your setup code here, to run once:
  pinMode(Solenoid, OUTPUT);
  digitalWrite(Solenoid, LOW);
  Stime = millis();
}

void loop() {
  
  // put your main code here, to run repeatedly:
  Time = millis();
  if((Time - Stime) > 4000){
    digitalWrite(Solenoid, HIGH);
    delay(4000);
    digitalWrite(Solenoid, LOW);
    Stime = millis();
  }
}
