 const int pressureInput = A0; //select the analog input pin for the pressure transducer
const float pressureZero = 102.4; //analog reading of pressure transducer at 0psi
const float pressureMax = 921.6; //analog reading of pressure transducer at 100psi
const float pressuretransducermaxPSI = 100; //psi value of transducer being used
const int baudRate = 9600; //constant integer to set the baud rate for serial monitor
const int sensorreadDelay = 1000; //constant integer to set the sensor read delay in milliseconds


float pressureValue = 0; //variable to store the value coming from the pressure transducer
float bar = 0;

void setup() //setup routine, runs once when system turned on or reset
{
  Serial.begin(baudRate); //initializes serial communication at set baud rate bits per second
}

void loop() //loop routine runs over and over again forever
{
  pressureValue = analogRead(pressureInput); //reads value from input pin and assigns to variable
  Serial.print("Raw value: ");
  Serial.println(pressureValue);
  //pressureValue = ((pressureValue-pressureZero)*pressuretransducermaxPSI)/(pressureMax-pressureZero); //conversion equation to convert analog reading to psi
  pressureValue = mapfloat(pressureValue, 102.4, 921.6, 0, pressuretransducermaxPSI) + 1;
  bar = (pressureValue/14.504);
  Serial.print(pressureValue, 1); //prints value from previous line to serial
  Serial.print("psi "); //prints label to serial
  Serial.print(bar);
  Serial.println("bar");
  delay(sensorreadDelay); //delay in milliseconds between read values
}

float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
 return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}
