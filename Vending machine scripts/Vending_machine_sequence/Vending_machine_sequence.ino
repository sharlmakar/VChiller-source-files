#include <Wire.h>
#include <Servo.h>
#include <Adafruit_MLX90614.h>

#define Winch 2
#define Winch_button_up 3
#define Winch_button_down 4
#define spin_PWM 4
#define spin_DIR 5
#define ProxIN_warm 6
#define ProxIN_cold 7
#define DCPump 8
#define servo_pin1 9
#define servo_pin2 10
#define limit_switch_up 11
#define limit_switch_down 12

#define Temp_pin A0

int difference = 0;
int temp_diff = 0;
int Trigger_winch = 0;
int Step = 0;
int temp_Step = 0;
unsigned long step_time = 0;
unsigned long spining_time = 0;
unsigned long Dir_time = 0;
bool spin_DIR_val = LOW;

Servo servo_motor1, servo_motor2; //our servo name
//Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  
  pinMode(ProxIN_warm,INPUT);
  pinMode(ProxIN_cold,INPUT);
  
  pinMode(limit_switch_up,INPUT);
  pinMode(limit_switch_down,INPUT);
  pinMode(Winch_button_up,INPUT);
  pinMode(Winch_button_down,INPUT);
  pinMode(spin_DIR,OUTPUT);

  pinMode(DCPump,OUTPUT);
  digitalWrite(DCPump, HIGH);
  
  servo_motor1.attach(servo_pin1);
  servo_motor2.attach(servo_pin2);

  servo_motor1.write(0);
  servo_motor2.write(180); //If bottle is present in spinner it drops it into cold storage
  delay(5000);
  servo_motor2.write(0);  //Spinner servo returns to open positionn to wait for next bottle
  
//  mlx.begin();
  analogWrite(spin_PWM,0);
  digitalWrite(spin_DIR, LOW);
  
  Serial.begin(9600);

}

void loop() {
  case_handler();
  
  switch(Step){
    case 1: servo_motor1.write(180);//drops bottle into spinner
            break;
    case 2: servo_motor1.write(0); //returns to original position
            break;
    case 3: servo_motor2.write(90); //spinner lock closes
            break;
    case 4: DC_motor_sequence(); //DC motor spins bottle
            break;
    case 5: Spin_shake_sequence(); //DC motor spins bottle
            break;        
    case 6: analogWrite(spin_PWM, 0);
            servo_motor2.write(180); //Lock on spinner drops bottle to cold storage
            break;
    case 7: servo_motor2.write(0); //Lock on spinner returns to open position to wait for next bottle
            break;
    case 8: Winch_func();
            break;
  }

}

void case_handler(){
  difference = millis() - step_time;

  if(digitalRead(limit_switch_up)==0){
    temp_Step = Step;
    temp_diff = difference;
    Step = 8;
  }

  else if(digitalRead(ProxIN_warm)==0 && digitalRead(ProxIN_cold)==1){  //drops bottle into spinner
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
    else if(Step == 7){
    if(difference>2000){
      Step = 0;
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

int spin_time(float temp_cold){
  
//  float difference = temp_hot - temp_cold;
  int spin_time; //= (int) difference * ((10*60*1000)/23); //spin time of every degree increase multiplied by degrees needed to increase (in ms)

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
  analogWrite(spin_PWM, 255);
  digitalWrite(DCPump, LOW);
  if(millis() - Dir_time > 3000){
    spin_DIR_val = !spin_DIR_val;
    digitalWrite(spin_DIR, spin_DIR_val);
    Dir_time = millis();
  }
  
}

void Spin_shake_sequence(){
  analogWrite(spin_PWM, 255);
  digitalWrite(DCPump, HIGH);
  if(millis() - Dir_time > 1500){
    spin_DIR_val = !spin_DIR_val;
    digitalWrite(spin_DIR, spin_DIR_val);
    Dir_time = millis();
  }
}

void Winch_func(){
  
  analogWrite(spin_PWM, 0);
  digitalWrite(DCPump, HIGH);

  if(digitalRead(Winch_button_down) == 1 && digitalRead(limit_switch_down) == 1){
    digitalWrite(Winch, HIGH);
    step_time = millis() - difference;
    Step = temp_Step;
  }
  
  else if(digitalRead(Winch_button_up) == 1 && digitalRead(limit_switch_up) == 1){
    digitalWrite(Winch, HIGH);
  }

  else{
    digitalWrite(Winch, LOW);
  }
  
}
