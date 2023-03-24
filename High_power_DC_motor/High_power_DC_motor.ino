#define spin_RIGHT 5
#define spin_LEFT 6

unsigned long Dir_time = 0;
bool spin_DIR_val = 0;


void setup() {

  Dir_time = millis();

  pinMode(spin_RIGHT, OUTPUT);
  pinMode(spin_LEFT, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  if (millis() - Dir_time > 3000) {
    spin_DIR_val = !spin_DIR_val;
    Dir_time = millis();
  }

  if (spin_DIR_val == 1) {
    Serial.println("cl");
    analogWrite(spin_RIGHT, 0);
    analogWrite(spin_LEFT, 255);
  }

  else {
    Serial.println("acl");
    analogWrite(spin_RIGHT, 255);
    analogWrite(spin_LEFT, 0);
  }

}
