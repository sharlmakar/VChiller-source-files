String rasp_com;

void setup() {
  Serial.begin(9600);

}

void loop() {
  
  if(Serial.available()){
    rasp_com= Serial.readString();// read the incoming data as string
    rasp_com.remove(rasp_com.length()-1); //remove endline character \n
  }
  Serial.println(rasp_com);
}
