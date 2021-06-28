#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ACPump 7
#define Fan1 19
#define Fan2 6

#define Solenoid 5
#define Vac_pump 4
#define DCPump 14
#define DCMotor 18
#define VC_pin 2
#define Tc_pin 15
#define Th_pin 17

int PressureSens = A0;
int PotPin = A1;
int Vpot, Vpres;

float set_T = 1.0;  //Set temp for triggering relays
float pressure = 0; 
float Tc, Th, VC_tempval; //Temp hot, cold and vacuum chamber

unsigned long Time;
unsigned long fTime;
unsigned long sTime;
unsigned long mTime;
unsigned long MotorTime;
int TriggerS, Triggerf, Triggerm, TriggerMotor = 0;
 
int aState;
int aLastState;  
 
OneWire oneWire(VC_pin);
OneWire oneWire1(Tc_pin);
OneWire oneWire2(Th_pin);
DallasTemperature VC_temp(&oneWire);
DallasTemperature Tc_temp(&oneWire1);
DallasTemperature Th_temp(&oneWire2);
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

void setup() {
  pinMode(ACPump, OUTPUT);
  pinMode(Fan1, OUTPUT);
  pinMode(Fan2, OUTPUT);
  pinMode(Solenoid, OUTPUT);
  pinMode(Vac_pump, OUTPUT);
  pinMode(DCPump, OUTPUT);
  pinMode(DCMotor, OUTPUT);
  
  digitalWrite(ACPump, HIGH);
  digitalWrite(Fan1, HIGH);
  digitalWrite(Fan2, HIGH);
  digitalWrite(Solenoid, HIGH);
  digitalWrite(Vac_pump, HIGH);
  digitalWrite(DCPump, HIGH);

  lcd.begin(16, 2);
  VC_temp.begin();
  Tc_temp.begin();
  Th_temp.begin();
  Serial.begin(9600);
}

void loop() {
  Vpot = analogRead(PotPin);
  
  VC_temp.requestTemperatures();
  VC_tempval = VC_temp.getTempCByIndex(0);

  Tc_temp.requestTemperatures();
  Tc = Tc_temp.getTempCByIndex(0);

  Th_temp.requestTemperatures();
  Th = Th_temp.getTempCByIndex(0);
  
  set_T = mapfloat(Vpot, 1023, 0, -15, 10);
  
  if(Triggerm == 0){
    Triggerm == 1;
    
    Vpres = analogRead(PressureSens);
    pressure = mapfloat(Vpres, 102.4, 921.6, 0, 100) + 3;
    
    mTime = millis();
    Serial.print("Cold Temperature: ");
    Serial.print(Tc);
    Serial.println(" C");
  
    Serial.print("Hot Temperature: ");
    Serial.print(Th);
    Serial.println(" C");
  
    Serial.print("Set Temperature: ");
    Serial.print(set_T);
    Serial.println(" C");
  
    Serial.print("Vacuum Chamber Pressure: ");
    Serial.print(pressure);
    Serial.println(" psi");
  }

  Time = millis();
  if((Time - mTime) > 1500){
    Triggerm = 0;
  }
  
  lcd.setCursor(0, 0);
  lcd.print("Temp = ");
  lcd.print(Tc);   
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Set temp = ");
  lcd.print(set_T, 1);   
  lcd.print(" C");
  
  Triggerf = 0;
  if(Tc > (set_T + 0.5)){
    
    TriggerMotor = 0;
    
    if(Th>32){
      digitalWrite(Fan1, LOW);
      if(Th>36){
        digitalWrite(Fan2, LOW);
      }
      else if(Th<31){
        digitalWrite(Fan2, HIGH);
      }
    }
    else if(Th<28){
      digitalWrite(Fan1, HIGH);
      digitalWrite(Fan2, HIGH);
    }
    digitalWrite(ACPump, LOW);
    digitalWrite(DCMotor, LOW);
    if(pressure<25){
      digitalWrite(Vac_pump, LOW);
    }
    else{
      digitalWrite(Vac_pump, HIGH); 
    }
    digitalWrite(DCPump, LOW);

    if(TriggerS==0){
      TriggerS=1;
      if(Tc<10){
        digitalWrite(Solenoid,LOW);
        delay(4000);
        digitalWrite(Solenoid,HIGH);
      }
      else{
        digitalWrite(Solenoid,LOW);
        delay(4000);
        digitalWrite(Solenoid,HIGH);
      }
      sTime = millis();
    }
    Time = millis();
    if(Tc>15){
      if((Time-sTime)>60000){
//        lcd.begin(16, 2);
        TriggerS = 0;
      }
    }
    else if((Tc<15) && (Tc>10)){
      if((Time-sTime)>120000){
//        lcd.begin(16, 2);
        TriggerS = 0;
      }
    }
    else{
      if((Time-sTime)>120000){
        TriggerS = 0;
      }
    }
  }
  else if(Tc<set_T){
    Time = millis();
    if(Th>30){
      digitalWrite(ACPump, LOW);
      digitalWrite(Fan1, LOW);
      if(Th>33){
        digitalWrite(Fan2, LOW);
      }
    }
    else if(Th<28){
      digitalWrite(Fan1, HIGH);
      digitalWrite(Fan2, HIGH);
      digitalWrite(ACPump, HIGH);
    }
    if (TriggerMotor==0){
      TriggerMotor = 1;
      MotorTime = millis();
      Time = millis();
    }
    else if((Time - MotorTime) >= 90000){
      digitalWrite(DCPump, HIGH);
    }
    
    digitalWrite(Vac_pump, HIGH);
    digitalWrite(DCMotor, HIGH);
    if(TriggerS == 1){
      TriggerS = 0;
      digitalWrite(Solenoid,LOW);
      delay(5000);
      digitalWrite(Solenoid,HIGH);
    }
  }
}



float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
 return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}
