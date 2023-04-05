#include <OneWire.h>
#include <DallasTemperature.h> //For temperature sensors

#define Temp_VC_p 15
#define Temp_Water_p 6
#define Temp_Rad_p 5
#define Temp_cold_stor_p 4
#define Solenoid 12

float T_VC, T_Water, T_Rad, T_cold_stor = 10;

OneWire oneWire1(Temp_VC_p);
OneWire oneWire2(Temp_Water_p );
OneWire oneWire3(Temp_Rad_p );
OneWire oneWire4(Temp_cold_stor_p);
DallasTemperature Temp_VC(&oneWire1);
DallasTemperature Temp_Water(&oneWire2);
DallasTemperature Temp_Rad(&oneWire3);
DallasTemperature Temp_cold_stor(&oneWire4);

void setup() {
  pinMode(Solenoid, OUTPUT);
  digitalWrite(Solenoid, HIGH);
//  Readings();
  Serial.begin(9600);
}

void loop() {
  Readings();
  // put your main code here, to run repeatedly:
  Serial.print("Cold Temperature: ");
  Serial.print(T_Water);
  Serial.println(" C");

  Serial.print("Hot Temperature: ");
  Serial.print(T_VC);
  Serial.println(" C");

  Serial.println();
  delay(5000);
  digitalWrite(Solenoid, LOW);
  delay(5000);
  digitalWrite(Solenoid, HIGH);
  delay(5000);
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
  if ((floor(reading1) == floor(reading2)) && (reading1 != 85 && reading2 != 85)){ //&& reading1 != -127 && reading2 != -127)){
    return reading2;
  }
  else{
    return reading0;
  }
}

void Readings(){ //Cycle readings

  T_VC = GetTemp(Temp_VC, T_VC);    //Temperature sensor readings
  T_Water = GetTemp(Temp_Water, T_Water);
  T_Rad = GetTemp(Temp_Rad, T_Rad);
  T_cold_stor = GetTemp(Temp_cold_stor, T_cold_stor);
  
}
