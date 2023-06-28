#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 6
//#define ONE_WIRE_BUS1 14

OneWire oneWire(ONE_WIRE_BUS);
//OneWire oneWire1(ONE_WIRE_BUS1);

DallasTemperature sensors(&oneWire);
//DallasTemperature sensors1(&oneWire1);

 float Celcius=0;
 float Fahrenheit=0;
void setup(void)
{
  
  Serial.begin(9600);
  sensors.begin();
}

void loop(void)
{ 
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  Serial.print("Sensor 1: C  ");
  Serial.print(Celcius);
  Serial.print(" F  ");
  Serial.println(Fahrenheit);

//  sensors1.requestTemperatures(); 
//  Celcius=sensors1.getTempCByIndex(0);
//  Fahrenheit=sensors1.toFahrenheit(Celcius);
//  Serial.print("Sensor 2: C  ");
//  Serial.print(Celcius);
//  Serial.print(" F  ");
//  Serial.println(Fahrenheit);
  delay(1000);
}
