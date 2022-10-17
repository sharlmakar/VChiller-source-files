/* Learn how to use a potentiometer to fade an LED with Arduino - Tutorial
   More info and circuit schematic: http://www.ardumotive.com/arduino-tutorials/arduino-fade-led
   Dev: Michalis Vasilakis / Date: 25/10/2014                                                   */
   

//Constants:
const int ledPin = 13;  //pin 9 has PWM funtion
const int potPin = A0; //pin A0 to read analog input

//Variables:
int value; //save analog value


void setup(){
  //Input or output?
//  pinMode(ledPin, OUTPUT); 
  pinMode(potPin, INPUT); //Optional 
  Serial.begin(9600);

}

void loop(){
  
  value = analogRead(potPin);          //Read and save analog value from potentiometer
  Serial.println(value);
//  value = map(value, 0, 1023, 0, 255); //Map value 0-1023 to 0-255 (PWM)
//  analogWrite(ledPin, 255);          //Send PWM value to led
  delay(100);                          //Small delay
  
}
