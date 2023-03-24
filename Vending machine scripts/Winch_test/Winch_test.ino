#define Winch_d 2
#define Winch_u 3

void setup() {
  pinMode(Winch_d, OUTPUT);
  pinMode(Winch_u, OUTPUT);

  digitalWrite(Winch_d, HIGH);
  digitalWrite(Winch_u,HIGH);

  Serial.begin(9600);
}

void loop() {
  digitalWrite(Winch_d, LOW);
  digitalWrite(Winch_u,HIGH);

  delay(10000);
  
  digitalWrite(Winch_d, HIGH);
  digitalWrite(Winch_u,LOW);

  delay(10000);
  
  digitalWrite(Winch_d, HIGH);
  digitalWrite(Winch_u,HIGH);

  delay(10000);
}
