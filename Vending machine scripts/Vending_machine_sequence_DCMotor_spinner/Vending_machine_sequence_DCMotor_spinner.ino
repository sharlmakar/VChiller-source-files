#include <Wire.h>
#include <Servo.h>
#include <Adafruit_MLX90614.h>

 
//#define Winch_button_down 4
#define ProxIN_warm 2
#define ProxIN_cold 3
#define spin_RIGHT 5
#define spin_LEFT 6
#define servo_pin1 4
#define servo_pin2 7
#define Winch 8
#define Winch_dir 9
#define limit_switch_up 10
#define ultrasonic_trig 11
#define ultrasonic_echo 12 
#define Winch_approv 13

#define DCPump A3

#define Temp_pin A0

int difference = 0;
int temp_diff = 0;
int Trigger_winch = 2;
int Step = 0;
int temp_Step = 0;
unsigned long step_time = 0;
unsigned long spining_time = 0;
unsigned long Dir_time = 0;
unsigned long Down_winch_time = 1000000;
bool spin_DIR_val = LOW;

String rasp_com;

Servo servo_motor1, servo_motor2; //our servo name
//Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  
  pinMode(ProxIN_warm,INPUT);
  pinMode(ProxIN_cold,INPUT);
  
  pinMode(limit_switch_up,INPUT);
  pinMode(ultrasonic_trig, OUTPUT);
  pinMode(ultrasonic_echo, INPUT);

  pinMode(Winch_dir, OUTPUT);
  pinMode(Winch, OUTPUT);
  pinMode(Winch_approv, OUTPUT);
//  pinMode(spin_DIR,OUTPUT);

  digitalWrite(Winch_approv, LOW);
  
  pinMode(DCPump,OUTPUT);
  digitalWrite(DCPump, HIGH);
  
  servo_motor1.attach(servo_pin1);
  servo_motor2.attach(servo_pin2);

  servo_motor1.write(0);
  servo_motor2.write(180); //If bottle is present in spinner it drops it into cold storage
  delay(5000);
  servo_motor2.write(0);  //Spinner servo returns to open positionn to wait for next bottle
  
  analogWrite(spin_RIGHT, 0);
  analogWrite(spin_LEFT, 0);
  
  Serial.begin(9600);

}

void loop() {
  
  read_Serial();
  case_handler();
  
  switch(Step){
    case 1: digitalWrite(Winch_approv, HIGH);
            servo_motor1.write(180);//drops bottle into spinner
            break;
    case 2: digitalWrite(Winch_approv, HIGH);
            servo_motor1.write(0); //returns to original position
            break;
    case 3: digitalWrite(Winch_approv, LOW);
            servo_motor2.write(90); //spinner lock closes
            break;
    case 4: digitalWrite(Winch_approv, LOW);
            DC_motor_sequence(); //DC motor spins bottle
            break;
    case 5: digitalWrite(Winch_approv, LOW);
            Spin_shake_sequence(); //DC motor spins bottle
            break;        
    case 6: digitalWrite(Winch_approv, LOW);
            analogWrite(spin_RIGHT, 0);
            analogWrite(spin_LEFT, 0);
            digitalWrite(DCPump, HIGH);
            servo_motor2.write(180); //Lock on spinner drops bottle to cold storage
            break;
    case 7: digitalWrite(Winch_approv, LOW);
            servo_motor2.write(0); //Lock on spinner returns to open position to wait for next bottle
            break;
    case 8: digitalWrite(Winch_approv, LOW);
            Winch_func();
            break;
    default:digitalWrite(Winch_approv, LOW);
            break;
  }

}

