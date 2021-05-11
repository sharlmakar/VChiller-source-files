#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ACPump 7
#define Fan2 6
#define Solenoid 5
#define Vac_pump 4
#define Fan1 3
#define DCPump 14
#define VC_pin 2
#define Tc_pin 15
#define Th_pin 17
#define Sol_piston 18
//#define encoderA 18
//#define encoderB 19

int PressureSens = A0;
int PotPin = A2;
int Vpot, Vpres;

float set_T = 1.0;  //Set temp for triggering relays
float pressure = 0; 
float Tc, Th, VC_tempval; //Temp hot, cold and vacuum chamber

unsigned long Time;
unsigned long fTime;
unsigned long sTime;
unsigned long mTime;
int TriggerS, Triggerf, TriggerT, TriggerSP, TriggerSolT = 0;
 
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
  pinMode(Fan2, OUTPUT);
  pinMode(Solenoid, OUTPUT);
  pinMode(Vac_pump, OUTPUT);
  pinMode(Fan1, OUTPUT);
  pinMode(DCPump, OUTPUT);
  pinMode(Sol_piston, OUTPUT);
//  pinMode (encoderA,INPUT);
//  pinMode (encoderB,INPUT);
  
  digitalWrite(ACPump, HIGH);
  digitalWrite(Fan2, HIGH);
  digitalWrite(Solenoid, HIGH);
  digitalWrite(Vac_pump, HIGH);
  digitalWrite(Fan1, HIGH);
  digitalWrite(DCPump, HIGH);
  digitalWrite(Sol_piston, HIGH);

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
  
  set_T = mapfloat(Vpot, 0, 1023, -7, 25);
  
  if(TriggerT == 0){
    TriggerT == 1;
    
    Vpres = analogRead(PressureSens);
    pressure = mapfloat(Vpres, 102.4, 921.6, 0, 100) + 2;
    
    mTime = millis();
    Serial.print("Cold Temperature: ");
    Serial.print(Tc);
    Serial.println(" C");
    
    Serial.print("Hot Temperature: ");
    Serial.print(Th);
    Serial.println(" C");
    
    Serial.print("Vacuum Chamber Temperature: ");
    Serial.print(VC_tempval);
    Serial.println(" C");
    
    Serial.print("Vacuum Chamber Pressure: ");
    Serial.print(pressure);
    Serial.println(" psi");
  }

  Time = millis();
  if((Time - mTime) > 1500){
    TriggerT = 0;
  }
  
  lcd.setCursor(0, 0);
  lcd.print("Temp = ");
  lcd.print(Tc);   
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Set temp = ");
  lcd.print(set_T, 1);   
  lcd.print(" C");
  
  if(Th>30){
    if(Triggerf==0){
      Triggerf = 1;
      fTime = millis();
    }
    digitalWrite(ACPump, HIGH);
    digitalWrite(Vac_pump, HIGH);
    digitalWrite(DCPump, HIGH);
    digitalWrite(Solenoid,HIGH);
    Time = millis();
    if((Time-fTime)<600000){
      digitalWrite(Fan1, LOW);
      if(Th>33){
        digitalWrite(Fan2, LOW);
      }
    }
    else{
      digitalWrite(Fan1, HIGH);
      digitalWrite(Fan2, HIGH);
      Serial.print("Error: water tank is too hot for unknown reason.");
      delay(1000);
    }
  }
  else if(Th<29){
    TriggerSolT = 0;
    Triggerf = 0;
    digitalWrite(Fan1, HIGH);
    digitalWrite(Fan2, HIGH);
    
    if(Tc >set_T){
      digitalWrite(ACPump, LOW);
      digitalWrite(DCPump, LOW);
      if(pressure<25){
        digitalWrite(Vac_pump, LOW);
      }
      else{
        digitalWrite(Vac_pump, HIGH); 
      }
      if(TriggerS==0){
        TriggerS=1;
        if(Tc<10){
          digitalWrite(Solenoid,LOW);
          delay(1000);
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
          TriggerS = 0;
        }
      }
      else if((Tc<15) && (Tc>10)){
        if((Time-sTime)>120000){
          TriggerS = 0;
        }
      }
      else{
        if((Time-sTime)>5000){
          TriggerS = 0;
        }
      }
    }
    else if(Tc<(set_T-0.5)){
      if(TriggerSolT == 0){
        digitalWrite(Solenoid, LOW);
        delay(5000);
        digitalWrite(Solenoid, HIGH);
      }
      TriggerSolT = 1;
      digitalWrite(ACPump, HIGH);
      digitalWrite(Vac_pump, HIGH);
      digitalWrite(DCPump, HIGH);
      digitalWrite(Sol_piston, HIGH);
    }
  }
}

float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
 return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}
