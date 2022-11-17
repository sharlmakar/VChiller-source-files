#define ultrasonic_echo 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define ultrasonic_trig 3 //attach pin D3 Arduino to pin Trig of HC-SR04
#define prox_sensor 4
#define Winch 8
#define Winch_dir 9
String rasp_com = "D";

void setup() {
  pinMode(Winch_dir, OUTPUT);
  pinMode(Winch, OUTPUT);

  pinMode(ultrasonic_trig, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(ultrasonic_echo, INPUT); // Sets the echoPin as an INPUT

  pinMode(prox_sensor, INPUT);

  Serial.begin(9600);
}

void loop() {
  read_Serial();
//  Serial.println(ultrasonic_listen());
  if(rasp_com == "DOWN\n" && ultrasonic_listen() > 10){
    digitalWrite(Winch, LOW);
    digitalWrite(Winch_dir,HIGH);
  }

  else if(rasp_com == "UP\n" && digitalRead(prox_sensor) == 1){
    digitalWrite(Winch, LOW);
    digitalWrite(Winch_dir,LOW);
  }

  else{
    digitalWrite(Winch, HIGH);
    digitalWrite(Winch_dir,HIGH);
  }
}

int ultrasonic_listen(){
  // Clears the trigPin condition
  digitalWrite(ultrasonic_trig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(ultrasonic_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonic_trig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  unsigned long duration1 = pulseIn(ultrasonic_echo, HIGH);
  delay(1);
  unsigned long duration2 = pulseIn(ultrasonic_echo, HIGH);
  // Calculating the distance
  return duration1 * 0.034 / 2; // Speed of sound wave divided by 2 (go and back). Distance measured in cm.
}

void read_Serial(){
  if(Serial.available()){
    rasp_com= Serial.readString();// read the incoming data as string
  }
}
