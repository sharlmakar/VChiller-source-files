#include <Wire.h>
#include <Servo.h>
#include <Adafruit_MLX90614.h>

//#define Winch_button_down 4
//#define ProxIN_warm 2
#define ProxIN_cold1 3
#define ProxIN_cold2 2
#define spin_RIGHT 5
#define spin_LEFT 6
#define servo_pin1 4
#define servo_pin2 7
#define Winch_lower 10
#define Winch_raise 9
#define limit_switch_up 8
#define ultrasonic_trig 11
#define ultrasonic_echo 12

#define DCPump A3

#define Temp_pin A0

int bott_counter = 0;
int difference = 0;
int temp_diff = 0;
int Trigger_winch = 2;
int Step = 0;
int temp_Step = 0;
int servo1_angle = 0;
int servo2_angle = 180;
unsigned long step_time = 0;
unsigned long spining_time = 0;
unsigned long spin_time_dir = 90000;
unsigned long Dir_time = 0;
unsigned long Down_winch_time = 1000000;
bool spin_DIR_val = HIGH;

//String rasp_com;
char rasp_com;

Servo servo_motor1, servo_motor2; //our servo name
//Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {

  //  pinMode(ProxIN_warm, INPUT);
  pinMode(ProxIN_cold1, INPUT);
  pinMode(ProxIN_cold2, INPUT);

  pinMode(limit_switch_up, INPUT);
  pinMode(ultrasonic_trig, OUTPUT);
  pinMode(ultrasonic_echo, INPUT);

  pinMode(spin_RIGHT, OUTPUT);
  pinMode(spin_LEFT, OUTPUT);

  pinMode(Winch_lower, OUTPUT);
  pinMode(Winch_raise, OUTPUT);

  digitalWrite(Winch_lower, HIGH);
  digitalWrite(Winch_raise, HIGH);

  pinMode(DCPump, OUTPUT);
  digitalWrite(DCPump, HIGH);

  servo_motor1.attach(servo_pin1);
  servo_motor2.attach(servo_pin2);

  servo_motor1.write(0);
  servo_motor2.write(0); //If bottle is present in spinner it drops it into cold storage
  delay(3000);
  servo_motor2.write(180);  //Spinner servo returns to open positionn to wait for next bottle

  analogWrite(spin_RIGHT, 0);
  analogWrite(spin_LEFT, 0);

  Serial.begin(9600);
  Serial.println("ARD4");
  delay(5000);

}

void loop() {

  read_Serial();
  case_handler();
  servo_command();

  switch (Step) {
    case 1: //digitalWrite(Winch_approv, HIGH);
      servo1_angle = 180;//drops bottle into spinner
      servo2_angle = 180;
      servo_command();
      break;
    case 2: //digitalWrite(Winch_approv, HIGH);
      servo1_angle = 0; //returns to original position
      servo2_angle = 180;
      servo_command();
      break;
    case 3: //digitalWrite(Winch_approv, LOW);
      servo2_angle = 80; //spinner lock closes
      servo1_angle = 0;
      servo_command();
      break;
    case 4: //digitalWrite(Winch_approv, LOW);
      DC_motor_sequence(); //DC motor spins bottle
      break;
    case 5: //digitalWrite(Winch_approv, LOW);
      Spin_shake_sequence(); //DC motor spins bottle
      break;
    case 6: //digitalWrite(Winch_approv, LOW);
      analogWrite(spin_RIGHT, 0);
      analogWrite(spin_LEFT, 0);
      digitalWrite(DCPump, HIGH);
      servo2_angle = 0; //Lock on spinner drops bottle to cold storage
      servo1_angle = 0;
      servo_command();
      break;
    case 7: //digitalWrite(Winch_approv, LOW);
      servo2_angle = 180; //Lock on spinner returns to open position to wait for next bottle
      servo1_angle = 0;
      servo_command();
      break;
    case 8: //digitalWrite(Winch_approv, LOW);
      Winch_func();
      break;
      //    default:digitalWrite(Winch_approv, LOW);
      //            break;
  }

}

void case_handler() {
  
  difference = millis() - step_time;
  Serial.println(temp_read_ntc());
//  Serial.print("Induction prox: ");
//  Serial.println(read_ind_prox());

  if (rasp_com == 'B' && Step != 1 && Step != 2 && Trigger_winch == 2) {
    temp_Step = Step;
    temp_diff = millis() - step_time;
    Down_winch_time = millis();
    Step = 8;
    Trigger_winch = 0;
  }

  else if (bott_counter < 10 && Step == 0 && read_ind_prox() == LOW && temp_read_ntc() < 0.75) { //Initiate sequence
    if (ultrasonic_listen() > 5) {
      Step = 1;
      bott_counter++;
      step_time = millis();
    }
  }
  else if (Step == 1 && read_ind_prox() == LOW) { //Drop bottle into spinner//Return to receive next bottle
    if (difference > 2000) {
      Step = 2;
      step_time = millis();
    }
  }
  else if (Step == 2 && read_ind_prox() == LOW) { //Return to receive next bottle
    if (difference > 3000) {
      Step = 3;
      step_time = millis();
    }
  }
  else if (Step == 3) { //Spinner lock closes
    if (difference > 1000) {
      Step = 4;
      spining_time = 900000;
      //      Serial.println(spining_time);
      //      Serial.println(temp_read_ntc());
      Dir_time = millis();
      step_time = millis();
    }
  }
  else if (Step == 4) { //DC motor spins bottle
    //    Serial.println(millis() - step_time);
    if (millis() - step_time > spining_time) {
      Step = 5;
      step_time = millis();
      Dir_time = millis();
    }
  }
  else if (Step == 5) {
    if (difference > 30000) { //DC motor shakes bottle
      Step = 6;
      step_time = millis();
    }
  }
  else if (Step == 6) { //Lock on spinner drops bottle to cold storage
    if (difference > 2000) {
      Step = 7;
    }
  }
  else if (Step == 7) { //Lock returns to original position to receive next bottle
    if (difference > 2000) {
      Step = 0; //Step 0 is the IDLE state
    }
  }

}

