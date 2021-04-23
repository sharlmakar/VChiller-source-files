//#include <ArduinoRS485.h>
//#include <ModbusMaster.h>
#include <SoftwareSerial.h>

//#define MAX485_DE 3
//#define MAX485_RE_NEG 2
#define TX_ENABLE_PIN 1

SoftwareSerial rs485(2, 3);

//ModbusMaster node;

uint8_t start_msg [] = {0x01, 0x06, 0x10, 0x11, 0x55, 0xD4, 0xE2};
uint8_t stop_msg [] = {0x01, 0x06, 0x10, 0x11, 0xAA, 0x94, 0xA2};

//void preTransmission(){
//  digitalWrite(MAX485_DE, HIGH);
//  digitalWrite(MAX485_RE_NEG, HIGH);
//}
//
//void postTransmission(){
//  digitalWrite(MAX485_DE, LOW);
//  digitalWrite(MAX485_RE_NEG, LOW);
//}

void setup() {
//  pinMode(MAX485_DE, OUTPUT);
//  pinMode(MAX485_RE_NEG, OUTPUT);
//  digitalWrite(MAX485_DE, LOW);
//  digitalWrite(MAX485_RE_NEG, LOW);
  pinMode(TX_ENABLE_PIN, OUTPUT);
  rs485.begin(9600);
  Serial.begin(9600);

//  node.begin(1,Serial);
//
//  node.preTransmission(preTransmission);
//  node.postTransmission(postTransmission);
}

void loop() {
  digitalWrite (TX_ENABLE_PIN, HIGH);
  Serial.println("Stopping");
//  RS485.beginTransmission();
  for (int i = 0 ; i < sizeof(stop_msg) ; i++)
  {
    rs485.write(stop_msg[i]);
  }
  delay(20000);
  Serial.println("Starting");
  for (int i = 0 ; i < sizeof(start_msg) ; i++)
  {
    rs485.write(start_msg[i]);
  }
  delay(10000);
  digitalWrite(TX_ENABLE_PIN, LOW);
//  RS485.endTransmission();
}
