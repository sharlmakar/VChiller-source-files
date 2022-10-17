void setup() {
  pinMode(7,INPUT);
  Serial.begin(9600);
  pinMode(13,OUTPUT);
}

void loop() {
  
Serial.print("IRSensorip  ");
Serial.println(digitalRead(7));

if(digitalRead(7)==0)
{
  Serial.println("Something");
  digitalWrite(13,HIGH);
}
else{
  Serial.println("Nothing");
  digitalWrite(13,LOW);
  }
delay(1000);
}