float temp_read_ntc() {

  int Vo = 0;
  int V1 = 1;
  float R1 = 10000;
  float logR2, R2, T;
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

  while (Vo != V1) {
    Vo = analogRead(Temp_pin);
    delay(20);
    V1 = analogRead(Temp_pin);

    if (Vo == V1) {
      R2 = R1 * (1023.0 / (float)Vo - 1.0);
      logR2 = log(R2);
      T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
      T = T - 273.15;

    }
  }
  //  Serial.println(T);
  return T;
}

unsigned long spin_time(float temp_cold) {

  //  float difference = temp_hot - temp_cold;
  unsigned long spin_time; //= (int) difference * ((10*60*1000)/23); //spin time of every degree increase multiplied by degrees needed to increase (in ms)

  if (temp_cold > 10) {
    spin_time = 720000;
  }
  else if (temp_cold > 5) {
    spin_time = 600000;
  }
  else if (temp_cold > 2) {
    spin_time = 480000;
  }
  else {
    spin_time = 420000;
  }

  return spin_time;
}

void DC_motor_sequence() {
  
  if (millis() - Dir_time > spin_time_dir) {
    spin_DIR_val = !spin_DIR_val;
    if (spin_DIR_val == 1) {
      spin_time_dir = 90000;
    }
    else {
      spin_time_dir = 30000;
    }
    Dir_time = millis();
  }

  if (spin_DIR_val == 1) {
    //    Serial.println("1");
    analogWrite(spin_RIGHT, 0);
    analogWrite(spin_LEFT, 255);
    digitalWrite(DCPump, LOW);
  }

  else {
    //      Serial.println("2");
    analogWrite(spin_RIGHT, 0);
    analogWrite(spin_LEFT, 0);
    digitalWrite(DCPump, HIGH);
  }
  
}

void Spin_shake_sequence() {

  digitalWrite(DCPump, HIGH);

  if (millis() - Dir_time > 5000) {
    spin_DIR_val = !spin_DIR_val;
    Dir_time = millis();
  }

  if (spin_DIR_val == 1) {
    analogWrite(spin_RIGHT, 0);
    analogWrite(spin_LEFT, 255);
  }

  else {
    analogWrite(spin_RIGHT, 255);
    analogWrite(spin_LEFT, 0);
  }
}

void Winch_func() {
  //  Serial.println(ultrasonic_listen());
  bott_counter = 0;
  analogWrite(spin_RIGHT, 0);
  analogWrite(spin_LEFT, 0);
  digitalWrite(DCPump, HIGH);

  Serial.println(rasp_com);
  //  Serial.println(ultrasonic_listen());
  if (rasp_com == 'L' && (Trigger_winch == 0 || (Trigger_winch == 1 && ((millis() - Down_winch_time) < 25000)))) { // && ultrasonic_listen() > 10)))){
    //    Serial.println("here");
    digitalWrite(Winch_lower, HIGH);
    digitalWrite(Winch_raise, LOW);
    if (Trigger_winch == 0) {
      Down_winch_time = millis();
      Trigger_winch = 1;
    }
  }

  else if (rasp_com == 'R' && read_ind_prox() == HIGH) {
    //    Serial.println("up");
    digitalWrite(Winch_lower, LOW);
    digitalWrite(Winch_raise, HIGH);
  }

  else if (rasp_com == 'E') {
    digitalWrite(Winch_lower, HIGH);
    digitalWrite(Winch_raise, HIGH);
    step_time = millis() - temp_diff;
    Step = temp_Step;
    Trigger_winch = 2;
  }

  else {
    digitalWrite(Winch_lower, HIGH);
    digitalWrite(Winch_raise, HIGH);
  }

}

int ultrasonic_listen() {
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

void read_Serial() {
  char temp_rasp_com = '\n';
  if (Serial.available()) {
    temp_rasp_com = Serial.read();
    if (temp_rasp_com != '\n') {
      rasp_com = temp_rasp_com;
      Serial.print("ACK:");
      Serial.println(rasp_com);
    }
  }
}

void servo_command() {
  servo_motor1.write(servo1_angle);
  servo_motor2.write(servo2_angle);
}

bool read_ind_prox() {
  if (digitalRead(limit_switch_up) == HIGH) return HIGH; //When the metal (the storage compartment) is not detected
  else  return LOW; //When the metal (the storage compartment) is detected

}
