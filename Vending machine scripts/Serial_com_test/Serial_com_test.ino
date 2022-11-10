String rasp_com;

void setup() {
  Serial.begin(9600);

}

void loop() {
  while(Serial.available()) {
    rasp_com= Serial.readString();// read the incoming data as string
  }

  Serial.println(rasp_com);
}