void case_handler(){
  difference = millis() - step_time;

  if(rasp_com=="START" && Step != 1 && Step != 2 && Trigger_winch == 2){
    temp_Step = Step;
    temp_diff = millis() - step_time;
    Step = 8;
    Trigger_winch = 0;
  }

  else if(digitalRead(ProxIN_warm)== 0 && digitalRead(ProxIN_cold)==1 && Step == 0){  //Initiate sequence
    Step = 1;
    step_time = millis();
  }
  else if(Step == 1){ //Drop bottle into spinner//Return to receive next bottle
    if(difference>2000){
      Step = 2;
      step_time = millis();
    }
  }
  else if(Step == 2){ //Return to receive next bottle
    if(difference>3000){
      Step = 3;
      step_time = millis();
    }
  }
  else if(Step == 3){ //Spinner lock closes
    if(difference>1000){
      Step = 4;
      spining_time = spin_time(temp_read_ntc());
      Serial.println(spining_time);
      Serial.println(temp_read_ntc());
      Dir_time = millis();
      step_time = millis();
    }
  }
  else if(Step == 4){ //DC motor spins bottle
    Serial.println(millis() - step_time);
    if(millis() - step_time>spining_time){
      Step = 5;
      step_time = millis();
      Dir_time = millis();
    }
  }
  else if(Step == 5){
    if(difference>30000){ //DC motor shakes bottle
      Step = 6;
      step_time = millis();
    }
  }
  else if(Step == 6){ //Lock on spinner drops bottle to cold storage
    if(difference>2000){  
      Step = 7;
    }
  }
  else if(Step == 7){ //Lock returns to original position to receive next bottle
    if(difference>2000){
      Step = 0; //Step 0 is the IDLE state
    }
  }
  
  
}

float temp_read_ntc(){

  int Vo=0;
  int V1=1;
  float R1 = 10000;
  float logR2, R2, T;
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;


  while(Vo != V1){
    Vo = analogRead(Temp_pin);
    delay(20);
    V1 = analogRead(Temp_pin);
    
    if(Vo == V1){
      R2 = R1 * (1023.0 / (float)Vo - 1.0);
      logR2 = log(R2);
      T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
      T = T - 273.15;
    }
  }
  return T;
}

unsigned long spin_time(float temp_cold){
  
//  float difference = temp_hot - temp_cold;
  unsigned long spin_time; //= (int) difference * ((10*60*1000)/23); //spin time of every degree increase multiplied by degrees needed to increase (in ms)

  if(temp_cold > 10){
    spin_time = 720000;
  }
  else if(temp_cold > 5){
    spin_time = 600000;
  }
  else if(temp_cold > 2){
    spin_time = 480000;
  }
  else{
    spin_time = 420000;
  }
  
  return spin_time; 
}

void DC_motor_sequence(){
  digitalWrite(DCPump, LOW);
  if(millis() - Dir_time > 3000){
    spin_DIR_val = !spin_DIR_val;
    Dir_time = millis();
  }

  if(spin_DIR_val == 1){
    analogWrite(spin_RIGHT, 0);
    analogWrite(spin_LEFT, 255);
  }

  else{
    analogWrite(spin_RIGHT, 255);
    analogWrite(spin_LEFT, 0);
  }
  
}

void Spin_shake_sequence(){

  digitalWrite(DCPump, HIGH);
  
  if(millis() - Dir_time > 1500){
    spin_DIR_val = !spin_DIR_val;
    Dir_time = millis();
  }

  if(spin_DIR_val == 1){
    analogWrite(spin_RIGHT, 0);
    analogWrite(spin_LEFT, 255);
  }

  else{
    analogWrite(spin_RIGHT, 255);
    analogWrite(spin_LEFT, 0);
  }
}

void Winch_func(){
  
  analogWrite(spin_RIGHT, 0);
  analogWrite(spin_LEFT, 0);
  digitalWrite(DCPump, HIGH);

  if(rasp_com == "DOWN" && ultrasonic_listen() < 10 && (millis() -Down_winch_time) < 40000){
    digitalWrite(Winch, LOW);
    if(Trigger_winch == 0){
      Down_winch_time = millis();
      Trigger_winch = 1;
    }
  }
  
  else if(rasp_com == "UP" && digitalRead(limit_switch_up) == 1){
    digitalWrite(Winch, LOW);
  }

  else if(rasp_com == "END"){
    digitalWrite(Winch, HIGH);
    step_time = millis() - temp_diff;
    Step = temp_Step;
    Trigger_winch = 2;
  }
  
  else{
    digitalWrite(Winch, HIGH);
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
    rasp_com.remove(rasp_com.length()-1); //remove endline character \n
  }
}
