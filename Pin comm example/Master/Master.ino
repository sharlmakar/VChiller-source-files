#define buttn A3
#define trigger 2

void setup() {
  pinMode(buttn, INPUT); 
  pinMode(trigger, OUTPUT); 

}

void loop() {
  if(digitalRead(buttn) == HIGH){
    digitalWrite(trigger, HIGH);
  }

  else{
    digitalWrite(trigger, LOW);
  }
}
