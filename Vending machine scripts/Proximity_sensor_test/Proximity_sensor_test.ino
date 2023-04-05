int IRSensor = 10; // connect ir sensor to arduino pin 2
//int LED = 13; // conect Led to arduino pin 13



void setup() 
{
  pinMode (IRSensor, INPUT); // sensor pin INPUT
  Serial.begin(9600);
}

void loop()
{
  int statusSensor = digitalRead (IRSensor);
  
  if (statusSensor == 1)
  {
    Serial.println("YES");
  }
  
  else
  {
    Serial.println("NO"); // LED High
  }
  
}
