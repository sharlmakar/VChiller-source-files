#include <Wire.h>
#include <Servo.h>
#include <Adafruit_MLX90614.h>

#define ProxIN 7


#define Temp_pin A0

#define spin_PWM 4
#define spin_DIR 3

#define DCPump 8

#define servo_pin1 11
#define servo_pin2 12

Servo servo_motor1, servo_motor2; //our servo name
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  
  pinMode(ProxIN,INPUT);
  
  pinMode(spin_DIR,OUTPUT);

  pinMode(DCPump,OUTPUT);
  digitalWrite(DCPump, HIGH);
  
  servo_motor1.attach(servo_pin1);
  servo_motor2.attach(servo_pin2);

  servo_motor1.write(0);
  servo_motor2.write(0);
  
  mlx.begin();
  analogWrite(spin_PWM,0);
  
  Serial.begin(9600);

}

void loop() {

  if(digitalRead(ProxIN)==1){
    servo_motor1.write(180);
    delay(2000);
    servo_motor1.write(0);

    delay(3000);

    servo_motor2.write(90);
    delay(1000);
    DC_motor_sequence();
    delay(1000);
    servo_motor2.write(180);
    delay(2000);
    servo_motor2.write(0);
    delay(2000);
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

  if(temp_cold > 4){
    spin_time = 20000;
  }
  else if(2 < temp_cold){
    spin_time = 10000;
  }
  else{
    spin_time = 5000;
  }
  
  return spin_time; 
}

void DC_motor_sequence(){
  
  float cold_temp = temp_read_ntc();
  int spin_time_var = spin_time(cold_temp);
  Serial.print("Cold temp: ");
  Serial.println(cold_temp);
  Serial.println(spin_time_var);
  unsigned long Rec_time = millis();
  unsigned long dir_time = millis();
  int Trig = 1;
  analogWrite(spin_PWM, 255);
  digitalWrite(spin_DIR, LOW);
  Serial.println(millis() - Rec_time);
  digitalWrite(DCPump, LOW);
  while(millis() - Rec_time < spin_time_var){
    if((millis() - dir_time) > 1500 && Trig == 1){
      Serial.println(Trig);
      dir_time = millis();
      Trig = 0;
      digitalWrite(spin_DIR, LOW);
    }

    else if((millis() - dir_time) > 1500 && Trig == 0){
      Serial.println("BYE");
      dir_time = millis();
      Trig = 1;
      digitalWrite(spin_DIR, HIGH);
    }
  }
  digitalWrite(DCPump, HIGH);
  digitalWrite(spin_DIR, LOW);
  delay(10000);
  digitalWrite(spin_DIR, HIGH);
  delay(10000);
  analogWrite(spin_PWM,0);
  
}
