#define Winch 8
#define Winch_dir 9

void setup() {
  pinMode(Winch_dir, OUTPUT);
  pinMode(Winch, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  digitalWrite(Winch, LOW);
  digitalWrite(Winch_dir,HIGH);

  delay(10000);
  
  digitalWrite(Winch, LOW);
  digitalWrite(Winch_dir,LOW);

  delay(10000);
  
  digitalWrite(Winch, HIGH);
  digitalWrite(Winch_dir,HIGH);

  delay(10000);
}
