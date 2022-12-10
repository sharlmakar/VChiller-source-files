// CHANGE ANY VLAUES CHANGED FOR TESTING:
//Spin time has to be changed back

#include <Wire.h>
#include <Servo.h>
#include <Adafruit_MLX90614.h>

#define ProxIN_warm 2
#define ProxIN_cold1 3
#define ProxIN_cold2 A2

#define spin_PWM 5
#define spin_DIR 6
#define servo_pin1 4
#define servo_pin2 7
#define Winch 8
#define Winch_dir 9

#define DCPump A3

#define limit_switch_up 10
#define ultrasonic_trig 11
#define ultrasonic_echo 12

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

char rasp_com;

Servo servo_motor1, servo_motor2; //our servo name
//Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  
  pinMode(ProxIN_warm,INPUT);
  pinMode(ProxIN_cold1,INPUT);
  pinMode(ProxIN_cold2,INPUT);
  
  pinMode(limit_switch_up,INPUT);
  pinMode(ultrasonic_trig, OUTPUT);
  pinMode(ultrasonic_echo, INPUT);
  
  pinMode(Winch_dir, OUTPUT);
  pinMode(Winch, OUTPUT);
  
  digitalWrite(Winch_dir, HIGH);
  digitalWrite(Winch, HIGH);
  
  pinMode(spin_DIR,OUTPUT);

  analogWrite(spin_PWM,0);
  digitalWrite(spin_DIR, LOW);

  pinMode(DCPump,OUTPUT);
  digitalWrite(DCPump, HIGH);
  
  servo_motor1.attach(servo_pin1);
  servo_motor2.attach(servo_pin2);

  servo_motor1.write(0);
  servo_motor2.write(90); //If bottle is present in spinner it drops it into cold storage
  delay(3000);
  servo_motor2.write(180);  //Spinner servo returns to open positionn to wait for next bottle
  
  Serial.begin(9600);
  Serial.println("ARD1");

}

void loop() {
  
  read_Serial();
  case_handler();
  
  switch(Step){
    case 1: servo_motor1.write(180);//drops bottle into spinner
            break;
    case 2: servo_motor1.write(0); //returns to original position
            break;
    case 3: servo_motor1.write(10);
            servo_motor2.write(160); //spinner lock closes
            break;
    case 4: DC_motor_sequence(); //DC motor spins bottle
            break;
    case 5: Spin_shake_sequence(); //DC motor spins bottle
            break;        
    case 6: analogWrite(spin_PWM, 0);
            digitalWrite(DCPump, HIGH);
            servo_motor2.write(90); //Lock on spinner drops bottle to cold storage
            break;
    case 7: servo_motor2.write(180); //Lock on spinner returns to open position to wait for next bottle
            break;
    case 8: Winch_func();
            break;
//    default:digitalWrite(Winch_approv, LOW);
//            break;
  }

}

void case_handler(){
  difference = millis() - step_time;
  if(rasp_com=='B' && Step != 1 && Step != 2 && Trigger_winch == 2){
//    Serial.println("2ana hena");
    temp_Step = Step;
    temp_diff = millis() - step_time;
    Step = 8;
    Trigger_winch = 0;
  }

  else if(digitalRead(ProxIN_warm)==0 && digitalRead(ProxIN_cold1)==1 && digitalRead(ProxIN_cold2)==1 && Step == 0){  //drops bottle into spinner
    Step = 1;
    step_time = millis();
  }
  else if(Step == 1){ //returns to original position
    if(difference>2000){
      Step = 2;
      step_time = millis();
    }
  }
  else if(Step == 2){ //spinner lock closes
    if(difference>3000){
      Step = 3;
      step_time = millis();
    }
  }
  else if(Step == 3){ //DC motor spins bottle
    if(difference>1000){
      Step = 4;
      spining_time = spin_time(temp_read_ntc());
      Dir_time = millis();
      step_time = millis();
    }
  }
  else if(Step == 4){ //DC motor shakes bottle
    if(difference>spining_time){
      Step = 5;
      step_time = millis();
      Dir_time = millis();
    }
  }
  else if(Step == 5){
    if(difference>30000){ //Spinner shakes bottle to dry it
      Step = 6;
      step_time = millis();
    }
  }
  else if(Step == 6){
    if(difference>2000){  //Lock on spinner drops bottle to cold storage
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
//    spin_time = 720000; CHANGE THIS FOR THE REAL THING
    spin_time = 30000;
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
  analogWrite(spin_PWM, 255);
  digitalWrite(DCPump, LOW);
  if(millis() - Dir_time > 10000){
    spin_DIR_val = !spin_DIR_val;
    digitalWrite(spin_DIR, spin_DIR_val);
    Dir_time = millis();
  }
  
}

void Spin_shake_sequence(){
  analogWrite(spin_PWM, 255);
  digitalWrite(DCPump, HIGH);
  if(millis() - Dir_time > 10000){
    spin_DIR_val = !spin_DIR_val;
    digitalWrite(spin_DIR, spin_DIR_val);
    Dir_time = millis();
  }
}

void Winch_func(){
  
  analogWrite(spin_PWM, 0);
  digitalWrite(DCPump, HIGH);

  Serial.println(rasp_com);

  if(rasp_com == 'L' && ultrasonic_listen() > 10 && (Trigger_winch == 0 || (Trigger_winch == 1 && ((millis()-Down_winch_time) < 10000 )))){
    digitalWrite(Winch, LOW);
    digitalWrite(Winch_dir, HIGH);
    if(Trigger_winch == 0){
      Down_winch_time = millis();
      Trigger_winch = 1;
    }
  }
  
  else if(rasp_com == 'R' && read_ind_prox() == HIGH){
    digitalWrite(Winch, LOW);
    digitalWrite(Winch_dir, LOW);
  }

  else if(rasp_com == 'E'){
    digitalWrite(Winch, HIGH);
    digitalWrite(Winch_dir, HIGH);
    step_time = millis() - temp_diff;
    Step = temp_Step;
    Trigger_winch = 2;
  }
  
  else{
    digitalWrite(Winch, HIGH);
    digitalWrite(Winch_dir, HIGH);
  }
  
}

int ultrasonic_listen(){ // Funuction reads the ultrasound proximity sensor of the warm storage at the lower limit
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
  char temp_rasp_com = '\n';
  if(Serial.available()){
      temp_rasp_com = Serial.read();
  }
  if(temp_rasp_com != '\n'){
    rasp_com = temp_rasp_com;
  }
}

bool read_ind_prox(){ //Function reads the proximity sensor of warm storage at the upper limit
  if(digitalRead(limit_switch_up) == HIGH) return HIGH; //When the metal (the storage compartment) is not detected
  else  return LOW; //When the metal (the storage compartment) is detected

}
