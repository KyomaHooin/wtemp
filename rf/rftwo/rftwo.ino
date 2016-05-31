//RF server
//#define RF_MOSI 11 -> Hardwared MOSI SPI(10 Mhz)
//#define RF_MISO 12 -> Hardwired MISO SPI(10 Mhz)
//#define RF_SCK 13  -> Hardwired SCK SPI (10 Mhz)
//#define RF_POWER 3

#include <SPI.h>
#include <RF24.h>
#include <stdio.h>

#define RF_CE 8 //RX/TX mode
#define RF_CSN 9 //SS SPI

//RF24  class instance na pinech CE/CSN.
RF24 rf(RF_CE, RF_CSN);
//RF24 data RX/TX pipe address:
const uint64_t rf_rx_pipe = 0xE8E8F0F0E1LL;
//RF payload struct
struct payloadStruct {
  float voltage;
  float temperature;
} payload;
//RF sync delay
unsigned long rf_timeout;

//SETUP

void setup() {
  //DEBUG
  Serial.begin(9600);
  //Wakeup level(1)
  rf.begin();
  //Set PA level
  rf.setPALevel(RF24_PA_LOW);
  //Open RX pipe
  rf.openReadingPipe(1, rf_rx_pipe);
}

//MAIN

void loop() {
  //RF start
  rf.powerUp();
  rf.startListening();
  rf_timeout = millis();
  //RF non-blocking sync(max. = sleep time) => time to sync 0-10ms
  while(!rf.available()) { if (millis() - rf_timeout > 500 ) { break; }}
  //RF get payload
  if (rf.available()){
    rf.read(&payload, sizeof(payload));
    Serial.println(payload.voltage);
    Serial.println(payload.temperature);
  } else { Serial.println("E"); }
  //RF stop
  rf.stopListening();
  rf.powerDown();
 //sleep goes here..
 delay(5000);
}
