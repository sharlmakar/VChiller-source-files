
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include <AccelStepper.h>

AccelStepper stepperX(1, 5, 4);   

// Home Switch
#define home_switch 2 

// Stepper Travel Variables
long TravelX; 
int move_finished=1;  
long initial_homing=1;  

// LCD I2C
LiquidCrystal_PCF8574 lcd(0x27);

// Input potentiometers
const int potPin1 = A3;  
const int potPin2 = A1; 

int potValue1 = 0;
int potValue2 = 0;

int potMap1 = 0;
int potMap2 = 0;

// Buttons & switches
int BUTTON = 3;
int state = HIGH;

// Motors
int motor1 = 8;
int motor2 = 10;

// Stepper motor
int dir = 4;
int stp = 5;

// Other variables
int brzina_int = 0;
int vreme_int = 0;
int brzina_ciklusa = 0;
int vreme_ciklusa = 0;
long vreme_ciklusa_ms;

void setup()
{
  delay(100);
  analogWrite(motor1, 255);
  analogWrite(motor2, 255);
  delay(100);
  
  // Start I2C communication with LCD
  Wire.begin();
  Wire.beginTransmission(0x27);

  // Setovanje BUTTONa i njegove interrupt funkcije
  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON), BUTTON_interrupt, FALLING);

  updateLCD_start();

  // Homing stepper motora
  pinMode(home_switch, INPUT_PULLUP);
  delay(5);  
  
  // Stepper speed & acceleration
  stepperX.setMaxSpeed(700.0);   
  stepperX.setAcceleration(300.0);
 
  // Start Homing procedure of Stepper Motor at startup

  // H O M I N G //
  lcd.clear();
  lcd.print("Homing procedure");
  delay(100);

  while (digitalRead(home_switch)) { 
    stepperX.moveTo(initial_homing);  
    initial_homing--;  
    stepperX.run(); 
    delay(5);
}

  stepperX.setCurrentPosition(0);  
  stepperX.setMaxSpeed(5000.0);    
  stepperX.setAcceleration(3000.0);  
  initial_homing=1;

  while (!digitalRead(home_switch)) { 
    stepperX.moveTo(initial_homing);  
    stepperX.run();
    initial_homing++;
    delay(5);
  }
  
  stepperX.setCurrentPosition(0);
  lcd.clear();
  lcd.print("Homing finished");
  delay(2000);

  stepperX.setMaxSpeed(1100.0);      // Set Max Speed of Stepper (Faster for regular movements)
  stepperX.setAcceleration(600.0);  // Set Acceleration of Stepper

  // H O M I N G //


  // Move stepper from zero position (protection)
  state = HIGH;

  // Pomeranje od nule
  delay(1000);
  stepperX.moveTo(80);
  while (stepperX.currentPosition() != 80) stepperX.run();
  stepperX.stop(); 
  stepperX.runToPosition(); 
  stepperX.setCurrentPosition(0);

  
} 

// Start button interrupt
void BUTTON_interrupt() 
{
  if(state == HIGH) {
    state = LOW;
  }
}


void updateLCD_start()
{
  lcd.clear();
  lcd.setBacklight(255);
  lcd.noCursor();
  lcd.home();
  delay(2000);
  lcd.print("CanCool");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);
  lcd.clear();
}

// Update LCD menu
void updateLCD_menu()
{
  //lcd.clear();
  lcd.setBacklight(255);
  lcd.noCursor();
  lcd.home();
  lcd.print("Speed      Time ");
  lcd.setCursor(0, 1);
  
  brzina_int = pot1_read();
  vreme_int = pot2_read();


  // Show speed on the LCD 
  if(brzina_int<10)
  {
    lcd.setCursor(0,1);
    lcd.print("  ");
    lcd.print(brzina_int);
    lcd.print("%");
    lcd.setCursor(11,1);
      if(vreme_int>9 && vreme_int<100)
      {
        lcd.print(" ");
        lcd.print(vreme_int);
        lcd.print("s");
      }
      else
      {
        lcd.print(vreme_int);
        lcd.print("s");
      }
  } else if(brzina_int>9 && brzina_int<100)
  {
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.print(brzina_int);
    lcd.print("%");
    lcd.setCursor(11,1);

      if(vreme_int>9 && vreme_int<100)
      {
        lcd.print(" ");
        lcd.print(vreme_int);
        lcd.print("s");
      }
      else
      {
        lcd.print(vreme_int);
        lcd.print("s");
      }
  }
  else
  {
    lcd.setCursor(0,1);
    lcd.print(brzina_int);
    lcd.print("%");
    lcd.setCursor(11,1);

      if(vreme_int>9 && vreme_int<100)
      {
        lcd.print(" ");
        lcd.print(vreme_int);
        lcd.print("s");
      }
      else
      {
        lcd.print(vreme_int);
        lcd.print("s");
      }
  }
  
}

// Update LCD-a kada je uredjaj ukljucen
void updateLCD_working()
{

}

int pot1_read()
{
  potValue1 = analogRead(potPin1);
  potMap1 = map(potValue1, 0, 1023, 50, 100);
  delay(100);

  return potMap1;
}

int pot2_read()
{
  potValue2 = analogRead(potPin2);
  potMap2 = map(potValue2, 0, 1023, 10, 120);
  delay(100);

  return potMap2;
}

void loop()
{
  updateLCD_menu();

  if (state == 1) updateLCD_menu();

  if (state == 0)
  {
   vreme_ciklusa = vreme_int;
   brzina_ciklusa = brzina_int; 

   int brzina_8bit = round(brzina_ciklusa*2.55);
   brzina_8bit = 256 - brzina_8bit;

   vreme_ciklusa_ms = vreme_ciklusa*1000;
   int i = vreme_ciklusa;


   // Spustanje step motora
   lcd.clear();
   lcd.print("Cycle on");
   //spusti_stepper();


  // Spustanje step motora
  lcd.setCursor(0,1);
  lcd.print("Motor lowering"); 
  
  stepperX.moveTo(8312);
  while (stepperX.currentPosition() != 8312) stepperX.run();
  stepperX.stop(); 
  stepperX.runToPosition(); 

   
   delay(1000);


   // Ukljuci DC motore
   lcd.setCursor(0,1);
   lcd.print("               ");
   lcd.setCursor(0,1);
   lcd.print("Time:           ");
   analogWrite(motor2,brzina_8bit);
   analogWrite(motor1,0);
   while(i>0)
   {
      i--;
      lcd.setCursor(7,1);
      lcd.print(i);
      delay(1000);
      lcd.setCursor(7,1);
      lcd.print("    ");
   }
   
   //delay(vreme_ciklusa_ms);
   analogWrite(motor2,255);
   analogWrite(motor1,255);

   // Podizanje step motora
   lcd.setCursor(0,1);
   //lcd.print("                ");
   lcd.print("Motor homing ");
     
   stepperX.moveTo(0);
   while (stepperX.currentPosition() != 0) stepperX.run();
   stepperX.stop(); 
   stepperX.runToPosition(); 
   lcd.clear();

  
   state = HIGH;
   
  }
  


  
} 
