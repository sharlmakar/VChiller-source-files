#include <OneWire.h>
#include <DallasTemperature.h> //For temperature sensors

#define test_pin 9
#define Temp_ColWater_p 2

int Tcold = 0;

OneWire oneWire1(Temp_ColWater_p);
DallasTemperature Temp_ColWater(&oneWire1);

void setup() {
  
  pinMode(test_pin, OUTPUT);
  Tcold = GetTemp(Temp_ColWater, Tcold);
  Serial.begin(9600);
}

void loop() {

  Tcold = GetTemp(Temp_ColWater, Tcold);

  if(Tcold<40){
    Serial.println("Working");
    Serial.println(Tcold);
    digitalWrite(test_pin,HIGH);
    delay(10000);
    digitalWrite(test_pin,LOW);
    delay(10000);
  }

  else{
    Serial.println("Not working");
    digitalWrite(test_pin,LOW);
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
