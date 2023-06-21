//Check if the relays

#include <OneWire.h>
#include <DallasTemperature.h> //For temperature sensors
#include <Wire.h> // For I2C
#include <LiquidCrystal_I2C.h> //I2C LCD library

#define Vac_pump 13
#define Solenoid 12
#define Fan1 11
#define Fan2 10
#define DCPump 9
#define AirBlow 8

#define Temp_VC_p 15
#define Temp_Water_p 6
#define Temp_Rad_p 5
#define Temp_cold_stor_p 4
#define Dir_val 3

int PotPin = A0;

float T_VC, T_Water, T_Rad, T_cold_stor = 10;
float Vpot = 0;
float set_T = -10; 

unsigned long Time;
unsigned long Sol_open_time;
unsigned long Sol_interv_time;
unsigned long print_time, lcd_time;
int TriggerSol_interv, Trigger_hot, Trigger_solenoid, Trigger_AB, Trigger_AirBlow  = 0;
int TriggerSol_open = 1;

OneWire oneWire1(Temp_VC_p);
OneWire oneWire2(Temp_Water_p );
OneWire oneWire3(Temp_Rad_p );
OneWire oneWire4(Temp_cold_stor_p);
DallasTemperature Temp_VC(&oneWire1);
DallasTemperature Temp_Water(&oneWire2);
DallasTemperature Temp_Rad(&oneWire3);
DallasTemperature Temp_cold_stor(&oneWire4);

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);

void setup() {

  pinMode(Fan1, OUTPUT);
  pinMode(Fan2, OUTPUT);
  pinMode(Solenoid, OUTPUT);
  pinMode(Vac_pump, OUTPUT);
  pinMode(DCPump, OUTPUT);
  pinMode(AirBlow, OUTPUT);
  pinMode(Dir_val, OUTPUT);
  
  digitalWrite(Fan1, HIGH);
  digitalWrite(Fan2, HIGH);
  digitalWrite(Solenoid, HIGH);
  digitalWrite(Vac_pump, HIGH);
  digitalWrite(DCPump, HIGH);
  digitalWrite(AirBlow, HIGH);
  digitalWrite(Dir_val, HIGH);
  
  lcd.begin (20,4); // 20 x 4 LCD module
  lcd.setBacklightPin(3,POSITIVE); // BL, BL_POL
  lcd.setBacklight(HIGH);
  lcd.home ();

  Serial.begin(9600);
  Serial.println("Ard5");
  
  Temp_VC.begin();
  Temp_Water.begin();
  Temp_Rad.begin();
  Temp_cold_stor.begin();
  
  Readings(); //Initial readings
  
  print_time = millis();
  lcd_time = millis();

  lcd_print(); 
  
  
}

void loop(){

  Time = millis();
  Readings(); //Readings done at the beginning of every loop

  if((Time-print_time)>2000){ //Print readings to serial monitor every 2sec
    print_time = millis();

    Serial.print("Cold Temperature: ");
    Serial.print(T_Water);
    Serial.println(" C");

    Serial.print("Hot Temperature: ");
    Serial.print(T_Rad);
    Serial.println(" C");
  
    Serial.print("Set Temperature: ");
    Serial.print(set_T);
    Serial.println(" C");

    Serial.print("Cold storage Temperature: ");
    Serial.print(T_cold_stor);
    Serial.println(" C");

    Serial.println();
  }
  lcd_print();
//  if((Time - lcd_time)>1000){ //Refresh LCD every sec
//    lcd_time = millis();
//    lcd_print();  
//  }

  if(T_Rad > 80 && T_Rad != 85.00 && Trigger_hot != 2){
    Trigger_hot = 1;
  }
  else if(T_Rad < 60){
    Trigger_hot = 0;
  }

  if(Trigger_hot == 0){
    
    if(T_Water > (set_T + 2)){
      Trigger_solenoid = 0;
      
      if(T_Rad>35){
        digitalWrite(Fan2, LOW);
      }
      
      else if(T_Rad<33){
        digitalWrite(Fan2, HIGH);
      }
      digitalWrite(Fan1, LOW);
      digitalWrite(DCPump, LOW);
      digitalWrite(Vac_pump, LOW);
      digitalWrite(Dir_val, HIGH);
      Airblow_off();
      Solenoid_op();
    }

    else if(T_Water<set_T){
      
      lcd_print();
       
      if(T_cold_stor > -3){
        Trigger_solenoid = 0;
        digitalWrite(DCPump, HIGH);
        digitalWrite(Vac_pump, LOW);
        digitalWrite(Fan1, LOW);
        Solenoid_op();
        digitalWrite(Dir_val, LOW);
        AirBlow_on();
        
        if(T_Rad>35){
          digitalWrite(Fan2, LOW);
        }
        
        else if(T_Rad<33){
          digitalWrite(Fan2, HIGH);
        }
      }

      else if(T_cold_stor <= -5){
        TriggerSol_interv = 0;
        if(Trigger_solenoid == 0){
          Trigger_solenoid = 1;
          digitalWrite(Solenoid, LOW);
          delay(2000);
          digitalWrite(Vac_pump, HIGH);
          delay(7000);
          digitalWrite(Solenoid, HIGH);
        }
        digitalWrite(DCPump, HIGH);
        digitalWrite(Fan1, HIGH);
        digitalWrite(Fan2, HIGH);
        digitalWrite(Dir_val, HIGH);
        Airblow_off();
      }
    }
  }
  
  else if (Trigger_hot == 1){
    Trigger_hot = 2;
    digitalWrite(Solenoid, LOW);
    lcd_print();
    delay(2000);
    digitalWrite(Vac_pump, HIGH);
    delay(10000);
    digitalWrite(Solenoid, HIGH);
    digitalWrite(DCPump, HIGH);
    digitalWrite(AirBlow, LOW);
    digitalWrite(Fan1, LOW);
    digitalWrite(Fan2, LOW);
    
    TriggerSol_interv = 0;
  }
}

