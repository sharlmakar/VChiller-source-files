#include <OneWire.h>
#include <DallasTemperature.h> //For temperature sensors
#include <Adafruit_MLX90614.h> //For infrared temperature sensor
#include <Wire.h> // For I2C
#include <LiquidCrystal_I2C.h> // Added library*

#define Vac_pump 13
#define Solenoid 12
#define Fan1 11
#define Fan2 10

#define DCPump 9
#define ACPump 8

#define Temp_ColWater_p 2
#define Temp_Refrig_p 14

int PressureSens = A3;
int PotPin = A4;
int Vpot, Vpres;

float set_T = -10;
float pressure = 0;
float Tcold, Thot = 5; //Temp cold water and hot refrigerant

unsigned long Time;
unsigned long Sol_open_time;
unsigned long Sol_interv_time;
unsigned long Mag_stir_time;
unsigned long print_time, lcd_time;
int TriggerSol_interv, Trigger_hot, Trigger_solenoid  = 0;
int TriggerSol_open = 1;

int spin_time_condition = 1000;
int throttle = 51;

OneWire oneWire1(Temp_ColWater_p);
OneWire oneWire2(Temp_Refrig_p);
DallasTemperature Temp_ColWater(&oneWire1);
DallasTemperature Temp_Refrig(&oneWire2);

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);

void setup() {

  pinMode(ACPump, OUTPUT);
  pinMode(Fan1, OUTPUT);
  pinMode(Fan2, OUTPUT);
  pinMode(Solenoid, OUTPUT);
  pinMode(Vac_pump, OUTPUT);
  pinMode(DCPump, OUTPUT);
  
  digitalWrite(ACPump, LOW);
  digitalWrite(Fan1, LOW);
  digitalWrite(Fan2, LOW);
  digitalWrite(Solenoid, LOW);
  digitalWrite(Vac_pump, LOW);
  digitalWrite(DCPump, LOW);
  
  lcd.begin (20,4); // 20 x 4 LCD module
  lcd.setBacklightPin(3,POSITIVE); // BL, BL_POL
  lcd.setBacklight(HIGH);
  lcd.home ();

  Serial.begin(9600);

  Tcold = GetTemp(Temp_ColWater, Tcold);
  Thot = GetTemp(Temp_Refrig, Thot);

  print_time = millis();
  lcd_time = millis();

  lcd_print(); 
  
  
}

void loop() {

  Time = millis();
  
  Vpot = analogRead(PotPin);
  set_T = mapfloat(Vpot, 1023, 0, -10, 20);
  Tcold = GetTemp(Temp_ColWater, Tcold);
  Thot = GetTemp(Temp_Refrig, Thot);

  if((Time-print_time)>2000){
    print_time = millis();

    Serial.print("Cold Temperature: ");
    Serial.print(Tcold);
    Serial.println(" C");

    Serial.print("Hot Temperature: ");
    Serial.print(Thot);
    Serial.println(" C");
  
    Serial.print("Set Temperature: ");
    Serial.print(set_T);
    Serial.println(" C");

    Serial.println();
  }
  if((Time - lcd_time)>1000){
    lcd_time = millis();
    lcd_print();  
  }

  if(Thot > 80 && Thot != 85.00 && Trigger_hot != 2){
    Trigger_hot = 1;
  }
  else if(Thot < 60){
    Trigger_hot = 0;
  }

  
  if(Trigger_hot == 0){
    if(Tcold > (set_T + 2)){
      Trigger_solenoid = 0;
      if(Thot>32){
        digitalWrite(Fan1, HIGH);
        if(Thot>36){
          digitalWrite(Fan2, HIGH);
        }
        else if(Thot<34){
          digitalWrite(Fan2, LOW);
        }
      else if(Thot<30){
        digitalWrite(Fan1, LOW);
        digitalWrite(Fan2, LOW);
      }
     }
      digitalWrite(DCPump, HIGH);
      digitalWrite(Vac_pump, HIGH);
      Solenoid_op();
   }
  
    else if(Tcold<set_T){
      throttle = 51;
      TriggerSol_interv = 0;
      spin_time_condition = 1000;
      
      if(Trigger_solenoid == 0){
        lcd_print();
        Trigger_solenoid = 1;
        digitalWrite(Solenoid, HIGH);
        delay(2000);
        digitalWrite(Vac_pump, LOW);
        delay(5000);
        digitalWrite(Solenoid, LOW);
      }
  
      digitalWrite(Solenoid, LOW);
      digitalWrite(DCPump, LOW);
      digitalWrite(Vac_pump, LOW);
      digitalWrite(Fan1, LOW);
      digitalWrite(Fan2, LOW);
  
    }
  }
  else if (Trigger_hot == 1){
    Trigger_hot = 2;
    digitalWrite(Solenoid, HIGH);
    lcd_print();
    delay(2000);
    digitalWrite(Vac_pump, LOW);
    delay(10000);
    digitalWrite(Solenoid, LOW);
    digitalWrite(DCPump, LOW);
    digitalWrite(Fan1, HIGH);
    digitalWrite(Fan2, HIGH);
    
    throttle = 51;
    TriggerSol_interv = 0;
    spin_time_condition = 1000;
  }
}

//
//void Mag_stir_func(){
//  if(TriggerMag == 0){
//    Mag_stir_time = millis();
//    TriggerMag = 1;
//    analogWrite(Mag_stir[0], throttle);
//  }
//
//  if(((Time - Mag_stir_time) > spin_time_condition) && (throttle < 255)){
//    throttle += 51;
//    analogWrite(Mag_stir[0], throttle);
//    spin_time_condition += 1000;
//  }
//}

void Solenoid_op(){
  if(TriggerSol_interv == 0){
    TriggerSol_interv = 1;
    TriggerSol_open = 0;
    digitalWrite(Solenoid, HIGH);
    Sol_interv_time = millis();
    Sol_open_time = millis();
  }

  else if(((Time - Sol_open_time) > 5000) && (TriggerSol_open == 0)){
    digitalWrite(Solenoid, LOW);
    Sol_interv_time = millis();
    TriggerSol_open = 1;
  }

  else if((Time - Sol_interv_time) > 60000){
    TriggerSol_interv = 0;
  }
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
  if (floor(reading1) == floor(reading2)){
    return reading2;
  }
  else{
    return reading0;
  }
}

float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
 return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

void lcd_print(){
  lcd.setCursor(0, 0);
  lcd.print("Col temp = ");
  lcd.print(Tcold);   
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Set temp = ");
  lcd.print(set_T);   
  lcd.print(" C");

  lcd.setCursor(0, 2);
  lcd.print("Hot temp = ");
  lcd.print(Thot);
  lcd.print(" C");

//  lcd.setCursor(0, 3);
//  lcd.print("Ref temp = ");
//  lcd.print(Trefrig);   
//  lcd.print(" C");
}
