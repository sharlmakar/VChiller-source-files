int relays[] = {13,12,11,10,9,8};
int pin_Count = 6;

void setup() {
   for (int thisPin = 0; thisPin < pin_Count; thisPin++) {

    pinMode(relays[thisPin], OUTPUT);
    digitalWrite(relays[thisPin], HIGH);

  }

}

void loop() {
  for (int thisPin = 0; thisPin < pin_Count; thisPin++) {

    // turn the pin on:

    digitalWrite(relays[thisPin], LOW);

    delay(2000);

    // turn the pin off:

    digitalWrite(relays[thisPin], HIGH);

  }

}
