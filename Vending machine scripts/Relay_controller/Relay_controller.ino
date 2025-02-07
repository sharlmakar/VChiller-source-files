//This script replicates signals coming from the reaspberry pi from a 3.3V logic to a 5V logic which is more suitable for relays.
#define In1 13
#define In2 12
#define In3 11
#define In4 10

#define Out1 9
#define Out2 8
#define Out3 7
#define Out4 6

void setup() {
  pinMode(In1, INPUT_PULLUP);
  pinMode(In2, INPUT_PULLUP);
  pinMode(In3, INPUT_PULLUP);
  pinMode(In4, INPUT_PULLUP);

  pinMode(Out1, OUTPUT);
  pinMode(Out2, OUTPUT);
  pinMode(Out3, OUTPUT);
  pinMode(Out4, OUTPUT);

  digitalWrite(Out1, HIGH);
  digitalWrite(Out2, HIGH);
  digitalWrite(Out3, HIGH);
  digitalWrite(Out4, HIGH);
}

void loop() {
  if(digitalRead(In1) == LOW) digitalWrite(Out1, HIGH);
  else digitalWrite(Out1, LOW);

  if(digitalRead(In2) == LOW) digitalWrite(Out2, HIGH);
  else digitalWrite(Out2, LOW);

  if(digitalRead(In3) == LOW) digitalWrite(Out3, HIGH);
  else digitalWrite(Out3, LOW);

  if(digitalRead(In4) == LOW) digitalWrite(Out4, HIGH);
  else digitalWrite(Out4, LOW);

}
