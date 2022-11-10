#define trigger 2

void setup() {
  pinMode(trigger, INPUT); 
  Serial.begin(9600);
}

void loop() {
  if(digitalRead(trigger) == HIGH){
    Serial.println("Highhh");
  }

  else{
    Serial.println("Lowwww");
  }
}
