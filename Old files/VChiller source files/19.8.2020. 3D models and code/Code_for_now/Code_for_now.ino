#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include <AccelStepper.h>
// 3D models.com
// AccelStepper Setup
// 3.33 steps per mm

AccelStepper stepperX(1, 5, 4);   

// Home Switch
#define home_switch 2 

// Step motor
int dir = 4;
int stp = 5;

// Stepper Travel Variables
long TravelX;  // Used to store the X value entered in the Serial Monitor
int move_finished=1;  // Used to check if move is completed
long initial_homing=-1;  // Used to Home Stepper at startup

// LCD I2C
LiquidCrystal_PCF8574 lcd(0x27); // Setovanje I2C adrese LCD displeja

// Input pots
const int potPin1 = A3;  
const int potPin2 = A1; 

int potValue1 = 0;
int potValue2 = 0;

int potMap1 = 0;
int potMap2 = 0;

// Tasteri
int BUTTON = 3;
int state = HIGH;

// Motori
int motor1 = 9;
int motor2 = 10;


// Pomocne promenljive 
int brzina_int = 0;
int vreme_int = 0;
int brzina_ciklusa = 0;
int vreme_ciklusa = 0;
long vreme_ciklusa_ms;



long hours = 0;

// Promenljive za merenje vremena 
unsigned long startMillis;  
unsigned long currentMillis;
const unsigned long period = 3600000;  // period od jednog sata u milisekundama, prilikom testiranja ovde stavi 1000 (1 s) da bi mogao da vidis promene

void setup()
{
  delay(100);
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, HIGH);
  delay(100);
  
  // Otvaranje I2C komunikacije sa LCD-om
  Wire.begin();
  Wire.beginTransmission(0x27);

  // Setovanje BUTTONa i njegove interrupt funkcije
  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON), BUTTON_interrupt, FALLING);

  // Cuvanje pocetnog vremena koda
  startMillis = millis();  
  
  updateLCD_start();

  // Homing stepper motora
  pinMode(home_switch, INPUT_PULLUP);
  delay(5);  

  // Maksimalna brzina i ubrzanje
  stepperX.setMaxSpeed(100.0);   
  stepperX.setAcceleration(100.0);
 

  // Start Homing procedure of Stepper Motor at startup
  lcd.clear();
  lcd.print("Homing procedura");
  delay(100);

  while (digitalRead(home_switch)) {  // Make the Stepper move CCW until the switch is activated   
    stepperX.moveTo(initial_homing);  // Set the position to move to
    initial_homing--;  // Decrease by 1 for next move if needed
    stepperX.run();  // Start moving the stepper
    delay(5);
}

  stepperX.setCurrentPosition(0);  // Set the current position as zero for now
  stepperX.setMaxSpeed(100.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperX.setAcceleration(100.0);  // Set Acceleration of Stepper
  initial_homing=1;

  while (!digitalRead(home_switch)) { // Make the Stepper move CW until the switch is deactivated
    stepperX.moveTo(initial_homing);  
    stepperX.run();
    initial_homing++;
    delay(5);
  }
  
  stepperX.setCurrentPosition(0);
  lcd.clear();
  lcd.print("Homing zavrsen");
  delay(2000);

  stepperX.setMaxSpeed(1000.0);      // Set Max Speed of Stepper (Faster for regular movements)
  stepperX.setAcceleration(1000.0);  // Set Acceleration of Stepper


  state = HIGH;

  
} 

// Interrupt funkcija tastera
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
  lcd.print("Hello world!");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);
  lcd.clear();
}

// Update LCD-a kada je uredjaj u meniju za izbor
void updateLCD_menu()
{
  //lcd.clear();
  lcd.setBacklight(255);
  lcd.noCursor();
  lcd.home();
  lcd.print("Brzina     Vreme");
  lcd.setCursor(0, 1);
  
  brzina_int = pot1_read();
  vreme_int = pot2_read();


  // ISPIS BRZINE
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
  potMap1 = map(potValue1, 0, 1023, 80, 100);
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

void spusti_stepper()
{
  move_finished=0;  
  
  TravelX = 340; // pomeraj motora
  lcd.setCursor(0,1);
  lcd.print("Spustanje u toku");  
  
  stepperX.moveTo(TravelX);
  stepperX.setSpeed(100);
  // stepperX.runSpeedToPosition();
  while (stepperX.distanceToGo() != 0) stepperX.runSpeedToPosition();
  //stepperX.stop();
  
  delay(1000);  
  
}

void podigni_stepper()
{
    while (digitalRead(home_switch)) {     
      stepperX.moveTo(initial_homing);  
      initial_homing--;  
      stepperX.run();  
      delay(5);
    }

    lcd.clear();
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
   lcd.print("Ciklus u toku");
   //spusti_stepper();
   delay(1000);


   // Ukljuci DC motore
   lcd.setCursor(0,1);
   lcd.print("               ");
   lcd.setCursor(0,1);
   lcd.print("Vreme:          ");

   // Ukljucivanje motora


   // Motor limenke

 /*   for (int br = 256; br == 240; br--)
    {
      analogWrite(motor2, br);
      delay(200);
    }
  
   for (int br = 246; br == brzina_8bit; br--)
  {
    analogWrite(motor2, br);
    delay(20);
  }

  */
  
   analogWrite(motor2,LOW);

   // Pumpa motor
   digitalWrite(motor1,LOW);
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
   digitalWrite(motor2,HIGH);
   digitalWrite(motor1,HIGH);

   // Podizanje step motora
   lcd.setCursor(0,1);
   //lcd.print("                ");
   lcd.print("Vracanje motora ");
   podigni_stepper();

  
   state = HIGH;
   
  }
  


  
} 
