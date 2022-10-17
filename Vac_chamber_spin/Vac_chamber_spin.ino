// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;

unsigned long Time;
unsigned long spin_time;
int trigger_spin = 0;
int spin_time_condition = 1000;
int throttle = 51;


void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  analogWrite(enA, throttle);
  Serial.begin(9600);

}

void loop() {
  Time = millis();
  if(trigger_spin == 0){
    spin_time = millis();
    trigger_spin = 1;
  }

  if(((Time - spin_time) > spin_time_condition) && (throttle < 255)){
    throttle += 51;
    Serial.println(throttle);
    analogWrite(enA, throttle);
    spin_time_condition += 1000;
  }
}
