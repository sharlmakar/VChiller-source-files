#include <Wire.h> // <-- remove spaces

int x =0;

int slaveAddress = 1;

const int buttonPin = A3; // the number of the pushbutton pin

int buttonState = 0; // variable for reading the pushbutton status

void setup()

{

  pinMode( buttonPin, INPUT ); // initialize the pushbutton pin as an input
  
  Wire.begin(); // join i2c bus (address optional for master)

}

void loop()

{

  // read the state of the pushbutton value:
  
  buttonState = digitalRead( buttonPin );
  
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  
  if ( buttonState == HIGH )
  
  {
  
    x = 1;
  
  }
  
  else
  
  {
  
    x = 0;
  
  }
  
  Wire.beginTransmission( slaveAddress ); // transmit to device #9
  
  Wire.write(x); // sends x
  
  Wire.endTransmission(); // stop transmitting
  
  delay(200);

}
