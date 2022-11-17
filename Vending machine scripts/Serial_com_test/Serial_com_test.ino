String rasp_com;

void setup() {
  Serial.begin(9600);

}

void loop() {
  
  if(Serial.available()){
//    while(Serial.available()) {
      rasp_com= Serial.readString();// read the incoming data as string
//    }
  }
  Serial.print(rasp_com);
}