void Readings(){ //Cycle readings
  
//  T_VC = temp_read_ntc(Temp_VC_p);    //initial sensor readings
//  T_Water = temp_read_ntc(Temp_Water_p);
//  T_Rad = temp_read_ntc(Temp_Rad_p);
//  T_cold_stor = temp_read_ntc(Temp_cold_stor_p);
  
  Vpot = analogRead(PotPin); //Set temperature inital reading
  set_T = mapfloat(Vpot, 1023, 0, -15, 30);

  T_VC = GetTemp(Temp_VC, T_VC);    //Temperature sensor readings
  T_Water = GetTemp(Temp_Water, T_Water);
  delay(500);
  T_Rad = GetTemp(Temp_Rad, T_Rad);
  T_cold_stor = GetTemp(Temp_cold_stor, T_cold_stor);
  
}

void Solenoid_op(){
  if(TriggerSol_interv == 0){
    TriggerSol_interv = 1;
    TriggerSol_open = 0;
    digitalWrite(Solenoid, LOW);
    Sol_interv_time = millis();
    Sol_open_time = millis();
  }

  else if(((Time - Sol_open_time) > 7000) && (TriggerSol_open == 0)){
    digitalWrite(Solenoid, HIGH);
    Sol_interv_time = millis();
    TriggerSol_open = 1;
  }

  else if(((Time - Sol_interv_time) > 120000) && T_Water > 25){
    TriggerSol_interv = 0;
  }

  else if(((Time - Sol_interv_time) > 150000) && T_Water > 15 && T_Water <= 25){
    TriggerSol_interv = 0;
  }

  else if(((Time - Sol_interv_time) > 180000) && T_Water > 10 && T_Water <= 15){
    TriggerSol_interv = 0;
  }

  else if(((Time - Sol_interv_time) > 240000) && T_Water <= 10){
    TriggerSol_interv = 0;
  }
}

float temp_read_ntc(int Temp_pin){

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

float mapfloat(long x, long in_min, long in_max, long out_min, long out_max) //finds the value of the analogue reading of device
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

void lcd_print(){
  lcd.setCursor(0, 0);
  lcd.print("Col temp = ");
  lcd.print(T_Water);   
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Set temp = ");
  lcd.print(set_T);   
  lcd.print(" C");

  lcd.setCursor(0, 2);-
  lcd.print("Hot temp = ");
  lcd.print(T_Rad);
//  lcd.print(T_cold_stor);
  lcd.print(" C");

  lcd.setCursor(0, 3);
  lcd.print("Ref temp = ");
  lcd.print(T_VC);
  lcd.print(" C");
}

float GetTemp(DallasTemperature temp, float reading0){
  
  float reading1,reading2 = 0;
  temp.requestTemperatures();
  delay(25);
  reading1 = temp.getTempCByIndex(0);
  delay(10);
  temp.requestTemperatures();
  delay(25);
  reading2 = temp.getTempCByIndex(0);
  if(reading1 == -127 && reading2 == -127){
    temp.begin();
  }
  if ((floor(reading1) == floor(reading2)) && (reading1 != 85 && reading2 != 85) || reading1 != -127){ //&& reading1 != -127 && reading2 != -127)){
    return reading2;
  }
  else{
    return reading0;
  }
}

void AirBlow_on(){
  if(Trigger_AirBlow == 0){
    digitalWrite(AirBlow, LOW);
    delay(3000);
    digitalWrite(AirBlow, HIGH);
    delay(2000);
    digitalWrite(AirBlow, LOW);
    delay(1500);
    digitalWrite(AirBlow, HIGH);
    Trigger_AirBlow = 1;
  }
}

void Airblow_off(){
  if(Trigger_AirBlow == 1){
    digitalWrite(AirBlow, LOW);
    delay(500);
    digitalWrite(AirBlow, HIGH);
    Trigger_AirBlow = 0;
  }
}
